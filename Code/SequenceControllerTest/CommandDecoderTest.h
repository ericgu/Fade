#pragma once

#include "ExecutionContext.h"
#include "CommandDecoder.h"

class MockExecutionFlow : public IExecutionFlow
{
public:
	CommandResult _commandResult;
	virtual CommandResultStatus RunProgram(int runCount = -1) 
	{
		return CommandResultStatus::CommandNone;
	}

	virtual	void ExecuteLedCommand(CommandResult* pCommandResult)
	{}

	virtual CommandResult* GetCommandResult()
	{
		return &_commandResult;
	}
};

class CommandDecoderTest
{
	static CommandResult* _pCallbackCommandResult;

	static void AreEqual(CommandResultStatus expected, CommandResultStatus actual)
	{
		if (expected != actual)
		{
			char buffer[128];
			sprintf_s(buffer, "Expected %i got %i", (int) expected, (int)actual);
			Assert::AreEqual((int) expected, (int) actual);
		}
	}

	static void AssertLedState(LedState ledState, int channel, float brightness, int cycleCount)
	{
		Assert::AreEqual(channel, ledState.GetChannel());
		Assert::AreEqual(brightness, ledState.GetBrightness());
		Assert::AreEqual(cycleCount, ledState.GetCycleCount());
	}

	static void ResultCallback(CommandResult* pCommandResult)
	{
		_pCallbackCommandResult = pCommandResult;
	}

	static void TestSingle()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("D(1,0,1.0)", 15), &executionFlow);

		Assert::AreEqual(1, executionFlow.GetCommandResult()->GetCount());

		AssertLedState(executionFlow.GetCommandResult()->GetTarget(0), 0, 1.0, 1);
		AreEqual(CommandResultStatus::CommandNone, executionFlow.GetCommandResult()->GetStatus());
	}

	static void TestSingleImmediate()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("DI(1,0,1.0)", 15), &executionFlow);

		Assert::AreEqual(1, executionFlow.GetCommandResult()->GetCount());

		AssertLedState(executionFlow.GetCommandResult()->GetTarget(0), 0, 1.0, 1);
		AreEqual(CommandResultStatus::CommandExecute, executionFlow.GetCommandResult()->GetStatus());
	}

	static void TestSingleWithVariables()
	{
		ExecutionContext executionContext;
		executionContext._variables.AddAndSet("A", 3, 1);
		executionContext._variables.AddAndSet("B", 1.0F, 1);

		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("D(1,A,B)", 15), &executionFlow);

		Assert::AreEqual(1, executionFlow.GetCommandResult()->GetCount());

		AssertLedState(executionFlow.GetCommandResult()->GetTarget(0), 3, 1.0, 1);
		AreEqual(CommandResultStatus::CommandNone, executionFlow.GetCommandResult()->GetStatus());
	}

	static void TestDouble()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("D(1,2,2.0,10,8.0)", 15), &executionFlow);

		Assert::AreEqual(2, executionFlow.GetCommandResult()->GetCount());

		AssertLedState(executionFlow.GetCommandResult()->GetTarget(0), 2, 2.0, 1);
		AssertLedState(executionFlow.GetCommandResult()->GetTarget(1), 10, 8.0, 1);
		AreEqual(CommandResultStatus::CommandNone, executionFlow.GetCommandResult()->GetStatus());
	}

	static void TestSequential()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("S(1,1.0)", 15), &executionFlow);

		Assert::AreEqual(1, executionFlow.GetCommandResult()->GetCount());

		AssertLedState(executionFlow.GetCommandResult()->GetTarget(0), 0, 1.0, 1);
		AreEqual(CommandResultStatus::CommandNone, executionFlow.GetCommandResult()->GetStatus());
	}

	static void TestSequentialImmediate()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("SI(1,1.0)", 15), &executionFlow);

		Assert::AreEqual(1, executionFlow.GetCommandResult()->GetCount());

		AssertLedState(executionFlow.GetCommandResult()->GetTarget(0), 0, 1.0, 1);
		AreEqual(CommandResultStatus::CommandExecute, executionFlow.GetCommandResult()->GetStatus());
	}

	static void TestSequentialWithVariables()
	{
		ExecutionContext executionContext;
		executionContext._variables.AddAndSet("A", 3, 1);
		executionContext._variables.AddAndSet("B", 13.33F, 1);

		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("S(1,B)", 15), &executionFlow);

		Assert::AreEqual(1, executionFlow.GetCommandResult()->GetCount());

		AssertLedState(executionFlow.GetCommandResult()->GetTarget(0), 0, 13.33F, 1);
		AreEqual(CommandResultStatus::CommandNone, executionFlow.GetCommandResult()->GetStatus());
	}

	static void TestSequentialSeries()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("S(1,1.0,2.0,3.0,4.0,5.0,6.0)", 15), &executionFlow);

		Assert::AreEqual(6, executionFlow.GetCommandResult()->GetCount());

		for (int i = 0; i < 6; i++)
		{
			AssertLedState(executionFlow.GetCommandResult()->GetTarget(i), i, (i + 1) * 1.0F, 1);
		}
		AreEqual(CommandResultStatus::CommandNone, executionFlow.GetCommandResult()->GetStatus());
	}

	static void ValidateVariable(ExecutionContext executionContext, ParseErrors* pParseErrors, const char *pVariableName, int active, int value)
	{
		Variable* pVariable = executionContext._variables.Get(pVariableName, executionContext._stack.GetFrameCount(), pParseErrors, -1);
		Assert::AreEqual(active, pVariable->GetActiveFlag());
		Assert::AreEqual(value, pVariable->GetValueInt());
	}

	static void TestLoopEnd()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("ENDFOR", 15), &executionFlow);

		AreEqual(CommandResultStatus::CommandEndOfLoop, executionFlow.GetCommandResult()->GetStatus());
	}

	static void TestAssignmentConstant()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("Var = 20", 15), &executionFlow);

		ValidateVariable(executionContext, &parseErrors, "Var", 1, 20);
	}

	static void TestAssignmentVariable()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("Var = 20", 15), &executionFlow);

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("Second = Var", 15), &executionFlow);
		ValidateVariable(executionContext, &parseErrors, "Var", 1, 20);
		ValidateVariable(executionContext, &parseErrors, "Second", 1, 20);
	}

	static void TestAssignmentOverride()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("Var = 20", 15), &executionFlow);

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("Var = 33", 15), &executionFlow);
		ValidateVariable(executionContext, &parseErrors, "Var", 1, 33);
	}

	static void TestAssignmentNoSpaces()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("Var=20", 15), &executionFlow);

		ValidateVariable(executionContext, &parseErrors, "Var", 1, 20);
	}

	static void TestAnimateConstant()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("A(50)", 15), &executionFlow);

		Assert::AreEqual(50, executionFlow.GetCommandResult()->GetCycleCount());
	}

	static void TestRandomAssignment()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		MyRandom::SetFirstValue(5);
		CommandDecoder::Decode(executionContext, &parseErrors, &Command("Var = RAND(0,10)", 15), &executionFlow);
		ValidateVariable(executionContext, &parseErrors, "Var", 1, 5);

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("Var = RAND(0,10)", 15), &executionFlow);
		ValidateVariable(executionContext, &parseErrors, "Var", 1, 7);
	}

	static void TestRandomUsage()
	{
		ExecutionContext executionContext;
		executionContext._variables.AddAndSet("B", 1.0F, 1);

		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		MyRandom::SetFirstValue(1);
		CommandDecoder::Decode(executionContext, &parseErrors, &Command("D(1,RAND(0,16),B)", 15), &executionFlow);

		Assert::AreEqual(1, executionFlow.GetCommandResult()->GetCount());

		AssertLedState(executionFlow.GetCommandResult()->GetTarget(0), 1, 1.0, 1);
		AreEqual(CommandResultStatus::CommandNone, executionFlow.GetCommandResult()->GetStatus());
	}

	static void TestPrintString()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		Serial.SetOutput(false);
		CommandDecoder::Decode(executionContext, &parseErrors, &Command("P(\"Hello\")", 0), &executionFlow);
		Serial.SetOutput(true);

		Assert::AreEqual("Hello", Serial.GetLastString());
	}

	static void TestPrintStringWithEquals()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		Serial.SetOutput(false);
		CommandDecoder::Decode(executionContext, &parseErrors, &Command("P(\"Varname = \")", 0), &executionFlow);
		Serial.SetOutput(true);

		Assert::AreEqual("Varname = ", Serial.GetLastString());
	}

	static void TestPrintVariable()
	{
		ExecutionContext executionContext;
		executionContext._variables.AddAndSet("Variable", 1.0F, 1);

		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		Serial.SetOutput(false);
		CommandDecoder::Decode(executionContext, &parseErrors, &Command("P(Variable)", 0), &executionFlow);
		Serial.SetOutput(true);

		Assert::AreEqual("1.000000", Serial.GetLastString());
	}

	static void TestPrintStringWithNewline()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		Serial.SetOutput(false);
		CommandDecoder::Decode(executionContext, &parseErrors, &Command("PL(\"Hello\")", 0), &executionFlow);
		Serial.SetOutput(true);

		Assert::AreEqual("Hello\n", Serial.GetLastString());
	}

	static void TestBlankLine()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("", 0), &executionFlow);
		Assert::AreEqual(0, parseErrors.GetErrorCount());
	}

	static void TestWhitespaceOnly()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("      ", 0), &executionFlow);
		Assert::AreEqual(0, parseErrors.GetErrorCount());
	}

	static void TestComment()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("// comment", 0), &executionFlow);
		Assert::AreEqual(0, parseErrors.GetErrorCount());
	}

	static void ValidateError(ExecutionContext& executionContext, ParseErrors* pParseErrors, int errorCount, const char* pExpectedString, int lineNumber)
	{
		Assert::AreEqual(errorCount, pParseErrors->GetErrorCount());
		if (errorCount != pParseErrors->GetErrorCount())
		{
			return;
		}
		Assert::AreEqual(pExpectedString, pParseErrors->GetError(errorCount - 1)._errorText);
		Assert::AreEqual(lineNumber, pParseErrors->GetError(errorCount - 1)._lineNumber);
	}

	static void TestErrorDirect()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("DX(", 0), &executionFlow);
		ValidateError(executionContext, &parseErrors, 1, "Unrecognized command: DX(", 0);

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("D(55, 1)", 2), &executionFlow);
		ValidateError(executionContext, &parseErrors, 2, "Invalid D command: missing brightness target", 2);

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("D(MissingVar, 5, 1.0)", 3), &executionFlow);
		ValidateError(executionContext, &parseErrors, 3, "Unrecognized identifier: MissingVar", 3);
	}

	static void TestErrorSequential()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("SX(", 0), &executionFlow);
		ValidateError(executionContext, &parseErrors, 1, "Unrecognized command: SX(", 0);

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("S()", 1), &executionFlow);
		ValidateError(executionContext, &parseErrors, 2, "Invalid S command: expected cycle count after (", 1);

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("S(MissingVar, 5, 1.0)", 3), &executionFlow);
		ValidateError(executionContext, &parseErrors, 3, "Unrecognized identifier: MissingVar", 3);
	}

	static void TestErrorAnimate()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("Axx(", 0), &executionFlow);
		ValidateError(executionContext, &parseErrors, 1, "Unrecognized command: Axx(", 0);

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("A()", 1), &executionFlow);
		ValidateError(executionContext, &parseErrors, 2, "Invalid A command: expected cycle count", 1);
	}

	static void TestUnrecognizedCommand()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("FASDF", 0), &executionFlow);
		ValidateError(executionContext, &parseErrors, 1, "Unrecognized command: FASDF", 0);
	}

	static void TestUnrecognizedCommand2()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("qqP(\"UP \")", 0), &executionFlow);
		ValidateError(executionContext, &parseErrors, 1, "Unrecognized command: FASDF", 0);
	}

	static void TestDefineFunction()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		Serial.SetOutput(false);
		CommandDecoder::Decode(executionContext, &parseErrors, &Command("FUNC MyFunction", 15), &executionFlow);
		CommandDecoder::Decode(executionContext, &parseErrors, &Command("PL(\"Hello\")", 16), &executionFlow);
		CommandDecoder::Decode(executionContext, &parseErrors, &Command("ENDFUNC", 17), &executionFlow);
		Serial.SetOutput(true);

		Assert::AreEqual(1, executionContext._functionStore.GetCount());
		FunctionDefinition* pFunctionDefinition = executionContext._functionStore.Lookup("MyFunction");

		Assert::AreEqual(15, pFunctionDefinition->SerialNumberStart);
		Assert::AreEqual(17, pFunctionDefinition->SerialNumberEnd);
	}

	static void TestReturn()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		MockExecutionFlow executionFlow;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("RETURN 15.5", 0), &executionFlow);

		Variable* pVariable = executionContext._variables.GetWithoutErrorCheck("<ReturnValue>", executionContext._stack.GetFrameCount());
		Assert::AreEqual(15.5, pVariable->GetValueFloat());
		//Assert:AreEqual(2, executionContext._stack.GetFrameCount());
	}

public:

	static int Run()
	{
		TestSingle();
		TestDouble();

		TestSequential();
		TestSequentialSeries();

		TestLoopEnd();

		TestSingleWithVariables();
		TestSequentialWithVariables();

		TestAssignmentConstant();
		TestAssignmentVariable();
		TestAssignmentOverride();
		TestAssignmentNoSpaces();

		TestAnimateConstant();
		TestSingleImmediate();
		TestSequentialImmediate();

		TestRandomAssignment();
		TestRandomUsage();

		TestPrintString();
		TestPrintStringWithEquals();
		TestPrintVariable();
		TestPrintStringWithNewline();

		TestBlankLine();
		TestWhitespaceOnly();
		TestComment();

		TestErrorDirect();
		TestErrorSequential();
		TestErrorAnimate();
		TestUnrecognizedCommand();
		//TestUnrecognizedCommand2();

		TestDefineFunction();
		TestReturn();		

		return 0;
	}
};

CommandResult* CommandDecoderTest::_pCallbackCommandResult;