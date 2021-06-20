#pragma once

#include "Command.h"

class CommandTest
{
	static void TestStartsWithMismatch()
	{
		Command command("fred", 0);

		Assert::AreEqual(0, command.StartsWith("mismatch"));
	}

	static void TestStartsWithMatch1()
	{
		Command command("D1213", 0);

		Assert::AreEqual(1, command.StartsWith("D"));
	}

	static void TestStartsWithMatch2()
	{
		Command command("for %D", 0);

		Assert::AreEqual(1, command.StartsWith("for"));
	}

	static void TestTooBig()
	{
		Serial.SetOutput(0);
        char buffer[1025];
        for (int i = 0; i < sizeof(buffer) - 1; i++)
        {
            buffer[i] = '0';
        }
		Command command(buffer, 0);
		Assert::AreEqual("Command string too long", Serial.GetLastString());
		Serial.SetOutput(1);
	}

public:
	static void Run()
	{
		TestStartsWithMismatch();
		TestStartsWithMatch1();
		TestStartsWithMatch2();
		TestTooBig();
	}
};