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
			Assert::Failed(buffer);
		}
	}

	static void AssertLedState(LedState ledState, int channel, float brightness)
	{
		Assert::AreEqual(channel, ledState.GetChannel());
		Assert::AreEqual(brightness, ledState.GetBrightness());
	}

	static void TestSingle()
	{
		ExecutionContext executionContext;
		CommandResult commandResult = CommandDecoder::Decode(executionContext, Command("D 1 0,1.0", 15));

		Assert::AreEqual(1, commandResult.GetCount());

		AssertLedState(commandResult.GetTarget(0), 0, 1.0);
		AreEqual(CommandResultStatus::CommandExecute, commandResult.GetStatus());
	}

	static void TestSingleWithVariables()
	{
		ExecutionContext executionContext;
		executionContext._variables.AddAndSet("A", 3);
		executionContext._variables.AddAndSet("B", 1.0F);

		CommandResult commandResult = CommandDecoder::Decode(executionContext, Command("D 1 A,B", 15));

		Assert::AreEqual(1, commandResult.GetCount());

		AssertLedState(commandResult.GetTarget(0), 3, 1.0);
		AreEqual(CommandResultStatus::CommandExecute, commandResult.GetStatus());
	}

	static void TestDouble()
	{
		ExecutionContext executionContext;
		CommandResult commandResult = CommandDecoder::Decode(executionContext, Command("D 1 2,2.0,10,8.0", 15));

		Assert::AreEqual(2, commandResult.GetCount());

		AssertLedState(commandResult.GetTarget(0), 2, 2.0);
		AssertLedState(commandResult.GetTarget(1), 10, 8.0);
		AreEqual(CommandResultStatus::CommandExecute, commandResult.GetStatus());
	}

	static void TestSequential()
	{
		ExecutionContext executionContext;
		CommandResult commandResult = CommandDecoder::Decode(executionContext, Command("S 1 1.0", 15));

		Assert::AreEqual(1, commandResult.GetCount());

		AssertLedState(commandResult.GetTarget(0), 0, 1.0);
		AreEqual(CommandResultStatus::CommandExecute, commandResult.GetStatus());
	}

	static void TestSequentialWithVariables()
	{
		ExecutionContext executionContext;
		executionContext._variables.AddAndSet("A", 3);
		executionContext._variables.AddAndSet("B", 13.33F);

		CommandResult commandResult = CommandDecoder::Decode(executionContext, Command("S 1 B", 15));

		Assert::AreEqual(1, commandResult.GetCount());

		AssertLedState(commandResult.GetTarget(0), 0, 13.33F);
		AreEqual(CommandResultStatus::CommandExecute, commandResult.GetStatus());
	}

	static void TestSequentialSeries()
	{
		ExecutionContext executionContext;
		CommandResult commandResult = CommandDecoder::Decode(executionContext, Command("S 1 1.0,2.0,3.0,4.0,5.0,6.0", 15));

		Assert::AreEqual(6, commandResult.GetCount());

		for (int i = 0; i < 6; i++)
		{
			AssertLedState(commandResult.GetTarget(i), i, (i + 1) * 1.0F);
		}
		AreEqual(CommandResultStatus::CommandExecute, commandResult.GetStatus());
	}

	static void ValidateVariable(ExecutionContext executionContext, const char *pVariableName, int active, int value)
	{
		Variable variable = executionContext._variables.Get(pVariableName);
		Assert::AreEqual(active, variable.GetActiveFlag());
		Assert::AreEqual(value, variable.GetValueInt());
	}

	static void TestLoopStart()
	{
		ExecutionContext executionContext;
		CommandResult commandResult = CommandDecoder::Decode(executionContext, Command("LOOP A 2:7", 15));

		ValidateVariable(executionContext, "A", 1, 2);

		AreEqual(CommandResultStatus::CommandSkipToNext, commandResult.GetStatus());

		Assert::AreEqual(1, executionContext._stack.GetFrameCount());
		Assert::AreEqual(15, executionContext._stack.GetTopFrame().SerialNumberStart);
	}

	static void TestLoopEnd()
	{
		ExecutionContext executionContext;
		CommandResult commandResult = CommandDecoder::Decode(executionContext, Command("ENDLOOP", 15));

		AreEqual(CommandResultStatus::CommandEndOfLoop, commandResult.GetStatus());
	}

	static void TestLoopStep()
	{
		ExecutionContext executionContext;
		CommandResult commandResult = CommandDecoder::Decode(executionContext, Command("LOOP A 2:7", 15));

		ValidateVariable(executionContext, "A", 1, 2);
		
		AreEqual(CommandResultStatus::CommandSkipToNext, commandResult.GetStatus());

		Assert::AreEqual(1, executionContext._stack.GetFrameCount());
		Assert::AreEqual(15, executionContext._stack.GetTopFrame().SerialNumberStart);

		commandResult = CommandDecoder::Decode(executionContext, Command("LOOP A 2:7", 15));

		ValidateVariable(executionContext, "A", 1, 3);

		AreEqual(CommandResultStatus::CommandSkipToNext, commandResult.GetStatus());
	}

	static void TestLoopEndDetection()
	{
		ExecutionContext executionContext;
		CommandResult commandResult = CommandDecoder::Decode(executionContext, Command("LOOP A 2:2", 15));

		ValidateVariable(executionContext, "A", 1, 2);

		AreEqual(CommandResultStatus::CommandSkipToNext, commandResult.GetStatus());

		Assert::AreEqual(1, executionContext._stack.GetFrameCount());
		Assert::AreEqual(15, executionContext._stack.GetTopFrame().SerialNumberStart);

		commandResult = CommandDecoder::Decode(executionContext, Command("LOOP A 2:2", 15));

		ValidateVariable(executionContext, "A", 0, 3);

		AreEqual(CommandResultStatus::CommandExitLoopBody, commandResult.GetStatus());
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

		return 0;
	}
};