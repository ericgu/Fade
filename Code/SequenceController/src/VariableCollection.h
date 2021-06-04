

class VariableCollection
{
    static const int VariableMaxCount = 100;
    static int _serialNumberNext;
    static int _debugVariables;

    Variable *_pVariables;
    int _serialNumber;

public:
  void printf(char* s) {}
  void printf(char* s, int v) {}
  void printf(char* s, int v, const char* x) {}
  void printf(char* s, int v, int x, int y) {}
  void printf(char* s, int v, const char* x, int y) {}
  void printf(char* s, int v, int x, const char* y) {}

    VariableCollection()
    {
        //if (int count = VariableStore::VariableStoreInstance.GetInUseCount() != 0)
        //{
        //    int k = 12;
        //}

        _pVariables = new Variable[VariableMaxCount];
        for (int i = 0; i < VariableMaxCount; i++)
        {
            _pVariables[i].SetStackLevel(-1); // mark as empty...
        }

        _serialNumber = _serialNumberNext++;
        if (_debugVariables)
            printf("Create variable collection %d\n", _serialNumber);
    }

    ~VariableCollection()
    {
        delete[] _pVariables;

        //if (int count = VariableStore::VariableStoreInstance.GetInUseCount() != 0)
        //{
        //    int k = 12;
        //}
    }

    int GetActiveVariableCount()
    {
        int count = 0;
        for (int i = 0; i < VariableMaxCount; i++)
        {
            if (_pVariables[i].GetStackLevel() != -1)
            {
                count++;
            }
        }

        return count;
    }

    bool Matches(Variable *pVariable, const char *pVariableName, int stackLevel)
    {
        return pVariable->GetStackLevel() == stackLevel && strcmp(pVariable->GetVariableName(), pVariableName) == 0;
    }

    Variable *Add(const char *pVariableName, int stackLevel)
    {
        int firstFree = -1;
        for (int i = 0; i < VariableMaxCount; i++)
        {
            if (firstFree == -1 && _pVariables[i].GetStackLevel() == -1)
            {
                firstFree = i;
            }

            if (Matches(_pVariables + i, pVariableName, stackLevel))
            {
                return _pVariables + i;
            }
        }

        if (firstFree == -1)
        {
            Serial.println("Too many variables");
            return 0;
        }

        _pVariables[firstFree].Clear();
        _pVariables[firstFree].SetVariableName(pVariableName);
        _pVariables[firstFree].SetStackLevel(stackLevel);
        if (_debugVariables)
            printf("%d Add: %s (%d)\n", _serialNumber, pVariableName, stackLevel);

        return _pVariables + firstFree;
    }

    void Dump()
    {
        Serial.println("+++");
        for (int i = 0; i < VariableMaxCount; i++)
        {
            int stackLevel = _pVariables[i].GetStackLevel();
            if (stackLevel == -1)
            {
                continue;
            }

            Serial.print(stackLevel);
            Serial.print("  ");
            Serial.print(_pVariables[i].GetVariableName());
            Serial.print(" = ");
            for (int j = 0; j < _pVariables[i].GetValueCount(); j++)
            {
                Serial.print(_pVariables[i].GetValueFloat(i));
                Serial.print(" ");
            }
            Serial.println();
        }
        Serial.println("+++");
    }

    void Clear()
    {
        for (int i = 0; i < VariableMaxCount; i++)
        {
            _pVariables[i].SetStackLevel(-1);

            if (_debugVariables)
                printf("%d Delc: %d %s\n", _serialNumber, i, _pVariables[i].GetVariableName());
        }
    }

    Variable *Get(int index)
    {
        if (index < 0 || index > VariableMaxCount)
        {
            return 0;
        }

        return &_pVariables[index];
    }

    Variable *GetWithoutErrorCheck(const char *pVariableName, int stackLevel)
    {
        for (int i = 0; i < VariableMaxCount; i++)
        {
            if (Matches(_pVariables + i, pVariableName, stackLevel))
            {
                return _pVariables + i;
            }
        }

        return 0;
    }

    void Delete(const char *pVariableName, int stackLevel)
    {
        for (int i = 0; i < VariableMaxCount; i++)
        {
            if (Matches(_pVariables + i, pVariableName, stackLevel))
            {
                if (_debugVariables)
                    printf("%d DelD: %d %s\n", _serialNumber, i, _pVariables[i].GetVariableName());

                _pVariables[i].Clear();
                _pVariables[i].SetStackLevel(-1);
                return;
            }
        }
    }

    void DeleteStackLevel(int stackLevel)
    {
        for (int i = 0; i < VariableMaxCount; i++)
        {
            if (_pVariables[i].GetStackLevel() == stackLevel)
            {
                if (_debugVariables)
                    printf("%d DelS: %d %s\n", _serialNumber, i, _pVariables[i].GetVariableName());

                _pVariables[i].Clear();
                _pVariables[i].SetStackLevel(-1);
            }
        }
    }

    Variable *Get(const char *pVariableName, int stackLevel, ParseErrors *pParseErrors, int lineNumber)
    {
        Variable *pResult = GetWithoutErrorCheck(pVariableName, stackLevel);

        if (pResult != 0)
        {
            return pResult;
        }

        pParseErrors->AddError("Undeclared variable: ", pVariableName, lineNumber);

        return 0;
    }

    static bool IsAlphaOrNum(char c)
    {
        if (c >= 'a' && c <= 'z')
            return true;
        if (c >= 'A' && c <= 'Z')
            return true;
        if (c >= '0' && c <= '9')
            return true;

        return false;
    }

    static const char *GetVariableName(const char *pCommand, char *pVariableName, int bufferLength)
    {
        const char *pStart = pCommand;

        while (IsAlphaOrNum(*pCommand))
        {
            pCommand++;
        }

        int length = (int) (pCommand - pStart);

        if (length >= bufferLength)
        {
            Serial.println("Variable name too long");
            *pVariableName = '\0';
        }
        else
        {
            strncpy(pVariableName, pStart, length);
            *(pVariableName + length) = '\0';
        }

        return pCommand;
    }

    Variable *Lookup(const char *pCommand, int stackLevel, ParseErrors *pParseErrors, int lineNumber)
    {
        char variableName[64];

        //Serial.print("  Variable name: "); Serial.println(pCommand);
        pCommand = GetVariableName(pCommand, variableName, sizeof(variableName));

        return Get(variableName, stackLevel, pParseErrors, lineNumber);
    }

    void AddAndSet(const char *variableName, Variable *pVariable, int stackLevel)
    {
        if (_debugVariables)
        {
            printf("%d AddAndSet: %s {", _serialNumber, variableName);
            for (int i = 0; i < pVariable->GetValueCount(); i++)
            {
                printf("%f, ", pVariable->GetValueFloat(i));
            }
            printf("} Stack = %d\n", stackLevel);
        }

        Variable *newVariable = Add(variableName, stackLevel);

        if (pVariable->GetValueCount() != 0)
        {
            for (int i = 0; i < pVariable->GetValueCount(); i++)
            {
                newVariable->SetValue(i, pVariable->GetValueFloat(i));
            }
        }
        else
        {
            newVariable->SetValue(pVariable->GetValueString());
        }
    }
};

int VariableCollection::_serialNumberNext = 0;
int VariableCollection::_debugVariables = 0;