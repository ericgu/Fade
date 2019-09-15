#include "CommandSource.h"

#pragma once
class CommandSourceTest
{
	static void AssertCommand(Command command, const char* commandString, int serialNumber)
	{
		Assert::AreEqual(commandString, command.GetString());
		Assert::AreEqual(serialNumber, command.GetSerialNumber());
	}

	static void TestSingle()
	{
		CommandSource commandSource;
		commandSource.SetCommand("$D 500 1,2,3,4,5,6,7,8");

		Command command = commandSource.GetNextCommand();

		AssertCommand(command, "D 500 1,2,3,4,5,6,7,8", 0);
	}

	static void TestSingleTwice()
	{
		CommandSource commandSource;
		commandSource.SetCommand("$D 500 1,2,3,4,5,6,7,8");

		Command command = commandSource.GetNextCommand();

		AssertCommand(command, "D 500 1,2,3,4,5,6,7,8", 0);

		command = commandSource.GetNextCommand();

		AssertCommand(command, "D 500 1,2,3,4,5,6,7,8", 0);
	}

	static void TestMultiple()
	{
		CommandSource commandSource;
		commandSource.SetCommand("$D 500 1,2,3,4,5,6,7,8$xxx");

		Command command = commandSource.GetNextCommand();

		AssertCommand(command, "D 500 1,2,3,4,5,6,7,8", 0);

		command = commandSource.GetNextCommand();

		AssertCommand(command, "xxx", 1);

		command = commandSource.GetNextCommand();

		AssertCommand(command, "D 500 1,2,3,4,5,6,7,8", 0);
	}

	static void TestMultipleWithNewline()
	{
		CommandSource commandSource;

		commandSource.SetCommand("$D1,2,3,4,5,6,7,8\n$xxx");

		Command command = commandSource.GetNextCommand();

		AssertCommand(command, "D1,2,3,4,5,6,7,8", 0);

		command = commandSource.GetNextCommand();

		AssertCommand(command, "xxx", 1);

		command = commandSource.GetNextCommand();

		AssertCommand(command, "D1,2,3,4,5,6,7,8", 0);
	}

	static void TestSetCommandToSerialNumber()
	{
		CommandSource commandSource;

		commandSource.SetCommand("$D1,2,3,4,5,6,7,8\n$xxx\n$zzz");

		Command command = commandSource.GetNextCommand();
		AssertCommand(command, "D1,2,3,4,5,6,7,8", 0);

		command = commandSource.GetNextCommand();
		AssertCommand(command, "xxx", 1);

		command = commandSource.GetNextCommand();
		AssertCommand(command, "zzz", 2);

		commandSource.SetCommandToSerialNumber(1);
		command = commandSource.GetNextCommand();
		AssertCommand(command, "xxx", 1);
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

