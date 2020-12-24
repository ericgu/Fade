#include <crtdbg.h>
#include "VariableStore.h"

class VariableStoreTest
{
    static void TestCreate()
    {
        VariableData* pVariableData = VariableStore::VariableStoreInstance.GetFreePoolEntry();
        Assert::AreEqual(1, pVariableData != 0);

        Assert::AreEqual(1, VariableStore::VariableStoreInstance.GetInUseCount());

        pVariableData->DecrementReferenceCount();

        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseCount());
    }

    static void TestCreateAndReferenceCounting()
    {
        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseCount());

        VariableData* pFirst = VariableStore::VariableStoreInstance.GetFreePoolEntry();
        VariableData* pSecond = VariableStore::VariableStoreInstance.GetFreePoolEntry();

        Assert::AreEqual(2, VariableStore::VariableStoreInstance.GetInUseCount());

        Assert::AreEqual(0, pFirst == pSecond);
        Assert::AreEqual(1, pFirst->GetReferenceCount());

        VariableStore::VariableStoreInstance.IncrementReferenceCount(pFirst);

        Assert::AreEqual(2, pFirst->GetReferenceCount());

        VariableStore::VariableStoreInstance.DecrementReferenceCount(pFirst);
        VariableStore::VariableStoreInstance.DecrementReferenceCount(pFirst);

        Assert::AreEqual(0, pFirst->GetReferenceCount());

        pSecond->DecrementReferenceCount();

        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseCount());
    }

    static void TestSplit()
    {
        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseCount());

        VariableData* pFirst = VariableStore::VariableStoreInstance.GetFreePoolEntry();
        pFirst->_valueCount = 15;

        VariableStore::VariableStoreInstance.IncrementReferenceCount(pFirst);
        VariableStore::VariableStoreInstance.IncrementReferenceCount(pFirst);
        Assert::AreEqual(3, pFirst->GetReferenceCount());

        VariableData* pSplit = VariableStore::VariableStoreInstance.SplitOffEntry(pFirst);

        Assert::AreEqual(2, pFirst->GetReferenceCount());
        Assert::AreEqual(1, pSplit->GetReferenceCount());

        Assert::AreEqual(15, pFirst->_valueCount);
        Assert::AreEqual(15, pSplit->_valueCount);

        pFirst->DecrementReferenceCount();
        pFirst->DecrementReferenceCount();
        pSplit->DecrementReferenceCount();

        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseCount());
    }

    static void TestCapacity()
    {
        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseCount());
        {
            const int variableCount = 100;

            VariableData* variableData[variableCount];

            for (int i = 0; i < variableCount; i++)
            {
                variableData[i] = VariableStore::VariableStoreInstance.GetFreePoolEntry();
                variableData[i]->_valueCount = i;
            }

            for (int i = 0; i < variableCount; i++)
            {
                Assert::AreEqual(i, variableData[i]->_valueCount);
                while (variableData[i]->GetReferenceCount() > 0)
                {
                  variableData[i]->DecrementReferenceCount();
                }
            }
        }

        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseCount());
    }


public:
    static void Run()
    {
        TestCreate();
        TestCreateAndReferenceCounting();
        TestSplit();
        TestCapacity();
    }
};


