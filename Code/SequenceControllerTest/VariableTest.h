#pragma once

#include "Variable.h"

class VariableTest
{
	static void TestParseInt()
	{
		Variable variable = Variable::ParseInt("15");
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
		Variable variable = Variable::ParseInt("15");
		variable.Increment();
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

public:

	static int Run()
	{
		TestParseInt();
		TestParseFloat();
		TestIncrement();
		TestConstructorInt();
		TestConstructorFloat();

		TestCollectionWorks();

		return 0;
	}
};
