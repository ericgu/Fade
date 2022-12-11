class Variable
{
    static const int MaxVariableNameLength = 64;
    static const int MaxStringVariableLength = 128;
    static int SerialNumber;
    static int TargetNumber;

    VariableData *_pVariableData;
    //fsint _serialNumber;

public:
    void AddSerialNumber()
    {
        if (SerialNumber == TargetNumber)
        {
            int k = 12;
        }
        //_serialNumber = SerialNumber++;
        return;
        Serial.print("!!\t");
        //Serial.print(_serialNumber);
        Serial.println("\t>");
    }

    void RemoveSerialNumber()
    {
        return;
        Serial.print("!!\t");
        //Serial.print(_serialNumber);
        Serial.println("\t<");
    }

    void SetClassification(const char* pClassification)
    {
        //_pVariableData->SetClassification(pClassification);
    }

    Variable()
    {
        _pVariableData = VariableStore::VariableStoreInstance.GetFreePoolEntry();
        //_pVariableData->SetVariableName("");
        //_pVariableData->SetStringValue("");
        _pVariableData->SetVariableName("");
        _pVariableData->SetStringValue("");
        _pVariableData->_valueCount = 0;
        SetClassification("C");
        AddSerialNumber();
    }

    Variable(int value) : Variable()
    {
        _pVariableData->SetValue(0, (float)value);
        _pVariableData->_valueCount = 1;
        _pVariableData->_stackLevel = 0;
        SetClassification("I");
    }

    Variable(float value) : Variable()
    {
        _pVariableData->SetValue(0, value);
        _pVariableData->_valueCount = 1;
        _pVariableData->_stackLevel = 0;
        SetClassification("F");
    }

    Variable(const char *pString) : Variable()
    {
        SetValue(pString);
        SetClassification("S");
    }

    Variable(const Variable &other)
    {
        // stack levels above zero mean that it's a program variable. Those variables may go away when we
        // delete stack levels, so we need to split it into a non-program variable.
        if (other._pVariableData->_stackLevel != 0)
        {
          other._pVariableData->IncrementReferenceCount(); // SplitOffEntry will decrement this; we pre-increment so that it doesn't get cleaned up
          _pVariableData = VariableStore::VariableStoreInstance.SplitOffEntry(other._pVariableData);
            _pVariableData->_stackLevel = 0;
            return;
        }

        _pVariableData = other._pVariableData;
        SetClassification("<");
        _pVariableData->IncrementReferenceCount();
        AddSerialNumber();
    }

    Variable &operator=(const Variable &other)
    {
        // stack levels above zero mean that it's a program variable. Those variables may go away when we
        // delete stack levels, so we need to split it into a non-program variable.
        if (_pVariableData->_stackLevel != other._pVariableData->_stackLevel)
        {
            VariableStore::VariableStoreInstance.DecrementReferenceCount(_pVariableData);
            other._pVariableData->IncrementReferenceCount(); // SplitOffEntry will decrement this; we pre-increment so that it doesn't get cleaned up
            _pVariableData = VariableStore::VariableStoreInstance.SplitOffEntry(other._pVariableData);
            _pVariableData->_stackLevel = 0;
            return *this;
        }

        RemoveSerialNumber();

        if (_pVariableData->GetReferenceCount() != 0)
        {
            VariableStore::VariableStoreInstance.DecrementReferenceCount(_pVariableData);
        }
        _pVariableData = other._pVariableData;
        _pVariableData->IncrementReferenceCount();
        SetClassification("=");
        AddSerialNumber();

        return *this;
    }

    ~Variable()
    {
        RemoveSerialNumber();
        if (_pVariableData != 0)
        {
            VariableStore::VariableStoreInstance.DecrementReferenceCount(_pVariableData);
        }
    }

    static Variable ParseFloat(const char *pCommand)
    {
        Variable variableNew;

        variableNew._pVariableData->SetValue(0, (float)atof(pCommand));
        variableNew._pVariableData->_valueCount = 1;
        variableNew._pVariableData->_stackLevel = 0;
        ;

        return variableNew;
    }

    // Getters

    int GetValueInt() { return (int)_pVariableData->GetValue(0); }

    int GetValueCount() { return _pVariableData->_valueCount; }

    float GetValueFloat(int index) 
    { 
        if (index >= _pVariableData->_valueCount)
        {
            return NAN;
        }

        return _pVariableData->GetValue(index); 
    }

    const char *GetValueString()
    {
        return _pVariableData->GetStringValue();
    }

    const char *GetVariableName()
    {
      return _pVariableData->GetVariableName();
    }

    int GetStackLevel() { return _pVariableData->_stackLevel; }

    void Clear()
    {
        _pVariableData->SetValue(0, 0.0);
        _pVariableData->_valueCount = 0;
        _pVariableData->SetVariableName("");
        _pVariableData->SetStringValue("");
        _pVariableData->_stackLevel = 0;
    }

    static Variable Empty()
    {
        Variable variable;
        variable.SetToNan();
        return variable;
    }

    bool IsNan()
    {
      return isnan(_pVariableData->GetValue(0));
    }

    void SplitVariableOnModify()
    {
        if (_pVariableData->GetReferenceCount() > 1)
        {
            _pVariableData = VariableStore::VariableStoreInstance.SplitOffEntry(_pVariableData);
        }
    }

    // Setters: All setters must split if other variables are sharing data with us...

    void Increment(Variable increment)
    {
        SplitVariableOnModify();

        _pVariableData->SetValue(0, _pVariableData->GetValue(0) + increment.GetValueFloat(0));
    }

    void SetValue(int index, float value)
    {
        SplitVariableOnModify();

        if (index + 1 > _pVariableData->_valueCount)
        {
            _pVariableData->_valueCount = index + 1;
        }

        _pVariableData->SetValue(index, value);
    }

    void SetValue(const char *pString)
    {
        SplitVariableOnModify();

        _pVariableData->_valueCount = 0;

        _pVariableData->SetStringValue(pString);
    }

    void SetVariableName(const char *pVariableName)
    {
        SplitVariableOnModify();

        if (pVariableName != NULL && strlen(pVariableName) >= MaxVariableNameLength)
        {
            Serial.println("Variable too long");
            return;
        }

        _pVariableData->SetVariableName(pVariableName);
    }

    void SetStackLevel(int stackLevel)
    {
        SplitVariableOnModify();

        _pVariableData->_stackLevel = stackLevel;
    }

    void SetToNan()
    {
        SplitVariableOnModify();

        _pVariableData->SetValue(0, NAN);
        _pVariableData->_valueCount = 0;
    }

    VariableData *TestOnlyGetVariableData()
    {
        return _pVariableData;
    }
};

int Variable::SerialNumber = 1;
int Variable::TargetNumber = -1;
