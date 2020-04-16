

class ExecutionFlowSimulator : public IExecutionFlow
{
	bool _called = false;
	int _lineNumberStart;
	IExecutionContext* _pExecutionContext;
	int _returnValue;
	float _firstParameter;
	CommandResult _commandResult;

public:
	ExecutionFlowSimulator(IExecutionContext* pExecutionContext, int returnValue):
		_commandResult(16)
	{
		_pExecutionContext = pExecutionContext;
		_returnValue = returnValue;
	}


	virtual CommandResultStatus RunProgram(int runCount)
	{
		_called = true;

        _lineNumberStart = _pExecutionContext->StackTopFrame()->LineNumberStart;

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

	int GetLineNumberStart()
	{
		return _lineNumberStart;
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
		Assert::AreEqual(10, executionFlow.GetLineNumberStart());
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
		Assert::AreEqual(10, executionFlow.GetLineNumberStart());
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
