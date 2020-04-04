#pragma once
#include "CommandFormatter.h"

class CommandFormatterTest
{
	static void SimpleTest()
	{
	}

	static void TestIndent()
	{
		Assert::AreEqual(0, CommandFormatter::GetIndentPre(&Command("FOR", 0)));
		Assert::AreEqual(0, CommandFormatter::GetIndentPre(&Command("D 15 1,15", 0)));
		Assert::AreEqual(0, CommandFormatter::GetIndentPre(&Command("S 15 1,2,3,4,5", 0)));
		Assert::AreEqual(-1, CommandFormatter::GetIndentPre(&Command("ENDFOR", 0)));

		Assert::AreEqual(1, CommandFormatter::GetIndentPost(&Command("FOR", 0)));
		Assert::AreEqual(0, CommandFormatter::GetIndentPost(&Command("D 15 1,15", 0)));
		Assert::AreEqual(0, CommandFormatter::GetIndentPost(&Command("S 15 1,2,3,4,5", 0)));
		Assert::AreEqual(0, CommandFormatter::GetIndentPost(&Command("ENDFOR", 0)));

	}


	static void TestFormat()
	{
		char buffer[1024];
		CommandFormatter::PrettyFormat("D1,2,3,4,5,6,7,8", buffer, 1024);
		Assert::AreEqual("D1,2,3,4,5,6,7,8", buffer);
	}

	static void TestFormatLoop()
	{
		char buffer[1024];
		CommandFormatter::PrettyFormat("FOR %D 0:1\nD1,2,3,4,5,6,7,8\nENDFOR", buffer, 1024);
		Assert::AreEqual("FOR %D 0:1\n  D1,2,3,4,5,6,7,8\nENDFOR", buffer);
	}

	static void TestFormatLoop2()
	{
		char buffer[1024];
		CommandFormatter::PrettyFormat("FOR %B 100:10 : -10\nFOR %A 0 : 7\nD %B %A, 1.0\nD %B %A, 0.0\nENDFOR\nENDFOR", buffer, 1024);
		Assert::AreEqual("FOR %B 100:10 : -10\n  FOR %A 0 : 7\n    D %B %A, 1.0\n    D %B %A, 0.0\n  ENDFOR\nENDFOR", buffer);
	}

	static void TestFormatLoopRepeat()
	{
		char buffer[1024];
		CommandFormatter::PrettyFormat("FOR %B 100:10 : -10\nFOR %A 0 : 7\nD %B %A, 1.0\nD %B %A, 0.0\nENDFOR\nENDFOR", buffer, 1024);
		Assert::AreEqual("FOR %B 100:10 : -10\n  FOR %A 0 : 7\n    D %B %A, 1.0\n    D %B %A, 0.0\n  ENDFOR\nENDFOR", buffer);

		char buffer2[1024];
		CommandFormatter::PrettyFormat(buffer, buffer2, 1024);
		Assert::AreEqual("FOR %B 100:10 : -10\n  FOR %A 0 : 7\n    D %B %A, 1.0\n    D %B %A, 0.0\n  ENDFOR\nENDFOR", buffer2);
	}

public:
	static void Run()
	{
		TestIndent();

		TestFormat();
		TestFormatLoop();

		TestFormatLoop2();

		//TestFormatLoopRepeat();
	}
};