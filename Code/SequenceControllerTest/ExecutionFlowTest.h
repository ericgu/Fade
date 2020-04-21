#pragma once

#include "ExecutionFlow.h"

#define ResultStorageSize 100

class LedMessageHandlerSimulator: public ILedMessageHandler
{
public:
	CommandResult _commandResults[ResultStorageSize];
	int _commandResultCount;
	char _ledType[128];
	int _ledCount;
	int _ledPin;

	LedMessageHandlerSimulator()
	{
		_commandResultCount = 0;
		_ledCount = 0;
		_ledPin = 0;
		_ledType[0] = 0;
	}

	void ExecuteLedCommandMember(CommandResult* pCommandResult)
	{
		_commandResults[_commandResultCount] = *pCommandResult;
		_commandResultCount++;
	}

	void Configure(const char* pLedType, int ledCount, int ledPin)
	{
		strcpy(_ledType, pLedType);
		_ledCount = ledCount;
		_ledPin = ledPin;
	}
};

class ExecutionFlowTest
{
	static void RunProgram(CommandSourceSimulator* pCommandSource, LedMessageHandlerSimulator* pLedMessageHandlerSimulator)
	{
		ParseErrors parseErrors;
		ExecutionFlow executionFlow(pCommandSource, &parseErrors, pLedMessageHandlerSimulator);

		executionFlow.RunProgram(1);
		Assert::AreEqual(0, parseErrors.GetErrorCount());
	}

	static void Test()
	{
		CommandSourceSimulator commandSource;
		ParseErrors parseErrors;

		commandSource.AddCommand("D(10,0,10.0)");
		commandSource.AddCommand("A(10)");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(1, ledMessageHandlerSimulator._commandResultCount);
		AssertResult(ledMessageHandlerSimulator._commandResults[0], 10, 0, 10.0F);
	}

	static void TestAutoRestart()
	{
		CommandSourceSimulator commandSource;
		ParseErrors parseErrors;
		LedMessageHandlerSimulator ledMessageHandlerSimulator;

		commandSource.AddCommand("DI(10,0,10.0)");

		ExecutionFlow executionFlow(&commandSource, &parseErrors, &ledMessageHandlerSimulator);


		executionFlow.RunProgram(2);

		Assert::AreEqual(2, ledMessageHandlerSimulator._commandResultCount);
		AssertResult(ledMessageHandlerSimulator._commandResults[0], 10, 0, 10.0F);
		AssertResult(ledMessageHandlerSimulator._commandResults[1], 10, 0, 10.0F);
	}

	static void TestFunctionDoubleRun()
	{
		// Validate that the function is skipped each time we execute the program. 

		CommandSourceSimulator commandSource;
		ParseErrors parseErrors;
		LedMessageHandlerSimulator ledMessageHandlerSimulator;

		commandSource.AddCommand("FUNC Test(a)");
		commandSource.AddCommand("DI(a,0,10.0)");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("Test(5)");

		ExecutionFlow executionFlow(&commandSource, &parseErrors, &ledMessageHandlerSimulator);

		executionFlow.RunProgram(2);

		Assert::AreEqual(0, parseErrors.GetErrorCount());
		Assert::AreEqual(2, ledMessageHandlerSimulator._commandResultCount);
		AssertResult(ledMessageHandlerSimulator._commandResults[0], 5, 0, 10.0F);
		AssertResult(ledMessageHandlerSimulator._commandResults[1], 5, 0, 10.0F);
	}

	static void AssertResult(CommandResult& commandResult, int cycleCount, int channel, float brightness)
	{
		Assert::AreEqual(cycleCount, commandResult.GetCycleCount());
		LedState ledState = commandResult.GetTarget(0);
		Assert::AreEqual(channel, ledState.GetChannel());
		Assert::AreEqual(brightness, ledState.GetBrightness()->GetValueFloat(0));
	}

	static void TestLoop()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FOR B 0:7");
		commandSource.AddCommand("D(7,B,10.0)");
		commandSource.AddCommand("A(7)");
		commandSource.AddCommand("ENDFOR");
		commandSource.AddCommand("D(15,15,15.0)");
		commandSource.AddCommand("A(15)");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(9, ledMessageHandlerSimulator._commandResultCount);

		for (int i = 0; i <= 7; i++)
		{
			AssertResult(ledMessageHandlerSimulator._commandResults[i], 7, i, 10.0F);
		}

		// should be command outside of loop
		AssertResult(ledMessageHandlerSimulator._commandResults[8], 15, 15, 15.0F);
	}

	static void TestNestedLoop()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FOR B 0:1");
		commandSource.AddCommand("FOR D 3:4");
		commandSource.AddCommand("D(7,B,D)");
		commandSource.AddCommand("A(7)");
		commandSource.AddCommand("ENDFOR");
		commandSource.AddCommand("ENDFOR");
		commandSource.AddCommand("D(15,15,15.0)");
		commandSource.AddCommand("A(15)");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(5, ledMessageHandlerSimulator._commandResultCount);

		AssertResult(ledMessageHandlerSimulator._commandResults[0], 7, 0, 3.0F);
		AssertResult(ledMessageHandlerSimulator._commandResults[1], 7, 0, 4.0F);
		AssertResult(ledMessageHandlerSimulator._commandResults[2], 7, 1, 3.0F);
		AssertResult(ledMessageHandlerSimulator._commandResults[3], 7, 1, 4.0F);

		// should be command outside of loop
		AssertResult(ledMessageHandlerSimulator._commandResults[4], 15, 15, 15.0F);
	}

	static void TestLoopWithIncrement()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FOR D 0:1:0.5");
		commandSource.AddCommand("D(7,1,D)");
		commandSource.AddCommand("A(7)");
		commandSource.AddCommand("ENDFOR");
		commandSource.AddCommand("D(15,15,15.0)");
		commandSource.AddCommand("A(15)");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(4, ledMessageHandlerSimulator._commandResultCount);

		AssertResult(ledMessageHandlerSimulator._commandResults[0], 7, 1, 0.0F);
		AssertResult(ledMessageHandlerSimulator._commandResults[1], 7, 1, 0.5F);
		AssertResult(ledMessageHandlerSimulator._commandResults[2], 7, 1, 1.0F);

		// should be command outside of loop
		AssertResult(ledMessageHandlerSimulator._commandResults[3], 15, 15, 15.0F);
	}

	static void TestLoopWithVariableCount()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FOR B 10:20:10");
		commandSource.AddCommand("D(B,0,10.0)");
		commandSource.AddCommand("A(B)");
		commandSource.AddCommand("ENDFOR");
		commandSource.AddCommand("D(15,15,15.0)");
		commandSource.AddCommand("A(15)");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(3, ledMessageHandlerSimulator._commandResultCount);

		AssertResult(ledMessageHandlerSimulator._commandResults[0], 10, 0, 10.0F);
		AssertResult(ledMessageHandlerSimulator._commandResults[1], 20, 0, 10.0F);

		// should be command outside of loop
		AssertResult(ledMessageHandlerSimulator._commandResults[2], 15, 15, 15.0F);
	}

	static void TestLoopWithFunctionCall()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FUNC GetValue");
		commandSource.AddCommand("RETURN 7");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("FOR B 0:GetValue()");
		commandSource.AddCommand("D(7,B,10.0)");
		commandSource.AddCommand("A(7)");
		commandSource.AddCommand("ENDFOR");
		commandSource.AddCommand("D(15,15,15.0)");
		commandSource.AddCommand("A(15)");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(9, ledMessageHandlerSimulator._commandResultCount);

		for (int i = 0; i <= 7; i++)
		{
			AssertResult(ledMessageHandlerSimulator._commandResults[i], 7, i, 10.0F);
		}

		// should be command outside of loop
		AssertResult(ledMessageHandlerSimulator._commandResults[8], 15, 15, 15.0F);
	}


	static void TestLoopDown()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FOR B 6:0:-3");
		commandSource.AddCommand("D(7,B,10.0)");
		commandSource.AddCommand("A(7)");
		commandSource.AddCommand("ENDFOR");
		commandSource.AddCommand("D(15,15,15.0)");
		commandSource.AddCommand("A(15)");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(4, ledMessageHandlerSimulator._commandResultCount);

		AssertResult(ledMessageHandlerSimulator._commandResults[0], 7, 6, 10.0F);
		AssertResult(ledMessageHandlerSimulator._commandResults[1], 7, 3, 10.0F);
		AssertResult(ledMessageHandlerSimulator._commandResults[2], 7, 0, 10.0F);

		// should be command outside of loop
		AssertResult(ledMessageHandlerSimulator._commandResults[3], 15, 15, 15.0F);
	}

	static void TestLoopDownWithVariables()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("Start=6");
		commandSource.AddCommand("End=0");
		commandSource.AddCommand("Inc=-3");
		commandSource.AddCommand("FOR B Start:End:Inc");
		commandSource.AddCommand("D(7,B,10.0)");
		commandSource.AddCommand("A(7)");
		commandSource.AddCommand("ENDFOR");
		commandSource.AddCommand("D(15,15,15.0)");
		commandSource.AddCommand("A(15)");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(4, ledMessageHandlerSimulator._commandResultCount);

		AssertResult(ledMessageHandlerSimulator._commandResults[0], 7, 6, 10.0F);
		AssertResult(ledMessageHandlerSimulator._commandResults[1], 7, 3, 10.0F);
		AssertResult(ledMessageHandlerSimulator._commandResults[2], 7, 0, 10.0F);

		// should be command outside of loop
		AssertResult(ledMessageHandlerSimulator._commandResults[3], 15, 15, 15.0F);
	}

	static void TestLoopImmediate()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FOR B 6:0:-3");
		commandSource.AddCommand("DI(7,B,10.0)");
		commandSource.AddCommand("ENDFOR");
		commandSource.AddCommand("DI(15,15,15.0)");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(4, ledMessageHandlerSimulator._commandResultCount);

		AssertResult(ledMessageHandlerSimulator._commandResults[0], 7, 6, 10.0F);
		AssertResult(ledMessageHandlerSimulator._commandResults[1], 7, 3, 10.0F);
		AssertResult(ledMessageHandlerSimulator._commandResults[2], 7, 0, 10.0F);

		// should be command outside of loop
		AssertResult(ledMessageHandlerSimulator._commandResults[3], 15, 15, 15.0F);
	}

	static void TestNestedLoopWithLongNames()
	{
		CommandSourceSimulator commandSource;
		LedMessageHandlerSimulator ledMessageHandlerSimulator;

		commandSource.AddCommand("FOR Bravo 0:1");
		commandSource.AddCommand("FOR Delta 3:4");
		commandSource.AddCommand("D(7,Bravo,Delta)");
		commandSource.AddCommand("A(7)");
		commandSource.AddCommand("ENDFOR");
		commandSource.AddCommand("ENDFOR");
		commandSource.AddCommand("D(15,15,15.0)");
		commandSource.AddCommand("A(15)");

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors, &ledMessageHandlerSimulator);
	}

	static void TestOverlappingAnimation()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("D(10,1,1.0)");
		commandSource.AddCommand("A(5)");
		commandSource.AddCommand("D(5,2,1.0)");
		commandSource.AddCommand("A(5)");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(2, ledMessageHandlerSimulator._commandResultCount);

		AssertResult(ledMessageHandlerSimulator._commandResults[0], 5, 1, 1.0F);
		AssertResult(ledMessageHandlerSimulator._commandResults[1], 5, 2, 1.0F);
	}

	static void TestMissingAnimation()
	{
		CommandSourceSimulator commandSource;
		commandSource.AddCommand("D(10,1,1.0)");

		ParseErrors parseErrors;
		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		ExecutionFlow executionFlow(&commandSource, &parseErrors, &ledMessageHandlerSimulator);


		CommandResultStatus commandResultStatus = executionFlow.RunProgram(1000);

		Assert::AreEqual((int) CommandResultStatus::CommandTargetCountExceeded, (int) commandResultStatus);
	}

	static void ValidateParseErrors(ParseErrors *pParseErrors, const char* pMessage, int lineNumber)
	{
		Assert::AreEqual(1, pParseErrors->GetErrorCount());
		ParseError* pParseError = pParseErrors->GetError(0);
		Assert::AreEqual(pMessage, pParseError->_errorText);
		Assert::AreEqual(lineNumber, pParseError->_lineNumber);
	}

	static void TestMissingEndFor()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FOR B 0:7");

		ParseErrors parseErrors;
		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		ExecutionFlow executionFlow(&commandSource, &parseErrors, &ledMessageHandlerSimulator);


		executionFlow.RunProgram(1);

		ValidateParseErrors(&parseErrors, "Missing loop end", -1);
}

	static void TestInvalidStatement()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("guar");

		ParseErrors parseErrors;
		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		ExecutionFlow executionFlow(&commandSource, &parseErrors, &ledMessageHandlerSimulator);

 		executionFlow.RunProgram(1);

		ValidateParseErrors(&parseErrors, "Undefined variable: guar", 0);
	}

	static void TestDoubleLoop()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FOR A 0:6");
		commandSource.AddCommand("ENDFOR");
		commandSource.AddCommand("FOR A 7:1:-1");
		commandSource.AddCommand("ENDFOR");
		commandSource.AddCommand("A(1)");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(1, ledMessageHandlerSimulator._commandResultCount);
	}

	static void TestPrint()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("PL(\"Hello\")");
		commandSource.AddCommand("DI(1, 2, 1.0)");

		ParseErrors parseErrors;
		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		ExecutionFlow executionFlow(&commandSource, &parseErrors, &ledMessageHandlerSimulator);

		Serial.SetOutput(false);
		executionFlow.RunProgram(1);
		Serial.SetOutput(true);

		Assert::AreEqual("Hello\n", Serial.GetLastString());
	}

	static void TestFunctionDef()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FUNC Function");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("DI(1, 2, 1.0)");

		ParseErrors parseErrors;
		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		ExecutionFlow executionFlow(&commandSource, &parseErrors, &ledMessageHandlerSimulator);

		executionFlow.RunProgram(1);
		Assert::AreEqual(1, executionFlow.GetExecutionContext()->_functionStore.GetCount());
		FunctionDefinition* pFunction = executionFlow.GetExecutionContext()->_functionStore.Lookup("Function");
		Assert::AreEqual("Function", pFunction->Name);
		Assert::AreEqual(0, pFunction->LineNumberStart);

		ExecutionContext* pExecutionContext = executionFlow.GetExecutionContext();
	}

	static void TestFunctionCall()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FUNC Function()");
		commandSource.AddCommand("RETURN 10.0");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("A=Function()");
		commandSource.AddCommand("DI(A, 2, 1.0)");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(1, ledMessageHandlerSimulator._commandResultCount);

		AssertResult(ledMessageHandlerSimulator._commandResults[0], 10, 2, 1.0F);
	}

	static void TestFunctionCallMultiValueReturn()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FUNC Function()");
		commandSource.AddCommand("RETURN {1, 2, 3}");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("A=Function()");
		commandSource.AddCommand("DI(1, 2, A)");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(1, ledMessageHandlerSimulator._commandResultCount);
		Variable* pBrightness = ledMessageHandlerSimulator._commandResults[0].GetTarget(0).GetBrightness();
		Assert::AreEqual(1.0F, pBrightness->GetValueFloat(0));
		Assert::AreEqual(2.0F, pBrightness->GetValueFloat(1));
		Assert::AreEqual(3.0F, pBrightness->GetValueFloat(2));
	}


	static void TestFunctionCallNested()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FUNC Function()");
		commandSource.AddCommand("RETURN 11.0");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("FUNC FunctionOuter");
		commandSource.AddCommand("A=Function()");
		commandSource.AddCommand("RETURN A");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("B=FunctionOuter()");
		commandSource.AddCommand("DI(B, 2, 1.0)");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(1, ledMessageHandlerSimulator._commandResultCount);

		AssertResult(ledMessageHandlerSimulator._commandResults[0], 11, 2, 1.0F);
	}

	static void TestFunctionCallAsArgument()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FUNC Function(A, B)");
		commandSource.AddCommand("RETURN A");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("FUNC Function2(X, Y, Z)");
		commandSource.AddCommand("RETURN X + Y + Z");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("B=Function2(3, Function(1, 3), 6)");
		commandSource.AddCommand("DI(B, 2, 1.0)");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(1, ledMessageHandlerSimulator._commandResultCount);

		AssertResult(ledMessageHandlerSimulator._commandResults[0], 10, 2, 1.0F);
	}

	static void TestFunctionCallRandomParameter()
	{
		CommandSourceSimulator commandSource;

		MyRandom::SetFirstValue(13);

		commandSource.AddCommand("FUNC Function(A)");
		commandSource.AddCommand("RETURN A");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("B=Function(RAND(0,10))");
		commandSource.AddCommand("DI(B, 2, 1.0)");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(1, ledMessageHandlerSimulator._commandResultCount);

		AssertResult(ledMessageHandlerSimulator._commandResults[0], 13, 2, 1.0F);
	}

	static void TestIgnoredFunctionResult()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FUNC Function");
		commandSource.AddCommand("RETURN 10.0");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("Function()");
		commandSource.AddCommand("DI(5, 2, 1.0)");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(1, ledMessageHandlerSimulator._commandResultCount);

		AssertResult(ledMessageHandlerSimulator._commandResults[0], 5, 2, 1.0F);
	}

	static void TestMethodCall()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FUNC Function");
		commandSource.AddCommand("PL(\"Hello\")");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("Function()");
		commandSource.AddCommand("DI(1, 2, 1.0)");

		Serial.SetOutput(false);
		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);
		Serial.SetOutput(true);

		Assert::AreEqual("Hello\n", Serial.GetLastString());
	}

	static void TestMethodCallWithParameter()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FUNC Function(V)");
		commandSource.AddCommand("PL(V)");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("Function(13)");

		Serial.SetOutput(false);
		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);
		Serial.SetOutput(true);

		Assert::AreEqual("13.000000\n", Serial.GetLastString());
	}

	static void TestMethodCallWithTwoParameters()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FUNC Function(V, W)"); // ignore leading whitespace... - add it to the ListParser 
		commandSource.AddCommand("PL(W)");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("Function(1, 12)");

		Serial.SetOutput(false);
		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);
		Serial.SetOutput(true);

		Assert::AreEqual("12.000000\n", Serial.GetLastString());
	}

	static void TestMethodCallWithParameterDuplicateName()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("V=15.0");
		commandSource.AddCommand("FUNC Function(V)");
		commandSource.AddCommand("PL(V)");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("Function(13)");

		ParseErrors parseErrors;
		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		ExecutionFlow executionFlow(&commandSource, &parseErrors, &ledMessageHandlerSimulator);

		Serial.SetOutput(false);
		executionFlow.RunProgram(1);
		Serial.SetOutput(true);

		Assert::AreEqual("13.000000\n", Serial.GetLastString());
		Assert::AreEqual(15.0F, executionFlow.GetExecutionContext()->GetVariableWithoutErrorCheck("V")->GetValueFloat(0));
	}

	static void TestMethodCannotAccessParentVariables()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("V=15.0");
		commandSource.AddCommand("FUNC Function()");
		commandSource.AddCommand("PL(V)");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("Function()");

		ParseErrors parseErrors;
		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		ExecutionFlow executionFlow(&commandSource, &parseErrors, &ledMessageHandlerSimulator);

		Serial.SetOutput(false);
		executionFlow.RunProgram(1);
		Serial.SetOutput(true);

		ValidateParseErrors(&parseErrors, "Undefined variable: V", 2);
	}

	static void TestMethodCallWithWrongArgumentCount()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FUNC Function(X)");
		commandSource.AddCommand("PL(X)");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("Function()");

		ParseErrors parseErrors;
		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		ExecutionFlow executionFlow(&commandSource, &parseErrors, &ledMessageHandlerSimulator);

		Serial.SetOutput(false);
		executionFlow.RunProgram(1);
		Serial.SetOutput(true);

		ValidateParseErrors(&parseErrors, "Mismatched Function Function requires 1 parameter(s) but was called with 0 argument(s).", 3);
	}

	static void TestMethodCallWithWrongArgumentCount2()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FUNC Function(X)");
		commandSource.AddCommand("PL(X)");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("Function(15.0, 35.0)");

		ParseErrors parseErrors;
		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		ExecutionFlow executionFlow(&commandSource, &parseErrors, &ledMessageHandlerSimulator);

		Serial.SetOutput(false);
		executionFlow.RunProgram(1);
		Serial.SetOutput(true);

		ValidateParseErrors(&parseErrors, "Mismatched Function Function requires 1 parameter(s) but was called with 2 argument(s).", 3);
	}

	static void TestUndefinedFunctionWithVariableParameter()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("V=15.0");
		commandSource.AddCommand("PR(V)");

		ParseErrors parseErrors;
		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		ExecutionFlow executionFlow(&commandSource, &parseErrors, &ledMessageHandlerSimulator);

		Serial.SetOutput(false);
		executionFlow.RunProgram(1);
		Serial.SetOutput(true);

		ValidateParseErrors(&parseErrors, "Unrecognized function: PR", 1);
	}

	static void TestDirectWithFunctionCall()
	{
		CommandSourceSimulator commandSource;
		ParseErrors parseErrors;

		commandSource.AddCommand("FUNC F(X)");
		commandSource.AddCommand("RETURN X");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("DI(F(1), F(2), F(3))");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(1, ledMessageHandlerSimulator._commandResultCount);
		AssertResult(ledMessageHandlerSimulator._commandResults[0], 1, 2, 3.0F);
	}

	static void TestFunctionCallWithMultipleReturnStatements()
	{
		CommandSourceSimulator commandSource;
		ParseErrors parseErrors;

		commandSource.AddCommand("FUNC F(X)");
		commandSource.AddCommand("  IF X==1");
		commandSource.AddCommand("    RETURN 13");
		commandSource.AddCommand("  ELSE");
		commandSource.AddCommand("     RETURN 25");
		commandSource.AddCommand("  ENDIF");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("P(F(1))");

		Serial.SetOutput(false);
		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);
		Serial.SetOutput(true);

		Assert::AreEqual("13.000000", Serial.GetLastString());
	}

	static void TestFunctionCallUsedInVector()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FUNC Function(red, green, blue)"); 
		commandSource.AddCommand("PL({red})");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("Function(1, 2, 3)");

		Serial.SetOutput(false);
		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);
		Serial.SetOutput(true);

		Assert::AreEqual("1.000000\n", Serial.GetLastString());
	}

	static void TestIfTrue()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("Value=3");
		commandSource.AddCommand("  IF Value==3");
		commandSource.AddCommand("DI(7,7,10.0)");
		commandSource.AddCommand("  ENDIF");
		commandSource.AddCommand("DI(15,15,15.0)");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(2, ledMessageHandlerSimulator._commandResultCount);

		AssertResult(ledMessageHandlerSimulator._commandResults[0], 7, 7, 10.0F);
		AssertResult(ledMessageHandlerSimulator._commandResults[1], 15, 15, 15.0F);
	}

	static void TestIfFalse()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("Value=2");
		commandSource.AddCommand("IF Value==3");
		commandSource.AddCommand("DI(7,7,10.0)");
		commandSource.AddCommand("ENDIF");
		commandSource.AddCommand("DI(15,15,15.0)");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(1, ledMessageHandlerSimulator._commandResultCount);

		AssertResult(ledMessageHandlerSimulator._commandResults[0], 15, 15, 15.0F);
	}

	static void TestIfElseTrue()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("Value=3");
		commandSource.AddCommand("IF Value==3");
		commandSource.AddCommand("DI(7,7,10.0)");
		commandSource.AddCommand("ELSE");
		commandSource.AddCommand("DI(15,15,15.0)");
		commandSource.AddCommand("ENDIF");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(1, ledMessageHandlerSimulator._commandResultCount);

		AssertResult(ledMessageHandlerSimulator._commandResults[0], 7, 7, 10.0F);
	}

	static void TestIfElseFalse()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("Value=2");
		commandSource.AddCommand("IF Value>=3");
		commandSource.AddCommand("DI(7,7,10.0)");
		commandSource.AddCommand("ELSE");
		commandSource.AddCommand("DI(15,15,15.0)");
		commandSource.AddCommand("ENDIF");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(1, ledMessageHandlerSimulator._commandResultCount);

		AssertResult(ledMessageHandlerSimulator._commandResults[0], 15, 15, 15.0F);
	}

	static void TestIfElseIfElseOne()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("Value=3");
		commandSource.AddCommand("IF Value==3");
		commandSource.AddCommand("DI(7,7,10.0)");
		commandSource.AddCommand("ELSEIF Value==4");
		commandSource.AddCommand("DI(10,10,10.0)");
		commandSource.AddCommand("ELSE");
		commandSource.AddCommand("DI(15,15,15.0)");
		commandSource.AddCommand("ENDIF");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(1, ledMessageHandlerSimulator._commandResultCount);

		AssertResult(ledMessageHandlerSimulator._commandResults[0], 7, 7, 10.0F);
	}

	static void TestIfElseIfElseTwo()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("Value=4");
		commandSource.AddCommand("IF Value==3");
		commandSource.AddCommand("DI(7,7,10.0)");
		commandSource.AddCommand("ELSEIF Value==4");
		commandSource.AddCommand("DI(10,10,10.0)");
		commandSource.AddCommand("ELSE");
		commandSource.AddCommand("DI(15,15,15.0)");
		commandSource.AddCommand("ENDIF");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(1, ledMessageHandlerSimulator._commandResultCount);

		AssertResult(ledMessageHandlerSimulator._commandResults[0], 10, 10, 10.0F);
	}

	static void TestIfElseIfElseThree()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("Value=5");
		commandSource.AddCommand("IF Value==3");
		commandSource.AddCommand("DI(7,7,10.0)");
		commandSource.AddCommand("ELSEIF Value==4");
		commandSource.AddCommand("DI(10,10,10.0)");
		commandSource.AddCommand("ELSE");
		commandSource.AddCommand("DI(15,15,15.0)");
		commandSource.AddCommand("ENDIF");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(1, ledMessageHandlerSimulator._commandResultCount);

		AssertResult(ledMessageHandlerSimulator._commandResults[0], 15, 15, 15.0F);
	}

	static void AddCommands(CommandSourceSimulator& commandSource)
	{

		commandSource.AddCommand("IF A==3");
		commandSource.AddCommand("  IF B==3");
		commandSource.AddCommand("    DI(1,1,1.0)");
		commandSource.AddCommand("  ELSE");
		commandSource.AddCommand("    DI(2,2,2.0)");
		commandSource.AddCommand("  ENDIF");
		commandSource.AddCommand("ELSE");
		commandSource.AddCommand("  IF B==3");
		commandSource.AddCommand("    DI(3,3,3.0)");
		commandSource.AddCommand("  ELSE");
		commandSource.AddCommand("    DI(4,4,4.0)");
		commandSource.AddCommand("  ENDIF");
		commandSource.AddCommand("ENDIF");
	}

	static void TestNestedIfElse1()
	{
		CommandSourceSimulator commandSource;
		commandSource.AddCommand("A=3");
		commandSource.AddCommand("B=3");
		AddCommands(commandSource);

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(1, ledMessageHandlerSimulator._commandResultCount);

		AssertResult(ledMessageHandlerSimulator._commandResults[0], 1, 1, 1.0F);
	}

	static void TestNestedIfElse2()
	{
		CommandSourceSimulator commandSource;
		commandSource.AddCommand("A=3");
		commandSource.AddCommand("B=4");
		AddCommands(commandSource);

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(1, ledMessageHandlerSimulator._commandResultCount);

		AssertResult(ledMessageHandlerSimulator._commandResults[0], 2, 2, 2.0F);
	}

	static void TestNestedIfElse3()
	{
		CommandSourceSimulator commandSource;
		commandSource.AddCommand("A=1");
		commandSource.AddCommand("B=3");
		AddCommands(commandSource);

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(1, ledMessageHandlerSimulator._commandResultCount);

		AssertResult(ledMessageHandlerSimulator._commandResults[0], 3, 3, 3.0F);
	}

	static void TestNestedIfElse4()
	{
		CommandSourceSimulator commandSource;
		commandSource.AddCommand("A=1");
		commandSource.AddCommand("B=2");
		AddCommands(commandSource);

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(1, ledMessageHandlerSimulator._commandResultCount);

		AssertResult(ledMessageHandlerSimulator._commandResults[0], 4, 4, 4.0F);
	}

	static void TestIncrement()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("Value=3");
		commandSource.AddCommand("Value++");
		commandSource.AddCommand("DI(Value, Value, Value)");
		commandSource.AddCommand("Value++");
		commandSource.AddCommand("DI(Value, Value, Value)");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(2, ledMessageHandlerSimulator._commandResultCount);

		AssertResult(ledMessageHandlerSimulator._commandResults[0], 4, 4, 4.0F);
		AssertResult(ledMessageHandlerSimulator._commandResults[1], 5, 5, 5.0F);
	}

	static void TestDecrement()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("Value=3");
		commandSource.AddCommand("Value--");
		commandSource.AddCommand("DI(Value, Value, Value)");
		commandSource.AddCommand("Value--");
		commandSource.AddCommand("DI(Value, Value, Value)");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(2, ledMessageHandlerSimulator._commandResultCount);

		AssertResult(ledMessageHandlerSimulator._commandResults[0], 2, 2, 2.0F);
		AssertResult(ledMessageHandlerSimulator._commandResults[1], 1, 1, 1.0F);
	}

	static void TestIncrementReference()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("Value=3");
		commandSource.AddCommand("Test=Value++");
		commandSource.AddCommand("DI(Test, Test, Test)");
		commandSource.AddCommand("DI(Value, Value, Value)");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(2, ledMessageHandlerSimulator._commandResultCount);

		AssertResult(ledMessageHandlerSimulator._commandResults[0], 3, 3, 3.0F);
		AssertResult(ledMessageHandlerSimulator._commandResults[1], 4, 4, 4.0F);
	}

	static void TestDirectWithLists()
	{
		CommandSourceSimulator commandSource;
		ParseErrors parseErrors;

		commandSource.AddCommand("DI(10, 15, {0.5, 1.0, 0.4})");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(1, ledMessageHandlerSimulator._commandResultCount);
		LedState ledState = ledMessageHandlerSimulator._commandResults[0].GetTarget(0);
		Assert::AreEqual(15, ledState.GetChannel());
		Assert::AreEqual(0.5F, ledState.GetBrightness()->GetValueFloat(0));
		Assert::AreEqual(1.0F, ledState.GetBrightness()->GetValueFloat(1));
		Assert::AreEqual(0.4F, ledState.GetBrightness()->GetValueFloat(2));
	}

	static void TestFunctionWithVectorUse()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FUNC Scan(red, green, blue)");
		commandSource.AddCommand("	FOR channel 0:6");
		commandSource.AddCommand("	DI(1, channel, { red, green, blue })");
		commandSource.AddCommand("	D(50, channel, { 0, 0, 0 })");
		commandSource.AddCommand("	A(12)");
		commandSource.AddCommand("	ENDFOR");
		commandSource.AddCommand("	FOR channel 7 : 1 : -1");
		commandSource.AddCommand("	DI(1, channel, { red, green, blue })");
		commandSource.AddCommand("	D(50, channel, { 0, 0, 0 })");
		commandSource.AddCommand("	A(12)");
		commandSource.AddCommand("	ENDFOR");
		commandSource.AddCommand("	ENDFUNC");
		commandSource.AddCommand("	Scan(1, 0, 0)");

		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);

		Assert::AreEqual(28, ledMessageHandlerSimulator._commandResultCount);
	}

	static void TestAssignmentOfFunctionVectorReturn()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FUNC AngleToRGB(angleInDegrees)");
		commandSource.AddCommand("	RETURN { 1, 0, 2 }");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("FOR test 0:1");
		commandSource.AddCommand("  rgb = AngleToRGB(1)");
		commandSource.AddCommand("  PL(rgb)");
		commandSource.AddCommand("ENDFOR");

		Serial.SetOutput(0);
		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);
		Serial.SetOutput(1);

		Assert::AreEqual("{1.000000, 0.000000, 2.000000}\n", Serial.GetLastString());
	}


	static void TestAbort()
	{
		CommandSourceSimulator commandSource;
		ParseErrors parseErrors;
		LedMessageHandlerSimulator ledMessageHandlerSimulator;

		commandSource.AddCommand("ABORT()");
		commandSource.AddCommand("DI(a,0,10.0)");

		ExecutionFlow executionFlow(&commandSource, &parseErrors, &ledMessageHandlerSimulator);


		CommandResultStatus status = executionFlow.RunProgram(1);

		Assert::AreEqual(true, executionFlow.IsAborting());
	}

	static void TestAbortInForLoop()
	{
		CommandSourceSimulator commandSource;
		ParseErrors parseErrors;
		LedMessageHandlerSimulator ledMessageHandlerSimulator;

		commandSource.AddCommand("FOR Test 0:1");
		commandSource.AddCommand("ABORT()");
		commandSource.AddCommand("DI(a,0,10.0)");
		commandSource.AddCommand("ENDFOR");

		ExecutionFlow executionFlow(&commandSource, &parseErrors, &ledMessageHandlerSimulator);


		CommandResultStatus status = executionFlow.RunProgram(1);

		Assert::AreEqual(true, executionFlow.IsAborting());
	}

	static void TestAbortInFunctionCall()
	{
		CommandSourceSimulator commandSource;
		ParseErrors parseErrors;
		LedMessageHandlerSimulator ledMessageHandlerSimulator;

		commandSource.AddCommand("FOR Test 0:1");
		commandSource.AddCommand("ABORT()");
		commandSource.AddCommand("DI(a,0,10.0)");
		commandSource.AddCommand("ENDFOR");

		ExecutionFlow executionFlow(&commandSource, &parseErrors, &ledMessageHandlerSimulator);


		CommandResultStatus status = executionFlow.RunProgram(1);

		Assert::AreEqual(true, executionFlow.IsAborting());
	}

	static void TestConfigLed()
	{
		CommandSourceSimulator commandSource;
		ParseErrors parseErrors;
		LedMessageHandlerSimulator ledMessageHandlerSimulator;

		commandSource.AddCommand("CONFIGLED(\"RGB\", 33, 13)");

		ExecutionFlow executionFlow(&commandSource, &parseErrors, &ledMessageHandlerSimulator);

		CommandResultStatus status = executionFlow.RunProgram(1);

		Assert::AreEqual("RGB", ledMessageHandlerSimulator._ledType);
		Assert::AreEqual(33, ledMessageHandlerSimulator._ledCount);
		Assert::AreEqual(13, ledMessageHandlerSimulator._ledPin);
		Assert::AreEqual(0, parseErrors.GetErrorCount());
	}


	static void TestAssignmentOfVarIntoVector()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("angleInDegrees = 0");
		commandSource.AddCommand("temp = angleInDegrees / 120");
		commandSource.AddCommand("value = { temp, 55, 1 - temp }");
		commandSource.AddCommand("PL(value)");

		Serial.SetOutput(0);
		LedMessageHandlerSimulator ledMessageHandlerSimulator;
		RunProgram(&commandSource, &ledMessageHandlerSimulator);
		Serial.SetOutput(1);

		Assert::AreEqual("{0.000000, 55.000000, 1.000000}\n", Serial.GetLastString());
	}

public:
	static int Run()
	{
		//TestFunctionDoubleRun();

#if fred

		TestDirectWithLists();

		Test();
		TestLoop();
		TestLoopDown();
		TestLoopDownWithVariables();
		TestNestedLoop();
		TestLoopWithIncrement();
		TestLoopWithVariableCount();
		TestNestedLoopWithLongNames();

		TestOverlappingAnimation();
		TestMissingAnimation();

		TestLoopImmediate();

		TestAutoRestart();
		TestMissingEndFor();
		TestInvalidStatement();

		TestDoubleLoop();
		TestPrint();
#endif

#if fred
        TestMethodCallWithParameter();
		TestMethodCallWithTwoParameters();
		TestFunctionDef();
		TestFunctionCall();
		TestFunctionCallNested();
		TestFunctionCallAsArgument();
		TestFunctionCallRandomParameter();
		TestMethodCall();
		TestIgnoredFunctionResult();
		TestMethodCallWithParameterDuplicateName();
		TestMethodCannotAccessParentVariables();
		TestMethodCallWithWrongArgumentCount();
		TestMethodCallWithWrongArgumentCount2();
		TestFunctionCallUsedInVector();
		TestFunctionWithVectorUse();
		TestFunctionCallMultiValueReturn();
		TestFunctionCallWithMultipleReturnStatements();
		TestUndefinedFunctionWithVariableParameter();
		TestAssignmentOfFunctionVectorReturn();
		TestAssignmentOfVarIntoVector();

		TestLoopWithFunctionCall();
		TestDirectWithFunctionCall();

		TestNestedIfElse1();
		TestNestedIfElse2();
		TestNestedIfElse3();
		TestNestedIfElse4();

		TestIfElseTrue();
		TestIfElseFalse();
		TestIfFalse();
		TestIfTrue();

		TestIfElseIfElseOne();
		TestIfElseIfElseTwo();
		TestIfElseIfElseThree();

		TestIncrement();
		TestDecrement();
		TestIncrementReference();

		TestAbort();
		TestAbortInForLoop();
		TestAbortInFunctionCall();

		TestConfigLed();
#endif
		return 0;
	}
};
