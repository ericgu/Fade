#pragma once

#include "Expression.h"

class ExpressionTest
{
	static void TestParseFloatOrVariable()
	{
		Expression expression;
		VariableCollection variableCollection;
		ParseErrors parseErrors;
		Stack stack;

		variableCollection.AddAndSet("C", 55.0F, stack.GetFrameCount());

		Variable* pParsed = expression.Parse("88.0", &variableCollection, 0, 0, &parseErrors, 1);
		Assert::AreEqual(88.0F, pParsed->GetValueFloat());

		pParsed = expression.Parse("C", &variableCollection, 0, &stack, &parseErrors, 1);
		Assert::AreEqual(55.0F, pParsed->GetValueFloat());
	}

	static void TestParseFloatOrVariableNamed()
	{
		Expression expression;
		VariableCollection variableCollection;
		ParseErrors parseErrors;
		Stack stack;

		variableCollection.AddAndSet("Fred", 55.0F, stack.GetFrameCount());

		Variable* pParsed = expression.Parse("Fred", &variableCollection, 0, &stack, &parseErrors, 1);
		Assert::AreEqual(55.0F, pParsed->GetValueFloat());
	}

	static void TestRandom()
	{
		Expression expression;
		VariableCollection variableCollection;
		ParseErrors parseErrors;
		Stack stack;

		MyRandom::SetFirstValue(1);
		Variable* pParsed = expression.Parse("R(0:10)", &variableCollection, 0, &stack, &parseErrors, 1);
		Assert::AreEqual(1.0F, pParsed->GetValueFloat());
	}

	static void TestLiteral()
	{
		Expression expression;
		VariableCollection variableCollection;
		ParseErrors parseErrors;
		Stack stack;

		Variable value = expression.Evaluate("33", &variableCollection, 0, &stack, &parseErrors, 1);
		Assert::AreEqual(33.0F, value.GetValueFloat());
	}

	static void TestVariable()
	{
		Expression expression;
		VariableCollection variableCollection;
		ParseErrors parseErrors;
		Stack stack;

		variableCollection.AddAndSet("Fred", 55.0F, stack.GetFrameCount());

		Variable value = expression.Evaluate("Fred", &variableCollection, 0, &stack, &parseErrors, 1);
		Assert::AreEqual(55.0F, value.GetValueFloat());
	}

	static void TestExpression(const char* pExpression, float expectedValue)
	{
		Expression expression;
		VariableCollection variableCollection;
		ParseErrors parseErrors;
		Stack stack;

		Variable value = expression.Evaluate(pExpression, &variableCollection, 0, &stack, &parseErrors, 1);
		Assert::AreEqual(expectedValue, value.GetValueFloat());
	}

	static void TestMultiply()
	{
		TestExpression("2*4", 8.0F);
	}

	static void TestDivide()
	{
		TestExpression("6/2", 3.0F);
	}

	static void TestMultiplyDivide()
	{
		TestExpression("9*2/3", 6.0F);
	}

	static void TestMod()
	{
		TestExpression("9%4", 1.0F);
	}

	static void TestPlusMinus()
	{
		TestExpression("9+2-3", 8.0F);
	}

	static void TestUnaryMinus()
	{
		TestExpression("-4", -4.0F);
	}

	static void TestUnaryMinus2()
	{
		TestExpression("9+-3", 6.0F);
	}

	static void TestUnaryPlus()
	{
		TestExpression("+5", 5.0F);
	}

	static void TestUnaryPlus2()
	{
		TestExpression("9*+3", 27.0F);
	}

	static void TestUnaryNot()
	{
		TestExpression("!1", 0.0F);
	}

	static void TestUnaryNot2()
	{
		TestExpression("1&&!0", 1.0F);
	}

	static void TestBasicPrecedence()
	{
		TestExpression("9+2*4-3", 14.0F);
	}

	static void TestComparisons()
	{
		TestExpression("9>2", 1.0F);
		TestExpression("9<2", 0.0F);
		TestExpression("3>=3", 1.0F);
		TestExpression("2>=3", 0.0F);
		TestExpression("4>=3", 1.0F);
		TestExpression("3<=3", 1.0F);
		TestExpression("2<=3", 1.0F);
		TestExpression("4<=3", 0.0F);

		TestExpression("9==2", 0.0F);
		TestExpression("9!=2", 1.0F);

		TestExpression("2==2", 1.0F);
		TestExpression("2!=2", 0.0F);
	}

	static void TestParens()
	{
		TestExpression("(9+2)*(4-2)", 22.0F);
	}

	static void TestNestedParens()
	{
		TestExpression("(9*(2-1)*3)*(4-2)", 54.0F);
	}

	static Variable FunctionCallHandlerImplementation(FunctionDefinition* pFunctionDefinition, VariableCollection* pVariableCollection, FunctionStore* pFunctionStore, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow)
	{
		Assert::AreEqual("MyFunction", pFunctionDefinition->Name);
		Assert::AreEqual(0, pVariableCollection->GetWithoutErrorCheck("#A", 2)->GetValueInt());

		return Variable(10.0F);
	}

	static void TestFunctionCall()
	{
		Expression expression;
		VariableCollection variableCollection;
		ParseErrors parseErrors;
		Stack stack;
		FunctionStore functionStore;

		stack.CreateFrame();
		FunctionDefinition functionDefinition("MyFunction", 10);

		functionStore.DefineStart(functionDefinition);

		Variable value = expression.Evaluate("MyFunction()", &variableCollection, &functionStore, &stack, &parseErrors, 1, FunctionCallHandlerImplementation);
		Assert::AreEqual(10.0F, value.GetValueFloat());
	}

	static Variable FunctionCallHandlerImplementation2(FunctionDefinition* pFunctionDefinition, VariableCollection* pVariableCollection, FunctionStore* pFunctionStore, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow)
	{
		Assert::AreEqual(1, pVariableCollection->GetWithoutErrorCheck("#A", 2)->GetValueInt());
		Assert::AreEqual(15.3F, pVariableCollection->GetWithoutErrorCheck("#A0", 2)->GetValueFloat());

		return *pVariableCollection->GetWithoutErrorCheck("#A0", 2);
	}

	static void TestFunctionCallWithParameters()
	{
		Expression expression;
		VariableCollection variableCollection;
		ParseErrors parseErrors;
		Stack stack;
		FunctionStore functionStore;

		stack.CreateFrame();
		FunctionDefinition functionDefinition("MyFunction", 10);

		functionStore.DefineStart(functionDefinition);

		Variable value = expression.Evaluate("MyFunction(15.3)", &variableCollection, &functionStore, &stack, &parseErrors, 1, FunctionCallHandlerImplementation2, 0);
		Assert::AreEqual(15.3F, value.GetValueFloat());
		Assert::AreEqual(1, stack.GetFrameCount());
	}

public:

	static int Run()
	{
		TestFunctionCallWithParameters();
		TestFunctionCall();

		TestParseFloatOrVariable();
		TestParseFloatOrVariableNamed();
		TestRandom();

		TestLiteral();
		TestVariable();

		TestUnaryMinus();
		TestUnaryMinus2();
		TestUnaryPlus();
		TestUnaryPlus2();
		TestUnaryNot();
		TestUnaryNot2();

		TestMultiply();
		TestDivide();
		TestMultiplyDivide();
		TestMod();

		TestPlusMinus();
		TestBasicPrecedence();

		TestComparisons();

		TestParens();
		TestNestedParens();


		return 0;
	}
};

