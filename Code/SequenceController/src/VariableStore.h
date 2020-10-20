#pragma once

#include "Math.h"

class VariableStore
{
    static const int MaxChunks = 200;

    int _chunkCount;

    VariableStoreChunk *_pChunks[MaxChunks];

public:
    static VariableStore VariableStoreInstance;

    VariableStore()
    {
        _chunkCount = 0;

        AddChunk(64);
    }

    ~VariableStore()
    {
        for (int i = 0; i < _chunkCount; i++)
        {
            delete _pChunks[i];
        }
    }

    void AddChunk(int chunkSize)
    {
        if (_chunkCount == MaxChunks)
        {
            Serial.println("VariableStore::Too many chunks");
        }

        VariableStoreChunk *pChunk = new VariableStoreChunk(chunkSize);
        _pChunks[_chunkCount] = pChunk;
        _chunkCount++;
    }

    VariableData *GetFreePoolEntry()
    {
        if (DebugFlags.LogHeapFreeOnAllocation)
        {
            Serial.print("VariableStore::GetFreePoolEntry:: HeapSize ");
            Serial.print(EspFunctions::GetFreeHeap());

            int variableCount = 0;
            for (int chunk = 0; chunk < _chunkCount; chunk++)
            {
                variableCount += _pChunks[chunk]->GetInUseCount();
            }
            Serial.print(" ");
            Serial.println(variableCount);
        }

        VariableData *pVariableData;

        int lastSize = 0;
        for (int chunk = 0; chunk < _chunkCount; chunk++)
        {
            pVariableData = _pChunks[chunk]->GetFreePoolEntry();
            if (pVariableData != 0)
            {
                return pVariableData;
            }

            lastSize = _pChunks[chunk]->GetSize();
        }

        if (_chunkCount == MaxChunks)
        {
            Serial.println("Out of space in VariableStore...");
            return 0;
        }

        AddChunk(lastSize);
        return GetFreePoolEntry();
    }

    int GetInUseCount()
    {
        int inUse = 0;

        for (int chunk = 0; chunk < _chunkCount; chunk++)
        {
            inUse += _pChunks[chunk]->GetInUseCount();
        }

        return inUse;
    }

    VariableData *SplitOffEntry(VariableData *pVariableData)
    {
        VariableData *pSplit = GetFreePoolEntry();
        if (pSplit == 0)
        {
            return 0;
        }

        memcpy(pSplit, pVariableData, sizeof(VariableData));
        pVariableData->_referenceCount--;
        pSplit->_referenceCount = 1;

        return pSplit;
    }

    void IncrementReferenceCount(VariableData *pVariableData)
    {
        pVariableData->_referenceCount++;
    }

    void DecrementReferenceCount(VariableData *pVariableData)
    {
        pVariableData->_referenceCount--;
    }

    VariableData *GetDataByIndex(int index)
    {
        for (int chunk = 0; chunk < _chunkCount; chunk++)
        {
            if (index < _pChunks[chunk]->GetSize())
            {
                return _pChunks[chunk]->GetDataByIndex(index);
            }

            index -= _pChunks[chunk]->GetSize();
        }

        return 0;
    }
};

VariableStore VariableStore::VariableStoreInstance;
