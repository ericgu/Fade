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
		Command command("FOR %D", 0);

		Assert::AreEqual(1, command.StartsWith("FOR"));
	}

	static void TestTooBig()
	{
		Serial.SetOutput(0);
		Command command("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890", 0);
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