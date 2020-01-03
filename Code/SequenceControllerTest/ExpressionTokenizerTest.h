#pragma once

#include "ExpressionTokenizer.h"

class ExpressionTokenizerTest
{
	static void Assert(ExpressionTokenizer expressionTokenizer, int tokenNumber, const char* pNodeString)
	{
		char buffer[128];

		int tokenLength = strlen(pNodeString);
		ExpressionNode* pNode = expressionTokenizer.GetNode(tokenNumber);
		strncpy(buffer, pNode->_pItem, pNode->_pItemLength);
		buffer[pNode->_pItemLength] = '\0';
		Assert::AreEqual(pNodeString, buffer);
		Assert::AreEqual(tokenLength, pNode->_pItemLength);
		Assert::AreEqual(true, pNode->_pValue.IsNan());
	}

	static void Simple()
	{
		ExpressionTokenizer expressionTokenizer;

		expressionTokenizer.Tokenize("13.0");

		Assert::AreEqual(1, expressionTokenizer.GetNodeCount());
		Assert(expressionTokenizer, 0, "13.0");
	}

	static void BinaryPlus()
	{
		ExpressionTokenizer expressionTokenizer;

		expressionTokenizer.Tokenize("13.0+10.0");

		Assert::AreEqual(3, expressionTokenizer.GetNodeCount());
		Assert(expressionTokenizer, 0, "13.0");
		Assert(expressionTokenizer, 1, "+");
		Assert(expressionTokenizer, 2, "10.0");
	}

	static void TestAll()
	{
		ExpressionTokenizer expressionTokenizer;

		expressionTokenizer.Tokenize("1+2-3*4/5==6!=7>=8>9<=10<11&&12||13!14%15,16");

		Assert::AreEqual(31, expressionTokenizer.GetNodeCount());
		Assert(expressionTokenizer, 0, "1");
		Assert(expressionTokenizer, 1, "+");
		Assert(expressionTokenizer, 2, "2");
		Assert(expressionTokenizer, 3, "-");
		Assert(expressionTokenizer, 4, "3");
		Assert(expressionTokenizer, 5, "*");
		Assert(expressionTokenizer, 6, "4");
		Assert(expressionTokenizer, 7, "/");
		Assert(expressionTokenizer, 8, "5");
		Assert(expressionTokenizer, 9, "==");
		Assert(expressionTokenizer, 10, "6");
		Assert(expressionTokenizer, 11, "!=");
		Assert(expressionTokenizer, 12, "7");
		Assert(expressionTokenizer, 13, ">=");
		Assert(expressionTokenizer, 14, "8");
		Assert(expressionTokenizer, 15, ">");
		Assert(expressionTokenizer, 16, "9");
		Assert(expressionTokenizer, 17, "<=");
		Assert(expressionTokenizer, 18, "10");
		Assert(expressionTokenizer, 19, "<");
		Assert(expressionTokenizer, 20, "11");
		Assert(expressionTokenizer, 21, "&&");
		Assert(expressionTokenizer, 22, "12");
		Assert(expressionTokenizer, 23, "||");
		Assert(expressionTokenizer, 24, "13");
		Assert(expressionTokenizer, 25, "!");
		Assert(expressionTokenizer, 26, "14");
		Assert(expressionTokenizer, 27, "%");
		Assert(expressionTokenizer, 28, "15");
		Assert(expressionTokenizer, 29, ",");
		Assert(expressionTokenizer, 30, "16");
	}

	static void TestParens()
	{
		ExpressionTokenizer expressionTokenizer;

		expressionTokenizer.Tokenize("(1+2)*3");

		Assert::AreEqual(7, expressionTokenizer.GetNodeCount());
		Assert(expressionTokenizer, 0, "(");
		Assert(expressionTokenizer, 1, "1");
		Assert(expressionTokenizer, 2, "+");
		Assert(expressionTokenizer, 3, "2");
		Assert(expressionTokenizer, 4, ")");
		Assert(expressionTokenizer, 5, "*");
		Assert(expressionTokenizer, 6, "3");
	}

	static void TestParens2()
	{
		ExpressionTokenizer expressionTokenizer;

		expressionTokenizer.Tokenize("(1+2)");

		Assert::AreEqual(5, expressionTokenizer.GetNodeCount());
		Assert(expressionTokenizer, 0, "(");
		Assert(expressionTokenizer, 1, "1");
		Assert(expressionTokenizer, 2, "+");
		Assert(expressionTokenizer, 3, "2");
		Assert(expressionTokenizer, 4, ")");
	}

	static void TestWithWhiteSpace()
	{
		ExpressionTokenizer expressionTokenizer;

		expressionTokenizer.Tokenize(" (	1 + 2 )	");

		Assert::AreEqual(5, expressionTokenizer.GetNodeCount());
		Assert(expressionTokenizer, 0, "(");
		Assert(expressionTokenizer, 1, "1");
		Assert(expressionTokenizer, 2, "+");
		Assert(expressionTokenizer, 3, "2");
		Assert(expressionTokenizer, 4, ")");
	}

	static void TestEmptyNodes()
	{
		ExpressionTokenizer expressionTokenizer;

		expressionTokenizer.Tokenize("1+2");

		ExpressionNode* pOne = expressionTokenizer.GetNode(0);
		ExpressionNode* pTwo = expressionTokenizer.GetNode(2);
		
		expressionTokenizer.SetNodeEmpty(2);
		expressionTokenizer.SetNodeEmpty(0);
		
		Assert::AreEqual(true, pOne->IsEmpty());
		Assert::AreEqual(0, (int) pOne->_pItem);
		Assert::AreEqual(true, pOne->_pValue.IsNan());
		Assert(expressionTokenizer, 1, "+");
		Assert::AreEqual(true, pTwo->IsEmpty());
		Assert::AreEqual(0, (int) pTwo->_pItem);
		Assert::AreEqual(true, pTwo->_pValue.IsNan());
	}

public:
	static void Run()
	{
		Simple();
		BinaryPlus();
		TestAll();
		TestParens();
		TestParens2();
		TestEmptyNodes();
		TestWithWhiteSpace();
	}
};