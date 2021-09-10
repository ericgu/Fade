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
      VariableStore::VariableStoreInstance.ResetCache();
      {

        CommandSourceSimulator commandSource;
        LedDeviceSimulator ledDevice(1000);
        LedDeviceCreatorSimulator ledCreator(&ledDevice);

        LedManager ledManager(&ledCreator);
        int pins[16];
        pins[0] = 555;

        ledManager.Configure(0, "Test", 16, 1, pins);

        commandSource.AddCommand("D(10,0,10.0)");
        commandSource.AddCommand("A(10)");

        ParseErrors parseErrors;
        _callbackCount = 0;
        Timebase timebase(&commandSource, &ledManager, &parseErrors, Callback, 0);

        char command[512];
        command[0] = '\0';
        strcpy(command, "D(10, 0, 10.0\nA(10)");

        timebase.RunProgram("D(10, 0, 10.0)\nA(10)");

        Assert::AreEqual(160, ledDevice.GetUpdateCount());
        for (int i = 0; i < 10; i++)
        {
          int state = i * 16;

          LedState ledState = ledDevice.GetUpdatedState(state);
          AssertLedState(ledState, 0, (float)(i + 1));
        }

        Assert::AreEqual(10, _callbackCount);
      }
        //timebase.RunProgram(1);

      VariableStore::VariableStoreInstance.ResetCache();

      Vector::RestartVectorDataProvider();
    }

	static void Test2()
	{
    VariableStore::VariableStoreInstance.ResetCache();
    {
      CommandSourceSimulator commandSource;
      LedDeviceSimulator ledDevice(1000);

      LedDeviceCreatorSimulator ledCreator(&ledDevice);

      LedManager ledManager(&ledCreator);
      int pins[16];
      pins[0] = 555;

      ledManager.Configure(0, "Test", 16, 1, pins);
      
      ParseErrors parseErrors;
      Timebase timebase(&commandSource, &ledManager, &parseErrors, 0, 0);

      timebase.RunProgram("D(10, 0, 10.0)\nA(10)\nD(10, 0, 0.0)\nA(10)");

      //timebase.RunProgram(1);

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
    VariableStore::VariableStoreInstance.ResetCache();

    Vector::RestartVectorDataProvider();
	}

    static void Test3()
    {
        VariableStore::VariableStoreInstance.ResetCache();

        {
            CommandSourceSimulator commandSource;
            LedDeviceSimulator ledDevice(10000);

            LedDeviceCreatorSimulator ledCreator(&ledDevice);

            LedManager ledManager(&ledCreator);
            int pins[16];
            pins[0] = 555;

            ledManager.Configure(0, "Test", 16, 1, pins);

            ParseErrors parseErrors;
            Timebase timebase(&commandSource, &ledManager, &parseErrors, 0, 0);

            timebase.RunProgram("for A 0:7\nD(2,A,1.0)\nA(2)\nD(2,A,0.0)\nA(2)\nendfor");

            //timebase.RunProgram(1);

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
        VariableStore::VariableStoreInstance.ResetCache();

        Vector::RestartVectorDataProvider();
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

int IntegrationTest::_callbackCount = 0;
