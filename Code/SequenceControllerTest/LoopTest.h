#pragma once

class LoopTest
{
	static void TestFirst()
	{
		Loop loop = Loop::Parse("LOOP %B 2:7");

		Assert::AreEqual(1, loop.GetMatch());
		Assert::AreEqual(1, loop.GetVariableNumber());
		Assert::AreEqual(2, loop.GetVariableMin());
		Assert::AreEqual(7, loop.GetVariableMax());
	}

	static void TestNoMatch()
	{
		Loop loop = Loop::Parse("junkjunkjunk");

		Assert::AreEqual(0, loop.GetMatch());
		Assert::AreEqual(0, loop.GetErrorOffset());
	}

	static void TestMissingPercent()
	{
		Loop loop = Loop::Parse("LOOP FFF");

		Assert::AreEqual(0, loop.GetMatch());
		Assert::AreEqual(5, loop.GetErrorOffset());
	}

	static void TestInvalidVariable()
	{
		Loop loop = Loop::Parse("LOOP %X");

		Assert::AreEqual(0, loop.GetMatch());
		Assert::AreEqual(6, loop.GetErrorOffset());
	}

	static void TestMissingColon()
	{
		Loop loop = Loop::Parse("LOOP %A 1");

		Assert::AreEqual(0, loop.GetMatch());
		Assert::AreEqual(9, loop.GetErrorOffset());
	}

public:

	static int Run()
	{
		TestFirst();
		TestNoMatch();
		TestMissingPercent();
		TestInvalidVariable();
		TestMissingColon();

		return 0;
	}
};

