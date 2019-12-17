#pragma once

#include "ExecutionContext.h"
#include "CommandDecoder.h"

class CommandDecoderTest
{
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

	static void TestSingle()
	{
		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("D(1,0,1.0)", 15), commandResult);

		Assert::AreEqual(1, commandResult.GetCount());

		AssertLedState(commandResult.GetTarget(0), 0, 1.0, 1);
		AreEqual(CommandResultStatus::CommandNone, commandResult.GetStatus());
	}

	static void TestSingleImmediate()
	{
		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("DI(1,0,1.0)", 15), commandResult);

		Assert::AreEqual(1, commandResult.GetCount());

		AssertLedState(commandResult.GetTarget(0), 0, 1.0, 1);
		AreEqual(CommandResultStatus::CommandExecute, commandResult.GetStatus());
	}

	static void TestSingleWithVariables()
	{
		ExecutionContext executionContext;
		executionContext._variables.AddAndSet("A", 3);
		executionContext._variables.AddAndSet("B", 1.0F);

		CommandResult commandResult;
		ParseErrors parseErrors;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("D(1,A,B)", 15), commandResult);

		Assert::AreEqual(1, commandResult.GetCount());

		AssertLedState(commandResult.GetTarget(0), 3, 1.0, 1);
		AreEqual(CommandResultStatus::CommandNone, commandResult.GetStatus());
	}

	static void TestDouble()
	{
		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("D(1,2,2.0,10,8.0)", 15), commandResult);

		Assert::AreEqual(2, commandResult.GetCount());

		AssertLedState(commandResult.GetTarget(0), 2, 2.0, 1);
		AssertLedState(commandResult.GetTarget(1), 10, 8.0, 1);
		AreEqual(CommandResultStatus::CommandNone, commandResult.GetStatus());
	}

	static void TestSequential()
	{
		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("S(1,1.0)", 15), commandResult);

		Assert::AreEqual(1, commandResult.GetCount());

		AssertLedState(commandResult.GetTarget(0), 0, 1.0, 1);
		AreEqual(CommandResultStatus::CommandNone, commandResult.GetStatus());
	}

	static void TestSequentialImmediate()
	{
		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("SI(1,1.0)", 15), commandResult);

		Assert::AreEqual(1, commandResult.GetCount());

		AssertLedState(commandResult.GetTarget(0), 0, 1.0, 1);
		AreEqual(CommandResultStatus::CommandExecute, commandResult.GetStatus());
	}

	static void TestSequentialWithVariables()
	{
		ExecutionContext executionContext;
		executionContext._variables.AddAndSet("A", 3);
		executionContext._variables.AddAndSet("B", 13.33F);

		CommandResult commandResult;
		ParseErrors parseErrors;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("S(1,B)", 15), commandResult);

		Assert::AreEqual(1, commandResult.GetCount());

		AssertLedState(commandResult.GetTarget(0), 0, 13.33F, 1);
		AreEqual(CommandResultStatus::CommandNone, commandResult.GetStatus());
	}

	static void TestSequentialSeries()
	{
		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("S(1,1.0,2.0,3.0,4.0,5.0,6.0)", 15), commandResult);

		Assert::AreEqual(6, commandResult.GetCount());

		for (int i = 0; i < 6; i++)
		{
			AssertLedState(commandResult.GetTarget(i), i, (i + 1) * 1.0F, 1);
		}
		AreEqual(CommandResultStatus::CommandNone, commandResult.GetStatus());
	}

	static void ValidateVariable(ExecutionContext executionContext, ParseErrors* pParseErrors, const char *pVariableName, int active, int value)
	{
		Variable* pVariable = executionContext._variables.Get(pVariableName, pParseErrors, -1);
		Assert::AreEqual(active, pVariable->GetActiveFlag());
		Assert::AreEqual(value, pVariable->GetValueInt());
	}

	static void TestLoopStart()
	{
		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("FOR A 2:7", 15), commandResult);

		ValidateVariable(executionContext, &parseErrors, "A", 1, 2);

		AreEqual(CommandResultStatus::CommandSkipToNext, commandResult.GetStatus());

		Assert::AreEqual(2, executionContext._stack.GetFrameCount());
		Assert::AreEqual(15, executionContext._stack.GetTopFrame()->SerialNumberStart);
	}

	static void TestLoopEnd()
	{
		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("ENDFOR", 15), commandResult);

		AreEqual(CommandResultStatus::CommandEndOfLoop, commandResult.GetStatus());
	}

	static void TestLoopStep()
	{
		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("FOR A 2:7", 15), commandResult);

		ValidateVariable(executionContext, &parseErrors, "A", 1, 2);
		
		AreEqual(CommandResultStatus::CommandSkipToNext, commandResult.GetStatus());

		Assert::AreEqual(2, executionContext._stack.GetFrameCount());
		Assert::AreEqual(15, executionContext._stack.GetTopFrame()->SerialNumberStart);

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("FOR A 2:7", 15), commandResult);

		ValidateVariable(executionContext, &parseErrors, "A", 1, 3);

		AreEqual(CommandResultStatus::CommandSkipToNext, commandResult.GetStatus());
	}

	static void TestLoopEndDetection()
	{
		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("FOR A 2:2", 15), commandResult);

		ValidateVariable(executionContext, &parseErrors, "A", 1, 2);

		AreEqual(CommandResultStatus::CommandSkipToNext, commandResult.GetStatus());

		Assert::AreEqual(2, executionContext._stack.GetFrameCount());
		Assert::AreEqual(15, executionContext._stack.GetTopFrame()->SerialNumberStart);

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("FOR A 2:2", 15), commandResult);

		Variable* pVariable = executionContext._variables.GetWithoutErrorCheck("A");
		Assert::AreEqual(0, (int) pVariable);

		AreEqual(CommandResultStatus::CommandExitLoopBody, commandResult.GetStatus());
	}

	static void TestAssignmentConstant()
	{
		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("Var = 20", 15), commandResult);

		ValidateVariable(executionContext, &parseErrors, "Var", 1, 20);
	}

	static void TestAssignmentVariable()
	{
		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("Var = 20", 15), commandResult);

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("Second = Var", 15), commandResult);
		ValidateVariable(executionContext, &parseErrors, "Var", 1, 20);
		ValidateVariable(executionContext, &parseErrors, "Second", 1, 20);
	}

	static void TestAssignmentOverride()
	{
		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("Var = 20", 15), commandResult);

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("Var = 33", 15), commandResult);
		ValidateVariable(executionContext, &parseErrors, "Var", 1, 33);
	}

	static void TestAssignmentNoSpaces()
	{
		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("Var=20", 15), commandResult);

		ValidateVariable(executionContext, &parseErrors, "Var", 1, 20);
	}

	static void TestAnimateConstant()
	{
		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("A(50)", 15), commandResult);

		Assert::AreEqual(50, commandResult.GetCycleCount());
	}

	static void TestRandomAssignment()
	{
		MyRandom::SetFirstValue(1);

		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("Var = R(0:10)", 15), commandResult);
		ValidateVariable(executionContext, &parseErrors, "Var", 1, 1);

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("Var = R(0:10)", 15), commandResult);
		ValidateVariable(executionContext, &parseErrors, "Var", 1, 3);
	}

	static void TestRandomUsage()
	{
		ExecutionContext executionContext;
		executionContext._variables.AddAndSet("B", 1.0F);

		CommandResult commandResult;
		ParseErrors parseErrors;

		MyRandom::SetFirstValue(1);
		CommandDecoder::Decode(executionContext, &parseErrors, &Command("D(1,R(0:16),B)", 15), commandResult);

		Assert::AreEqual(1, commandResult.GetCount());

		AssertLedState(commandResult.GetTarget(0), 1, 1.0, 1);
		AreEqual(CommandResultStatus::CommandNone, commandResult.GetStatus());

		CommandResult commandResult2;
		CommandDecoder::Decode(executionContext, &parseErrors, &Command("D(1,R(0:16),B)", 15), commandResult2);

		Assert::AreEqual(1, commandResult2.GetCount());

		AssertLedState(commandResult2.GetTarget(0), 3, 1.0, 1);
		AreEqual(CommandResultStatus::CommandNone, commandResult2.GetStatus());
	}

	static void TestPrintString()
	{
		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		Serial.SetOutput(false);
		CommandDecoder::Decode(executionContext, &parseErrors, &Command("P(\"Hello\")", 0), commandResult);
		Serial.SetOutput(true);

		Assert::AreEqual("Hello", Serial.GetLastString());
	}

	static void TestPrintStringWithEquals()
	{
		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		Serial.SetOutput(false);
		CommandDecoder::Decode(executionContext, &parseErrors, &Command("P(\"Varname = \")", 0), commandResult);
		Serial.SetOutput(true);

		Assert::AreEqual("Varname = ", Serial.GetLastString());
	}

	static void TestPrintVariable()
	{
		ExecutionContext executionContext;
		executionContext._variables.AddAndSet("Variable", 1.0F);

		CommandResult commandResult;
		ParseErrors parseErrors;

		Serial.SetOutput(false);
		CommandDecoder::Decode(executionContext, &parseErrors, &Command("P(Variable)", 0), commandResult);
		Serial.SetOutput(true);

		Assert::AreEqual("1.000000", Serial.GetLastString());
	}

	static void TestPrintStringWithNewline()
	{
		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		Serial.SetOutput(false);
		CommandDecoder::Decode(executionContext, &parseErrors, &Command("PL(\"Hello\")", 0), commandResult);
		Serial.SetOutput(true);

		Assert::AreEqual("Hello\n", Serial.GetLastString());
	}

	static void TestBlankLine()
	{
		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("", 0), commandResult);
		Assert::AreEqual(0, parseErrors.GetErrorCount());
	}

	static void TestWhitespaceOnly()
	{
		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("      ", 0), commandResult);
		Assert::AreEqual(0, parseErrors.GetErrorCount());
	}

	static void TestComment()
	{
		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("// comment", 0), commandResult);
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
		CommandResult commandResult;
		ParseErrors parseErrors;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("DX(", 0), commandResult);
		ValidateError(executionContext, &parseErrors, 1, "Invalid D command: expected I or (", 0);

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("D(", 1), commandResult);
		ValidateError(executionContext, &parseErrors, 2, "Invalid D command: expected cycle count after (", 1);

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("D(55, 1)", 2), commandResult);
		ValidateError(executionContext, &parseErrors, 3, "Invalid D command: missing brightness target", 2);

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("D(MissingVar, 5, 1.0)", 3), commandResult);
		ValidateError(executionContext, &parseErrors, 4, "Undeclared variable: MissingVar", 3);

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("D33", 4), commandResult);
		ValidateError(executionContext, &parseErrors, 5, "Invalid D command: expected I or (", 4);
	}

	static void TestErrorSequential()
	{
		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("SX(", 0), commandResult);
		ValidateError(executionContext, &parseErrors, 1, "Invalid S command: expected I or (", 0);

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("S(", 1), commandResult);
		ValidateError(executionContext, &parseErrors, 2, "Invalid S command: expected cycle count after (", 1);

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("S(MissingVar, 5, 1.0)", 3), commandResult);
		ValidateError(executionContext, &parseErrors, 3, "Undeclared variable: MissingVar", 3);

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("S", 4), commandResult);
		ValidateError(executionContext, &parseErrors, 4, "Invalid S command: expected I or (", 4);
	}

	static void TestErrorAnimate()
	{
		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("Axx(", 0), commandResult);
		ValidateError(executionContext, &parseErrors, 1, "Invalid A command: expected (", 0);

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("A(", 1), commandResult);
		ValidateError(executionContext, &parseErrors, 2, "Invalid A command: expected cycle count", 1);
	}

	static void TestUnrecognizedCommand()
	{
		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("FASDF", 0), commandResult);
		ValidateError(executionContext, &parseErrors, 1, "Unrecognized command: FASDF", 0);
	}

	static void TestUnrecognizedCommand2()
	{
		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("qqP(\"UP \")", 0), commandResult);
		ValidateError(executionContext, &parseErrors, 1, "Unrecognized command: FASDF", 0);
	}

	static void TestDefineFunction()
	{
		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		Serial.SetOutput(false);
		CommandDecoder::Decode(executionContext, &parseErrors, &Command("FUNC MyFunction", 15), commandResult);
		CommandDecoder::Decode(executionContext, &parseErrors, &Command("PL(\"Hello\")", 16), commandResult);
		CommandDecoder::Decode(executionContext, &parseErrors, &Command("ENDFUNC", 17), commandResult);
		Serial.SetOutput(true);

		Assert::AreEqual(1, executionContext._functionStore.GetCount());
		FunctionDefinition* pFunctionDefinition = executionContext._functionStore.Lookup("MyFunction");

		Assert::AreEqual(15, pFunctionDefinition->SerialNumberStart);
		Assert::AreEqual(17, pFunctionDefinition->SerialNumberEnd);
	}

	static void TestFunctionCall()
	{
		ExecutionContext executionContext;
		CommandResult commandResult;
		ParseErrors parseErrors;

		CommandDecoder::Decode(executionContext, &parseErrors, &Command("MyFunction()", 15), commandResult);

		//Assert:AreEqual(2, executionContext._stack.GetFrameCount());
	}

public:

	static int Run()
	{
		TestSingle();
		TestDouble();

		TestSequential();
		TestSequentialSeries();

		TestLoopStart();
		TestLoopEnd();
		TestLoopStep();
		TestLoopEndDetection();

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
		TestFunctionCall();

		return 0;
	}
};