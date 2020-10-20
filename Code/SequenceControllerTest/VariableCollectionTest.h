#include "VariableCollection.h"

class VariableCollectionTest
{
    static void AddAndSetFloat(VariableCollection* pVariableCollection, const char* pName, float value, int stackLevel)
    {
        Variable variable(value);
        pVariableCollection->AddAndSet(pName, &variable, stackLevel);
    }

    static void TestCollectionWorks()
    {
        //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_CHECK_CRT_DF | _CRTDBG_LEAK_CHECK_DF);

        VariableCollection variableCollection;

        AddAndSetFloat(&variableCollection, "Fred", 55.0F, 1);
        AddAndSetFloat(&variableCollection, "Barney", 55.0F, 1);
        AddAndSetFloat(&variableCollection, "Wilma", 55.0F, 1);

        Variable* pVariable = variableCollection.Get(2);
        pVariable->SetValue(0, 15.0F);
    }

    static void TestCollectionCleanup()
    {
      int maxVariables = 100;

        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseCount());
        {
            VariableCollection variableCollection;

            Assert::AreEqual(maxVariables, VariableStore::VariableStoreInstance.GetInUseCount());
        }

        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseCount());
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
        Assert::AreEqual(0, pParsed);
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

        Variable* pFred = variableCollection.GetWithoutErrorCheck("Fred", 1);
        pFred->SetValue(1, 4.4F);

        variableCollection.Delete("Fred", 1);
        Assert::AreEqual(1, variableCollection.GetActiveVariableCount());

        pFred = variableCollection.GetWithoutErrorCheck("Fred", 1);
        Assert::AreEqual(0, pFred);

        // Add to make sure re-use is a uninitialized variable...
        AddAndSetFloat(&variableCollection, "Wilma", 12, 1);
        Variable* pWilma = variableCollection.GetWithoutErrorCheck("Wilma", 1);
        Assert::AreEqual(1, pWilma->GetValueCount());
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

        int maxVariables = 100;

        for (int i = 0; i < maxVariables; i++)
        {
            char buffer[10];

            sprintf(buffer, "Var%d", i);
            variableCollection.Add(buffer, 0);
        }

        Assert::AreEqual(maxVariables, variableCollection.GetActiveVariableCount());

        Serial.SetOutput(0);
        variableCollection.Add("VarTooMany", 0);
        Serial.SetOutput(1);

        Assert::AreEqual(maxVariables, variableCollection.GetActiveVariableCount());

        Assert::AreEqual("Too many variables", Serial.GetLastString());
    }

    static void TestAdd()
    {
        VariableCollection variableCollection;
        Variable* pVariable = variableCollection.Add("Hello", 12);

        Assert::AreEqual("Hello", pVariable->GetVariableName());
        Assert::AreEqual(12, pVariable->GetStackLevel());
    }

public:

    static int Run()
    {
        TestCollectionWorks();

        TestAddVariableTwice();

        TestGetVariableName();

        TestMissingVariable();
        TestMissingVariableNoErrorCheck();
        TestClearCollection();
        TestDelete();
        TestDuplicateNameOnNewLevelAndDelete();

        TestAddFloatList();

        TestGetVariableNameBufferTooSmall();

        TestTooMany();

        TestAdd();

        TestCollectionCleanup();

        return 0;
    }
};
#pragma once
