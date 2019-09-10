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

		commandSource.AddCommand(Command("10", "D0,10.0", 0));

		Timebase timebase(&commandSource, &ledManager);

		for (int i = 0; i < 10; i++)
		{
			timebase.DoTick();
		}

		Assert::AreEqual(160, ledPwm.GetUpdateCount());
		for (int i = 0; i < 10; i++)
		{
			int state = i * 16;

			LedState ledState = ledPwm.GetUpdatedState(state);
			AssertLedState(ledState, 0, (float) (i + 1));
		}

		timebase.DoTick();
	}

	static void Test2()
	{
		CommandSourceSimulator commandSource;
		LedPwmSimulator ledPwm(1000);

		LedManager ledManager(&ledPwm, 16);

		commandSource.AddCommand(Command("10", "D0,10.0", 0));
		commandSource.AddCommand(Command("10", "D0,0.0", 1));

		Timebase timebase(&commandSource, &ledManager);

		for (int i = 0; i < 10; i++)
		{
			timebase.DoTick();
		}

		for (int i = 0; i < 10; i++)
		{
			timebase.DoTick();
		}

		Assert::AreEqual(320, ledPwm.GetUpdateCount());
		for (int i = 0; i < 10; i++)
		{
			int state = i * 16;

			LedState ledState = ledPwm.GetUpdatedState(state);
			AssertLedState(ledState, 0, (float)(i + 1));
		}

		for (int i = 9; i >= 0; i--)
		{
			int state = i * 16;

			LedState ledState = ledPwm.GetUpdatedState(state);
			AssertLedState(ledState, 0, (float)(i + 1));
		}
	}

	static void Test3()
	{
		CommandSourceSimulator commandSource;
		LedPwmSimulator ledPwm(10000);

		LedManager ledManager(&ledPwm, 16);

		//"$1$LOOP %A 0:7\n$100$D%A,1.0$100$D%A,0.0\n$1$ENDLOOP"
		commandSource.AddCommand(Command("1", "LOOP %A 0:7", 0));
		commandSource.AddCommand(Command("2", "D%A,1.0", 1));
		commandSource.AddCommand(Command("2", "D%A,0.0", 2));
		commandSource.AddCommand(Command("1", "ENDLOOP", 3));

		Timebase timebase(&commandSource, &ledManager);

		for (int i = 0; i < 100; i++)
		{
			timebase.DoTick();
		}

		// Validate that after channel one loop is done it stops changing and moves on to channel two. 
		Assert::AreEqual(0.5, ledPwm.GetUpdatedState(0).GetBrightness());
		Assert::AreEqual(1.0, ledPwm.GetUpdatedState(16).GetBrightness());
		Assert::AreEqual(0.5, ledPwm.GetUpdatedState(32).GetBrightness());
		Assert::AreEqual(0.0, ledPwm.GetUpdatedState(48).GetBrightness());
		Assert::AreEqual(0.5, ledPwm.GetUpdatedState(0).GetBrightness());
		Assert::AreEqual(0.5, ledPwm.GetUpdatedState(0).GetBrightness());
		Assert::AreEqual(0.0, ledPwm.GetUpdatedState(64).GetBrightness());
		Assert::AreEqual(0.5, ledPwm.GetUpdatedState(65).GetBrightness());
		Assert::AreEqual(0.0, ledPwm.GetUpdatedState(80).GetBrightness());
		Assert::AreEqual(1.0, ledPwm.GetUpdatedState(81).GetBrightness());
	}

public:

	static int Run()
	{
		Test();
		Test2();
		Test3();

		return 0;
	}
};
