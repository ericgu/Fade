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
    }

    ~VariableStore()
    {
        ResetCache();
    }

    void ResetCache()
    {
        for (int i = 0; i < _chunkCount; i++)
        {
            delete _pChunks[i];
            _pChunks[i] = 0;
        }

        _chunkCount = 0;
    }

    void AddChunk()
    {
        if (_chunkCount == MaxChunks)
        {
            Serial.println("VariableStore::Too many chunks");
        }

        VariableStoreChunk *pChunk = new VariableStoreChunk(Environment.VariableStoreChunkSize);
        _pChunks[_chunkCount] = pChunk;
        _chunkCount++;
    }

    VariableData *GetFreePoolEntry()
    {
        if (Environment.DebugLogHeapFreeOnAllocation)
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

        int lastSize;
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

        AddChunk();
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

        *pSplit = *pVariableData;

        pVariableData->DecrementReferenceCount();
        //pSplit->IncrementReferenceCount();

        return pSplit;
    }

    void IncrementReferenceCount(VariableData *pVariableData)
    {
        pVariableData->IncrementReferenceCount();
    }

    void DecrementReferenceCount(VariableData *pVariableData)
    {
        pVariableData->DecrementReferenceCount();

        if (pVariableData->GetReferenceCount() == 0)
        {
            //int k = -12;
        }
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
