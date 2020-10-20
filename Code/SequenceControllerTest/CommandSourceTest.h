#include "CommandSource.h"

#pragma once
class CommandSourceTest
{
	static void AssertCommand(Command* pCommand, const char* commandString, int lineNumber)
	{
		Assert::AreEqual(commandString, pCommand->GetString());
		Assert::AreEqual(lineNumber, pCommand->GetLineNumber());
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
		Assert::AreEqual(0, pCommand);
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

		Assert::AreEqual(0, pCommand);
	}

	static void TestManyLinesForward()
	{
		CommandSource commandSource;

		commandSource.SetCommand("  a\n b\n c\n d\n e\n f");

		AssertCommand(commandSource.GetCommand(0), "a", 0);
		AssertCommand(commandSource.GetCommand(1), "b", 1);
		AssertCommand(commandSource.GetCommand(2), "c", 2);
		AssertCommand(commandSource.GetCommand(3), "d", 3);
		AssertCommand(commandSource.GetCommand(4), "e", 4);
		AssertCommand(commandSource.GetCommand(5), "f", 5);
	}

	static void TestManyLinesBackward()
	{
		CommandSource commandSource;

		commandSource.SetCommand("  a\n b\n c\n d\n e\n f");

		AssertCommand(commandSource.GetCommand(5), "f", 5);
		AssertCommand(commandSource.GetCommand(4), "e", 4);
		AssertCommand(commandSource.GetCommand(3), "d", 3);
		AssertCommand(commandSource.GetCommand(2), "c", 2);
		AssertCommand(commandSource.GetCommand(1), "b", 1);
		AssertCommand(commandSource.GetCommand(0), "a", 0);
	}

	static void TestBeyondEnd()
	{
		CommandSource commandSource;

		commandSource.SetCommand("  a");

		Command* pCommand = commandSource.GetCommand(0);

		AssertCommand(commandSource.GetCommand(0), "a", 0);

		Assert::AreEqual(0, commandSource.GetCommand(1));
		Assert::AreEqual(0, commandSource.GetCommand(2));
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

		TestManyLinesForward();
		TestManyLinesBackward();

		TestBeyondEnd();
		
		return 0;
	}
};

