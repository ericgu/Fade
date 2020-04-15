#pragma once

#include "Variable.h"

class VariableTest
{
	static void TestParseInt()
	{
		Variable variable = Variable::ParseFloat("15");
		Assert::AreEqual(15, variable.GetValueInt());
		Assert::AreEqual(15.0F, variable.GetValueFloat(0));
	}

	static void TestParseFloat()
	{
		Variable variable = Variable::ParseFloat("15.88");
		Assert::AreEqual(15.88F, variable.GetValueFloat(0));
	}

	static void TestParseNegative()
	{
		Variable variable = Variable::ParseFloat("-2.0");
		Assert::AreEqual(-2.0F, variable.GetValueFloat(0));
	}

	static void TestIncrement()
	{
		Variable variable = Variable::ParseFloat("15");
		variable.Increment(1.0F);
		Assert::AreEqual(16, variable.GetValueInt());
		Assert::AreEqual(16.0F, variable.GetValueFloat(0));
	}

	static void TestConstructorInt()
	{
		Variable variable = Variable(15);
		Assert::AreEqual(15, variable.GetValueInt());
		Assert::AreEqual(15.0F, variable.GetValueFloat(0));
	}

	static void TestConstructorFloat()
	{
		Variable variable = Variable(15.0F);
		Assert::AreEqual(15.0F, variable.GetValueFloat(0));
	}

	static void TestSetValueSetsValueCount()
	{
		Variable variable;
		variable.SetValue(0, 15.0F);
		Assert::AreEqual(15.0F, variable.GetValueFloat(0));
		Assert::AreEqual(1, variable.GetValueCount());
	}

	static void TestClear()
	{
		Variable variable = Variable(15);
		variable.SetStackLevel(55);
		variable.SetVariableName("fred");
		Assert::AreEqual(15.0F, variable.GetValueFloat(0));
		Assert::AreEqual(55, variable.GetStackLevel());
		Assert::AreEqual("fred", variable.GetVariableName());

		variable.Clear();

		Assert::AreEqual(0.0F, variable.GetValueFloat(0));
		Assert::AreEqual(0, variable.GetStackLevel());
	}

	static void TestConstructorString()
	{
		Variable variable = Variable("MyNewString");
		Assert::AreEqual("MyNewString", variable.GetValueString());
		Assert::AreEqual(0, variable.GetValueCount());
	}

	static void TestFloatList()
	{
		Variable variable;
		
		variable.SetValue(0, 33.33F);
		variable.SetValue(1, 15.88F);
		variable.SetValue(2, 1234.567F);

		Assert::AreEqual(3, variable.GetValueCount());
		Assert::AreEqual(33.33F, variable.GetValueFloat(0));
		Assert::AreEqual(15.88F, variable.GetValueFloat(1));
		Assert::AreEqual(1234.567F, variable.GetValueFloat(2));
	}

	static void TestFloatListTooMany()
	{
		Variable variable;

		variable.SetValue(0, 33.33F);
		variable.SetValue(1, 15.88F);
		variable.SetValue(2, 1234.567F);
		variable.SetValue(3, 12.0F);
		variable.SetValue(4, 24.0F);

		Assert::AreEqual(4, variable.GetValueCount());
		Assert::AreEqual(33.33F, variable.GetValueFloat(0));
		Assert::AreEqual(15.88F, variable.GetValueFloat(1));
		Assert::AreEqual(1234.567F, variable.GetValueFloat(2));
		Assert::AreEqual(12.0F, variable.GetValueFloat(3));
	}

	static void AddAndSetFloat(VariableCollection* pVariableCollection, const char* pName, float value, int stackLevel)
	{
		Variable variable(value);
		pVariableCollection->AddAndSet(pName, &variable, stackLevel);
	}

	static void TestNameTooLong()
	{
		Variable variable;

		Serial.SetOutput(0);
		variable.SetVariableName("A234567890A234567890A234567890A234567890A234567890A234567890A234567890");
		Serial.SetOutput(1);

		Assert::AreEqual("", variable.GetVariableName());
		Assert::AreEqual("Variable too long", Serial.GetLastString());
	}

	static void TestCollectionWorks()
	{
		VariableCollection variableCollection;

		AddAndSetFloat(&variableCollection, "Fred", 55.0F, 1);
		AddAndSetFloat(&variableCollection, "Barney", 55.0F, 1);
		AddAndSetFloat(&variableCollection, "Wilma", 55.0F, 1);

		Variable* pVariable = variableCollection.Get(2);
		pVariable->SetValue(0, 15.0F);
	}

	static void TestAddVariableTwice()
	{
		VariableCollection variableCollection;
		ParseErrors parseErrors;

		AddAndSetFloat(&variableCollection, "Fred", 55.0F, 1);
		AddAndSetFloat(&variableCollection, "Fred", 55.0F, 1);

		Assert::AreEqual(1, variableCollection.GetActiveVariableCount());
		Variable* pParsed = variableCollection.Lookup("Fred", 1, &parseErrors, 1);
		Assert::AreEqual(55.0F, pParsed->GetValueFloat(0));
	}

	static void TestDuplicateNameOnNewLevelAndDelete()
	{
		VariableCollection variableCollection;
		ParseErrors parseErrors;

		AddAndSetFloat(&variableCollection, "Fred", 55.0F, 1);
		AddAndSetFloat(&variableCollection, "Fred", 100.0F, 2);

		Assert::AreEqual(2, variableCollection.GetActiveVariableCount());
		Variable* pParsed = variableCollection.Lookup("Fred", 2, &parseErrors, 1);
		Assert::AreEqual(100.0F, pParsed->GetValueFloat(0));

		variableCollection.DeleteStackLevel(2);

		Assert::AreEqual(1, variableCollection.GetActiveVariableCount());
		pParsed = variableCollection.Lookup("Fred", 1, &parseErrors, 1);
		Assert::AreEqual(55.0F, pParsed->GetValueFloat(0));
	}

	static void TestGetVariableName()
	{
		const char* pCommand = "Test=15";

		char variableName[64];
		const char *pRemaining = VariableCollection::GetVariableName(pCommand, variableName, sizeof(variableName));

		Assert::AreEqual("Test", variableName);
		Assert::AreEqual("=15", pRemaining);
	}

	static void TestGetVariableNameBufferTooSmall()
	{
		const char* pCommand = "Test=15";

		char variableName[2];
		Serial.SetOutput(0);
		const char *pRemaining = VariableCollection::GetVariableName(pCommand, variableName, sizeof(variableName));
		Serial.SetOutput(1);

		Assert::AreEqual("Variable name too long", Serial.GetLastString());
		Assert::AreEqual("", variableName);
	}

	static void TestMissingVariable()
	{
		VariableCollection variableCollection;
		ParseErrors parseErrors;

		Variable* pParsed = variableCollection.Lookup("Fred", 1, &parseErrors, 15);
		Assert::AreEqual(1, parseErrors.GetErrorCount());
		Assert::AreEqual("Undeclared variable: Fred", parseErrors.GetError(0)->_errorText);
		Assert::AreEqual(15, parseErrors.GetError(0)->_lineNumber);
	}

	static void TestMissingVariableNoErrorCheck()
	{
		VariableCollection variableCollection;
		ParseErrors parseErrors;

		Variable* pParsed = variableCollection.GetWithoutErrorCheck("Fred", 1);
		Assert::AreEqual(0, (int) pParsed);
	}


	static void TestClearCollection()
	{
		VariableCollection variableCollection;
		ParseErrors parseErrors;

		AddAndSetFloat(&variableCollection, "Fred", 55.0F, 1);
		Assert::AreEqual(1, variableCollection.GetActiveVariableCount());

		variableCollection.Clear();
		Assert::AreEqual(0, variableCollection.GetActiveVariableCount());
	}

	static void TestDelete()
	{
		VariableCollection variableCollection;
		AddAndSetFloat(&variableCollection, "Fred", 55.0F, 1);
		AddAndSetFloat(&variableCollection, "Barney", 15.0F, 1);
		Assert::AreEqual(2, variableCollection.GetActiveVariableCount());

		variableCollection.Delete("Fred", 1);
		Assert::AreEqual(1, variableCollection.GetActiveVariableCount());

		Variable* pParsed = variableCollection.GetWithoutErrorCheck("Fred", 1);
		Assert::AreEqual(0, (int)pParsed);
	}

	static void TestRename()
	{
		VariableCollection variableCollection;
		ParseErrors parseErrors;

		AddAndSetFloat(&variableCollection, "Fred", 55.0F, 1);

		Assert::AreEqual(1, variableCollection.GetActiveVariableCount());
		Variable* pParsed = variableCollection.Lookup("Fred", 1, &parseErrors, 1);
		Assert::AreEqual(55.0F, pParsed->GetValueFloat(0));
	}

	static void TestAddFloatList()
	{
		Variable variable;

		variable.SetValue(0, 33.33F);
		variable.SetValue(1, 15.88F);
		variable.SetValue(2, 1234.567F);

		Assert::AreEqual(3, variable.GetValueCount());
		Assert::AreEqual(33.33F, variable.GetValueFloat(0));
		Assert::AreEqual(15.88F, variable.GetValueFloat(1));
		Assert::AreEqual(1234.567F, variable.GetValueFloat(2));

		VariableCollection variableCollection;
		ParseErrors parseErrors;

		variableCollection.AddAndSet("Fred", &variable, 1);

		Assert::AreEqual(1, variableCollection.GetActiveVariableCount());
		Variable* pParsed = variableCollection.Lookup("Fred", 1, &parseErrors, 1);
		Assert::AreEqual(3, pParsed->GetValueCount());
		Assert::AreEqual(33.33F, pParsed->GetValueFloat(0));
		Assert::AreEqual(15.88F, pParsed->GetValueFloat(1));
		Assert::AreEqual(1234.567F, pParsed->GetValueFloat(2));
	}

	static void TestTooMany()
	{
		VariableCollection variableCollection;
		ParseErrors parseErrors;

		for (int i = 0; i < 30; i++)
		{
			char buffer[10];

			sprintf(buffer, "Var%d", i);
			variableCollection.Add(buffer, 0);
		}

		Assert::AreEqual(30, variableCollection.GetActiveVariableCount());

		Serial.SetOutput(0);
		variableCollection.Add("Var31", 0);
		Serial.SetOutput(1);

		Assert::AreEqual(30, variableCollection.GetActiveVariableCount());

		Assert::AreEqual("Too many variables", Serial.GetLastString());
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
		TestClear();
		TestSetValueSetsValueCount();

		TestCollectionWorks();

		TestAddVariableTwice();

		TestGetVariableName();
		TestNameTooLong();

		TestMissingVariable();
		TestMissingVariableNoErrorCheck();
		TestClearCollection();
		TestDelete();
		TestDuplicateNameOnNewLevelAndDelete();

		TestFloatList();
		TestFloatListTooMany();
		TestAddFloatList();

		TestGetVariableNameBufferTooSmall();

		TestTooMany();

		TestConstructorString();

		return 0;
	}
};
