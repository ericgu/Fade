#pragma once

#include "Command.h"

class CommandTest
{
	static void TestStartsWithMismatch()
	{
		Command command("", "fred", 0);

		Assert::AreEqual(0, command.StartsWith("mismatch"));
	}

	static void TestStartsWithMatch1()
	{
		Command command("", "D1213", 0);

		Assert::AreEqual(1, command.StartsWith("D"));
	}

	static void TestStartsWithMatch2()
	{
		Command command("", "LOOP %D", 0);

		Assert::AreEqual(1, command.StartsWith("LOOP"));
	}

public:
	static void Run()
	{
		TestStartsWithMismatch();
		TestStartsWithMatch1();
		TestStartsWithMatch2();
	}
};