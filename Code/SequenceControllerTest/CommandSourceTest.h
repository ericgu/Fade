#include "CommandSource.h"

#pragma once
class CommandSourceTest
{
	static void AssertCommand(Command* pCommand, const char* commandString, int serialNumber)
	{
		Assert::AreEqual(commandString, pCommand->GetString());
		Assert::AreEqual(serialNumber, pCommand->GetSerialNumber());
	}

	static void TestSingle()
	{
		CommandSource commandSource;
		commandSource.SetCommand("D 500 1,2,3,4,5,6,7,8");

		Command* pCommand = commandSource.GetCommand(0);

		AssertCommand(pCommand, "D 500 1,2,3,4,5,6,7,8", 0);
	}

	static void TestSingleTwice()
	{
		CommandSource commandSource;
		commandSource.SetCommand("D 500 1,2,3,4,5,6,7,8");

		Command* pCommand = commandSource.GetCommand(0);

		AssertCommand(pCommand, "D 500 1,2,3,4,5,6,7,8", 0);

		pCommand = commandSource.GetCommand(0);

		AssertCommand(pCommand, "D 500 1,2,3,4,5,6,7,8", 0);
	}

	static void TestMultiple()
	{
		CommandSource commandSource;
		commandSource.SetCommand("D 500 1,2,3,4,5,6,7,8\nxxx");

		Command* pCommand = commandSource.GetCommand(0);

		AssertCommand(pCommand, "D 500 1,2,3,4,5,6,7,8", 0);

		pCommand = commandSource.GetCommand(1);

		AssertCommand(pCommand, "xxx", 1);

		pCommand = commandSource.GetCommand(2);
		Assert::AreEqual(0, (int) pCommand);
	}

	static void TestMultipleWithNewline()
	{
		CommandSource commandSource;

		commandSource.SetCommand("D1,2,3,4,5,6,7,8\nxxx");

		Command* pCommand = commandSource.GetCommand(0);

		AssertCommand(pCommand, "D1,2,3,4,5,6,7,8", 0);

		pCommand = commandSource.GetCommand(1);

		AssertCommand(pCommand, "xxx", 1);
	}

	static void TestMultipleWithNewlineAndCarriageReturn()
	{
		CommandSource commandSource;

		commandSource.SetCommand("D1,2,3,4,5,6,7,8\r\nxxx");

		Command* pCommand = commandSource.GetCommand(0);

		AssertCommand(pCommand, "D1,2,3,4,5,6,7,8", 0);

		pCommand = commandSource.GetCommand(1);

		AssertCommand(pCommand, "xxx", 1);
	}

	static void TestMultipleWithSpaces()
	{
		CommandSource commandSource;

		commandSource.SetCommand("  D1,2,3,4,5,6,7,8\n   xxx");

		Command* pCommand = commandSource.GetCommand(0);

		AssertCommand(pCommand, "D1,2,3,4,5,6,7,8", 0);

		pCommand = commandSource.GetCommand(1);

		AssertCommand(pCommand, "xxx", 1);
	}

	static void TestWithoutSetCommand()
	{
		CommandSource commandSource;

		Command* pCommand = commandSource.GetCommand(0);

		Assert::AreEqual(0, (int)pCommand);
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

