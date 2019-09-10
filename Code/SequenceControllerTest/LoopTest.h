#pragma once

class LoopTest
{
	static void TestSimple()
	{
		Loop loop = Loop::Parse("LOOP %B 2:7");

		Assert::AreEqual(1, loop.GetMatch());
		Assert::AreEqual(1, loop.GetVariableNumber());
		Assert::AreEqual(2.0F, loop.GetVariableStart());
		Assert::AreEqual(7.0F, loop.GetVariableEnd());
		Assert::AreEqual(1.0F, loop.GetVariableInc());
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

	static void TestWithIncrement()
	{
		Loop loop = Loop::Parse("LOOP %B 2:3:0.5");

		Assert::AreEqual(1, loop.GetMatch());
		Assert::AreEqual(1, loop.GetVariableNumber());
		Assert::AreEqual(2.0F, loop.GetVariableStart());
		Assert::AreEqual(3.0F, loop.GetVariableEnd());
		Assert::AreEqual(0.5F, loop.GetVariableInc());
	}

	static void TestInRangeCheck()
	{
		Loop loop = Loop::Parse("LOOP %B 2:3:0.5");

		Assert::AreEqual(0, loop.GetIsInRange(0.99F));
		Assert::AreEqual(1, loop.GetIsInRange(2.0F));
		Assert::AreEqual(1, loop.GetIsInRange(2.5F));
		Assert::AreEqual(1, loop.GetIsInRange(3.0F));
		Assert::AreEqual(0, loop.GetIsInRange(3.01F));
	}

public:

	static int Run()
	{
		TestSimple();
		TestNoMatch();
		TestMissingPercent();
		TestInvalidVariable();
		TestMissingColon();
		TestWithIncrement();

		TestInRangeCheck();

		return 0;
	}
};

