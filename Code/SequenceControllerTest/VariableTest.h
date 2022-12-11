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
        Variable variable(15);
        Assert::AreEqual(15, variable.GetValueInt());
        Assert::AreEqual(15.0F, variable.GetValueFloat(0));
    }

    static void TestConstructorFloat()
    {
        Variable variable(15.0F);
        Assert::AreEqual(15.0F, variable.GetValueFloat(0));
    }

    static void TestSetValueSetsValueCount()
    {
        Variable variable;
        variable.SetValue(0, 15.0F);
        Assert::AreEqual(15.0F, variable.GetValueFloat(0));
        Assert::AreEqual(1, variable.GetValueCount());
    }

    static void TestGetValueReturnsNanIfNotSet()
    {
        Variable variable;
        Assert::AreEqual(1, isnan(variable.GetValueFloat(0)));
        Assert::AreEqual(1, isnan(variable.GetValueFloat(1)));
        Assert::AreEqual(1, isnan(variable.GetValueFloat(2)));
        Assert::AreEqual(1, isnan(variable.GetValueFloat(3)));

        variable.SetValue(0, 1.0F);
        variable.SetValue(1, 2.0F);
        variable.SetValue(2, 4.0F);
        variable.SetValue(3, 8.0F);

        Assert::AreEqual(1.0F, variable.GetValueFloat(0));
        Assert::AreEqual(2.0F, variable.GetValueFloat(1));
        Assert::AreEqual(4.0F, variable.GetValueFloat(2));
        Assert::AreEqual(8.0F, variable.GetValueFloat(3));

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

        Assert::AreEqual(0, variable.GetValueCount());
        Assert::AreEqual(0, variable.GetStackLevel());
    }


    static void TestConstructorString()
    {
        Variable variable("MyNewString");
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

    static void TestNameTooLong()
    {
        Variable variable;

        Serial.SetOutput(0);
        variable.SetVariableName("A234567890A234567890A234567890A234567890A234567890A234567890A234567890");
        Serial.SetOutput(1);

        Assert::AreEqual("", variable.GetVariableName());
        Assert::AreEqual("Variable too long", Serial.GetLastString());
    }

    static void TestDestructorDecrementsReferenceCount()
    {
        VariableData* pVariableData;
        {
            Variable variable(15);
            pVariableData = variable.TestOnlyGetVariableData();
            Assert::AreEqual(1, pVariableData->GetReferenceCount());
        }

        Assert::AreEqual(0, pVariableData->GetReferenceCount());
    }

    static void TestDestructorCleansUpVectorUsage()
    {
        Vector::RestartVectorDataProvider();
        {
            Vector v; // force creation of data provider...
        }

        VariableData* pVariableData;

        Assert::AreEqual(0, Vector::_pVectorDataItemProvider->GetInUseCount());
        {
            Variable variable(15);
            pVariableData = variable.TestOnlyGetVariableData();
            Assert::AreEqual(1, pVariableData->GetReferenceCount());

            Assert::AreEqual(1, Vector::_pVectorDataItemProvider->GetInUseCount());
        }
        Assert::AreEqual(0, Vector::_pVectorDataItemProvider->GetInUseCount());
    }


    static void TestCopyUsesSameVariableData()
    {
        Variable variable(15);

        Variable copy = variable;

        Assert::AreEqual(variable.TestOnlyGetVariableData(), copy.TestOnlyGetVariableData());
        Assert::AreEqual(2, variable.TestOnlyGetVariableData()->GetReferenceCount());
    }

    static void TestCopyConstructorSplitsDataIfSourceStackLevelIsNotZero()
    {
        Variable variable(15);
        variable.TestOnlyGetVariableData()->_stackLevel = 1;

        Variable copy = variable;

        Assert::AreEqual(0, variable.TestOnlyGetVariableData() == copy.TestOnlyGetVariableData());
        Assert::AreEqual(1, variable.TestOnlyGetVariableData()->GetReferenceCount());
        Assert::AreEqual(1, variable.GetValueCount());
        Assert::AreEqual(15.0F, variable.GetValueFloat(0));
        Assert::AreEqual(1, copy.TestOnlyGetVariableData()->GetReferenceCount());
        Assert::AreEqual(0, copy.TestOnlyGetVariableData()->_stackLevel);
    }

    static void TestCopyAndSetValuePreservesValues()
    {
        Variable variable(15);

        Variable copy = variable;

        variable.SetValue(0, 22.0F);

        Assert::AreEqual(22.0F, variable.GetValueFloat(0));
        Assert::AreEqual(15.0F, copy.GetValueFloat(0));

        Assert::AreEqual(1, variable.TestOnlyGetVariableData()->GetReferenceCount());
        Assert::AreEqual(1, copy.TestOnlyGetVariableData()->GetReferenceCount());
    }

    static void TestCopyAndIncrementSplits()
    {
        Variable variable(15);

        Variable copy = variable;

        variable.Increment(6);

        Assert::AreEqual(21.0F, variable.GetValueFloat(0));
        Assert::AreEqual(15.0F, copy.GetValueFloat(0));

        Assert::AreEqual(1, variable.TestOnlyGetVariableData()->GetReferenceCount());
        Assert::AreEqual(1, copy.TestOnlyGetVariableData()->GetReferenceCount());
    }

    static void TestCopyAndSetStringValueSplits()
    {
        Variable variable(15);

        Variable copy = variable;

        variable.SetValue("Hello");

        Assert::AreEqual("Hello", variable.GetValueString());
        Assert::AreEqual(15.0F, copy.GetValueFloat(0));

        Assert::AreEqual(1, variable.TestOnlyGetVariableData()->GetReferenceCount());
        Assert::AreEqual(1, copy.TestOnlyGetVariableData()->GetReferenceCount());
    }

    static void TestCopyAndSetVariableNameSplits()
    {
        Variable variable(15);

        Variable copy = variable;

        variable.SetVariableName("Barney");

        Assert::AreEqual("Barney", variable.GetVariableName());
        Assert::AreEqual(15.0F, copy.GetValueFloat(0));

        Assert::AreEqual(1, variable.TestOnlyGetVariableData()->GetReferenceCount());
        Assert::AreEqual(1, copy.TestOnlyGetVariableData()->GetReferenceCount());
    }

    static void TestCopyAndSetStackLevelSplits()
    {
        Variable variable(15);

        Variable copy = variable;

        variable.SetStackLevel(222);

        Assert::AreEqual(222, variable.GetStackLevel());
        Assert::AreEqual(15.0F, copy.GetValueFloat(0));

        Assert::AreEqual(1, variable.TestOnlyGetVariableData()->GetReferenceCount());
        Assert::AreEqual(1, copy.TestOnlyGetVariableData()->GetReferenceCount());
    }

    static void TestCopyAndSetNaNSplits()
    {
        Variable variable(15);

        Variable copy = variable;

        variable.SetToNan();

        Assert::AreEqual(1, variable.IsNan());
        Assert::AreEqual(15.0F, copy.GetValueFloat(0));

        Assert::AreEqual(1, variable.TestOnlyGetVariableData()->GetReferenceCount());
        Assert::AreEqual(1, copy.TestOnlyGetVariableData()->GetReferenceCount());
    }

    static void TestAssignmentOperator()
    {
        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseCount());
        {
            Variable variable(15);
            Variable copy(10);

            VariableData* pCopyVariableData = copy.TestOnlyGetVariableData();

            copy = variable;

            Assert::AreEqual(2, variable.TestOnlyGetVariableData()->GetReferenceCount());
            Assert::AreEqual(0, pCopyVariableData->GetReferenceCount());

        }
        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseCount());
    }

    static void TestAssignmentOperatorSplitsIfSourceIsAProgramVariable()
    {
        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseCount());
        {
            Variable variable(15);
            variable.TestOnlyGetVariableData()->_stackLevel = 1;
            Variable copy(10);

            copy = variable;

            Assert::AreEqual(1, variable.TestOnlyGetVariableData()->GetReferenceCount());
            Assert::AreEqual(1, copy.TestOnlyGetVariableData()->GetReferenceCount());
            Assert::AreEqual(0, copy.TestOnlyGetVariableData()->_stackLevel);
        }
        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseCount());
    }

    static void TestCopyConstructor()
    {
        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseCount());
        {
            Variable variable(15);
            Variable copy = variable;

            Assert::AreEqual(2, variable.TestOnlyGetVariableData()->GetReferenceCount());

        }
        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseCount());
    }

    static void TestCleanup()
    {
        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseCount());

        {
            Variable variable(15);
        }

        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseCount());
    }

    static Variable ReturnValue()
    {
        return Variable(333);
    }

    static void TestReturnValue()
    {
        Variable variable = ReturnValue();

        Assert::AreEqual(1, variable.TestOnlyGetVariableData()->GetReferenceCount());
    }

    static void TestNaN()
    {
        Variable variable(15.5F);

        Assert::AreEqual(0, variable.IsNan());
        variable.SetToNan();
        Assert::AreEqual(1, variable.IsNan());
        Assert::AreEqual(0, variable.GetValueCount());

    }


    static void TestEmpty()
    {
        Variable variable = Variable::Empty();

        Assert::AreEqual(1, variable.IsNan());
    }

    static void TestStoreFreeList()
    {
        VariableStore::VariableStoreInstance.TrackInUseList(true);

        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseCount());
        Assert::AreEqual(128, VariableStore::VariableStoreInstance.GetFreeListCount());
        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseListCount());
        {
            Variable variable(15);
            Assert::AreEqual(1, VariableStore::VariableStoreInstance.GetInUseCount());
            Assert::AreEqual(127, VariableStore::VariableStoreInstance.GetFreeListCount());
            Assert::AreEqual(1, VariableStore::VariableStoreInstance.GetInUseListCount());

            {
                Variable* pVariable2 = new Variable(10);
                Variable* pVariable3 = new Variable(20);
                Variable* pVariable4 = new Variable(30);
                Variable* pVariable5 = new Variable(40);
                Assert::AreEqual(5, VariableStore::VariableStoreInstance.GetInUseCount());
                Assert::AreEqual(123, VariableStore::VariableStoreInstance.GetFreeListCount());
                Assert::AreEqual(5, VariableStore::VariableStoreInstance.GetInUseListCount());

                delete pVariable4;
                delete pVariable3;
                delete pVariable2;
                delete pVariable5;
            }
            Assert::AreEqual(1, VariableStore::VariableStoreInstance.GetInUseCount());
            Assert::AreEqual(127, VariableStore::VariableStoreInstance.GetFreeListCount());
            Assert::AreEqual(1, VariableStore::VariableStoreInstance.GetInUseListCount());
        }

        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseCount());
        Assert::AreEqual(128, VariableStore::VariableStoreInstance.GetFreeListCount());
        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseListCount());

        VariableStore::VariableStoreInstance.TrackInUseList(false);
    }

public:
    static void Run()
    {
        TestParseInt();
        TestParseFloat();
        TestParseNegative();
        TestIncrement();

        TestConstructorInt();
        TestConstructorFloat();

        TestSetValueSetsValueCount();
        TestGetValueReturnsNanIfNotSet();
        TestClear();

        TestConstructorString();

        TestFloatList();

        TestNameTooLong();

        TestNaN();
        TestEmpty();

        TestDestructorDecrementsReferenceCount();
        TestCopyUsesSameVariableData();
        TestCopyAndSetValuePreservesValues();
        TestReturnValue();
        TestCopyAndIncrementSplits();
        TestCopyAndSetStringValueSplits();
        TestCopyAndSetVariableNameSplits();
        TestCopyAndSetStackLevelSplits();
        TestCopyAndSetNaNSplits();
        TestCleanup();

        TestCopyConstructor();
        TestAssignmentOperator();
        TestCopyConstructorSplitsDataIfSourceStackLevelIsNotZero();
        TestAssignmentOperatorSplitsIfSourceIsAProgramVariable();

        TestDestructorCleansUpVectorUsage();

        TestStoreFreeList();
    }
};
