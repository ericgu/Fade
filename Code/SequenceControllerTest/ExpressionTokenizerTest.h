#pragma once

#include "ExpressionTokenizer.h"

class ExpressionTokenizerTest
{
	static void Assert(ExpressionTokenizer expressionTokenizer, int tokenNumber, const char* pNodeString)
	{
		char buffer[128];

		int tokenLength = strlen(pNodeString);
		ExpressionNode* pNode = expressionTokenizer.GetNode(tokenNumber);
		strncpy(buffer, pNode->_pItem, pNode->_itemLength);
		buffer[pNode->_itemLength] = '\0';
		Assert::AreEqual(pNodeString, buffer);
		Assert::AreEqual(tokenLength, pNode->_itemLength);
		Assert::AreEqual(true, pNode->_value.IsNan());
	}

	static void Simple()
	{
		ExpressionTokenizer expressionTokenizer;

		ParseErrors parseErrors;
		expressionTokenizer.Tokenize("13.0", &parseErrors, 1);

		Assert::AreEqual(1, expressionTokenizer.GetNodeCount());
		Assert(expressionTokenizer, 0, "13.0");
	}

	static void BinaryPlus()
	{
		ExpressionTokenizer expressionTokenizer;

		ParseErrors parseErrors;
		expressionTokenizer.Tokenize("13.0+10.0", &parseErrors, 1);

		Assert::AreEqual(3, expressionTokenizer.GetNodeCount());
		Assert(expressionTokenizer, 0, "13.0");
		Assert(expressionTokenizer, 1, "+");
		Assert(expressionTokenizer, 2, "10.0");
	}

	static void TestAll()
	{
		ExpressionTokenizer expressionTokenizer;

		ParseErrors parseErrors;
		expressionTokenizer.Tokenize("1+2-3*4/5==6!=7>=8>9<=10<11&&12||13!14%15,16=17++18--19{20}21", &parseErrors, 1);

		Assert::AreEqual(41, expressionTokenizer.GetNodeCount());
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
		Assert(expressionTokenizer, 31, "=");
		Assert(expressionTokenizer, 32, "17");
		Assert(expressionTokenizer, 33, "++");
		Assert(expressionTokenizer, 34, "18");
		Assert(expressionTokenizer, 35, "--");
		Assert(expressionTokenizer, 36, "19");
		Assert(expressionTokenizer, 37, "{");
		Assert(expressionTokenizer, 38, "20");
		Assert(expressionTokenizer, 39, "}");
		Assert(expressionTokenizer, 40, "21");
	}

	static void TestParens()
	{
		ExpressionTokenizer expressionTokenizer;

		ParseErrors parseErrors;
		expressionTokenizer.Tokenize("(1+2)*3", &parseErrors, 1);

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

		ParseErrors parseErrors;
		expressionTokenizer.Tokenize("(1+2)", &parseErrors, 1);

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

		ParseErrors parseErrors;
		expressionTokenizer.Tokenize(" (	1 + 2 )	", &parseErrors, 1);

		Assert::AreEqual(5, expressionTokenizer.GetNodeCount());
		Assert(expressionTokenizer, 0, "(");
		Assert(expressionTokenizer, 1, "1");
		Assert(expressionTokenizer, 2, "+");
		Assert(expressionTokenizer, 3, "2");
		Assert(expressionTokenizer, 4, ")");
	}

	static void TestQuotedString()
	{
		ExpressionTokenizer expressionTokenizer;
		ParseErrors parseErrors;

		expressionTokenizer.Tokenize("= \" x + y = z \" *", &parseErrors, 1);

		Assert::AreEqual(3, expressionTokenizer.GetNodeCount());
		Assert(expressionTokenizer, 0, "=");
		Assert(expressionTokenizer, 1, "\" x + y = z \"");
		Assert(expressionTokenizer, 2, "*");
	}

	static void TestEmptyNodes()
	{
		ExpressionTokenizer expressionTokenizer;
		ParseErrors parseErrors;

		expressionTokenizer.Tokenize("1+2", &parseErrors, 1);

		ExpressionNode* pOne = expressionTokenizer.GetNode(0);
		ExpressionNode* pTwo = expressionTokenizer.GetNode(2);
		
		expressionTokenizer.SetNodeEmpty(2);
		expressionTokenizer.SetNodeEmpty(0);
		
		Assert::AreEqual(true, pOne->IsEmpty());
		Assert::AreEqual(0, (int) pOne->_pItem);
		Assert::AreEqual(true, pOne->_value.IsNan());
		Assert(expressionTokenizer, 1, "+");
		Assert::AreEqual(true, pTwo->IsEmpty());
		Assert::AreEqual(0, (int) pTwo->_pItem);
		Assert::AreEqual(true, pTwo->_value.IsNan());
	}

	static void TestFindMatchingCloseParen()
	{
		ExpressionTokenizer expressionTokenizer;
		ParseErrors parseErrors;

		expressionTokenizer.Tokenize("( 3 + 4 * ( 2 + 5))", &parseErrors, 1);

		expressionTokenizer.GetNode(1)->_value.SetValue(0, 15.0F);
		expressionTokenizer.GetNode(1)->_pItem = 0;

		Assert::AreEqual(11, expressionTokenizer.GetNodeCount());

		int matching = expressionTokenizer.FindMatchingParen(0);
		Assert::AreEqual(10, matching);

		matching = expressionTokenizer.FindMatchingParen(5);
		Assert::AreEqual(9, matching);

		matching = expressionTokenizer.FindMatchingParen(3);
		Assert::AreEqual(-1, matching);

		ExpressionTokenizer expressionTokenizer2;
		expressionTokenizer2.Tokenize("( 3 + 4 * ( 2 + 5", &parseErrors, 1);

		matching = expressionTokenizer2.FindMatchingParen(0);
		Assert::AreEqual(-2, matching);
	}

	static void TestFindFirstValue()
	{
		ExpressionTokenizer expressionTokenizer;
		ParseErrors parseErrors;

		int k = sizeof(ExpressionTokenizer);

		expressionTokenizer.Tokenize("( 3 + 4 )", &parseErrors, 1);

		expressionTokenizer.SetNodeEmpty(0);
		expressionTokenizer.SetNodeEmpty(1);
		expressionTokenizer.SetNodeEmpty(2);
		expressionTokenizer.SetNodeEmpty(4);

		expressionTokenizer.GetNode(3)->_pItem = 0;
		expressionTokenizer.GetNode(3)->_value.SetValue(0, 33.0F);

		Variable value = expressionTokenizer.FindFirstValue(1, 4);

		Assert::AreEqual(33.0F, value.GetValueFloat(0));
	}

	static void TestNodeIsNumber()
	{
		ExpressionNode expressionNode;
		expressionNode._pItem = "13";
		Assert::AreEqual(true, expressionNode.IsNumber());

		expressionNode._pItem = "Bob";
		Assert::AreEqual(false, expressionNode.IsNumber());
	}

	static void TestNodeIsIdentifier()
	{
		ExpressionNode expressionNode;
		expressionNode._pItem = "13";
		Assert::AreEqual(false, expressionNode.IsIdentifier());

		expressionNode._pItem = "Bob";
		Assert::AreEqual(true, expressionNode.IsIdentifier());
	}

	static void TestNodeIs()
	{
		ExpressionNode expressionNode;
		expressionNode._pItem = "+";
		expressionNode._itemLength = 1;
		Assert::AreEqual(true, expressionNode.Is("+"));
		Assert::AreEqual(false, expressionNode.Is("="));

		expressionNode._pItem = 0;
		expressionNode._itemLength = 0;
		Assert::AreEqual(false, expressionNode.Is("="));
	}

	static void TestAfterLastNode()
	{
		ExpressionTokenizer expressionTokenizer;
		ParseErrors parseErrors;

		expressionTokenizer.Tokenize("13.0", &parseErrors, 1);

		Assert::AreEqual(0, (int) expressionTokenizer.GetNode(1));
	}

	static void TestTooManyNodes()
	{
		ExpressionTokenizer expressionTokenizer;
		ParseErrors parseErrors;

		expressionTokenizer.Tokenize("1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+", &parseErrors, 1);
		
		Assert::AreEqual(1, parseErrors.GetErrorCount());
		Assert::AreEqual("Expression: Too many nodes in expression", parseErrors.GetError(0)._errorText);
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
		TestQuotedString();

		TestFindMatchingCloseParen();
		TestFindFirstValue();

		TestNodeIsNumber();
		TestNodeIsIdentifier();
		TestNodeIs();
		TestAfterLastNode();

		TestTooManyNodes();
	}
};