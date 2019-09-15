#pragma once

#include "Timebase.h"

class LedManagerSimulator : public ILedManager
{
	CommandResult _commandResult;
	int _tickCount = 0;

public:
	void SetDelta(CommandResult commandResult)
	{
		_commandResult = commandResult;
	}

	void Tick()
	{
		_tickCount++;
	}

	CommandResult GetCommandResult()
	{
		return _commandResult;
	}

	int GetTickCount()
	{
		return _tickCount;
	}
};

class TimebaseTest
{
	static void Test()
	{
		CommandSourceSimulator commandSource;
		LedManagerSimulator ledManager;

		commandSource.AddCommand(Command("D 10 0,10.0", 0));

		Timebase timebase(&commandSource, &ledManager);

		for (int i = 0; i < 10; i++)
		{
			timebase.DoTick();
		}

		Assert::AreEqual(10, ledManager.GetCommandResult().GetCycleCount());

		CommandResult commandResult = ledManager.GetCommandResult();
		Assert::AreEqual(1, commandResult.GetCount());
		LedState ledState = commandResult.GetTarget(0);
		Assert::AreEqual(0, ledState.GetChannel());
		Assert::AreEqual(10.0F, ledState.GetBrightness());

		Assert::AreEqual(10, ledManager.GetTickCount());

		commandSource.AddCommand(Command("Test", 1));

		timebase.DoTick();
	}

	static void TestLoop()
	{
		CommandSourceSimulator commandSource;
		LedManagerSimulator ledManager;

		commandSource.AddCommand(Command("LOOP %B 0:7", 0));
		commandSource.AddCommand(Command("D 1 %B,10.0", 1));
		commandSource.AddCommand(Command("ENDLOOP", 2));

		Timebase timebase(&commandSource, &ledManager);

		timebase.DoTick();

		CommandResult commandResult = ledManager.GetCommandResult();
		Assert::AreEqual(1, commandResult.GetCount());
		LedState ledState = commandResult.GetTarget(0);
		Assert::AreEqual(0, ledState.GetChannel());
		Assert::AreEqual(10.0F, ledState.GetBrightness());

		timebase.DoTick();

		commandResult = ledManager.GetCommandResult();
		Assert::AreEqual(1, commandResult.GetCount());
		ledState = commandResult.GetTarget(0);
		Assert::AreEqual(0, ledState.GetChannel());
		Assert::AreEqual(10.0F, ledState.GetBrightness());

		Assert::AreEqual(10, ledManager.GetTickCount());
	} 

	static void TestLoop2()
	{
		CommandSourceSimulator commandSource;
		LedManagerSimulator ledManager;

		//"$1$LOOP %A 0:7\n$100$D%A,1.0$100$D%A,0.0\n$1$ENDLOOP"
		commandSource.AddCommand(Command("LOOP %A 0:7", 0));
		commandSource.AddCommand(Command("D 100 %A,1.0", 1));
		commandSource.AddCommand(Command("D 100 %A,0.0", 2));
		commandSource.AddCommand(Command("ENDLOOP", 3));

		Timebase timebase(&commandSource, &ledManager);

		for (int i = 0; i < 200; i++)
		{
			timebase.DoTick();

			CommandResult commandResult = ledManager.GetCommandResult();
			LedState ledState = commandResult.GetTarget(0);

			//printf("%d %f\n", ledState.GetChannel(), ledState.GetBrightness());
		}
	}


public:

	static int Run()
	{
		Test();
		//TestLoop();
		TestLoop2();

		return 0;
	}
};
