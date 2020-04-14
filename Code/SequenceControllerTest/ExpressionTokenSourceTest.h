#include "ExpressionTokenSource.h"

class ExpressionTokenSourceTest
{
	static void Assert(ExpressionTokenSource* pExpressionTokenSource, const char* pNodeString)
	{
		int tokenLength = strlen(pNodeString);
		ExpressionNode* pNode = pExpressionTokenSource->GetCurrentNode();

		Assert::AreEqual(pNodeString, pNode->_pItem);
		Assert::AreEqual(true, pNode->_value.IsNan());

		pExpressionTokenSource->Advance();
	}

	static void Simple()
	{
		ExpressionTokenSource expressionTokenSource("13.0");

		Assert(&expressionTokenSource, "13.0");

		Assert::AreEqual(0, (int) expressionTokenSource.GetCurrentNode());
	}

	static void BinaryPlus()
	{
		ExpressionTokenSource expressionTokenSource("13.0 + 10.0");

		Assert(&expressionTokenSource, "13.0");
		Assert(&expressionTokenSource, "+");
		Assert(&expressionTokenSource, "10.0");

		Assert::AreEqual(0, (int)expressionTokenSource.GetCurrentNode());
	}

	static void TestAll()
	{
		ExpressionTokenSource expressionTokenSource("1+2-3*4/5==6!=7>=8>9<=10<11&&12||13!14%15,16=17++18--19{20}21");

		Assert(&expressionTokenSource, "1");
		Assert(&expressionTokenSource, "+");
		Assert(&expressionTokenSource, "2");
		Assert(&expressionTokenSource, "-");
		Assert(&expressionTokenSource, "3");
		Assert(&expressionTokenSource, "*");
		Assert(&expressionTokenSource, "4");
		Assert(&expressionTokenSource, "/");
		Assert(&expressionTokenSource, "5");
		Assert(&expressionTokenSource, "==");
		Assert(&expressionTokenSource, "6");
		Assert(&expressionTokenSource, "!=");
		Assert(&expressionTokenSource, "7");
		Assert(&expressionTokenSource, ">=");
		Assert(&expressionTokenSource, "8");
		Assert(&expressionTokenSource, ">");
		Assert(&expressionTokenSource, "9");
		Assert(&expressionTokenSource, "<=");
		Assert(&expressionTokenSource, "10");
		Assert(&expressionTokenSource, "<");
		Assert(&expressionTokenSource, "11");
		Assert(&expressionTokenSource, "&&");
		Assert(&expressionTokenSource, "12");
		Assert(&expressionTokenSource, "||");
		Assert(&expressionTokenSource, "13");
		Assert(&expressionTokenSource, "!");
		Assert(&expressionTokenSource, "14");
		Assert(&expressionTokenSource, "%");
		Assert(&expressionTokenSource, "15");
		Assert(&expressionTokenSource, ",");
		Assert(&expressionTokenSource, "16");
		Assert(&expressionTokenSource, "=");
		Assert(&expressionTokenSource, "17");
		Assert(&expressionTokenSource, "++");
		Assert(&expressionTokenSource, "18");
		Assert(&expressionTokenSource, "--");
		Assert(&expressionTokenSource, "19");
		Assert(&expressionTokenSource, "{");
		Assert(&expressionTokenSource, "20");
		Assert(&expressionTokenSource, "}");
		Assert(&expressionTokenSource, "21");

		Assert::AreEqual(0, (int)expressionTokenSource.GetCurrentNode());
	}

	static void TestParens()
	{
		ExpressionTokenSource expressionTokenSource("(1+2)*3");

		Assert(&expressionTokenSource, "(");
		Assert(&expressionTokenSource, "1");
		Assert(&expressionTokenSource, "+");
		Assert(&expressionTokenSource, "2");
		Assert(&expressionTokenSource, ")");
		Assert(&expressionTokenSource, "*");
		Assert(&expressionTokenSource, "3");

		Assert::AreEqual(0, (int)expressionTokenSource.GetCurrentNode());
	}

	static void TestParens2()
	{
		ExpressionTokenSource expressionTokenSource("(1+2)");

		Assert(&expressionTokenSource, "(");
		Assert(&expressionTokenSource, "1");
		Assert(&expressionTokenSource, "+");
		Assert(&expressionTokenSource, "2");
		Assert(&expressionTokenSource, ")");

		Assert::AreEqual(0, (int)expressionTokenSource.GetCurrentNode());
	}

	static void TestWithWhiteSpace()
	{
		ExpressionTokenSource expressionTokenSource(" (	1 + 2 )	");

		Assert(&expressionTokenSource, "(");
		Assert(&expressionTokenSource, "1");
		Assert(&expressionTokenSource, "+");
		Assert(&expressionTokenSource, "2");
		Assert(&expressionTokenSource, ")");

		Assert::AreEqual(0, (int)expressionTokenSource.GetCurrentNode());
	}

	static void TestQuotedString()
	{
		ExpressionTokenSource expressionTokenSource("= \" x + y = z \" *");

		Assert(&expressionTokenSource, "=");
		Assert(&expressionTokenSource, "\" x + y = z ");
		Assert(&expressionTokenSource, "*");

		Assert::AreEqual(0, (int)expressionTokenSource.GetCurrentNode());
	}

	static void TestIdentifiers()
	{
		ExpressionTokenSource expressionTokenSource("x + y * z");

		Assert(&expressionTokenSource, "x");
		Assert(&expressionTokenSource, "+");
		Assert(&expressionTokenSource, "y");
		Assert(&expressionTokenSource, "*");
		Assert(&expressionTokenSource, "z");

		Assert::AreEqual(0, (int)expressionTokenSource.GetCurrentNode());
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

	static void TestUnexpectedCharacter()
	{
		ParseErrors parseErrors;
		ExpressionTokenSource expressionTokenSource("x $ z", &parseErrors);

		while (expressionTokenSource.GetCurrentNode() != 0)
		{
			expressionTokenSource.Advance();
		}

		Assert::AreEqual(1, parseErrors.GetErrorCount());
		Assert::AreEqual("Unrecognized character: $", parseErrors.GetError(0)._errorText);
	}

	static void TestUnclosedStringConstant()
	{
		ParseErrors parseErrors;
		ExpressionTokenSource expressionTokenSource("\"abcd", &parseErrors);

		while (expressionTokenSource.GetCurrentNode() != 0)
		{
			expressionTokenSource.Advance();
		}

		Assert::AreEqual(1, parseErrors.GetErrorCount());
		Assert::AreEqual("Missing closing \" in string", parseErrors.GetError(0)._errorText);
	}

public:

	static void Run()
	{
		Simple();
		BinaryPlus();
		TestAll();
		TestParens();
		TestParens2();
		TestWithWhiteSpace();
		TestQuotedString();
		TestIdentifiers();
		TestUnexpectedCharacter();
		TestUnclosedStringConstant();



		TestNodeIsNumber();
		TestNodeIsIdentifier();
		TestNodeIs();

	}
};