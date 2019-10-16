#pragma once

#include "Variable.h"

class VariableTest
{
	static void TestParseInt()
	{
		Variable variable = Variable::ParseFloat("15");
		Assert::AreEqual(15, variable.GetValueInt());
		Assert::AreEqual(15.0F, variable.GetValueFloat());
	}

	static void TestParseFloat()
	{
		Variable variable = Variable::ParseFloat("15.88");
		Assert::AreEqual(15.88F, variable.GetValueFloat());
	}

	static void TestParseNegative()
	{
		Variable variable = Variable::ParseFloat("-2.0");
		Assert::AreEqual(-2.0F, variable.GetValueFloat());
	}

	static void TestIncrement()
	{
		Variable variable = Variable::ParseFloat("15");
		variable.Increment(1.0F);
		Assert::AreEqual(16, variable.GetValueInt());
		Assert::AreEqual(16.0F, variable.GetValueFloat());
	}

	static void TestConstructorInt()
	{
		Variable variable = Variable(15);
		Assert::AreEqual(15, variable.GetValueInt());
		Assert::AreEqual(15.0F, variable.GetValueFloat());
		Assert::AreEqual(true, variable.GetActiveFlag());
	}

	static void TestConstructorFloat()
	{
		Variable variable = Variable(15.0F);
		Assert::AreEqual(15.0F, variable.GetValueFloat());
		Assert::AreEqual(true, variable.GetActiveFlag());
	}

	static void TestCollectionWorks()
	{
		VariableCollection variableCollection;

		Variable* pVariable = variableCollection.Get(2);
		pVariable->SetActiveFlag(true);
	}

	static void TestParseFloatOrVariable()
	{
		VariableCollection variableCollection;
		ParseErrors parseErrors;

		variableCollection.AddAndSet("C", 55.0F);

		Variable* pParsed = variableCollection.ParseFloatOrVariable("88.0", &parseErrors, 1);
		Assert::AreEqual(88.0F, pParsed->GetValueFloat());

		pParsed = variableCollection.ParseFloatOrVariable("C", &parseErrors, 1);
		Assert::AreEqual(55.0F, pParsed->GetValueFloat());
	}

	static void TestParseFloatOrVariableNamed()
	{
		VariableCollection variableCollection;
		ParseErrors parseErrors;

		variableCollection.AddAndSet("Fred", 55.0F);

		Variable* pParsed = variableCollection.ParseFloatOrVariable("Fred", &parseErrors, 1);
		Assert::AreEqual(55.0F, pParsed->GetValueFloat());
	}

	static void TestAddVariableTwice()
	{
		VariableCollection variableCollection;
		ParseErrors parseErrors;

		variableCollection.AddAndSet("Fred", 55.0F);
		variableCollection.AddAndSet("Fred", 55.0F);

		Assert::AreEqual(1, variableCollection.GetActiveVariableCount());
		Variable* pParsed = variableCollection.ParseFloatOrVariable("Fred", &parseErrors, 1);
		Assert::AreEqual(55.0F, pParsed->GetValueFloat());
	}

	static void TestRandom()
	{
		VariableCollection variableCollection;
		ParseErrors parseErrors;

		MyRandom::SetFirstValue(1);
		Variable* pParsed = variableCollection.ParseFloatOrVariable("R(0:10)", &parseErrors, 1);
		Assert::AreEqual(1.0F, pParsed->GetValueFloat());
	}

	static void TestGetVariableName()
	{
		const char* pCommand = "Test=15";

		char variableName[64];
		const char *pRemaining = VariableCollection::GetVariableName(pCommand, variableName);

		Assert::AreEqual("Test", variableName);
		Assert::AreEqual("=15", pRemaining);
	}

	static void TestMissingVariable()
	{
		VariableCollection variableCollection;
		ParseErrors parseErrors;

		Variable* pParsed = variableCollection.ParseFloatOrVariable("Fred", &parseErrors, 15);
		Assert::AreEqual(1, parseErrors.GetErrorCount());
		Assert::AreEqual("Undeclared variable: Fred", parseErrors.GetError(0)._errorText);
		Assert::AreEqual(15, parseErrors.GetError(0)._lineNumber);
	}

	static void TestClear()
	{
		VariableCollection variableCollection;
		ParseErrors parseErrors;

		variableCollection.AddAndSet("Fred", 55.0F);
		Assert::AreEqual(1, variableCollection.GetActiveVariableCount());

		variableCollection.Clear();
		Assert::AreEqual(0, variableCollection.GetActiveVariableCount());
	}

public:

	static int Run()
	{
		TestParseInt();
		TestParseFloat();
		TestParseNegative();
		TestIncrement();
		TestConstructorInt();
		TestConstructorFloat();

		TestCollectionWorks();

		TestParseFloatOrVariable();
		TestParseFloatOrVariableNamed();

		TestAddVariableTwice();

		TestRandom();

		TestGetVariableName();

		TestMissingVariable();
		TestClear();

		return 0;
	}
};
