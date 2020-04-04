#pragma once

class LoopTest
{
	static void TestSimple()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;

		Loop loop;
		int result = loop.Parse("FOR B11 2:7", &executionContext, &parseErrors, 3);

		Assert::AreEqual(1, result);
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
		Loop loop;
		int result = loop.Parse("junkjunkjunk", &executionContext, &parseErrors, 3);

		Assert::AreEqual(0, result);
	}

	static void TestMissingVariable()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		Loop loop;
		loop.Parse("FOR ", &executionContext, &parseErrors, 3);

		ValidateError(parseErrors, 1, "Error in FOR: missing variable name", 3);
	}

	static void TestMissingMinimum()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		Loop loop;
		loop.Parse("FOR A ", &executionContext, &parseErrors, 3);

		ValidateError(parseErrors, 1, "Error in FOR: missing range value(s)", 3);
	}

	static void TestMissingColon()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		Loop loop;
		loop.Parse("FOR A 1", &executionContext, &parseErrors, 3);

		ValidateError(parseErrors, 1, "Error in FOR: missing range value(s)", 3);
	}

	static void TestMissingMin()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		Loop loop;
		loop.Parse("FOR A :", &executionContext, &parseErrors, 3);

		ValidateError(parseErrors, 1, "Error in FOR: missing range value(s)", 3);
	}

	static void TestMissingMax()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		Loop loop;
		loop.Parse("FOR A 1:", &executionContext, &parseErrors, 3);

		ValidateError(parseErrors, 1, "Error in FOR: missing range value(s)", 3);
	}

	static void TestWithIncrement()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		Loop loop;
		int result = loop.Parse("FOR Variable 2:3:0.5", &executionContext, &parseErrors, 3);

		Assert::AreEqual(1, result);
		Assert::AreEqual("Variable", loop.GetVariableName());
		Assert::AreEqual(2.0F, loop.GetVariableStart().GetValueFloat(0));
		Assert::AreEqual(3.0F, loop.GetVariableEnd().GetValueFloat(0));
		Assert::AreEqual(0.5F, loop.GetVariableInc().GetValueFloat(0));
	}

	static void TestInRangeCheck()
	{
		Assert::AreEqual(0, Loop::GetIsInRange(2, 3, 0.99F));
		Assert::AreEqual(1, Loop::GetIsInRange(2, 3, 2.0F));
		Assert::AreEqual(1, Loop::GetIsInRange(2, 3, 2.5F));
		Assert::AreEqual(1, Loop::GetIsInRange(2, 3, 3.0F));
		Assert::AreEqual(0, Loop::GetIsInRange(2, 3, 3.01F));
	}

	static void TestWithDoubleSecondCallNotAFor()
	{
		ExecutionContext executionContext;
		ParseErrors parseErrors;
		Loop loop;
		int result = loop.Parse("FOR Variable 2:3:0.5", &executionContext, &parseErrors, 3);
		Assert::AreEqual(1, result);

		loop.Parse("junk", &executionContext, &parseErrors, 3);

		Assert::AreEqual("Variable", loop.GetVariableName());
		Assert::AreEqual(2.0F, loop.GetVariableStart().GetValueFloat(0));
		Assert::AreEqual(3.0F, loop.GetVariableEnd().GetValueFloat(0));
		Assert::AreEqual(0.5F, loop.GetVariableInc().GetValueFloat(0));
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
		TestWithDoubleSecondCallNotAFor();

		return 0;
	}
};

