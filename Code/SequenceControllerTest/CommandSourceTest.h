#include "CommandSource.h"

#pragma once
class CommandSourceTest
{
	static void AssertCommand(Command command, const char* commandString, const char* countString, int serialNumber)
	{
		Assert::AreEqual(commandString, command.GetString());
		Assert::AreEqual(countString, command.GetCount());
		Assert::AreEqual(serialNumber, command.GetSerialNumber());
	}

	static void TestSingle()
	{
		CommandSource commandSource;
		commandSource.SetCommand("$500$D1,2,3,4,5,6,7,8");

		Command command = commandSource.GetNextCommand();

		AssertCommand(command, "D1,2,3,4,5,6,7,8", "500", 0);
	}

	static void TestSingleTwice()
	{
		CommandSource commandSource;
		commandSource.SetCommand("$500$D1,2,3,4,5,6,7,8");

		Command command = commandSource.GetNextCommand();

		AssertCommand(command, "D1,2,3,4,5,6,7,8", "500", 0);

		command = commandSource.GetNextCommand();

		AssertCommand(command, "D1,2,3,4,5,6,7,8", "500", 0);
	}

	static void TestMultiple()
	{
		CommandSource commandSource;
		commandSource.SetCommand("$500$D1,2,3,4,5,6,7,8$35$xxx");

		Command command = commandSource.GetNextCommand();

		AssertCommand(command, "D1,2,3,4,5,6,7,8", "500", 0);

		command = commandSource.GetNextCommand();

		AssertCommand(command, "xxx", "35", 1);

		command = commandSource.GetNextCommand();

		AssertCommand(command, "D1,2,3,4,5,6,7,8", "500", 0);
	}

	static void TestMultipleWithNewline()
	{
		CommandSource commandSource;

		commandSource.SetCommand("$500$D1,2,3,4,5,6,7,8\n$35$xxx");

		Command command = commandSource.GetNextCommand();

		AssertCommand(command, "D1,2,3,4,5,6,7,8", "500", 0);

		command = commandSource.GetNextCommand();

		AssertCommand(command, "xxx", "35", 1);

		command = commandSource.GetNextCommand();

		AssertCommand(command, "D1,2,3,4,5,6,7,8", "500", 0);
	}

	static void TestSetCommandToSerialNumber()
	{
		CommandSource commandSource;

		commandSource.SetCommand("$500$D1,2,3,4,5,6,7,8\n$35$xxx\n$111$zzz");

		Command command = commandSource.GetNextCommand();
		AssertCommand(command, "D1,2,3,4,5,6,7,8", "500", 0);

		command = commandSource.GetNextCommand();
		AssertCommand(command, "xxx", "35", 1);

		command = commandSource.GetNextCommand();
		AssertCommand(command, "zzz", "111", 2);

		commandSource.SetCommandToSerialNumber(1);
		command = commandSource.GetNextCommand();
		AssertCommand(command, "xxx", "35", 1);
	}

public:

	static int Run()
	{
		TestSingle();
		TestMultiple();
		TestMultipleWithNewline();
		TestSetCommandToSerialNumber();

		return 0;
	}
};

