#include <crtdbg.h>
#include "VariableStore.h"

class VariableStoreTest
{
    static void TestCreate()
    {
        VariableData* pVariableData = VariableStore::VariableStoreInstance.GetFreePoolEntry();
        Assert::AreEqual(1, pVariableData != 0);

        Assert::AreEqual(1, VariableStore::VariableStoreInstance.GetInUseCount());

        pVariableData->_referenceCount--;

        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseCount());
    }

    static void TestCreateAndReferenceCounting()
    {
        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseCount());

        VariableData* pFirst = VariableStore::VariableStoreInstance.GetFreePoolEntry();
        VariableData* pSecond = VariableStore::VariableStoreInstance.GetFreePoolEntry();

        Assert::AreEqual(2, VariableStore::VariableStoreInstance.GetInUseCount());

        Assert::AreEqual(0, pFirst == pSecond);
        Assert::AreEqual(1, pFirst->_referenceCount);

        VariableStore::VariableStoreInstance.IncrementReferenceCount(pFirst);

        Assert::AreEqual(2, pFirst->_referenceCount);

        VariableStore::VariableStoreInstance.DecrementReferenceCount(pFirst);
        VariableStore::VariableStoreInstance.DecrementReferenceCount(pFirst);

        Assert::AreEqual(0, pFirst->_referenceCount);

        pSecond->_referenceCount--;

        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseCount());
    }

    static void TestSplit()
    {
        Assert::AreEqual(0, VariableStore::VariableStoreInstance.GetInUseCount());

        VariableData* pFirst = VariableStore::VariableStoreInstance.GetFreePoolEntry();
        pFirst->_valueCount = 15;

        VariableStore::VariableStoreInstance.IncrementReferenceCount(pFirst);
        VariableStore::VariableStoreInstance.IncrementReferenceCount(pFirst);
        Assert::AreEqual(3, pFirst->_referenceCount);

        VariableData* pSplit = VariableStore::VariableStoreInstance.SplitOffEntry(pFirst);

        Assert::AreEqual(2, pFirst->_referenceCount);
        Assert::AreEqual(1, pSplit->_referenceCount);

        Assert::AreEqual(15, pFirst->_valueCount);
        Assert::AreEqual(15, pSplit->_valueCount);

        pFirst->_referenceCount--;
        pFirst->_referenceCount--;
        pSplit->_referenceCount--;

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
                variableData[i]->_referenceCount = 0;
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


