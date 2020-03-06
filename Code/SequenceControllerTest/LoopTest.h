#pragma once

class LoopTest
{
	static void TestSimple()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;

		Loop loop = Loop::Parse("FOR B11 2:7", &executionContext, &parseErrors, 3);

		Assert::AreEqual(1, loop.GetMatch());
		Assert::AreEqual("B11", loop.GetVariableName());
		Assert::AreEqual(2.0F, loop.GetVariableStart().GetValueFloat(0));
		Assert::AreEqual(7.0F, loop.GetVariableEnd().GetValueFloat(0));
		Assert::AreEqual(1.0F, loop.GetVariableInc().GetValueFloat(0));
	}

	static void ValidateError(ParseErrors& parseErrors, int errorCount, const char* pExpectedString, int lineNumber)
	{
		Assert::AreEqual(errorCount, parseErrors.GetErrorCount());
		if (errorCount != parseErrors.GetErrorCount())
		{
			return;
		}
		Assert::AreEqual(pExpectedString, parseErrors.GetError(errorCount - 1)._errorText);
		Assert::AreEqual(lineNumber, parseErrors.GetError(errorCount - 1)._lineNumber);
	}

	static void TestNoFor()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		Loop loop = Loop::Parse("junkjunkjunk", &executionContext, &parseErrors, 3);

		Assert::AreEqual(0, loop.GetMatch());
	}

	static void TestMissingVariable()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		Loop loop = Loop::Parse("FOR ", &executionContext, &parseErrors, 3);

		ValidateError(parseErrors, 1, "Error in FOR: missing variable name", 3);
	}

	static void TestMissingMinimum()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		Loop loop = Loop::Parse("FOR A ", &executionContext, &parseErrors, 3);

		ValidateError(parseErrors, 1, "Error in FOR: missing range value(s)", 3);
	}

	static void TestMissingColon()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		Loop loop = Loop::Parse("FOR A 1", &executionContext, &parseErrors, 3);

		ValidateError(parseErrors, 1, "Error in FOR: missing range value(s)", 3);
	}

	static void TestMissingMin()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		Loop loop = Loop::Parse("FOR A :", &executionContext, &parseErrors, 3);

		ValidateError(parseErrors, 1, "Error in FOR: missing range value(s)", 3);
	}

	static void TestMissingMax()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		Loop loop = Loop::Parse("FOR A 1:", &executionContext, &parseErrors, 3);

		ValidateError(parseErrors, 1, "Error in FOR: missing range value(s)", 3);
	}

	static void TestWithIncrement()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		Loop loop = Loop::Parse("FOR Variable 2:3:0.5", &executionContext, &parseErrors, 3);

		Assert::AreEqual(1, loop.GetMatch());
		Assert::AreEqual("Variable", loop.GetVariableName());
		Assert::AreEqual(2.0F, loop.GetVariableStart().GetValueFloat(0));
		Assert::AreEqual(3.0F, loop.GetVariableEnd().GetValueFloat(0));
		Assert::AreEqual(0.5F, loop.GetVariableInc().GetValueFloat(0));
	}

	static void TestInRangeCheck()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		Loop loop = Loop::Parse("FOR B 2:3:0.5", &executionContext, &parseErrors, 3);

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
		TestNoFor();
		TestMissingVariable();
		TestMissingMinimum();
		TestMissingColon();
		TestMissingMin();
		TestMissingMax();
		TestWithIncrement();

		TestInRangeCheck();

		return 0;
	}
};

