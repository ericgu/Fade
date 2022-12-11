class VariableData
{
    static const int ValuesPerVariable = 4;

    static InternedStrings _internedStrings;

    const char* _pVariableName;
    const char* _pStringValue;

    Vector _values;
public:
    VariableData* _pNext;
    //char _classification[16];

    VariableData()
    {
      _pVariableName = 0;
      _pStringValue = 0;
      _valueCount = 0;
      _stackLevel = 0;
      _referenceCount = 0;
      _pNext = 0;
      //_classification[0] = 0;
    }

    int _valueCount;
    int _stackLevel;
    int _referenceCount;

#if classifications
    void SetClassification(char* pClassification)
    {
        if (_classification[1] == '*' && pClassification[0] == '<')
        {
            int k = 13;
        }

        if (strlen(_classification) < sizeof(_classification) - 1)
        {
            strcat(_classification, pClassification);
        }
    }
#endif

    void SetValue(int index, float value)
    {
      _values.SetItem(index, value);
    }

    float GetValue(int index)
    {
      return _values.GetItem(index);
    }

  const char* GetVariableName()
  {
    return _pVariableName;
  }

  void SetVariableName(const char* pVariableName)
  {
    _pVariableName = _internedStrings.LookupOrAdd(pVariableName);
  }

  const char* GetStringValue()
  {
    return _pStringValue;
  }

  void SetStringValue(const char* pStringValue)
  {
    _pStringValue = _internedStrings.LookupOrAdd(pStringValue);
  }

  VariableData& operator= (const VariableData& source)
  {
    _pVariableName = source._pVariableName;
    _pStringValue = source._pStringValue;
    _stackLevel = source._stackLevel;
    _valueCount = source._valueCount;
    //_referenceCount = source._referenceCount;

    for (int itemIndex = 0; itemIndex < source._values._itemCount; itemIndex++)
    {
      float value = source._values.GetItem(itemIndex);
      _values.SetItem(itemIndex, value);
    }

    return *this;
  }

  int GetReferenceCount()
    {
    return _referenceCount;
    }

  void IncrementReferenceCount()
    {
    _referenceCount++;
    }

  void DecrementReferenceCountInternal()
    {
    _referenceCount--;

      if (_referenceCount <= 0)
      {
        _values.Cleanup();
        _valueCount = 0;
        _referenceCount = 0;
        //_classification[0] = 0;
      }
    }


};

InternedStrings VariableData::_internedStrings;