#pragma once

#include "ExecutionFlow.h"

class ExecutionFlowTest
{
	static void Test()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand(Command(10, "D0,10.0", 0));

		ExecutionFlow executionFlow(&commandSource);

		LedCommand ledCommand = executionFlow.GetNextLedCommand();

		Assert::AreEqual(10, ledCommand._cycleCount);
		LedState ledState = ledCommand._brightnessTarget.GetTarget(0);
		Assert::AreEqual(0, ledState.GetChannel());
		Assert::AreEqual(10.0F, ledState.GetBrightness());
	}

	static void TestLoop()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand(Command(1, "LOOP %B 0:7", 0));
		commandSource.AddCommand(Command(7, "D%B,10.0", 1));
		commandSource.AddCommand(Command(1, "ENDLOOP", 2));
		commandSource.AddCommand(Command(15, "D15, 15.0", 3));

		ExecutionFlow executionFlow(&commandSource);

		LedCommand ledCommand = executionFlow.GetNextLedCommand();

		Assert::AreEqual(7, ledCommand._cycleCount);
		LedState ledState = ledCommand._brightnessTarget.GetTarget(0);
		Assert::AreEqual(0, ledState.GetChannel());
		Assert::AreEqual(10.0F, ledState.GetBrightness());

		ledCommand = executionFlow.GetNextLedCommand(); 

		Assert::AreEqual(7, ledCommand._cycleCount);
		ledState = ledCommand._brightnessTarget.GetTarget(0);
		Assert::AreEqual(1, ledState.GetChannel());
		Assert::AreEqual(10.0F, ledState.GetBrightness());

		for (int i = 2; i <= 7; i++)
		{
			ledCommand = executionFlow.GetNextLedCommand();
			ledState = ledCommand._brightnessTarget.GetTarget(0);
			Assert::AreEqual(i, ledState.GetChannel());
		}

		// should be command outside of loop
		ledCommand = executionFlow.GetNextLedCommand();

		Assert::AreEqual(15, ledCommand._cycleCount);
		ledState = ledCommand._brightnessTarget.GetTarget(0);
		Assert::AreEqual(15, ledState.GetChannel());
		Assert::AreEqual(15.0F, ledState.GetBrightness());

	

	}

public:

	static int Run()
	{
		Test();
		TestLoop();

		return 0;
	}
};
