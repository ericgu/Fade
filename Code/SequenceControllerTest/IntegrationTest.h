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

		commandSource.AddCommand(Command("D(10,0,10.0)", 0));
		commandSource.AddCommand(Command("A(10)", 1));

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

		commandSource.AddCommand(Command("D(10,0,10.0)", 0));
		commandSource.AddCommand(Command("A(10)", 1));
		commandSource.AddCommand(Command("D(10,0,0.0)", 2));
		commandSource.AddCommand(Command("A(10)", 3));

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

		//"$1$FOR %A 0:7\n$100$D%A,1.0$100$D%A,0.0\n$1$ENDFOR"
		commandSource.AddCommand(Command("FOR A 0:7", 0));
		commandSource.AddCommand(Command("D(2,A,1.0)", 1));
		commandSource.AddCommand(Command("A(2)", 2));
		commandSource.AddCommand(Command("D(2,A,0.0)", 3));
		commandSource.AddCommand(Command("A(2)", 4));
		commandSource.AddCommand(Command("ENDFOR", 5));

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

	static void Test4()
	{
		CommandSourceSimulator commandSource;
		LedPwmSimulator ledPwm(16000);

		LedManager ledManager(&ledPwm, 16);

		// FOR A 0:7
		// 	PL(A)
		// 	DI(5, A, 1.0)
		// 	D(100, A, 0.0)
		// 	A(25);
		// ENDFOR

		commandSource.AddCommand(Command("FOR A 0:7			  ", 0));
		commandSource.AddCommand(Command("DI(5, A, 1.0)	  ", 1));
		commandSource.AddCommand(Command("D(20, A, 0.0)	  ", 2));
		commandSource.AddCommand(Command("A(5);			  ", 3));
		commandSource.AddCommand(Command("ENDFOR			  ", 4));

		Timebase timebase(&commandSource, &ledManager);

		for (int i = 0; i < 100; i++)
		{
			timebase.DoTick();
		}

		for (int i = 0; i < ledPwm.GetUpdateCount(); i += 16)
		{
			for (int channel = 0; channel < 16; channel++)
			{
				LedState ledState = ledPwm.GetUpdatedState(i + channel);
				//printf("%0.2f ", ledState.GetBrightness());
			}
			//puts("");
		}
	}

public:

	static int Run()
	{
		Test();
		Test2();
		Test3();
		Test4();

		return 0;
	}
};
