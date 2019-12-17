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
		commandSource.SetCommand("D 500 1,2,3,4,5,6,7,8");

		Command command = commandSource.GetCommand(0);

		AssertCommand(command, "D 500 1,2,3,4,5,6,7,8", 0);
	}

	static void TestSingleTwice()
	{
		CommandSource commandSource;
		commandSource.SetCommand("D 500 1,2,3,4,5,6,7,8");

		Command command = commandSource.GetCommand(0);

		AssertCommand(command, "D 500 1,2,3,4,5,6,7,8", 0);

		command = commandSource.GetCommand(0);

		AssertCommand(command, "D 500 1,2,3,4,5,6,7,8", 0);
	}

	static void TestMultiple()
	{
		CommandSource commandSource;
		commandSource.SetCommand("D 500 1,2,3,4,5,6,7,8\nxxx");

		Command command = commandSource.GetCommand(0);

		AssertCommand(command, "D 500 1,2,3,4,5,6,7,8", 0);

		command = commandSource.GetCommand(1);

		AssertCommand(command, "xxx", 1);

		command = commandSource.GetCommand(2);
		Assert::AreEqual(-1, command.GetSerialNumber());
	}

	static void TestMultipleWithNewline()
	{
		CommandSource commandSource;

		commandSource.SetCommand("D1,2,3,4,5,6,7,8\nxxx");

		Command command = commandSource.GetCommand(0);

		AssertCommand(command, "D1,2,3,4,5,6,7,8", 0);

		command = commandSource.GetCommand(1);

		AssertCommand(command, "xxx", 1);
	}

	static void TestMultipleWithNewlineAndCarriageReturn()
	{
		CommandSource commandSource;

		commandSource.SetCommand("D1,2,3,4,5,6,7,8\r\nxxx");

		Command command = commandSource.GetCommand(0);

		AssertCommand(command, "D1,2,3,4,5,6,7,8", 0);

		command = commandSource.GetCommand(1);

		AssertCommand(command, "xxx", 1);
	}

	static void TestMultipleWithSpaces()
	{
		CommandSource commandSource;

		commandSource.SetCommand("  D1,2,3,4,5,6,7,8\n   xxx");

		Command command = commandSource.GetCommand(0);

		AssertCommand(command, "D1,2,3,4,5,6,7,8", 0);

		command = commandSource.GetCommand(1);

		AssertCommand(command, "xxx", 1);
	}

	static void TestWithoutSetCommand()
	{
		CommandSource commandSource;

		Command command = commandSource.GetCommand(0);

		Assert::AreEqual(-1, command.GetSerialNumber());
	}
public:

	static int Run()
	{
		TestSingle();
		TestMultiple();
		TestMultipleWithNewline();
		TestMultipleWithSpaces();
		TestMultipleWithNewlineAndCarriageReturn();
		TestWithoutSetCommand();
		
		return 0;
	}
};

