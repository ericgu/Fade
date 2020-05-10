#pragma once

#include "LedManager.h"

class LedDeviceCreatorSimulator : public ILedDeviceCreator
{
	ILedDevice* _pLedDevice;

public:
	LedDeviceCreatorSimulator(ILedDevice* pLedDevice)
	{
		_pLedDevice = pLedDevice;
	}

	ILedDevice* Create(const char* pLedType, int ledCount, int pin)
	{
		return _pLedDevice;
	}
};

class LedDeviceSimulator : public ILedDevice
{
	int _updateCount;
	int _maxUpdates;
	LedState* _pUpdatedStates;

public:
	LedDeviceSimulator(int maxUpdates)
	{
		_updateCount = 0;
		_maxUpdates = maxUpdates;
		_pUpdatedStates = new LedState[_maxUpdates];
	}

	~LedDeviceSimulator()
	{
		delete [] _pUpdatedStates;
		_pUpdatedStates = 0;
	}

	void UpdateLed(LedState ledState)
	{
		if (_updateCount >= _maxUpdates)
		{
			Serial.println("max updates exceeded");
			return;
		}

		*(_pUpdatedStates + _updateCount) = ledState;

		_updateCount++;
	}

	void Show()
	{

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
		Assert::AreEqual(brightness, ledState.GetBrightness()->GetValueFloat(0));
	}

	static void AssertLedState(LedState ledState, int channel, float brightness0, float brightness1, float brightness2)
	{
		Assert::AreEqual(channel, ledState.GetChannel());
		Assert::AreEqual(brightness0, ledState.GetBrightness()->GetValueFloat(0));
		Assert::AreEqual(brightness1, ledState.GetBrightness()->GetValueFloat(1));
		Assert::AreEqual(brightness2, ledState.GetBrightness()->GetValueFloat(2));
	}

	static void Test()
	{
		LedDeviceSimulator ledDevice(100);
		LedDeviceCreatorSimulator ledCreator(&ledDevice);
		LedManager ledManager(&ledCreator);
		ledManager.Configure("", 1, 555);

		CommandResult commandResult(16);
		commandResult.AddTarget(LedState(0, 1.0, 1));
		commandResult.SetCycleCount(1);

		ledManager.SetDelta(&commandResult);
		ledManager.Tick();

		Assert::AreEqual(1, ledDevice.GetUpdateCount());
		AssertLedState(ledDevice.GetUpdatedState(0), 0, 1);
	}

	static void TestTwoChannelsTwoUpdates()
	{
		LedDeviceSimulator ledDevice(100);

		LedDeviceCreatorSimulator ledCreator(&ledDevice);
		LedManager ledManager(&ledCreator);
		ledManager.Configure("", 2, 555);

		CommandResult commandResult(16);
		commandResult.AddTarget(LedState(0, 1.0, 1));
		commandResult.AddTarget(LedState(1, 2.0, 1));
		commandResult.SetCycleCount(1);

		ledManager.SetDelta(&commandResult);

		ledManager.Tick();

		Assert::AreEqual(2, ledDevice.GetUpdateCount());
		AssertLedState(ledDevice.GetUpdatedState(0), 0, 1.0);
		AssertLedState(ledDevice.GetUpdatedState(1), 1, 2.0);

		ledManager.Tick();

		Assert::AreEqual(4, ledDevice.GetUpdateCount());
		AssertLedState(ledDevice.GetUpdatedState(0), 0, 1.0);
		AssertLedState(ledDevice.GetUpdatedState(1), 1, 2.0);
		AssertLedState(ledDevice.GetUpdatedState(2), 0, 1.0);
		AssertLedState(ledDevice.GetUpdatedState(3), 1, 2.0);
	}

	static void TestMultipleSteps()
	{
		LedDeviceSimulator ledDevice(100);

		LedDeviceCreatorSimulator ledCreator(&ledDevice);
		LedManager ledManager(&ledCreator);
		ledManager.Configure("", 1, 555);

		CommandResult commandResult(16);
		commandResult.AddTarget(LedState(0, 20.0, 10));
		commandResult.SetCycleCount(10);

		ledManager.SetDelta(&commandResult);

		for (int i = 1; i < 6; i++)
		{
			ledManager.Tick();
			Assert::AreEqual(i, ledDevice.GetUpdateCount());
			AssertLedState(ledDevice.GetUpdatedState(i - 1), 0, i * 2.0F);
		}
	}

	static void TestTwoChannelsDifferentUpdateRates()
	{
		LedDeviceSimulator ledDevice(100);

		LedDeviceCreatorSimulator ledCreator(&ledDevice);
		LedManager ledManager(&ledCreator);
		ledManager.Configure("", 2, 555);

		CommandResult commandResult(16);
		commandResult.AddTarget(LedState(0, 10.0, 5));
		commandResult.AddTarget(LedState(1, 20.0, 20));
		commandResult.SetCycleCount(5);

		ledManager.SetDelta(&commandResult);

		ledManager.Tick();

		Assert::AreEqual(2, ledDevice.GetUpdateCount());
		AssertLedState(ledDevice.GetUpdatedState(0), 0, 2.0);
		AssertLedState(ledDevice.GetUpdatedState(1), 1, 1.0);

		ledManager.Tick();

		Assert::AreEqual(4, ledDevice.GetUpdateCount());
		AssertLedState(ledDevice.GetUpdatedState(0), 0, 2.0);
		AssertLedState(ledDevice.GetUpdatedState(1), 1, 1.0);
		AssertLedState(ledDevice.GetUpdatedState(2), 0, 4.0);
		AssertLedState(ledDevice.GetUpdatedState(3), 1, 2.0);
	}

	static void TestMultiValue()
	{
		LedDeviceSimulator ledDevice(100);
		LedDeviceCreatorSimulator ledCreator(&ledDevice);
		LedManager ledManager(&ledCreator);
		ledManager.Configure("", 1, 555);

		CommandResult commandResult(16);
		Variable target;
		target.SetValue(0, 1.0F);
		target.SetValue(1, 0.5F);
		target.SetValue(2, 0.25F);
		commandResult.AddTarget(LedState(0, &target, 1));
		commandResult.SetCycleCount(1);

		ledManager.SetDelta(&commandResult);
		ledManager.Tick();

		Assert::AreEqual(1, ledDevice.GetUpdateCount());
		AssertLedState(ledDevice.GetUpdatedState(0), 0, 1.0F, 0.5F, 0.25F);
	}

	static void TestMultiValueTwoStep()
	{
		LedDeviceSimulator ledDevice(100);
		LedDeviceCreatorSimulator ledCreator(&ledDevice);
		LedManager ledManager(&ledCreator);
		ledManager.Configure("", 1, 555);

		CommandResult commandResult(16);
		Variable target;
		target.SetValue(0, 1.0F);
		target.SetValue(1, 2.0F);
		target.SetValue(2, 4.0F);
		commandResult.AddTarget(LedState(0, &target, 2));
		commandResult.SetCycleCount(2);

		ledManager.SetDelta(&commandResult);
		ledManager.Tick();

		Assert::AreEqual(1, ledDevice.GetUpdateCount());
		AssertLedState(ledDevice.GetUpdatedState(0), 0, 0.5F, 1.0F, 2.0F);

		ledManager.Tick();

		Assert::AreEqual(2, ledDevice.GetUpdateCount());
		AssertLedState(ledDevice.GetUpdatedState(1), 0, 1.0F, 2.0F, 4.0F);
	}

	static void TestInvalidChannel()
	{
		LedDeviceSimulator ledDevice(100);
		LedDeviceCreatorSimulator ledCreator(&ledDevice);
		LedManager ledManager(&ledCreator);
		ledManager.Configure("", 1, 555);

		CommandResult commandResult(16);
		commandResult.AddTarget(LedState(1, 1.0, 1));
		commandResult.SetCycleCount(1);

		ledManager.SetDelta(&commandResult);
		ledManager.Tick();

		Assert::AreEqual(1, ledDevice.GetUpdateCount());
		AssertLedState(ledDevice.GetUpdatedState(0), 0, 0);
	}

public:

	static int Run()
	{
		TestTwoChannelsDifferentUpdateRates();
		TestMultipleSteps();
		TestTwoChannelsTwoUpdates();
		Test();

		TestMultiValue();
		TestMultiValueTwoStep();
		TestInvalidChannel();

		return 0;
	}
};
