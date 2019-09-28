#pragma once

class LoopTest
{
	static void TestSimple()
	{
		Loop loop = Loop::Parse("FOR B11 2:7");

		Assert::AreEqual(1, loop.GetMatch());
		Assert::AreEqual("B11", loop.GetVariableName());
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

	static void TestMissingColon()
	{
		Loop loop = Loop::Parse("FOR A 1");

		Assert::AreEqual(0, loop.GetMatch());
		Assert::AreEqual(7, loop.GetErrorOffset());
	}

	static void TestWithIncrement()
	{
		Loop loop = Loop::Parse("FOR Variable 2:3:0.5");

		Assert::AreEqual(1, loop.GetMatch());
		Assert::AreEqual("Variable", loop.GetVariableName());
		Assert::AreEqual(2.0F, loop.GetVariableStart());
		Assert::AreEqual(3.0F, loop.GetVariableEnd());
		Assert::AreEqual(0.5F, loop.GetVariableInc());
	}

	static void TestInRangeCheck()
	{
		Loop loop = Loop::Parse("FOR B 2:3:0.5");

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
		TestMissingColon();
		TestWithIncrement();

		TestInRangeCheck();

		return 0;
	}
};

