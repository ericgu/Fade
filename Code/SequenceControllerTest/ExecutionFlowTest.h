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


class ExecutionFlowTest
{


public:
	static int Run()
	{

		return 0;
	}
};
