#pragma once

#include "Math.h"

class VariableStore
{
    static const int MaxChunks = 200;

    int _chunkCount;

    VariableStoreChunk *_pChunks[MaxChunks];

    VariableData* _pFreeList;
    int _inUseCount;

public:
    static VariableStore VariableStoreInstance;

    VariableStore()
    {
        _chunkCount = 0;
        _pFreeList = 0;
        _inUseCount = 0;
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
        _pFreeList = 0;
        _inUseCount = 0;
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

        VariableData* pVariableData = pChunk->GetDataByIndex(0);

        for (int chunk = 0; chunk < pChunk->GetSize(); chunk++)
        {
            MoveToFreeList(pVariableData);
            pVariableData++;
        }
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

        VariableData *pVariableData = _pFreeList;

        if (pVariableData)
        {
            _pFreeList = pVariableData->_pNext;
            _inUseCount++;
            pVariableData->IncrementReferenceCount();
            return pVariableData;
        }



#if fred
        //int lastSize;
        for (int chunk = 0; chunk < _chunkCount; chunk++)
        {
            pVariableData = _pChunks[chunk]->GetFreePoolEntry();
            if (pVariableData != 0)
            {
                return pVariableData;
            }

            //lastSize = _pChunks[chunk]->GetSize();
        }
#endif

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
        return _inUseCount;
    }

    VariableData *SplitOffEntry(VariableData *pVariableData)
    {
        VariableData *pSplit = GetFreePoolEntry();
        if (pSplit == 0)
        {
            return 0;
        }

        *pSplit = *pVariableData;

        DecrementReferenceCount(pVariableData);
        //pSplit->IncrementReferenceCount();

        return pSplit;
    }

    void IncrementReferenceCount(VariableData *pVariableData)
    {
        pVariableData->IncrementReferenceCount();
    }

    void MoveToFreeList(VariableData *pVariableData)
    {
        pVariableData->_pNext = _pFreeList;
        _pFreeList = pVariableData;
    }


    void DecrementReferenceCount(VariableData *pVariableData)
    {
        pVariableData->DecrementReferenceCountInternal();

        if (pVariableData->GetReferenceCount() <= 0)
        {
            MoveToFreeList(pVariableData);
            _inUseCount--;
        }
    }

#if fred
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
#endif
};

VariableStore VariableStore::VariableStoreInstance;
