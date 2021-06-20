#include <math.h>

#define DATAITEMUNUSED ((VectorDataItem*) 0xFFFFFFFFFFFFFFFF)

class VectorDataItem
{
public:
  VectorDataItem()
  {
    _data = 0;
    _pNext = DATAITEMUNUSED;
  }

  float           _data;
  VectorDataItem* _pNext;
};

class VectorDataItemProviderChunk
{
    int _size;
    int _probe;  // first spot to look for free items. Is set to 1 after the location where
                      // the previous free item was found.

    VectorDataItem* _pDataItems;

public:
    VectorDataItemProviderChunk(int size)
    {
        _size = size;
        _pDataItems = new VectorDataItem[size];
        _probe = 0;
    }

    VectorDataItem* GetDataItem()
    {
        // Walk through and find the first free item...


        for (int i = 0; i < _size; i++)
        {
            VectorDataItem* pItem = _pDataItems + _probe;

            if (pItem->_pNext == DATAITEMUNUSED)
            {
                //Serial.print("Probe: "); Serial.println(_probe);
                pItem->_pNext = 0;

                _probe = (_probe + 1) % _size;
                return pItem;
            }
            _probe = (_probe + 1) % _size;
        }

        return 0;
    }

    int GetSize()
    {
        return _size;
    }
};

class VectorDataItemProvider
{
    const int MaxChunks = 16;

    VectorDataItemProviderChunk* _pDataItemChunk[16];
    int _chunkCount;
    int _inUseCount;

public:
    VectorDataItemProvider()
    {
        _pDataItemChunk[0] = new VectorDataItemProviderChunk(Environment.VectorItemDataPoolCount);
        _chunkCount = 1;
        _inUseCount = 0;
    }

    ~VectorDataItemProvider()
    {
        for (int chunk = 0; chunk < _chunkCount; chunk++)
        {
            delete _pDataItemChunk[chunk];
        }
        _chunkCount = 0;
    }

    VectorDataItem* GetDataItem()
    {
        for (int chunk = 0; chunk < _chunkCount; chunk++)
        {
            VectorDataItem* pDataItem = _pDataItemChunk[chunk]->GetDataItem();
            if (pDataItem)
            {
                _inUseCount++;
                return pDataItem;
            }
        }

        // Allocate new chunk...

        if (_chunkCount == MaxChunks)
        {
            Serial.println("System Error: VectorDataItemProvider Maximum Chunks");
            return 0;
        }

        _pDataItemChunk[_chunkCount] = new VectorDataItemProviderChunk(Environment.VectorItemDataPoolCount);
        _chunkCount++;

        return _pDataItemChunk[_chunkCount - 1]->GetDataItem();
    }

    int GetInUseCount()
    {
        return _inUseCount;
    }

    void ReleaseDataItem(VectorDataItem* pItem)
    {
        _inUseCount--;
        pItem->_pNext = DATAITEMUNUSED;
    }

};

class Vector
{
public:
    static VectorDataItemProvider* _pVectorDataItemProvider;
    static int _pVectorDataItemProviderSize;

    VectorDataItem* _pFirstItem;
    int _itemCount;

    VectorDataItem* _pLastMatch;
    int _lastMatchIndex;

    static void RestartVectorDataProvider()
    {
        if (_pVectorDataItemProvider != 0)
        {
            delete _pVectorDataItemProvider;
            _pVectorDataItemProvider = 0;
        }
    }

    Vector()
    {
        if (_pVectorDataItemProvider == 0)
        {
            _pVectorDataItemProvider = new VectorDataItemProvider();
        }

        _itemCount = 0;
        _pFirstItem = 0;
    }

    ~Vector()
    {
        Cleanup();
    }

    void Cleanup()
    {
        if (_pFirstItem == 0)
        {
            return;
        }

        VectorDataItem* pItem = _pFirstItem;

        while (pItem->_pNext != 0)
        {
            VectorDataItem* pNext = pItem->_pNext;

            _pVectorDataItemProvider->ReleaseDataItem(pItem);

            pItem = pNext;
        }
        _pVectorDataItemProvider->ReleaseDataItem(pItem);

        _pFirstItem = 0;
        _itemCount = 0;
        _pLastMatch = 0;
        _lastMatchIndex = 0;
    }

    VectorDataItem* GetItemByIndex(int index)
    {
        VectorDataItem* pItem;
        int i;

        if (_pFirstItem == 0)
        {
            _pFirstItem = _pVectorDataItemProvider->GetDataItem();
            _pFirstItem->_pNext = 0;
            _pLastMatch = _pFirstItem;
            _lastMatchIndex = 0;
            _itemCount = 1;
        }

        // Forward traversal optimization; we keep track of the index and the pointer of the last get
        if (index >= _lastMatchIndex)
        {
            i = _lastMatchIndex;
            pItem = _pLastMatch;
        }
        else
        {
            i = 0;
            pItem = _pFirstItem;
        }

        for (; i < index; i++)
        {
            if (pItem->_pNext == 0)
            {
                pItem->_pNext = _pVectorDataItemProvider->GetDataItem();
                if (pItem->_pNext == 0)
                {
                    return 0;
                }
                pItem->_pNext->_data = NAN;

                _itemCount++;
                pItem->_pNext->_pNext = 0;
            }

            pItem = pItem->_pNext;
        }

        _pLastMatch = pItem;
        _lastMatchIndex = index;

        return pItem;
    }


    bool SetItem(int index, float value)
    {
        VectorDataItem* pItem = GetItemByIndex(index);

        if (pItem == 0)
        {
            Serial.print("VectorItemAddFail: Environment.VectorItemDataPoolCount exceeded - currently ");
            Serial.println(Environment.VectorItemDataPoolCount);
            return false;
        }

        pItem->_data = value;
        return true;
    }

    float GetItem(int index) const
    {
        VectorDataItem* pItem = ((Vector*)this)->GetItemByIndex(index);  // stupid const correctness. 

        return pItem->_data;
    }

    int GetItemCount()
    {
        return _itemCount;
    }
};

VectorDataItemProvider* Vector::_pVectorDataItemProvider = 0;
int Vector::_pVectorDataItemProviderSize = -1;