#pragma once

#include "Math.h"

class VariableStore
{
    static const int MaxChunks = 200;

    int _chunkCount;

    VariableStoreChunk *_pChunks[MaxChunks];

    VariableData* _pFreeList;
    VariableData* _pInUseList;
    int _inUseCount;
    bool _trackInUseList;

public:
    static VariableStore VariableStoreInstance;

    VariableStore()
    {
        _chunkCount = 0;
        _pFreeList = 0;
        _inUseCount = 0;
        _trackInUseList = false;
    }

    ~VariableStore()
    {
        ResetCache();
    }

    void TrackInUseList(bool enable)
    {
        _trackInUseList = enable;
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
            //Serial.print("VariableStore::GetFreePoolEntry:: HeapSize ");
            //Serial.print(EspFunctions::GetFreeHeap());

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

            if (_trackInUseList)
            {
                pVariableData->_pNext = _pInUseList;
                _pInUseList = pVariableData;
            }
            else
            {
                pVariableData->_pNext = 0;
            }

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

    void MyOutputDebugString(const char* pString)
    {
#if classification
        const size_t cSize = strlen(pString) + 1;
        wchar_t* wc = new wchar_t[cSize];
        mbstowcs(wc, pString, cSize);
        OutputDebugString(wc);
        delete wc;
#endif
    }


    int GetFreeListCount()
    {
        VariableData* pData = _pFreeList;

        int count = 0;
        while (pData != 0)
        {
            //MyOutputDebugString(pData->_classification);
            pData = pData->_pNext;
            count++;
        }


        return count;
    }


    int GetInUseListCount()
    {
        VariableData* pData = _pInUseList;

        int count = 0;
        while (pData != 0)
        {
            //MyOutputDebugString(pData->_classification);
            //MyOutputDebugString(" ");
            pData = pData->_pNext;
            count++;
        }

        return count;
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
            if (_trackInUseList)
            {
                if (pVariableData == _pInUseList)    // First item on the in-use list...
                {
                    _pInUseList = pVariableData->_pNext;
                }
                else
                {
                    VariableData* pNode = _pInUseList;

                    while (pNode->_pNext != 0)
                    {
                        if (pNode->_pNext == pVariableData)
                        {
                            pNode->_pNext = pNode->_pNext->_pNext;
                            break;
                        }
                        pNode = pNode->_pNext;
                    }
                }

            }

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
