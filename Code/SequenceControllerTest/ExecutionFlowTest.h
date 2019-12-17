#pragma once

#include "ExecutionFlow.h"

class ExecutionFlowTest
{
	static void Test()
	{
		CommandSourceSimulator commandSource;
		ParseErrors parseErrors;

		commandSource.AddCommand(Command("D(10,0,10.0)", 0));
		commandSource.AddCommand(Command("A(10)", 0));

		ExecutionFlow executionFlow(&commandSource, &parseErrors);

		AssertResult(executionFlow.GetNextLedCommand(), 10, 0, 10.0F);
	}

	static void TestAutoRestart()
	{
		CommandSourceSimulator commandSource;
		ParseErrors parseErrors;

		commandSource.AddCommand(Command("DI(10,0,10.0)", 0));

		ExecutionFlow executionFlow(&commandSource, &parseErrors);

		AssertResult(executionFlow.GetNextLedCommand(), 10, 0, 10.0F);
		AssertResult(executionFlow.GetNextLedCommand(), 10, 0, 10.0F);
	}

	static void AssertResult(LedCommand ledCommand, int cycleCount, int channel, float brightness)
	{
		Assert::AreEqual(cycleCount, ledCommand._commandResult.GetCycleCount());
		LedState ledState = ledCommand._commandResult.GetTarget(0);
		Assert::AreEqual(channel, ledState.GetChannel());
		Assert::AreEqual(brightness, ledState.GetBrightness());
	}

	static void TestLoop()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand(Command("FOR B 0:7", 0));
		commandSource.AddCommand(Command("D(7,B,10.0)", 1));
		commandSource.AddCommand(Command("A(7)", 0));
		commandSource.AddCommand(Command("ENDFOR", 2));
		commandSource.AddCommand(Command("D(15,15,15.0)", 3));
		commandSource.AddCommand(Command("A(15)", 0));

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors);

		AssertResult(executionFlow.GetNextLedCommand(), 7, 0, 10.0F);

		AssertResult(executionFlow.GetNextLedCommand(), 7, 1, 10.0F);

		for (int i = 2; i <= 7; i++)
		{
			AssertResult(executionFlow.GetNextLedCommand(), 7, i, 10.0F);
		}

		// should be command outside of loop
		AssertResult(executionFlow.GetNextLedCommand(), 15, 15, 15.0F);
	}

	static void TestNestedLoop()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand(Command("FOR B 0:1", 0));
		commandSource.AddCommand(Command("FOR D 3:4", 1));
		commandSource.AddCommand(Command("D(7,B,D)", 2));
		commandSource.AddCommand(Command("A(7)", 3));
		commandSource.AddCommand(Command("ENDFOR", 4));
		commandSource.AddCommand(Command("ENDFOR", 5));
		commandSource.AddCommand(Command("D(15,15,15.0)", 6));
		commandSource.AddCommand(Command("A(15)", 7));

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors);

		AssertResult(executionFlow.GetNextLedCommand(), 7, 0, 3.0F);

		AssertResult(executionFlow.GetNextLedCommand(), 7, 0, 4.0F);

		AssertResult(executionFlow.GetNextLedCommand(), 7, 1, 3.0F);

		AssertResult(executionFlow.GetNextLedCommand(), 7, 1, 4.0F);

		// should be command outside of loop
		AssertResult(executionFlow.GetNextLedCommand(), 15, 15, 15.0F);
	}

	static void TestLoopWithIncrement()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand(Command("FOR D 0:1:0.5", 0));
		commandSource.AddCommand(Command("D(7,1,D)", 2));
		commandSource.AddCommand(Command("A(7)", 0));
		commandSource.AddCommand(Command("ENDFOR", 3));
		commandSource.AddCommand(Command("D(15,15,15.0)", 4));
		commandSource.AddCommand(Command("A(15)", 0));

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors);

		AssertResult(executionFlow.GetNextLedCommand(), 7, 1, 0.0F);

		AssertResult(executionFlow.GetNextLedCommand(), 7, 1, 0.5F);

		AssertResult(executionFlow.GetNextLedCommand(), 7, 1, 1.0F);

		// should be command outside of loop
		AssertResult(executionFlow.GetNextLedCommand(), 15, 15, 15.0F);
	}

	static void TestLoopWithVariableCount()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand(Command("FOR B 10:20:10", 0));
		commandSource.AddCommand(Command("D(B,0,10.0)", 1));
		commandSource.AddCommand(Command("A(B)", 0));
		commandSource.AddCommand(Command("ENDFOR", 2));
		commandSource.AddCommand(Command("D(15,15,15.0)", 3));
		commandSource.AddCommand(Command("A(15)", 0));

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors);

		AssertResult(executionFlow.GetNextLedCommand(), 10, 0, 10.0F);
		AssertResult(executionFlow.GetNextLedCommand(), 20, 0, 10.0F);

		// should be command outside of loop
		AssertResult(executionFlow.GetNextLedCommand(), 15, 15, 15.0F);
	}

	static void TestLoopDown()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand(Command("FOR B 6:0:-3", 0));
		commandSource.AddCommand(Command("D(7,B,10.0)", 1));
		commandSource.AddCommand(Command("A(7)", 2));
		commandSource.AddCommand(Command("ENDFOR", 3));
		commandSource.AddCommand(Command("D(15,15,15.0)", 4));
		commandSource.AddCommand(Command("A(15)", 5));

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors);

		AssertResult(executionFlow.GetNextLedCommand(), 7, 6, 10.0F);
		AssertResult(executionFlow.GetNextLedCommand(), 7, 3, 10.0F);
		AssertResult(executionFlow.GetNextLedCommand(), 7, 0, 10.0F);

		AssertResult(executionFlow.GetNextLedCommand(), 15, 15, 15.0F);
	}

	static void TestLoopDownWithVariables()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand(Command("Start=6", 0));
		commandSource.AddCommand(Command("End=0", 1));
		commandSource.AddCommand(Command("Inc=-3", 2));
		commandSource.AddCommand(Command("FOR B Start:End:Inc", 3));
		commandSource.AddCommand(Command("D(7,B,10.0)", 4));
		commandSource.AddCommand(Command("A(7)", 5));
		commandSource.AddCommand(Command("ENDFOR", 6));
		commandSource.AddCommand(Command("D(15,15,15.0)", 7));
		commandSource.AddCommand(Command("A(15)", 8));

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors);

		AssertResult(executionFlow.GetNextLedCommand(), 7, 6, 10.0F);
		AssertResult(executionFlow.GetNextLedCommand(), 7, 3, 10.0F);
		AssertResult(executionFlow.GetNextLedCommand(), 7, 0, 10.0F);

		AssertResult(executionFlow.GetNextLedCommand(), 15, 15, 15.0F);
	}

	static void TestLoopImmediate()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand(Command("FOR B 6:0:-3", 0));
		commandSource.AddCommand(Command("DI(7,B,10.0)", 1));
		commandSource.AddCommand(Command("ENDFOR", 2));
		commandSource.AddCommand(Command("DI(15,15,15.0)", 3));

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors);

		AssertResult(executionFlow.GetNextLedCommand(), 7, 6, 10.0F);
		AssertResult(executionFlow.GetNextLedCommand(), 7, 3, 10.0F);
		AssertResult(executionFlow.GetNextLedCommand(), 7, 0, 10.0F);

		AssertResult(executionFlow.GetNextLedCommand(), 15, 15, 15.0F);
	}

	static void TestNestedLoopWithLongNames()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand(Command("FOR Bravo 0:1", 0));
		commandSource.AddCommand(Command("FOR Delta 3:4", 1));
		commandSource.AddCommand(Command("D(7,Bravo,Delta)", 2));
		commandSource.AddCommand(Command("A(7)", 2));
		commandSource.AddCommand(Command("ENDFOR", 3));
		commandSource.AddCommand(Command("ENDFOR", 4));
		commandSource.AddCommand(Command("D(15,15,15.0)", 5));
		commandSource.AddCommand(Command("A(15)", 2));

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors);

		AssertResult(executionFlow.GetNextLedCommand(), 7, 0, 3.0F);

		AssertResult(executionFlow.GetNextLedCommand(), 7, 0, 4.0F);

		AssertResult(executionFlow.GetNextLedCommand(), 7, 1, 3.0F);

		AssertResult(executionFlow.GetNextLedCommand(), 7, 1, 4.0F);

		// should be command outside of loop
		AssertResult(executionFlow.GetNextLedCommand(), 15, 15, 15.0F);
	}

	static void TestOverlappingAnimation()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand(Command("D(10,1,1.0)", 0));
		commandSource.AddCommand(Command("A(5)", 1));
		commandSource.AddCommand(Command("D(5,2,1.0)", 2));
		commandSource.AddCommand(Command("A(5)", 3));

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors);

		LedCommand ledCommand = executionFlow.GetNextLedCommand();

		Assert::AreEqual(1, ledCommand._commandResult.GetCount());
		LedState ledState = ledCommand._commandResult.GetTarget(0);
		Assert::AreEqual(1.0, ledState.GetBrightness());
		Assert::AreEqual(1, ledState.GetChannel());
		Assert::AreEqual(10, ledState.GetCycleCount());

		ledCommand = executionFlow.GetNextLedCommand();

		Assert::AreEqual(1, ledCommand._commandResult.GetCount());
		ledState = ledCommand._commandResult.GetTarget(0);
		Assert::AreEqual(1.0, ledState.GetBrightness());
		Assert::AreEqual(2, ledState.GetChannel());
		Assert::AreEqual(5, ledState.GetCycleCount());
	}

	static void TestMissingAnimation()
	{
		CommandSourceSimulator commandSource;
		commandSource.AddCommand(Command("D(10,1,1.0)", 0));

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors);

		LedCommand ledCommand = executionFlow.GetNextLedCommand();

		Assert::AreEqual((int) CommandResultStatus::CommandTargetCountExceeded, (int) ledCommand._commandResult.GetStatus());
	}

	static void TestMissingEndFor()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand(Command("FOR B 0:7", 0));

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors);

		executionFlow.GetNextLedCommand();

		Assert::AreEqual(1, parseErrors.GetErrorCount());
		ParseError parseError = parseErrors.GetError(0);
	}

	static void TestInvalidStatement()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand(Command("guar", 0));

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors);

		executionFlow.GetNextLedCommand();

		Assert::AreEqual(1, parseErrors.GetErrorCount());
		ParseError parseError = parseErrors.GetError(0);
		Assert::AreEqual("Unrecognized command: guar", parseError._errorText);
	}

	static void TestDoubleLoop()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand(Command("FOR A 0:6", 0));
		commandSource.AddCommand(Command("ENDFOR", 1));
		commandSource.AddCommand(Command("FOR A 7:1:-1", 2));
		commandSource.AddCommand(Command("ENDFOR", 3));
		commandSource.AddCommand(Command("A(1)", 4));

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors);

		executionFlow.GetNextLedCommand();
	}

	static void TestCommandReset()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand(Command("FOR A 0:6", 0));
		commandSource.AddCommand(Command("ENDFOR", 1));
		commandSource.AddCommand(Command("FOR A 7:1:-1", 2));
		commandSource.AddCommand(Command("ENDFOR", 3));
		commandSource.AddCommand(Command("A(1)", 4));

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors);

		executionFlow.GetNextLedCommand();
		//executionFlow

		//ExecutionContext executionContext = executionFlow.GetExecutionContext();
		//Assert::Are
	}

	static void TestPrint()
	{
		CommandSourceSimulator commandSource;

		Command printCommand = Command("PL(\"Hello\")", 0);
		commandSource.AddCommand(printCommand);
		commandSource.AddCommand(Command("DI(1, 2, 1.0)", 2));

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors);

		Serial.SetOutput(false);
		executionFlow.GetNextLedCommand();
		Serial.SetOutput(true);

		Assert::AreEqual("Hello\n", Serial.GetLastString());
	}

	static void TestFunctionDef()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand(Command("FUNC Function", 0));
		commandSource.AddCommand(Command("ENDFUNC", 1));
		commandSource.AddCommand(Command("DI(1, 2, 1.0)", 2));

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors);

		executionFlow.GetNextLedCommand();
		Assert::AreEqual(1, executionFlow.GetExecutionContext()._functionStore.GetCount());
		FunctionDefinition* pFunction = executionFlow.GetExecutionContext()._functionStore.Lookup("Function");
		Assert::AreEqual("Function", pFunction->Name);
		Assert::AreEqual(0, pFunction->SerialNumberStart);
		Assert::AreEqual(1, pFunction->SerialNumberEnd);

		ExecutionContext executionContext = executionFlow.GetExecutionContext();
	}

	static void TestFunctionCall()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand(Command("FUNC Function", 0));
		commandSource.AddCommand(Command("PL(\"Hello\")", 1));
		commandSource.AddCommand(Command("ENDFUNC", 2));
		commandSource.AddCommand(Command("A=Function()", 4));
		commandSource.AddCommand(Command("DI(1, 2, 1.0)", 4));

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors);

		Serial.SetOutput(false);
		executionFlow.GetNextLedCommand();
		Serial.SetOutput(true);

		Assert::AreEqual("Hello\n", Serial.GetLastString());
	}

	static void TestMethodCall()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand(Command("FUNC Function", 0));
		commandSource.AddCommand(Command("PL(\"Hello\")", 1));
		commandSource.AddCommand(Command("ENDFUNC", 2));
		commandSource.AddCommand(Command("Function()", 4));
		commandSource.AddCommand(Command("DI(1, 2, 1.0)", 4));

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors);

		Serial.SetOutput(false);
		executionFlow.GetNextLedCommand();
		Serial.SetOutput(true);

		Assert::AreEqual("Hello\n", Serial.GetLastString());
	}

public:

	static int Run()
	{
		TestLoopDown();
		TestLoopDownWithVariables();

		Test();
		TestLoop();
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
		TestCommandReset();
		TestPrint();

		TestFunctionDef();
		TestFunctionCall();
		TestMethodCall();

		return 0;
	}
};
