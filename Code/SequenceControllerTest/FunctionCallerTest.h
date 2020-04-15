#include "FunctionCaller.h"

class ExecutionFlowSimulator : public IExecutionFlow
{
	bool _called = false;
	int _serialNumberStart;
	Stack* _pStack;
	VariableCollection* _pVariableCollection;
	int _returnValue;
	float _firstParameter;
	CommandResult _commandResult;

public:
	ExecutionFlowSimulator(Stack* pStack, VariableCollection* pVariableCollection, int returnValue)
	{
		_pStack = pStack;
		_pVariableCollection = pVariableCollection;
		_returnValue = returnValue;
	}


	virtual CommandResultStatus RunProgram(int runCount)
	{
		_called = true;

		_serialNumberStart = _pStack->GetTopFrame()->SerialNumberStart;

		_pVariableCollection->AddAndSet("<ReturnValue>", &Variable(_returnValue), _pStack->GetFrameCount());

		Variable* pArgumentCount = _pVariableCollection->GetWithoutErrorCheck("#A", _pStack->GetFrameCount());
		if (pArgumentCount->GetValueInt() >= 1)
		{
			_firstParameter = _pVariableCollection->GetWithoutErrorCheck("#A0", _pStack->GetFrameCount())->GetValueFloat(0);
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
		VariableCollection variableCollection;
		ParseErrors parseErrors;
		Stack stack;
		FunctionStore functionStore;
		ExecutionFlowSimulator executionFlow(&stack, &variableCollection, 123456);
		FunctionCaller functionCaller(&functionStore, &stack, &variableCollection, &parseErrors, &executionFlow);

		stack.CreateFrame();
		variableCollection.AddAndSet("#A", &Variable(0), 1);

		functionStore.DefineStart("MyFunction", &parseErrors, 10);

		Variable returnValue = functionCaller.Call("MyFunction", 1);

		Assert::AreEqual(true, executionFlow.GetCalled());
		Assert::AreEqual(123456.0, returnValue.GetValueFloat(0));
		Assert::AreEqual(10, executionFlow.GetSerialNumberStart());
	}

	static void TestFunctionCallWithParameter()
	{
		VariableCollection variableCollection;
		ParseErrors parseErrors;
		Stack stack;
		FunctionStore functionStore;
		ExecutionFlowSimulator executionFlow(&stack, &variableCollection, 123456);
		FunctionCaller functionCaller(&functionStore, &stack, &variableCollection, &parseErrors, &executionFlow);

		stack.CreateFrame();
		variableCollection.AddAndSet("#A", &Variable(1), 1);
		variableCollection.AddAndSet("#A0", &Variable(15.5F), 1);

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
