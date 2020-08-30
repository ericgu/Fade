#pragma once

#include "ExecutionFlow.h"

#define ResultStorageSize 100

class LedMessageHandlerSimulator: public ILedMessageHandler
{
public:
	CommandResult _commandResults[ResultStorageSize];
	int _commandResultCount;
	char _ledType[128];
	int _ledCount;
	int _ledPin;

	LedMessageHandlerSimulator()
	{
		_commandResultCount = 0;
		_ledCount = 0;
		_ledPin = 0;
		_ledType[0] = 0;
	}

	void ExecuteLedCommandMember(CommandResult* pCommandResult)
	{
		_commandResults[_commandResultCount] = *pCommandResult;
		_commandResultCount++;
	}

	void Configure(const char* pLedType, int ledCount, int ledPin)
	{
		strcpy(_ledType, pLedType);
		_ledCount = ledCount;
		_ledPin = ledPin;
	}
};

class MockButton : public IButton
{
    bool _returnValue;

public:
    MockButton(bool returnValue)
    {
        _returnValue = returnValue;
    }

    virtual bool GetButtonStatus()
    {
        return _returnValue;
    }
};

class MockButtonCreator : public IButtonCreator
{
public:
    IButton *Create(const char *pButtonType, int pin, int parameter1)
    {
        return 0;
    }
};


class ExecutionFlowTest
{
    static void TestButtonRegistration()
    {
        ExecutionFlow executionFlow(0, 0, 0, 0);
        Assert::AreEqual(0, executionFlow.GetButtonCount());


        MockButton mockButton(1);
        executionFlow.AddButton(&mockButton);

        Assert::AreEqual(1, executionFlow.GetButtonCount());
        Assert::AreEqual(1, executionFlow.GetButtonState(0));

        MockButton mockButton2(0);
        executionFlow.AddButton(&mockButton2);

        Assert::AreEqual(2, executionFlow.GetButtonCount());
        Assert::AreEqual(0, executionFlow.GetButtonState(1));
    }

public:
	static int Run()
	{
        TestButtonRegistration();


		return 0;
	}
};
