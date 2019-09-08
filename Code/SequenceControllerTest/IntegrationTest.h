#pragma once


class IntegrationTest
{
	static void AssertLedState(LedState ledState, int channel, float brightness)
	{
		Assert::AreEqual(channel, ledState.GetChannel());
		Assert::AreEqual(brightness, ledState.GetBrightness());
	}

	static void Test()
	{
		CommandSourceSimulator commandSource;
		LedPwmSimulator ledPwm(1000);

		LedManager ledManager(&ledPwm, 16);

		commandSource.AddCommand(Command(10, "D0,10.0", 0));

		Timebase timebase(&commandSource, &ledManager);

		for (int i = 0; i < 10; i++)
		{
			timebase.DoTick();
		}

		Assert::AreEqual(160, ledPwm.GetUpdateCount());
		AssertLedState(ledPwm.GetUpdatedState(0), 0, 10);

		timebase.DoTick();
	}

public:

	static int Run()
	{
		Test();

		return 0;
	}
};
