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

	void SetDelta(CommandResult commandResult)
	{
		_commandResult = commandResult;
	}

	void ResetState()
	{
		_tickCount = 0;
	}

	void Tick()
	{
		_tickCount++;
	}

	CommandResult GetCommandResult()
	{
		return _commandResult;
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

		commandSource.AddCommand("D(10,0,10.0)");
		commandSource.AddCommand("A(10)");

		Timebase timebase(&commandSource, &ledManager, &parseErrors, 0);

		timebase.RunProgram(1);

		Assert::AreEqual(10, ledManager.GetCommandResult().GetCycleCount());

		CommandResult commandResult = ledManager.GetCommandResult();
		Assert::AreEqual(1, commandResult.GetCount());
		LedState ledState = commandResult.GetTarget(0);
		Assert::AreEqual(0, ledState.GetChannel());
		Assert::AreEqual(10.0F, ledState.GetBrightness()->GetValueFloat(0));

		Assert::AreEqual(10, ledManager.GetTickCount());

		commandSource.AddCommand("Test");

		timebase.RunProgram(1);
	}

	static void TestLoop()
	{
		CommandSourceSimulator commandSource;
		LedManagerSimulator ledManager;
		ParseErrors parseErrors;

		commandSource.AddCommand("FOR B 0:7");
		commandSource.AddCommand("D(1,B,10.0)");
		commandSource.AddCommand("A(10)");
		commandSource.AddCommand("ENDFOR");

		Timebase timebase(&commandSource, &ledManager, &parseErrors, 0);

		timebase.RunProgram(1);

		CommandResult commandResult = ledManager.GetCommandResult();
		Assert::AreEqual(1, commandResult.GetCount());
		LedState ledState = commandResult.GetTarget(0);
		Assert::AreEqual(0, ledState.GetChannel());
		Assert::AreEqual(10.0F, ledState.GetBrightness()->GetValueFloat(0));

		timebase.RunProgram(1);

		commandResult = ledManager.GetCommandResult();
		Assert::AreEqual(1, commandResult.GetCount());
		ledState = commandResult.GetTarget(0);
		Assert::AreEqual(0, ledState.GetChannel());
		Assert::AreEqual(10.0F, ledState.GetBrightness()->GetValueFloat(0));

		Assert::AreEqual(10, ledManager.GetTickCount());
	} 

	static void TestLoop2()
	{
		CommandSourceSimulator commandSource;
		LedManagerSimulator ledManager;
		ParseErrors parseErrors;

		//"$1$FOR %A 0:7\n$100$D%A,1.0$100$D%A,0.0\n$1$ENDFOR"
		commandSource.AddCommand("FOR A 0:7");
		commandSource.AddCommand("D(100,A,1.0)");
		commandSource.AddCommand("A(100)");
		commandSource.AddCommand("D(100,A,0.0)");
		commandSource.AddCommand("A(100)");
		commandSource.AddCommand("ENDFOR");

		Timebase timebase(&commandSource, &ledManager, &parseErrors, 0);

		timebase.RunProgram(1);

		CommandResult commandResult = ledManager.GetCommandResult();
		LedState ledState = commandResult.GetTarget(0);

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
