#pragma once
#include "ListParser.h"

class ListParserTest
{
	static void TestZero()
	{
		ListParser listParser(",", "");

		Assert::AreEqual(0, listParser.GetCount());
	}

	static void TestOne()
	{
		ListParser listParser(",", "value");

		Assert::AreEqual(1, listParser.GetCount());
		Assert::AreEqual("value", listParser.GetItem(0));
	}

	static void TestThree()
	{
		ListParser listParser(",", "one,two,three");

		Assert::AreEqual(3, listParser.GetCount());
		Assert::AreEqual("one", listParser.GetItem(0));
		Assert::AreEqual("two", listParser.GetItem(1));
		Assert::AreEqual("three", listParser.GetItem(2));
	}

	static void TestThreeWithColon()
	{
		ListParser listParser(":", "one:two:three");

		Assert::AreEqual(3, listParser.GetCount());
		Assert::AreEqual("one", listParser.GetItem(0));
		Assert::AreEqual("two", listParser.GetItem(1));
		Assert::AreEqual("three", listParser.GetItem(2));
	}

	static void TestThreeWithTwoDelimiters()
	{
		ListParser listParser(":$", "one:$two$:three");

		Assert::AreEqual(3, listParser.GetCount());
		Assert::AreEqual("one", listParser.GetItem(0));
		Assert::AreEqual("two", listParser.GetItem(1));
		Assert::AreEqual("three", listParser.GetItem(2));
	}

public:
	static void Run()
	{
		TestZero();
		TestOne();
		TestThree();
		TestThreeWithColon();
		TestThreeWithTwoDelimiters();
	}
};