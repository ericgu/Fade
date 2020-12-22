class VariableData
{
    static const int ValuesPerVariable = 4;

    static InternedStrings _internedStrings;

    const char* _pVariableName;
    const char* _pStringValue;

public:
    VariableData()
    {
      _pVariableName = 0;
      _pStringValue = 0;
    }

    float _value[ValuesPerVariable];
    int _valueCount;
    int _stackLevel;
    int _referenceCount;

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


};

InternedStrings VariableData::_internedStrings;