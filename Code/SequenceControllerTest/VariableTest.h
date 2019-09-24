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

		return 0;
	}
};
