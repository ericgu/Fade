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

		Variable variable = variableCollection.Get(2);
		variable.SetActiveFlag(true);
	}

	static void TestParseFloatOrVariable()
	{
		VariableCollection variableCollection;

		variableCollection.AddAndSet("C", 55.0F);

		Variable parsed = variableCollection.ParseFloatOrVariable("88.0");
		Assert::AreEqual(88.0F, parsed.GetValueFloat());

		parsed = variableCollection.ParseFloatOrVariable("C");
		Assert::AreEqual(55.0F, parsed.GetValueFloat());
	}

	static void TestParseFloatOrVariableNamed()
	{
		VariableCollection variableCollection;

		variableCollection.AddAndSet("Fred", 55.0F);

		Variable parsed = variableCollection.ParseFloatOrVariable("Fred");
		Assert::AreEqual(55.0F, parsed.GetValueFloat());
	}

	static void TestAddVariableTwice()
	{
		VariableCollection variableCollection;

		variableCollection.AddAndSet("Fred", 55.0F);
		variableCollection.AddAndSet("Fred", 55.0F);

		Assert::AreEqual(1, variableCollection.GetActiveVariableCount());
		Variable parsed = variableCollection.ParseFloatOrVariable("Fred");
		Assert::AreEqual(55.0F, parsed.GetValueFloat());
	}

	static void TestRandom()
	{
		VariableCollection variableCollection;

		MyRandom::SetFirstValue(1);
		Variable parsed = variableCollection.ParseFloatOrVariable("R(0:10)");
		Assert::AreEqual(1.0F, parsed.GetValueFloat());
	}

	static void TestGetVariableName()
	{
		const char* pCommand = "Test=15";

		char variableName[64];
		const char *pRemaining = VariableCollection::GetVariableName(pCommand, variableName);

		Assert::AreEqual("Test", variableName);
		Assert::AreEqual("=15", pRemaining);
	}

public:

	static int Run()
	{
		TestParseInt();
		TestParseFloat();
		TestIncrement();
		TestConstructorInt();
		TestConstructorFloat();

		TestCollectionWorks();

		TestParseFloatOrVariable();
		TestParseFloatOrVariableNamed();

		TestAddVariableTwice();

		TestRandom();

		TestGetVariableName();

		return 0;
	}
};
