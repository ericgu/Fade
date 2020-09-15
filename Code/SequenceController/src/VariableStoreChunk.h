#pragma once

class VariableStoreChunk
{
    int _poolSize;
    VariableData *_pPool;

public:
    VariableStoreChunk(int poolSize)
    {
        _poolSize = poolSize;
        _pPool = new VariableData[_poolSize];
        memset(_pPool, 0, sizeof(VariableData) * _poolSize);
    }

    ~VariableStoreChunk()
    {
        delete[] _pPool;
    }

    int GetSize()
    {
        return _poolSize;
    }

    VariableData *GetFreePoolEntry()
    {
        VariableData *pVariableData = _pPool;

        for (int i = 0; i < _poolSize; i++)
        {
            pVariableData = _pPool + i;

            if (pVariableData->_referenceCount == 0)
            {
                pVariableData->_referenceCount = 1;
                return pVariableData;
            }
        }

        return 0;
    }

    int GetInUseCount()
    {
        int inUse = 0;

        VariableData *pVariableData = _pPool;

        for (int i = 0; i < _poolSize; i++)
        {
            if (pVariableData->_referenceCount != 0)
            {
                inUse++;
            }

            pVariableData++;
        }

        return inUse;
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
        return _pPool + index;
    }
};