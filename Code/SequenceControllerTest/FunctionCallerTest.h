

class ExecutionFlowSimulator : public IExecutionFlow
{
	bool _called = false;
	int _serialNumberStart;
	IExecutionContext* _pExecutionContext;
	int _returnValue;
	float _firstParameter;
	CommandResult _commandResult;

public:
	ExecutionFlowSimulator(IExecutionContext* pExecutionContext, int returnValue)
	{
		_pExecutionContext = pExecutionContext;
		_returnValue = returnValue;
	}


	virtual CommandResultStatus RunProgram(int runCount)
	{
		_called = true;

		_serialNumberStart = _pExecutionContext->StackTopFrame()->SerialNumberStart;

		_pExecutionContext->AddVariableAndSet("<ReturnValue>", &Variable(_returnValue));

		Variable* pArgumentCount = _pExecutionContext->GetVariableWithoutErrorCheck("#A");
		if (pArgumentCount->GetValueInt() >= 1)
		{
			_firstParameter = _pExecutionContext->GetVariableWithoutErrorCheck("#A0")->GetValueFloat(0);
		}

		_commandResult.ClearAbort();
		_commandResult.SetStatus(CommandResultStatus::CommandNone);

		return CommandResultStatus::CommandNone;
	}

	virtual	void ExecuteLedCommand(CommandResult* pCommandResult) {}
	virtual CommandResult* GetCommandResult() { return &_commandResult; }
	virtual Command* GetCommand(int commandNumber) { return 0; }
	virtual void AbortExecution() {}
	virtual bool IsAborting() { return false; }

	virtual void ConfigureLeds(const char* pLedType, int ledCount, int pin) {}

	bool GetCalled()
	{
		return _called;
	}

	int GetSerialNumberStart()
	{
		return _serialNumberStart;
	}

	float GetFirstParameter()
	{
		return _firstParameter;
	}
};

class FunctionCallerTest
{

	static void TestFunctionCall()
	{
		ParseErrors parseErrors;
		ExecutionContext executionContext;
		FunctionStore functionStore;
		ExecutionFlowSimulator executionFlow(&executionContext, 123456);
		FunctionCaller functionCaller(&functionStore, &executionContext, &parseErrors, &executionFlow);

		executionContext.AddVariableAndSet("#A", &Variable(0));

		functionStore.DefineStart("MyFunction", &parseErrors, 10);

		Variable returnValue = functionCaller.Call("MyFunction", 1);

		Assert::AreEqual(true, executionFlow.GetCalled());
		Assert::AreEqual(123456.0, returnValue.GetValueFloat(0));
		Assert::AreEqual(10, executionFlow.GetSerialNumberStart());
	}

	static void TestFunctionCallWithParameter()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		FunctionStore functionStore;
		ExecutionFlowSimulator executionFlow(&executionContext, 123456);
		FunctionCaller functionCaller(&functionStore, &executionContext, &parseErrors, &executionFlow);

		executionContext.AddVariableAndSet("#A", &Variable(1));
		executionContext.AddVariableAndSet("#A0", &Variable(15.5F), 1);

		functionStore.DefineStart("MyFunction", &parseErrors, 10);

		Variable returnValue = functionCaller.Call("MyFunction", 1);

		Assert::AreEqual(true, executionFlow.GetCalled());
		Assert::AreEqual(123456.0, returnValue.GetValueFloat(0));
		Assert::AreEqual(10, executionFlow.GetSerialNumberStart());
		Assert::AreEqual(15.5F, executionFlow.GetFirstParameter());
	}

	static void TestArgumentNameGenerator()
	{
		const char* pArgumentName = FunctionCaller::GenerateArgumentName(1);
		Assert::AreEqual("#A1", pArgumentName);

		pArgumentName = FunctionCaller::GenerateArgumentName(2);
		Assert::AreEqual("#A2", pArgumentName);
	}

public:

	static void Run()
	{
		TestFunctionCall();
		TestFunctionCallWithParameter();

		TestArgumentNameGenerator();
	}

};

#if fred
static Variable FunctionCallHandlerImplementation(FunctionDefinition* pFunctionDefinition, VariableCollection* pVariableCollection, FunctionStore* pFunctionStore, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow)
{
	Assert::AreEqual("MyFunction", pFunctionDefinition->Name);
	Assert::AreEqual(0, pVariableCollection->GetWithoutErrorCheck("#A", 2)->GetValueInt());

	return Variable(10.0F);
}

static void TestFunctionCall()
{
	Expression expression;
	VariableCollection variableCollection;
	ParseErrors parseErrors;
	Stack stack;
	FunctionStore functionStore;

	stack.CreateFrame();

	functionStore.DefineStart("MyFunction", &parseErrors, 10);

	Variable value = expression.Evaluate("MyFunction()", &variableCollection, &functionStore, &stack, &parseErrors, 1, FunctionCallHandlerImplementation);
	Assert::AreEqual(10.0F, value.GetValueFloat(0));
}

static Variable FunctionCallHandlerImplementation2(FunctionDefinition* pFunctionDefinition, VariableCollection* pVariableCollection, FunctionStore* pFunctionStore, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow)
{
	Assert::AreEqual(1, pVariableCollection->GetWithoutErrorCheck("#A", 2)->GetValueInt());
	Assert::AreEqual(15.3F, pVariableCollection->GetWithoutErrorCheck("#A0", 2)->GetValueFloat(0));

	return *pVariableCollection->GetWithoutErrorCheck("#A0", 2);
}

static void TestFunctionCallWithParameters()
{
	Expression expression;
	VariableCollection variableCollection;
	ParseErrors parseErrors;
	Stack stack;
	FunctionStore functionStore;

	stack.CreateFrame();

	functionStore.DefineStart("MyFunction", &parseErrors, 10);

	Variable value = expression.Evaluate("MyFunction(15.3)", &variableCollection, &functionStore, &stack, &parseErrors, 1, FunctionCallHandlerImplementation2, 0);
	Assert::AreEqual(15.3F, value.GetValueFloat(0));
	Assert::AreEqual(1, stack.GetFrameCount());
}

#endif
