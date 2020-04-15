#pragma once


class IntegrationTest
{
	static int _callbackCount;

	static void AssertLedState(LedState ledState, int channel, float brightness)
	{
		Assert::AreEqual(channel, ledState.GetChannel());
		Assert::AreEqual(brightness, ledState.GetBrightness()->GetValueFloat(0));
	}

	static void Callback()
	{
		_callbackCount++;
	}

	static void Test()
	{
		CommandSourceSimulator commandSource;
		LedDeviceSimulator ledDevice(1000);
		LedDeviceCreatorSimulator ledCreator(&ledDevice);

		LedManager ledManager(&ledCreator);
		ledManager.Configure("", 16, 555);

		commandSource.AddCommand("D(10,0,10.0)");
		commandSource.AddCommand("A(10)");

		ParseErrors parseErrors;
		_callbackCount = 0;
		Timebase timebase(&commandSource, &ledManager, &parseErrors, Callback);

		timebase.RunProgram(1);
		
		Assert::AreEqual(160, ledDevice.GetUpdateCount());
		for (int i = 0; i < 10; i++)
		{
			int state = i * 16;

			LedState ledState = ledDevice.GetUpdatedState(state);
			AssertLedState(ledState, 0, (float) (i + 1));
		}

		Assert::AreEqual(10, _callbackCount);

		timebase.RunProgram(1);
	}

	static void Test2()
	{
		CommandSourceSimulator commandSource;
		LedDeviceSimulator ledDevice(1000);

		LedDeviceCreatorSimulator ledCreator(&ledDevice);

		LedManager ledManager(&ledCreator);
		ledManager.Configure("", 16, 555);
		
		commandSource.AddCommand("D(10,0,10.0)");
		commandSource.AddCommand("A(10)");
		commandSource.AddCommand("D(10,0,0.0)");
		commandSource.AddCommand("A(10)");

		ParseErrors parseErrors;
		Timebase timebase(&commandSource, &ledManager, &parseErrors, 0);

		timebase.RunProgram(1);

		Assert::AreEqual(320, ledDevice.GetUpdateCount());
		for (int i = 0; i < 10; i++)
		{
			int state = i * 16;

			LedState ledState = ledDevice.GetUpdatedState(state);
			AssertLedState(ledState, 0, (float)(i + 1));
		}

		for (int i = 9; i >= 0; i--)
		{
			int state = i * 16;

			LedState ledState = ledDevice.GetUpdatedState(state);
			AssertLedState(ledState, 0, (float)(i + 1));
		}
	}

	static void Test3()
	{
		CommandSourceSimulator commandSource;
		LedDeviceSimulator ledDevice(10000);

		LedDeviceCreatorSimulator ledCreator(&ledDevice);

		LedManager ledManager(&ledCreator);
		ledManager.Configure("", 16, 555);

		//"$1$FOR %A 0:7\n$100$D%A,1.0$100$D%A,0.0\n$1$ENDFOR"
		commandSource.AddCommand("FOR A 0:7");
		commandSource.AddCommand("D(2,A,1.0)");
		commandSource.AddCommand("A(2)");
		commandSource.AddCommand("D(2,A,0.0)");
		commandSource.AddCommand("A(2)");
		commandSource.AddCommand("ENDFOR");

		ParseErrors parseErrors;
		Timebase timebase(&commandSource, &ledManager, &parseErrors, 0);

		timebase.RunProgram(1);

		// Validate that after channel one loop is done it stops changing and moves on to channel two. 
		Assert::AreEqual(0.5, ledDevice.GetUpdatedState(0).GetBrightness()->GetValueFloat(0));
		Assert::AreEqual(1.0, ledDevice.GetUpdatedState(16).GetBrightness()->GetValueFloat(0));
		Assert::AreEqual(0.5, ledDevice.GetUpdatedState(32).GetBrightness()->GetValueFloat(0));
		Assert::AreEqual(0.0, ledDevice.GetUpdatedState(48).GetBrightness()->GetValueFloat(0));
		Assert::AreEqual(0.5, ledDevice.GetUpdatedState(0).GetBrightness()->GetValueFloat(0));
		Assert::AreEqual(0.5, ledDevice.GetUpdatedState(0).GetBrightness()->GetValueFloat(0));
		Assert::AreEqual(0.0, ledDevice.GetUpdatedState(64).GetBrightness()->GetValueFloat(0));
		Assert::AreEqual(0.5, ledDevice.GetUpdatedState(65).GetBrightness()->GetValueFloat(0));
		Assert::AreEqual(0.0, ledDevice.GetUpdatedState(80).GetBrightness()->GetValueFloat(0));
		Assert::AreEqual(1.0, ledDevice.GetUpdatedState(81).GetBrightness()->GetValueFloat(0));
	}

	static void Test4()
	{
		CommandSourceSimulator commandSource;
		LedDeviceSimulator ledDevice(16000);
		LedDeviceCreatorSimulator ledCreator(&ledDevice);

		LedManager ledManager(&ledCreator);
		ledManager.Configure("", 16, 555);

		// FOR A 0:7
		// 	PL(A)
		// 	DI(5, A, 1.0)
		// 	D(100, A, 0.0)
		// 	A(25);
		// ENDFOR

		commandSource.AddCommand("FOR A 0:7			  ");
		commandSource.AddCommand("DI(5, A, 1.0)	  ");
		commandSource.AddCommand("D(20, A, 0.0)	  ");
		commandSource.AddCommand("A(5);			  ");
		commandSource.AddCommand("ENDFOR			  ");

		ParseErrors parseErrors;
		Timebase timebase(&commandSource, &ledManager, &parseErrors, 0);

		timebase.RunProgram(1);

		for (int i = 0; i < ledDevice.GetUpdateCount(); i += 16)
		{
			for (int channel = 0; channel < 16; channel++)
			{
				LedState ledState = ledDevice.GetUpdatedState(i + channel);
				//printf("%0.2f ", ledState.GetBrightness());
			}
			//puts("");
		}
	}

	static void Test5()
	{
		CommandSourceSimulator commandSource;
		LedDeviceSimulator ledDevice(16000);
		LedDeviceCreatorSimulator ledCreator(&ledDevice);

		LedManager ledManager(&ledCreator);
		ledManager.Configure("", 16, 555);

		// FOR A 0:7
		// 	PL(A)
		// 	DI(5, A, 1.0)
		// 	D(100, A, 0.0)
		// 	A(25);
		// ENDFOR

		commandSource.AddCommand("CONFIGLED(\"RGB\", 33, 13)");
		commandSource.AddCommand("FUNC AngleToRGB(angleInDegrees)");
		commandSource.AddCommand("PL(angleInDegrees)");
		commandSource.AddCommand("");
		commandSource.AddCommand("IF (angleInDegrees <= 120)");
		commandSource.AddCommand("temp = angleInDegrees / 120");
		commandSource.AddCommand("value = { temp, 0, 1 - temp }");
		commandSource.AddCommand("ELSEIF (angleInDegrees <= 240)");
		commandSource.AddCommand("temp = (angleInDegrees - 120) / 120");
		commandSource.AddCommand("value = { 1 - temp, temp, 0 }");
		commandSource.AddCommand("ELSE");
		commandSource.AddCommand("temp = (angleInDegrees - 240) / 120");
		commandSource.AddCommand("value = { 0, 1 - temp, temp }");
		commandSource.AddCommand("ENDIF");
		commandSource.AddCommand("RETURN value");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("FUNC DoChunk(chunk, offset, angle)");
		commandSource.AddCommand("PL(\"DoChunk\")");
		commandSource.AddCommand("rgb = AngleToRGB(angle)");
		commandSource.AddCommand("//rgb = {1, 2, 3}");
		commandSource.AddCommand("PL(rgb)");
		commandSource.AddCommand("start = chunk * 3");
		commandSource.AddCommand("D(10, start, rgb)");
		commandSource.AddCommand("D(10, start + 1, rgb)");
		commandSource.AddCommand("D(10, start + 2, rgb)");
		commandSource.AddCommand("A(10)");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("");
		commandSource.AddCommand("PL(\"hello\")");
		commandSource.AddCommand("FOR angle 0:5:5");
		commandSource.AddCommand("PL(angle)");
		commandSource.AddCommand("DoChunk(0, 0, angle)");
		commandSource.AddCommand("ENDFOR");

		ParseErrors parseErrors;
		Timebase timebase(&commandSource, &ledManager, &parseErrors, 0);

		//StackWatcher::Init();
		//StackWatcher::Enable();
		timebase.RunProgram(1);
		//StackWatcher::Disable();

		for (int i = 0; i < ledDevice.GetUpdateCount(); i += 16)
		{
			for (int channel = 0; channel < 16; channel++)
			{
				LedState ledState = ledDevice.GetUpdatedState(i + channel);
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
		//Test5();

		return 0;
	}
};

int IntegrationTest::_callbackCount = 0;
