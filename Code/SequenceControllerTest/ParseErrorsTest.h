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

	static void TestFormat()
	{
		ParseErrors parseErrors;

		parseErrors.AddError("Another error: ", "bad stuff", 5);
		parseErrors.AddError("Invalid value: ", "Value x", 2);

		char* pErrors = parseErrors.FormatErrors();

		Assert::AreEqual("\n\nInvalid value: Value x\n\n\nAnother error: bad stuff\n", pErrors);
	}

	static void TestFormat2()
	{
		ParseErrors parseErrors;

		parseErrors.AddError("minus line number: ", "bad stuff", -1);

		char* pErrors = parseErrors.FormatErrors();

		Assert::AreEqual("minus line number: bad stuff", pErrors);
	}

	static void TestClear()
	{
		ParseErrors parseErrors;

		parseErrors.AddError("Invalid value: ", "Value x", 1);
		parseErrors.Clear();

		Assert::AreEqual(0, parseErrors.GetErrorCount());
	}

	static void TestRepeatedError()
	{
		ParseErrors parseErrors;

		parseErrors.AddError("Invalid value: ", "Value x", 1);
		parseErrors.AddError("Invalid value: ", "Value x", 1);

		Assert::AreEqual(1, parseErrors.GetErrorCount());
		ParseError parseError = parseErrors.GetError(0);
		Assert::AreEqual("Invalid value: Value x", parseError._errorText);
		Assert::AreEqual(1, parseError._lineNumber);
	}

public:
	static void Run()
	{
		Test();
		TestSingleError();
		TestFormat();
		TestFormat2();
		TestClear();
		TestRepeatedError();
	}
};