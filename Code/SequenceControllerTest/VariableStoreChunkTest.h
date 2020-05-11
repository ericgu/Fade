#pragma once

#include "VariableStoreChunk.h"

class VariableStoreChunkTest
{
    static void TestCreate()
    {
        VariableStoreChunk variableStoreChunk(32);
        Assert::AreEqual(32, variableStoreChunk.GetSize());

        VariableData* pVariableData = variableStoreChunk.GetFreePoolEntry();
        Assert::AreEqual(1, pVariableData != 0);

        Assert::AreEqual(1, variableStoreChunk.GetInUseCount());

        pVariableData->_referenceCount--;

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
        Assert::AreEqual(1, pFirst->_referenceCount);

        variableStoreChunk.IncrementReferenceCount(pFirst);

        Assert::AreEqual(2, pFirst->_referenceCount);

        variableStoreChunk.DecrementReferenceCount(pFirst);
        variableStoreChunk.DecrementReferenceCount(pFirst);

        Assert::AreEqual(0, pFirst->_referenceCount);

        pSecond->_referenceCount--;

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
                variableData[i]->_referenceCount = 0;
            }
        }

        Assert::AreEqual(0, variableStoreChunk.GetInUseCount());
    }


public:
    static void Run()
    {
        TestCreate();
        TestCreateAndReferenceCounting();
        TestCapacity();
    }
};


