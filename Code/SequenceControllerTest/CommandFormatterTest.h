#pragma once
#include "CommandFormatter.h"

class CommandFormatterTest
{
	static void SimpleTest()
	{
	}

	static void TestIndent()
	{
		Assert::AreEqual(0, CommandFormatter::GetIndentPre(&Command("for", 0)));
		Assert::AreEqual(0, CommandFormatter::GetIndentPre(&Command("func", 0)));
		Assert::AreEqual(0, CommandFormatter::GetIndentPre(&Command("if", 0)));
		Assert::AreEqual(0, CommandFormatter::GetIndentPre(&Command("D 15 1,15", 0)));
		Assert::AreEqual(0, CommandFormatter::GetIndentPre(&Command("S 15 1,2,3,4,5", 0)));
		Assert::AreEqual(-1, CommandFormatter::GetIndentPre(&Command("endfor", 0)));
		Assert::AreEqual(-1, CommandFormatter::GetIndentPre(&Command("endfunc", 0)));
		Assert::AreEqual(-1, CommandFormatter::GetIndentPre(&Command("endif", 0)));
		Assert::AreEqual(-1, CommandFormatter::GetIndentPre(&Command("ELSEif", 0)));
		Assert::AreEqual(-1, CommandFormatter::GetIndentPre(&Command("ELSE", 0)));

		Assert::AreEqual(1, CommandFormatter::GetIndentPost(&Command("for", 0)));
		Assert::AreEqual(1, CommandFormatter::GetIndentPost(&Command("func", 0)));
		Assert::AreEqual(1, CommandFormatter::GetIndentPost(&Command("if", 0)));
		Assert::AreEqual(0, CommandFormatter::GetIndentPost(&Command("D 15 1,15", 0)));
		Assert::AreEqual(0, CommandFormatter::GetIndentPost(&Command("S 15 1,2,3,4,5", 0)));
		Assert::AreEqual(0, CommandFormatter::GetIndentPost(&Command("endfor", 0)));
		Assert::AreEqual(0, CommandFormatter::GetIndentPost(&Command("endfunc", 0)));
		Assert::AreEqual(0, CommandFormatter::GetIndentPost(&Command("endif", 0)));
		Assert::AreEqual(1, CommandFormatter::GetIndentPost(&Command("ELSEif", 0)));
		Assert::AreEqual(1, CommandFormatter::GetIndentPost(&Command("ELSE", 0)));
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
		CommandFormatter::PrettyFormat("for %D 0:1\nD1,2,3,4,5,6,7,8\nendfor", buffer, 1024);
		Assert::AreEqual("for %D 0:1\n  D1,2,3,4,5,6,7,8\nendfor", buffer);
	}

	static void TestFormatLoop2()
	{
		char buffer[1024];
		CommandFormatter::PrettyFormat("for %B 100:10 : -10\nfor %A 0 : 7\nD %B %A, 1.0\nD %B %A, 0.0\nendfor\nendfor", buffer, 1024);
		Assert::AreEqual("for %B 100:10 : -10\n  for %A 0 : 7\n    D %B %A, 1.0\n    D %B %A, 0.0\n  endfor\nendfor", buffer);
	}

	static void TestFormatLoopRepeat()
	{
		char buffer[1024];
		CommandFormatter::PrettyFormat("for %B 100:10 : -10\nfor %A 0 : 7\nD %B %A, 1.0\nD %B %A, 0.0\nendfor\nendfor", buffer, 1024);
		Assert::AreEqual("for %B 100:10 : -10\n  for %A 0 : 7\n    D %B %A, 1.0\n    D %B %A, 0.0\n  endfor\nendfor", buffer);

		char buffer2[1024];
		CommandFormatter::PrettyFormat(buffer, buffer2, 1024);
		Assert::AreEqual("for %B 100:10 : -10\n  for %A 0 : 7\n    D %B %A, 1.0\n    D %B %A, 0.0\n  endfor\nendfor", buffer2);
	}

	static void TestPreserveWhitespace()
	{
		const char* pProgram = "for A 1:10\n\r\n\rendfor";

		char buffer[1024];
		CommandFormatter::PrettyFormat(pProgram, buffer, 1024);
		Assert::AreEqual("for A 1:10\n  \nendfor", buffer);
	}

public:
	static void Run()
	{
		TestIndent();

		TestFormat();
		TestFormatLoop();

		TestFormatLoop2();

		TestPreserveWhitespace();

		//TestFormatLoopRepeat();
	}
};