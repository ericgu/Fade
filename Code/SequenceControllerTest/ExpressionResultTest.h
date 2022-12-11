//#include "ExpressionResult.h"

class ExpressionResultTest
{
    static void TestExpressionResultCleanup()
    {
        Vector::RestartVectorDataProvider();
        {
            Vector v; // force creation of data provider...
        }

        VariableData* pVariableData;

        Assert::AreEqual(0, Vector::_pVectorDataItemProvider->GetInUseCount());
        {
            ExpressionResult expressionResult;
            expressionResult._variable.SetValue(0, 15.0f);
            pVariableData = expressionResult._variable.TestOnlyGetVariableData();
            Assert::AreEqual(1, pVariableData->GetReferenceCount());

            Assert::AreEqual(1, Vector::_pVectorDataItemProvider->GetInUseCount());
        }
        Assert::AreEqual(0, Vector::_pVectorDataItemProvider->GetInUseCount());
        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseCount());
    }

    static void TestExpressionResultOperatorEqualsCleanup()
    {
        Vector::RestartVectorDataProvider();
        {
            Vector v; // force creation of data provider...
        }

        VariableData* pVariableData;

        Assert::AreEqual(0, Vector::_pVectorDataItemProvider->GetInUseCount());
        {
            ExpressionResult expressionResult;
            expressionResult._variable.SetValue(0, 15.0f);
            ExpressionResult expressionResult2;
            expressionResult2 = expressionResult;
            pVariableData = expressionResult._variable.TestOnlyGetVariableData();
            Assert::AreEqual(2, pVariableData->GetReferenceCount());

            Assert::AreEqual(1, Vector::_pVectorDataItemProvider->GetInUseCount());
        }
        Assert::AreEqual(0, Vector::_pVectorDataItemProvider->GetInUseCount());
        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseCount());
    }

    static void TestExpressionResultCopyConstructorCleanup()
    {
        Vector::RestartVectorDataProvider();
        {
            Vector v; // force creation of data provider...
        }

        VariableData* pVariableData;

        Assert::AreEqual(0, Vector::_pVectorDataItemProvider->GetInUseCount());
        {
            ExpressionResult expressionResult;
            expressionResult._variable.SetValue(0, 15.0f);
            ExpressionResult expressionResult2 = expressionResult;
            pVariableData = expressionResult._variable.TestOnlyGetVariableData();
            Assert::AreEqual(2, pVariableData->GetReferenceCount());

            Assert::AreEqual(1, Vector::_pVectorDataItemProvider->GetInUseCount());
        }
        Assert::AreEqual(0, Vector::_pVectorDataItemProvider->GetInUseCount());
        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseCount());
    }

public:
    static void Run()
    {
        TestExpressionResultCleanup();
        TestExpressionResultOperatorEqualsCleanup();
        TestExpressionResultCopyConstructorCleanup();
    }
};
#pragma once
