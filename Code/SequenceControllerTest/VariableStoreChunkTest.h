#pragma once

#include "VariableStoreChunk.h"

class VariableStoreChunkTest
{
#if fred
    static void TestCreate()
    {
        VariableStoreChunk variableStoreChunk(32);
        Assert::AreEqual(32, variableStoreChunk.GetSize());

        VariableData* pVariableData = variableStoreChunk.GetFreePoolEntry();
        Assert::AreEqual(1, pVariableData != 0);

        Assert::AreEqual(1, variableStoreChunk.GetInUseCount());

        pVariableData->DecrementReferenceCount();

        Assert::AreEqual(0, variableStoreChunk.GetInUseCount());
    }

    static void TestCreateAndReferenceCounting()
    {
        VariableStoreChunk variableStoreChunk(32);

        Assert::AreEqual(0, variableStoreChunk.GetInUseCount());

        VariableData* pFirst = variableStoreChunk.GetFreePoolEntry();
        VariableData* pSecond = variableStoreChunk.GetFreePoolEntry();

        Assert::AreEqual(2, variableStoreChunk.GetInUseCount());

        Assert::AreEqual(0, pFirst == pSecond);
        Assert::AreEqual(1, pFirst->GetReferenceCount());

        pFirst->IncrementReferenceCount();

        Assert::AreEqual(2, pFirst->GetReferenceCount());

        pFirst->DecrementReferenceCount();
        pFirst->DecrementReferenceCount();

        Assert::AreEqual(0, pFirst->GetReferenceCount());

        pSecond->DecrementReferenceCount();

        Assert::AreEqual(0, variableStoreChunk.GetInUseCount());
    }


    static void TestCapacity()
    {
        VariableStoreChunk variableStoreChunk(101);

        Assert::AreEqual(0, variableStoreChunk.GetInUseCount());
        {
            const int variableCount = 100;

            VariableData* variableData[variableCount];

            for (int i = 0; i < variableCount; i++)
            {
                variableData[i] = variableStoreChunk.GetFreePoolEntry();
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

        Assert::AreEqual(0, variableStoreChunk.GetInUseCount());
    }
#endif


public:
    static void Run()
    {
//        TestCreate();
  //      TestCreateAndReferenceCounting();
    //    TestCapacity();
    }
};


