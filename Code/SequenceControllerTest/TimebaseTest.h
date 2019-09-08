#pragma once

#include "Timebase.h"



class LedManagerSimulator : public ILedManager
{
	BrightnessTarget _brightnessTarget;
	int _steps;
	int _tickCount = 0;

public:
	void SetDelta(BrightnessTarget brightnessTarget, int steps)
	{
		_brightnessTarget = brightnessTarget;
		_steps = steps;
	}

	void Tick()
	{
		_tickCount++;
	}

	BrightnessTarget GetBrightnessTarget()
	{
		return _brightnessTarget;
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

		commandSource.AddCommand(Command(10, "D0,10.0", 0));

		Timebase timebase(&commandSource, &ledManager);

		for (int i = 0; i < 10; i++)
		{
			timebase.DoTick();
		}

		BrightnessTarget brightnessTarget = ledManager.GetBrightnessTarget();
		Assert::AreEqual(1, brightnessTarget.GetCount());
		LedState ledState = brightnessTarget.GetTarget(0);
		Assert::AreEqual(0, ledState.GetChannel());
		Assert::AreEqual(10.0F, ledState.GetBrightness());

		Assert::AreEqual(10, ledManager.GetTickCount());

		commandSource.AddCommand(Command(2, "Test", 1));

		timebase.DoTick();
	}

	static void TestLoop()
	{
		CommandSourceSimulator commandSource;
		LedManagerSimulator ledManager;

		commandSource.AddCommand(Command(1, "LOOP %B 0:7", 0));
		commandSource.AddCommand(Command(1, "D%B,10.0", 1));
		commandSource.AddCommand(Command(1, "ENDLOOP", 2));

		Timebase timebase(&commandSource, &ledManager);

		timebase.DoTick();

		BrightnessTarget brightnessTarget = ledManager.GetBrightnessTarget();
		Assert::AreEqual(1, brightnessTarget.GetCount());
		LedState ledState = brightnessTarget.GetTarget(0);
		Assert::AreEqual(0, ledState.GetChannel());
		Assert::AreEqual(10.0F, ledState.GetBrightness());

		timebase.DoTick();

		brightnessTarget = ledManager.GetBrightnessTarget();
		Assert::AreEqual(1, brightnessTarget.GetCount());
		ledState = brightnessTarget.GetTarget(0);
		Assert::AreEqual(0, ledState.GetChannel());
		Assert::AreEqual(10.0F, ledState.GetBrightness());

		Assert::AreEqual(10, ledManager.GetTickCount());
	}

public:

	static int Run()
	{
		Test();
		// TestLoop();

		return 0;
	}
};
