#pragma once
#include "CommandFormatter.h"

class CommandFormatterTest
{
	static void SimpleTest()
	{
	}

	static void TestIndent()
	{
		Assert::AreEqual(0, CommandFormatter::GetIndentPre(Command("LOOP", 0)));
		Assert::AreEqual(0, CommandFormatter::GetIndentPre(Command("D 15 1,15", 0)));
		Assert::AreEqual(0, CommandFormatter::GetIndentPre(Command("S 15 1,2,3,4,5", 0)));
		Assert::AreEqual(-1, CommandFormatter::GetIndentPre(Command("ENDLOOP", 0)));

		Assert::AreEqual(1, CommandFormatter::GetIndentPost(Command("LOOP", 0)));
		Assert::AreEqual(0, CommandFormatter::GetIndentPost(Command("D 15 1,15", 0)));
		Assert::AreEqual(0, CommandFormatter::GetIndentPost(Command("S 15 1,2,3,4,5", 0)));
		Assert::AreEqual(0, CommandFormatter::GetIndentPost(Command("ENDLOOP", 0)));

	}


	static void TestFormat()
	{
		char buffer[1024];
		CommandFormatter::PrettyFormat("$D1,2,3,4,5,6,7,8", buffer, 1024);
		Assert::AreEqual("$D1,2,3,4,5,6,7,8", buffer);
	}

	static void TestFormatLoop()
	{
		char buffer[1024];
		CommandFormatter::PrettyFormat("$LOOP %D 0:1$D1,2,3,4,5,6,7,8$ENDLOOP", buffer, 1024);
		Assert::AreEqual("$LOOP %D 0:1\n  $D1,2,3,4,5,6,7,8\n$ENDLOOP", buffer);
	}

public:
	static void Run()
	{
		TestIndent();

		TestFormat();
		TestFormatLoop();
	}
};