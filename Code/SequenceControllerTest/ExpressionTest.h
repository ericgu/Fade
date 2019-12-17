#pragma once

#include "Expression.h"

class ExpressionTest
{
	static void TestParseFloatOrVariable()
	{
		Expression expression;
		VariableCollection variableCollection;
		ParseErrors parseErrors;

		variableCollection.AddAndSet("C", 55.0F);

		Variable* pParsed = expression.Parse("88.0", &variableCollection, 0, 0, &parseErrors, 1);
		Assert::AreEqual(88.0F, pParsed->GetValueFloat());

		pParsed = expression.Parse("C", &variableCollection, 0, 0, &parseErrors, 1);
		Assert::AreEqual(55.0F, pParsed->GetValueFloat());
	}

	static void TestParseFloatOrVariableNamed()
	{
		Expression expression;
		VariableCollection variableCollection;
		ParseErrors parseErrors;

		variableCollection.AddAndSet("Fred", 55.0F);

		Variable* pParsed = expression.Parse("Fred", &variableCollection, 0, 0, &parseErrors, 1);
		Assert::AreEqual(55.0F, pParsed->GetValueFloat());
	}

	static void TestRandom()
	{
		Expression expression;
		VariableCollection variableCollection;
		ParseErrors parseErrors;

		MyRandom::SetFirstValue(1);
		Variable* pParsed = expression.Parse("R(0:10)", &variableCollection, 0, 0, &parseErrors, 1);
		Assert::AreEqual(1.0F, pParsed->GetValueFloat());
	}

	static void TestFunctionCall()
	{
		Expression expression;
		VariableCollection variableCollection;
		ParseErrors parseErrors;
		FunctionStore functionStore;
		Stack stack;

		Variable* pParsed = expression.Parse("R(0:10)", &variableCollection, &functionStore, &stack, &parseErrors, 1);
		Assert::AreEqual(1.0F, pParsed->GetValueFloat());
	}

public:

	static int Run()
	{
		TestParseFloatOrVariable();
		TestParseFloatOrVariableNamed();
		TestRandom();
		//TestFunctionCall();

		return 0;
	}
};

