#include "SafeString.h"

class SafeStringTest
{
	static void TestStringLength()
	{
		Assert::AreEqual(0, SafeString::StringLength(""));

		Assert::AreEqual(1, SafeString::StringLength("a"));
		Assert::AreEqual(6, SafeString::StringLength("abdddd"));
	}

	static void TestStringCopySubset()
	{
		char buffer[10];

		SafeString::StringCopy(buffer, "12345", sizeof(buffer));
		Assert::AreEqual("12345", buffer);
	}

	static void TestStringCopyMax()
	{
		char buffer[10];

		SafeString::StringCopy(buffer, "123456789", sizeof(buffer));
		Assert::AreEqual("123456789", buffer);
	}

	static void TestStringCopyOverflow()
	{
		char buffer[10];

		SafeString::StringCopy(buffer, "1234567890", sizeof(buffer));
		Assert::AreEqual("123456789", buffer);
	}

	static void TestStringCatSimple()
	{
		char buffer[10];

		SafeString::StringCopy(buffer, "123", sizeof(buffer));
		SafeString::StringCat(buffer, "987", sizeof(buffer));
		Assert::AreEqual("123987", buffer);
	}

	static void TestStringCatMax()
	{
		char buffer[10];

		SafeString::StringCopy(buffer, "123", sizeof(buffer));
		SafeString::StringCat(buffer, "987654", sizeof(buffer));
		Assert::AreEqual("123987654", buffer);
	}

	static void TestStringCatOverflow()
	{
		char buffer[10];

		SafeString::StringCopy(buffer, "123", sizeof(buffer));
		SafeString::StringCat(buffer, "987654abc", sizeof(buffer));
		Assert::AreEqual("123987654", buffer);
	}


	static void TestStringCopyCountSimple()
	{
		char buffer[10];

		SafeString::StringCopyCount(buffer, "1234567890", sizeof(buffer), 2);
		Assert::AreEqual("12", buffer);
	}

	static void TestStringCopyCountBufferMax()
	{
		char buffer[10];

		SafeString::StringCopyCount(buffer, "123456789", sizeof(buffer), 9);
		Assert::AreEqual("123456789", buffer);
	}


	static void TestStringCopyCountBufferOverflow()
	{
		char buffer[10];

		SafeString::StringCopyCount(buffer, "1234567890123", sizeof(buffer), 13);
		Assert::AreEqual("123456789", buffer);
	}

public:
	static void Run()
	{
		TestStringLength();

		TestStringCopySubset();
		TestStringCopyMax();
		TestStringCopyOverflow();

		TestStringCatSimple();
		TestStringCatMax();
		TestStringCatOverflow();

		TestStringCopyCountSimple();
		TestStringCopyCountBufferMax();
		TestStringCopyCountBufferOverflow();
	}
};
