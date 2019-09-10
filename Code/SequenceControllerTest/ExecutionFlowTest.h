#pragma once

#include "ExecutionFlow.h"

class ExecutionFlowTest
{
	static void Test()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand(Command("10", "D0,10.0", 0));

		ExecutionFlow executionFlow(&commandSource);

		AssertResult(executionFlow.GetNextLedCommand(), 10, 0, 10.0F);
	}

	static void AssertResult(LedCommand ledCommand, int cycleCount, int channel, float brightness)
	{
		Assert::AreEqual(cycleCount, ledCommand._cycleCount);
		LedState ledState = ledCommand._commandResult.GetTarget(0);
		Assert::AreEqual(channel, ledState.GetChannel());
		Assert::AreEqual(brightness, ledState.GetBrightness());
	}

	static void TestLoop()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand(Command("1", "LOOP %B 0:7", 0));
		commandSource.AddCommand(Command("7", "D%B,10.0", 1));
		commandSource.AddCommand(Command("1", "ENDLOOP", 2));
		commandSource.AddCommand(Command("15", "D15, 15.0", 3));

		ExecutionFlow executionFlow(&commandSource);

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

		commandSource.AddCommand(Command("1", "LOOP %B 0:1", 0));
		commandSource.AddCommand(Command("2", "LOOP %D 3:4", 1));
		commandSource.AddCommand(Command("7", "D%B,%D", 2));
		commandSource.AddCommand(Command("1", "ENDLOOP", 3));
		commandSource.AddCommand(Command("1", "ENDLOOP", 4));
		commandSource.AddCommand(Command("15", "D15, 15.0", 5));

		ExecutionFlow executionFlow(&commandSource);

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

		commandSource.AddCommand(Command("1", "LOOP %D 0:1:0.5", 0));
		commandSource.AddCommand(Command("7", "D1,%D", 2));
		commandSource.AddCommand(Command("1", "ENDLOOP", 3));
		commandSource.AddCommand(Command("15", "D15, 15.0", 4));

		ExecutionFlow executionFlow(&commandSource);

		AssertResult(executionFlow.GetNextLedCommand(), 7, 1, 0.0F);

		AssertResult(executionFlow.GetNextLedCommand(), 7, 1, 0.5F);

		AssertResult(executionFlow.GetNextLedCommand(), 7, 1, 1.0F);

		// should be command outside of loop
		AssertResult(executionFlow.GetNextLedCommand(), 15, 15, 15.0F);
	}

	static void TestLoopWithVariableCount()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand(Command("1", "LOOP %B 10:20:10", 0));
		commandSource.AddCommand(Command("%B", "D0,10.0", 1));
		commandSource.AddCommand(Command("1", "ENDLOOP", 2));
		commandSource.AddCommand(Command("15", "D15, 15.0", 3));

		ExecutionFlow executionFlow(&commandSource);

		AssertResult(executionFlow.GetNextLedCommand(), 10, 0, 10.0F);
		AssertResult(executionFlow.GetNextLedCommand(), 20, 0, 10.0F);

		// should be command outside of loop
		AssertResult(executionFlow.GetNextLedCommand(), 15, 15, 15.0F);
	}

	static void TestLoopDown()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand(Command("1", "LOOP %B 6:0:-3", 0));
		commandSource.AddCommand(Command("7", "D%B,10.0", 1));
		commandSource.AddCommand(Command("1", "ENDLOOP", 2));
		commandSource.AddCommand(Command("15", "D15, 15.0", 3));

		ExecutionFlow executionFlow(&commandSource);

		AssertResult(executionFlow.GetNextLedCommand(), 7, 6, 10.0F);
		AssertResult(executionFlow.GetNextLedCommand(), 7, 3, 10.0F);
		AssertResult(executionFlow.GetNextLedCommand(), 7, 0, 10.0F);

		AssertResult(executionFlow.GetNextLedCommand(), 15, 15, 15.0F);
	}


public:

	static int Run()
	{
		TestLoopDown();

		Test();
		TestLoop();
		TestNestedLoop();
		TestLoopWithIncrement();
		TestLoopWithVariableCount();

		return 0;
	}
};
