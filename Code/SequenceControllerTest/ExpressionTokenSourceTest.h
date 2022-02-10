#include "ExpressionNode.h"
#include "CharacterClassifier.h"
#include "ExpressionTokenSource.h"

class ExpressionTokenSourceTest
{
	static void Assert(ExpressionTokenSource* pExpressionTokenSource, const char* pNodeString)
	{
		//int tokenLength = strlen(pNodeString);
    Assert::AreEqual(1, pExpressionTokenSource->EqualTo(pNodeString));

		pExpressionTokenSource->Advance();
	}

    static void One()
    {
        ExpressionTokenSource expressionTokenSource("1");

        Assert(&expressionTokenSource, "1");

        Assert::AreEqual(1, expressionTokenSource.AtEnd());
    }

	static void Simple()
	{
		ExpressionTokenSource expressionTokenSource("13.0");

		Assert(&expressionTokenSource, "13.0");

        Assert::AreEqual(1, expressionTokenSource.AtEnd());
    }

	static void BinaryPlus()
	{
		ExpressionTokenSource expressionTokenSource("13.0 + 10.0");

		Assert(&expressionTokenSource, "13.0");
		Assert(&expressionTokenSource, "+");
		Assert(&expressionTokenSource, "10.0");

        Assert::AreEqual(1, expressionTokenSource.AtEnd());
    }

    static void TestNewLine()
    {
        ExpressionTokenSource expressionTokenSource("\r\n");

        Assert(&expressionTokenSource, "\n");

        Assert::AreEqual(1, expressionTokenSource.AtEnd());
    }

    static void TestNewLine2()
    {
        ExpressionTokenSource expressionTokenSource("\n\r");

        Assert(&expressionTokenSource, "\n");

        Assert::AreEqual(1, expressionTokenSource.AtEnd());
    }

    static void TestAdvanceToNewLine1()
    {
        ExpressionTokenSource expressionTokenSource("+\n\r*");
        expressionTokenSource.AdvanceToNewLine();

        Assert::AreEqual(1, expressionTokenSource.EqualTo("*"));
    }

    static void TestAdvanceToNewLine2()
    {
        ExpressionTokenSource expressionTokenSource("\n*");
        expressionTokenSource.AdvanceToNewLine();

        Assert::AreEqual(1, expressionTokenSource.EqualTo("*"));
    }

    static void TestAdvanceToNewLine3()
    {
        ExpressionTokenSource expressionTokenSource("*");
        expressionTokenSource.AdvanceToNewLine();

        Assert::AreEqual(1, expressionTokenSource.AtEnd());
    }


    static void TestAll()
    {
        ParseErrors parseErrors;
        ExpressionTokenSource expressionTokenSource("1+2-3*4/5==6!=7>=8>9<=10<11&&12||13!14%15,16=17++18--19{20}21[22]23?24:25", &parseErrors);

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
        Assert(&expressionTokenSource, "[");
        Assert(&expressionTokenSource, "22");
        Assert(&expressionTokenSource, "]");
        Assert(&expressionTokenSource, "23");
        Assert(&expressionTokenSource, "?");
        Assert(&expressionTokenSource, "24");
        Assert(&expressionTokenSource, ":");
        Assert(&expressionTokenSource, "25");

        Assert::AreEqual(1, expressionTokenSource.AtEnd());
        Assert::AreEqual(0, parseErrors.GetErrorCount());
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

        Assert::AreEqual(1, expressionTokenSource.AtEnd());
	}

	static void TestParens2()
	{
		ExpressionTokenSource expressionTokenSource("(1+2)");

		Assert(&expressionTokenSource, "(");
		Assert(&expressionTokenSource, "1");
		Assert(&expressionTokenSource, "+");
		Assert(&expressionTokenSource, "2");
		Assert(&expressionTokenSource, ")");

        Assert::AreEqual(1, expressionTokenSource.AtEnd());
	}

	static void TestWithWhiteSpace()
	{
		ExpressionTokenSource expressionTokenSource(" (	1 + 2 )	");

		Assert(&expressionTokenSource, "(");
		Assert(&expressionTokenSource, "1");
		Assert(&expressionTokenSource, "+");
		Assert(&expressionTokenSource, "2");
		Assert(&expressionTokenSource, ")");

        Assert::AreEqual(1, expressionTokenSource.AtEnd());
	}

	static void TestQuotedString()
	{
		ExpressionTokenSource expressionTokenSource("= \" x + y = z \" *");

		Assert(&expressionTokenSource, "=");
		Assert(&expressionTokenSource, "\" x + y = z ");
		Assert(&expressionTokenSource, "*");

        Assert::AreEqual(1, expressionTokenSource.AtEnd());
	}

	static void TestIdentifiers()
	{
		ExpressionTokenSource expressionTokenSource("x + y * z");

		Assert(&expressionTokenSource, "x");
		Assert(&expressionTokenSource, "+");
		Assert(&expressionTokenSource, "y");
		Assert(&expressionTokenSource, "*");
		Assert(&expressionTokenSource, "z");

        Assert::AreEqual(1, expressionTokenSource.AtEnd());
	}

    static void TestComment()
    {
        ExpressionTokenSource expressionTokenSource("x + 7 // add them together\n13 // done");

        Assert(&expressionTokenSource, "x");
        Assert(&expressionTokenSource, "+");
        Assert(&expressionTokenSource, "7");
        Assert(&expressionTokenSource, "\n");
        Assert(&expressionTokenSource, "13");

        Assert::AreEqual(1, expressionTokenSource.AtEnd());
    }

    static void TestComment2()
    {
        ExpressionTokenSource expressionTokenSource("x = 5 // init\n\n // space\nx  // eval");

        Assert(&expressionTokenSource, "x");
        Assert(&expressionTokenSource, "=");
        Assert(&expressionTokenSource, "5");
        Assert(&expressionTokenSource, "\n");
        Assert(&expressionTokenSource, "\n");
        Assert(&expressionTokenSource, "\n");
        Assert(&expressionTokenSource, "x");

        Assert::AreEqual(1, expressionTokenSource.AtEnd());
    }

    static void TestNewlineLinefeed()
    {
        ExpressionTokenSource expressionTokenSource("x\n\ry\r\nz");

        Assert(&expressionTokenSource, "x");
        Assert(&expressionTokenSource, "\n");
        Assert(&expressionTokenSource, "y");
        Assert(&expressionTokenSource, "\n");
        Assert(&expressionTokenSource, "z");

        Assert::AreEqual(1, expressionTokenSource.AtEnd());
    }

    static void TestGetCurrentToken()
    {
        ExpressionTokenSource expressionTokenSource("fred + barney");

        Assert::AreEqual("fred", expressionTokenSource.GetCurrentToken());
        expressionTokenSource.Advance();
        Assert::AreEqual("+", expressionTokenSource.GetCurrentToken());
        expressionTokenSource.Advance();
        Assert::AreEqual("barney", expressionTokenSource.GetCurrentToken());
        expressionTokenSource.Advance();

        Assert::AreEqual(1, expressionTokenSource.AtEnd());
    }


	static void TestIsNumber()
	{
        ExpressionTokenSource expressionTokenSource("13 Bob");
		Assert::AreEqual(true, expressionTokenSource.IsNumber());

        expressionTokenSource.Advance();

        Assert::AreEqual(false, expressionTokenSource.IsNumber());
    }

	static void TestIsIdentifier()
	{
        ExpressionTokenSource expressionTokenSource("13 Bob");
        Assert::AreEqual(false, expressionTokenSource.IsIdentifier());

        expressionTokenSource.Advance();

        Assert::AreEqual(true, expressionTokenSource.IsIdentifier());
       
	}

	static void TestUnexpectedCharacter()
	{
		ParseErrors parseErrors;
		ExpressionTokenSource expressionTokenSource("x $ z", &parseErrors, 15);

		while (!expressionTokenSource.AtEnd())
		{
			expressionTokenSource.Advance();
		}

		Assert::AreEqual(1, parseErrors.GetErrorCount());
		Assert::AreEqual("Unrecognized character: $", parseErrors.GetError(0)->_errorText);
		Assert::AreEqual(15, parseErrors.GetError(0)->_lineNumber);
	}

	static void TestUnclosedStringConstant()
	{
		ParseErrors parseErrors;
		ExpressionTokenSource expressionTokenSource("\"abcd", &parseErrors, 55);

		while (!expressionTokenSource.AtEnd())
		{
			expressionTokenSource.Advance();
		}

		Assert::AreEqual(1, parseErrors.GetErrorCount());
		Assert::AreEqual("Missing closing \" in string", parseErrors.GetError(0)->_errorText);
		Assert::AreEqual(55, parseErrors.GetError(0)->_lineNumber);
	}

    static void TestGetParseLocationSetParseLocation()
    {
        ParseErrors parseErrors;
        ExpressionTokenSource expressionTokenSource("a + b * c / d % e", &parseErrors, 55);

        Assert::AreEqual(1, expressionTokenSource.EqualTo("a"));
        int spot = expressionTokenSource.GetParseLocation();

        expressionTokenSource.Advance();
        expressionTokenSource.Advance();

        Assert::AreEqual(1, expressionTokenSource.EqualTo("b"));

        expressionTokenSource.SetParseLocation(spot);

        Assert::AreEqual(1, expressionTokenSource.EqualTo("a"));

        expressionTokenSource.Advance();
        expressionTokenSource.Advance();
        expressionTokenSource.Advance();
        expressionTokenSource.Advance();

        Assert::AreEqual(1, expressionTokenSource.EqualTo("c"));
        spot = expressionTokenSource.GetParseLocation();

        expressionTokenSource.Advance();
        expressionTokenSource.Advance();
        expressionTokenSource.Advance();

        Assert::AreEqual(1, expressionTokenSource.EqualTo("%"));
        expressionTokenSource.SetParseLocation(spot);

        Assert::AreEqual(1, expressionTokenSource.EqualTo("c"));
    }

    static void TestGetSet(ExpressionTokenSource* pExpressionTokenSource, const char* pToken)
    {
        Assert::AreEqual(pToken, pExpressionTokenSource->GetCurrentToken());
        pExpressionTokenSource->SetParseLocation(pExpressionTokenSource->GetParseLocation());
        Assert::AreEqual(pToken, pExpressionTokenSource->GetCurrentToken());

        pExpressionTokenSource->Advance();
    }

    static void TestGetParseLocationSetParseLocation2()
    {
        ParseErrors parseErrors;
        ExpressionTokenSource expressionTokenSource("x = a + b // free\ny * c / d % e\nz = x * y", &parseErrors, 55);

        TestGetSet(&expressionTokenSource, "x");
        TestGetSet(&expressionTokenSource, "=");
        TestGetSet(&expressionTokenSource, "a");
        TestGetSet(&expressionTokenSource, "+");
        TestGetSet(&expressionTokenSource, "b");
        TestGetSet(&expressionTokenSource, "\n");

        TestGetSet(&expressionTokenSource, "y");
        TestGetSet(&expressionTokenSource, "*");
        TestGetSet(&expressionTokenSource, "c");
        TestGetSet(&expressionTokenSource, "/");
        TestGetSet(&expressionTokenSource, "d");
        TestGetSet(&expressionTokenSource, "%");
        TestGetSet(&expressionTokenSource, "e");
        TestGetSet(&expressionTokenSource, "\n");

        TestGetSet(&expressionTokenSource, "z");
        TestGetSet(&expressionTokenSource, "=");
        TestGetSet(&expressionTokenSource, "x");
        TestGetSet(&expressionTokenSource, "*");
        TestGetSet(&expressionTokenSource, "y");

    }

    static void TestAtEnd()
    {
        ParseErrors parseErrors;
        ExpressionTokenSource expressionTokenSource("a b", &parseErrors, 55);

        Assert::AreEqual(0, expressionTokenSource.AtEnd());
        expressionTokenSource.Advance();
        Assert::AreEqual(0, expressionTokenSource.AtEnd());
        expressionTokenSource.Advance();
        Assert::AreEqual(1, expressionTokenSource.AtEnd());
    }

    static void CheckTokenAndLineNumber(ExpressionTokenSource* pExpressionTokenSource, const char* pExpectedToken, int expectedLineNumber)
    {
        Assert::AreEqual(pExpectedToken, pExpressionTokenSource->GetCurrentToken());
        Assert::AreEqual(expectedLineNumber, pExpressionTokenSource->GetLineNumber());
    }

    static void TestLineNumbers()
    {
        ParseErrors parseErrors;
        ExpressionTokenSource expressionTokenSource("a\nb * 12\nc\nd++\ne\nf", &parseErrors, 55);

        CheckTokenAndLineNumber(&expressionTokenSource, "a", 0);
        expressionTokenSource.Advance();

        CheckTokenAndLineNumber(&expressionTokenSource, "\n", 0);
        expressionTokenSource.Advance();

        CheckTokenAndLineNumber(&expressionTokenSource, "b", 1);
        expressionTokenSource.Advance();

        CheckTokenAndLineNumber(&expressionTokenSource, "*", 1);
        expressionTokenSource.Advance();

        CheckTokenAndLineNumber(&expressionTokenSource, "12", 1);
        expressionTokenSource.Advance();

        CheckTokenAndLineNumber(&expressionTokenSource, "\n", 1);
        expressionTokenSource.Advance();

        CheckTokenAndLineNumber(&expressionTokenSource, "c", 2);
        expressionTokenSource.Advance();

        CheckTokenAndLineNumber(&expressionTokenSource, "\n", 2);
        expressionTokenSource.Advance();

        CheckTokenAndLineNumber(&expressionTokenSource, "d", 3);
        expressionTokenSource.Advance();

        CheckTokenAndLineNumber(&expressionTokenSource, "++", 3);
        expressionTokenSource.Advance();

        CheckTokenAndLineNumber(&expressionTokenSource, "\n", 3);
        expressionTokenSource.Advance();

        CheckTokenAndLineNumber(&expressionTokenSource, "e", 4);
        expressionTokenSource.Advance();

        CheckTokenAndLineNumber(&expressionTokenSource, "\n", 4);
        expressionTokenSource.Advance();

        CheckTokenAndLineNumber(&expressionTokenSource, "f", 5);
        expressionTokenSource.Advance();
    }

    static void TestLineNumbersWithBlankLines()
    {
        ParseErrors parseErrors;
        ExpressionTokenSource expressionTokenSource("a\n\n\nb * 12", &parseErrors, 55);

        CheckTokenAndLineNumber(&expressionTokenSource, "a", 0);
        expressionTokenSource.Advance();

        CheckTokenAndLineNumber(&expressionTokenSource, "\n", 0);
        expressionTokenSource.Advance();

        CheckTokenAndLineNumber(&expressionTokenSource, "\n", 1);
        expressionTokenSource.Advance();

        CheckTokenAndLineNumber(&expressionTokenSource, "\n", 2);
        expressionTokenSource.Advance();

        CheckTokenAndLineNumber(&expressionTokenSource, "b", 3);
        expressionTokenSource.Advance();

        CheckTokenAndLineNumber(&expressionTokenSource, "*", 3);
        expressionTokenSource.Advance();

        CheckTokenAndLineNumber(&expressionTokenSource, "12", 3);
        expressionTokenSource.Advance();
    }


public:

	static void Run()
	{
        One();
		Simple();
		BinaryPlus();
		TestAll();
		TestParens();
		TestParens2();
		TestWithWhiteSpace();
		TestQuotedString();
		TestIdentifiers();
        TestComment();
        TestComment2();
		TestUnexpectedCharacter();
		TestUnclosedStringConstant();
        TestNewLine();
        TestNewLine2();
        TestAdvanceToNewLine1();
        TestAdvanceToNewLine2();
        TestAdvanceToNewLine3();
        TestGetParseLocationSetParseLocation();
        TestGetParseLocationSetParseLocation2();
        TestAtEnd();

        TestGetCurrentToken();
        TestNewlineLinefeed();

        TestLineNumbers();
        TestLineNumbersWithBlankLines();

		TestIsNumber();
		TestIsIdentifier();
	}
};