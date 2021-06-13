#pragma once

#include "Timebase.h"

class LedManagerSimulator : public ILedManager
{
	CommandResult _commandResult;
	int _tickCount = 0;

public:
	char _ledType[128];
	int _ledCount;
    int _ledPin1;
    int _ledPin2;
    int _ledPin3;
    int _ledPin4;
    int _ledGroupNumber;

	void SetDelta(CommandResult* pCommandResult)
	{
		_commandResult = *pCommandResult;
	}

	void ResetState()
	{
		_tickCount = 0;
	}

	void Tick()
	{
		_tickCount++;
	}

	CommandResult* GetCommandResult()
	{
		return &_commandResult;
	}

	int GetTickCount()
	{
		return _tickCount;
	}

    int GetLedCount()
    {
        return _ledCount;
    }

	void Configure(int ledGroupNumber, const char* pLedType, int ledCount, int ledPin1, int ledPin2, int ledPin3, int ledPin4)
	{
        _ledGroupNumber = ledGroupNumber;
		strcpy(_ledType, pLedType);
		_ledCount = ledCount;
        _ledPin1 = ledPin1;
        _ledPin2 = ledPin2;
        _ledPin3 = ledPin3;
        _ledPin4 = ledPin4;
    }
};

class TimebaseTest
{
	static void Test()
	{
		CommandSourceSimulator commandSource;
		LedManagerSimulator ledManager;
		ParseErrors parseErrors;

		Timebase timebase(&commandSource, &ledManager, &parseErrors, 0, 0);

		timebase.RunProgram("D(10,0,10.0)\nA(10)");

		Assert::AreEqual(10, ledManager.GetCommandResult()->GetCycleCount());

		CommandResult* pCommandResult = ledManager.GetCommandResult();
		Assert::AreEqual(1, pCommandResult->GetCount());
		LedState ledState = pCommandResult->GetTarget(0);
		Assert::AreEqual(0, ledState.GetChannel());
		Assert::AreEqual(10.0F, ledState.GetBrightness()->GetValueFloat(0));

		Assert::AreEqual(10, ledManager.GetTickCount());

		//commandSource.AddCommand("Test");

		//timebase.RunProgram(1);
	}

	static void TestLoop()
	{
		CommandSourceSimulator commandSource;
		LedManagerSimulator ledManager;
		ParseErrors parseErrors;

		Timebase timebase(&commandSource, &ledManager, &parseErrors, 0, 0);

		timebase.RunProgram("for B 0:7\nD(1,B,10.0)\nA(10)\nendfor");

		CommandResult* pCommandResult = ledManager.GetCommandResult();
		Assert::AreEqual(1, pCommandResult->GetCount());
		LedState ledState = pCommandResult->GetTarget(0);
		Assert::AreEqual(0, ledState.GetChannel());
		Assert::AreEqual(10.0F, ledState.GetBrightness()->GetValueFloat(0));

        timebase.RunProgram("for B 0:7\nD(1,B,10.0)\nA(10)\nendfor");

		pCommandResult = ledManager.GetCommandResult();
		Assert::AreEqual(1, pCommandResult->GetCount());
		ledState = pCommandResult->GetTarget(0);
		Assert::AreEqual(0, ledState.GetChannel());
		Assert::AreEqual(10.0F, ledState.GetBrightness()->GetValueFloat(0));

		Assert::AreEqual(10, ledManager.GetTickCount());
	} 

	static void TestLoop2()
	{
		CommandSourceSimulator commandSource;
		LedManagerSimulator ledManager;
		ParseErrors parseErrors;

		Timebase timebase(&commandSource, &ledManager, &parseErrors, 0, 0);

		timebase.RunProgram("for A 0:7\nD(100,A,1.0)\nA(100)\nD(100,A,0.0)\nA(100)\nendfor");

		CommandResult* pCommandResult = ledManager.GetCommandResult();
		LedState ledState = pCommandResult->GetTarget(0);
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
