#pragma once

#include "LedManager.h"

class LedPwmSimulator : public ILedPwm
{
	int _updateCount;
	int _maxUpdates;
	LedState* _pUpdatedStates;

public:
	LedPwmSimulator(int maxUpdates)
	{
		_updateCount = 0;
		_maxUpdates = maxUpdates;
		_pUpdatedStates = new LedState[_maxUpdates];
	}

	void UpdateLed(LedState ledState)
	{
		if (_updateCount >= _maxUpdates)
		{
			puts("max updates exceeded");
			return;
		}

		*(_pUpdatedStates + _updateCount) = ledState;

		_updateCount++;
	}

	int GetUpdateCount() { return _updateCount; }

	LedState GetUpdatedState(int index)
	{
		return *(_pUpdatedStates + index);
	}
};

class LedManagerTest
{
	static void AssertLedState(LedState ledState, int channel, float brightness)
	{
		Assert::AreEqual(channel, ledState.GetChannel());
		Assert::AreEqual(brightness, ledState.GetBrightness());
	}

	static void Test()
	{
		LedPwmSimulator ledPwm(100);
		LedManager ledManager(&ledPwm, 1);

		CommandResult commandResult;
		commandResult.AddTarget(LedState(0, 1.0, 1));
		commandResult.SetCycleCount(1);

		ledManager.SetDelta(commandResult);
		ledManager.Tick();

		Assert::AreEqual(1, ledPwm.GetUpdateCount());
		AssertLedState(ledPwm.GetUpdatedState(0), 0, 1);
	}

	static void TestTwoChannelsTwoUpdates()
	{
		LedPwmSimulator ledPwm(100);

		LedManager ledManager(&ledPwm, 2);

		CommandResult commandResult;
		commandResult.AddTarget(LedState(0, 1.0, 1));
		commandResult.AddTarget(LedState(1, 2.0, 1));
		commandResult.SetCycleCount(1);

		ledManager.SetDelta(commandResult);

		ledManager.Tick();

		Assert::AreEqual(2, ledPwm.GetUpdateCount());
		AssertLedState(ledPwm.GetUpdatedState(0), 0, 1.0);
		AssertLedState(ledPwm.GetUpdatedState(1), 1, 2.0);

		ledManager.Tick();

		Assert::AreEqual(4, ledPwm.GetUpdateCount());
		AssertLedState(ledPwm.GetUpdatedState(0), 0, 1.0);
		AssertLedState(ledPwm.GetUpdatedState(1), 1, 2.0);
		AssertLedState(ledPwm.GetUpdatedState(2), 0, 1.0);
		AssertLedState(ledPwm.GetUpdatedState(3), 1, 2.0);
	}

	static void TestMultipleSteps()
	{
		LedPwmSimulator ledPwm(100);

		LedManager ledManager(&ledPwm, 1);

		CommandResult commandResult;
		commandResult.AddTarget(LedState(0, 20.0, 10));
		commandResult.SetCycleCount(10);

		ledManager.SetDelta(commandResult);

		for (int i = 1; i < 6; i++)
		{
			ledManager.Tick();
			Assert::AreEqual(i, ledPwm.GetUpdateCount());
			AssertLedState(ledPwm.GetUpdatedState(i - 1), 0, i * 2.0F);
		}
	}

	static void TestTwoChannelsDifferentUpdateRates()
	{
		LedPwmSimulator ledPwm(100);

		LedManager ledManager(&ledPwm, 2);

		CommandResult commandResult;
		commandResult.AddTarget(LedState(0, 10.0, 5));
		commandResult.AddTarget(LedState(1, 20.0, 20));
		commandResult.SetCycleCount(5);

		ledManager.SetDelta(commandResult);

		ledManager.Tick();

		Assert::AreEqual(2, ledPwm.GetUpdateCount());
		AssertLedState(ledPwm.GetUpdatedState(0), 0, 2.0);
		AssertLedState(ledPwm.GetUpdatedState(1), 1, 1.0);

		ledManager.Tick();

		Assert::AreEqual(4, ledPwm.GetUpdateCount());
		AssertLedState(ledPwm.GetUpdatedState(0), 0, 2.0);
		AssertLedState(ledPwm.GetUpdatedState(1), 1, 1.0);
		AssertLedState(ledPwm.GetUpdatedState(2), 0, 4.0);
		AssertLedState(ledPwm.GetUpdatedState(3), 1, 2.0);
	}

public:

	static int Run()
	{
		TestTwoChannelsDifferentUpdateRates();
		TestMultipleSteps();
		TestTwoChannelsTwoUpdates();
		Test();

		return 0;
	}
};
