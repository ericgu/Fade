#pragma once

#include "ParseErrors.h"

class ParseErrorsTest
{
	static void Test()
	{
		ParseErrors parseErrors;

		Assert::AreEqual(0, parseErrors.GetErrorCount());
	}

	static void TestSingleError()
	{
		ParseErrors parseErrors;

		parseErrors.AddError("Invalid value: ", "Value x", 1);

		Assert::AreEqual(1, parseErrors.GetErrorCount());
		ParseError parseError = parseErrors.GetError(0);
		Assert::AreEqual("Invalid value: Value x", parseError._errorText);
		Assert::AreEqual(1, parseError._lineNumber);
	}

	// line number

public:
	static void Run()
	{
		Test();
		TestSingleError();
	}
};