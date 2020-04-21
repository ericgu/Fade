#pragma once

#include "Timebase.h"

class LedManagerSimulator : public ILedManager
{
	CommandResult _commandResult;
	int _tickCount = 0;

public:
	char _ledType[128];
	int _ledCount;
	int _ledPin;

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

	void Configure(const char* pLedType, int ledCount, int ledPin)
	{
		strcpy(_ledType, pLedType);
		_ledCount = ledCount;
		_ledPin = ledPin;
	}
};

class TimebaseTest
{
	static void Test()
	{
		CommandSourceSimulator commandSource;
		LedManagerSimulator ledManager;
		ParseErrors parseErrors;

		Timebase timebase(&commandSource, &ledManager, &parseErrors, 0);

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

		Timebase timebase(&commandSource, &ledManager, &parseErrors, 0);

		timebase.RunProgram("FOR B 0:7\nD(1,B,10.0)\nA(10)\nENDFOR");

		CommandResult* pCommandResult = ledManager.GetCommandResult();
		Assert::AreEqual(1, pCommandResult->GetCount());
		LedState ledState = pCommandResult->GetTarget(0);
		Assert::AreEqual(0, ledState.GetChannel());
		Assert::AreEqual(10.0F, ledState.GetBrightness()->GetValueFloat(0));

        timebase.RunProgram("FOR B 0:7\nD(1,B,10.0)\nA(10)\nENDFOR");

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

		Timebase timebase(&commandSource, &ledManager, &parseErrors, 0);

		timebase.RunProgram("FOR A 0:7\nD(100,A,1.0)\nA(100)\nD(100,A,0.0)\nA(100)\nENDFOR");

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
