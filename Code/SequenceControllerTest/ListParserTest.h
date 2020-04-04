#pragma once
#include "ListParser.h"

class ListParserTest
{
	static void TestZero()
	{
		ListParser listParser(128, 128);
		listParser.Parse(",", "");

		Assert::AreEqual(0, listParser.GetCount());
	}

	static void TestOne()
	{
		ListParser listParser(128, 128);
		listParser.Parse(",", "value");

		Assert::AreEqual(1, listParser.GetCount());
		Assert::AreEqual("value", listParser.GetItem(0));
	}

	static void TestThree()
	{
		ListParser listParser(128, 128);
		listParser.Parse(",", "one,two,three");

		Assert::AreEqual(3, listParser.GetCount());
		Assert::AreEqual("one", listParser.GetItem(0));
		Assert::AreEqual("two", listParser.GetItem(1));
		Assert::AreEqual("three", listParser.GetItem(2));
	}

	static void TestThreeWithColon()
	{
		ListParser listParser(128, 128);
		listParser.Parse(":", "one:two:three");

		Assert::AreEqual(3, listParser.GetCount());
		Assert::AreEqual("one", listParser.GetItem(0));
		Assert::AreEqual("two", listParser.GetItem(1));
		Assert::AreEqual("three", listParser.GetItem(2));
	}

	static void TestThreeWithTwoDelimiters()
	{
		ListParser listParser(128, 128);
		listParser.Parse(":$", "one:$two$:three");

		Assert::AreEqual(3, listParser.GetCount());
		Assert::AreEqual("one", listParser.GetItem(0));
		Assert::AreEqual("two", listParser.GetItem(1));
		Assert::AreEqual("three", listParser.GetItem(2));
	}

	static void TestTooMany()
	{
		ListParser listParser(12, 1);

		Serial.SetOutput(0);
		listParser.Parse(",", "one,two");
		Serial.SetOutput(1);

		Assert::AreEqual("List to parse had too many items...", Serial.GetLastString());
	}

public:
	static void Run()
	{
		TestZero();
		TestOne();
		TestThree();
		TestThreeWithColon();
		TestThreeWithTwoDelimiters();

		TestTooMany();
	}
};