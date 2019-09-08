#pragma once

#include "ExecutionContext.h"
#include "CommandDecoder.h"

class CommandDecoderTest
{
	static void AreEqual(CommandResult expected, CommandResult actual)
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
		CommandDecoder::Decode(executionContext, Command(0, "D0,1.0", 15));

		Assert::AreEqual(1, executionContext._brightnessTarget.GetCount());

		AssertLedState(executionContext._brightnessTarget.GetTarget(0), 0, 1.0);
		AreEqual(CommandResult::CommandExecute, executionContext._commandResult);
	}

	static void TestSingleWithVariables()
	{
		ExecutionContext executionContext;
		executionContext._variables.Get(0) = Variable(3);
		executionContext._variables.Get(1) = Variable(1.0F);

		CommandDecoder::Decode(executionContext, Command(0, "D%A,%B", 15));

		Assert::AreEqual(1, executionContext._brightnessTarget.GetCount());

		AssertLedState(executionContext._brightnessTarget.GetTarget(0), 3, 1.0);
		AreEqual(CommandResult::CommandExecute, executionContext._commandResult);
	}

	static void TestDouble()
	{
		ExecutionContext executionContext;
		CommandDecoder::Decode(executionContext, Command(0, "D2,2.0,10,8.0", 15));

		Assert::AreEqual(2, executionContext._brightnessTarget.GetCount());

		AssertLedState(executionContext._brightnessTarget.GetTarget(0), 2, 2.0);
		AssertLedState(executionContext._brightnessTarget.GetTarget(1), 10, 8.0);
		AreEqual(CommandResult::CommandExecute, executionContext._commandResult);
	}

	static void TestSequential()
	{
		ExecutionContext executionContext;
		CommandDecoder::Decode(executionContext, Command(0, "S1.0", 15));

		Assert::AreEqual(1, executionContext._brightnessTarget.GetCount());

		AssertLedState(executionContext._brightnessTarget.GetTarget(0), 0, 1.0);
		AreEqual(CommandResult::CommandExecute, executionContext._commandResult);
	}

	static void TestSequentialWithVariables()
	{
		ExecutionContext executionContext;
		executionContext._variables.Get(0) = Variable(3);
		executionContext._variables.Get(1) = Variable(13.33F);

		CommandDecoder::Decode(executionContext, Command(0, "S%B", 15));

		Assert::AreEqual(1, executionContext._brightnessTarget.GetCount());

		AssertLedState(executionContext._brightnessTarget.GetTarget(0), 0, 13.33F);
		AreEqual(CommandResult::CommandExecute, executionContext._commandResult);
	}

	static void TestSequentialSeries()
	{
		ExecutionContext executionContext;
		CommandDecoder::Decode(executionContext, Command(0, "S1.0,2.0,3.0,4.0,5.0,6.0", 15));

		Assert::AreEqual(6, executionContext._brightnessTarget.GetCount());

		for (int i = 0; i < 6; i++)
		{
			AssertLedState(executionContext._brightnessTarget.GetTarget(i), i, (i + 1) * 1.0F);
		}
		AreEqual(CommandResult::CommandExecute, executionContext._commandResult);
	}

	static void ValidateVariable(ExecutionContext executionContext, int variableNumber, int active, int value)
	{
		Variable variable = executionContext._variables.Get(variableNumber);
		Assert::AreEqual(active, variable.GetActiveFlag());
		Assert::AreEqual(value, variable.GetValueInt());
	}

	static void TestLoopStart()
	{
		ExecutionContext executionContext;
		CommandDecoder::Decode(executionContext, Command(0, "LOOP %A 2:7", 15));

		ValidateVariable(executionContext, 0, 1, 2);

		AreEqual(CommandResult::CommandSkipToNext, executionContext._commandResult);

		Assert::AreEqual(1, executionContext._stack.GetFrameCount());
		Assert::AreEqual(15, executionContext._stack.GetTopFrame().SerialNumberStart);
	}

	static void TestLoopEnd()
	{
		ExecutionContext executionContext;
		CommandDecoder::Decode(executionContext, Command(0, "ENDLOOP", 15));

		AreEqual(CommandResult::CommandEndOfLoop, executionContext._commandResult);
	}

	static void TestLoopStep()
	{
		ExecutionContext executionContext;
		CommandDecoder::Decode(executionContext, Command(0, "LOOP %A 2:7", 15));

		ValidateVariable(executionContext, 0, 1, 2);
		
		AreEqual(CommandResult::CommandSkipToNext, executionContext._commandResult);

		Assert::AreEqual(1, executionContext._stack.GetFrameCount());
		Assert::AreEqual(15, executionContext._stack.GetTopFrame().SerialNumberStart);

		CommandDecoder::Decode(executionContext, Command(0, "LOOP %A 2:7", 15));

		ValidateVariable(executionContext, 0, 1, 3);

		AreEqual(CommandResult::CommandSkipToNext, executionContext._commandResult);
	}

	static void TestLoopEndDetection()
	{
		ExecutionContext executionContext;
		CommandDecoder::Decode(executionContext, Command(0, "LOOP %A 2:2", 15));

		ValidateVariable(executionContext, 0, 1, 2);

		AreEqual(CommandResult::CommandSkipToNext, executionContext._commandResult);

		Assert::AreEqual(1, executionContext._stack.GetFrameCount());
		Assert::AreEqual(15, executionContext._stack.GetTopFrame().SerialNumberStart);

		CommandDecoder::Decode(executionContext, Command(0, "LOOP %A 2:2", 15));

		ValidateVariable(executionContext, 0, 0, 3);

		AreEqual(CommandResult::CommandExitLoopBody, executionContext._commandResult);
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