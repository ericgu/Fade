#pragma once

class LoopTest
{
	static void TestSimple()
	{
		ExecutionContext executionContext;
		Loop loop = Loop::Parse("FOR B11 2:7", executionContext, 3);

		Assert::AreEqual(1, loop.GetMatch());
		Assert::AreEqual("B11", loop.GetVariableName());
		Assert::AreEqual(2.0F, loop.GetVariableStart().GetValueFloat());
		Assert::AreEqual(7.0F, loop.GetVariableEnd().GetValueFloat());
		Assert::AreEqual(1.0F, loop.GetVariableInc().GetValueFloat());
	}

	static void ValidateError(ExecutionContext& executionContext, int errorCount, const char* pExpectedString, int lineNumber)
	{
		Assert::AreEqual(errorCount, executionContext._parseErrors.GetErrorCount());
		if (errorCount != executionContext._parseErrors.GetErrorCount())
		{
			return;
		}
		Assert::AreEqual(pExpectedString, executionContext._parseErrors.GetError(errorCount - 1)._errorText);
		Assert::AreEqual(lineNumber, executionContext._parseErrors.GetError(errorCount - 1)._lineNumber);
	}

	static void TestNoFor()
	{
		ExecutionContext executionContext;
		Loop loop = Loop::Parse("junkjunkjunk", executionContext, 3);

		Assert::AreEqual(0, loop.GetMatch());
	}

	static void TestMissingVariable()
	{
		ExecutionContext executionContext;
		Loop loop = Loop::Parse("FOR ", executionContext, 3);

		ValidateError(executionContext, 1, "Error in FOR: missing variable name", 3);
	}

	static void TestMissingMinimum()
	{
		ExecutionContext executionContext;
		Loop loop = Loop::Parse("FOR A ", executionContext, 3);

		ValidateError(executionContext, 1, "Error in FOR: missing range value(s)", 3);
	}

	static void TestMissingColon()
	{
		ExecutionContext executionContext;
		Loop loop = Loop::Parse("FOR A 1", executionContext, 3);

		ValidateError(executionContext, 1, "Error in FOR: missing range value(s)", 3);
	}

	static void TestMissingMin()
	{
		ExecutionContext executionContext;
		Loop loop = Loop::Parse("FOR A :", executionContext, 3);

		ValidateError(executionContext, 1, "Error in FOR: missing range value(s)", 3);
	}

	static void TestMissingMax()
	{
		ExecutionContext executionContext;
		Loop loop = Loop::Parse("FOR A 1:", executionContext, 3);

		ValidateError(executionContext, 1, "Error in FOR: missing range value(s)", 3);
	}

	static void TestWithIncrement()
	{
		ExecutionContext executionContext;
		Loop loop = Loop::Parse("FOR Variable 2:3:0.5", executionContext, 3);

		Assert::AreEqual(1, loop.GetMatch());
		Assert::AreEqual("Variable", loop.GetVariableName());
		Assert::AreEqual(2.0F, loop.GetVariableStart().GetValueFloat());
		Assert::AreEqual(3.0F, loop.GetVariableEnd().GetValueFloat());
		Assert::AreEqual(0.5F, loop.GetVariableInc().GetValueFloat());
	}

	static void TestInRangeCheck()
	{
		ExecutionContext executionContext;
		Loop loop = Loop::Parse("FOR B 2:3:0.5", executionContext, 3);

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

