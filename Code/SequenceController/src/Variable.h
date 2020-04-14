
#include <math.h>

class Variable
{
	static const int ValuesPerVariable = 4;
	static const int MaxVariableNameLength = 64;
	static const int MaxStringVariableLength = 128;

	char _variableName[MaxVariableNameLength];
	char _stringValue[MaxStringVariableLength];
	float _value[ValuesPerVariable];
	int _valueCount;
	int _stackLevel;

public:
	Variable()
	{
		for (int i = 0; i < ValuesPerVariable; i++)
		{
			_value[i] = 0.0;
		}
		_valueCount = 0;
		_stackLevel = 0;
		_variableName[0] = '\0';
		_stringValue[0] = '\0';
	}

	Variable(const char* pString)
	{
		SetValue(pString);
	}

	Variable(int value)
	{
		_value[0] = (float)value;
		_valueCount = 1;
		_stackLevel = 0;
		_variableName[0] = '\0';
		_stringValue[0] = '\0';
	}

	Variable(float value)
	{
		_value[0] = value;
		_valueCount = 1;
		_stackLevel = 0;
		_variableName[0] = '\0';
		_stringValue[0] = '\0';
	}

	static Variable ParseFloat(const char* pCommand)
	{
		Variable variable;
		variable._value[0] = (float)atof(pCommand);
		variable._valueCount = 1;
		variable._stackLevel = 0;

		return variable;
	}

	static Variable Empty()
	{
		Variable variable;
		variable.SetToNan();
		return variable;
	}

	void SetToNan()
	{
		_value[0] = NAN;
	}

	bool IsNan()
	{
		return isnan(_value[0]);
	}

	void Increment(Variable increment)
	{
		_value[0] += increment.GetValueFloat(0);
		//Serial.println(_value);
	}

	void Clear()
	{
		_value[0] = 0.0;
		_valueCount = 0;
		_variableName[0] = '\0';
		_stackLevel = 0;
	}

	int GetValueCount() { return _valueCount; }

	int GetValueInt() { return (int)_value[0]; }

	void SetValue(float value)
	{
		_value[0] = value;
		_valueCount = 1;
	}

	float GetValueFloat(int index) { return _value[index]; }
	void SetValue(int index, float value)
	{
		if (_valueCount >= ValuesPerVariable)
		{
			return;
		}

		if (index + 1 > _valueCount)
		{
			_valueCount = index + 1;
		}

		_value[index] = value;
	}

	void SetValue(const char* pString)
	{
		_valueCount = 0;

		int copyCount = strlen(pString) < MaxStringVariableLength ? strlen(pString) : MaxStringVariableLength - 1;

		strncpy(_stringValue, pString, copyCount);
		_stringValue[copyCount] = '\0';
	}

	const char* GetValueString()
	{
		return _stringValue;
	}

	void SetVariableName(const char* pVariableName)
	{
		if (strlen(pVariableName) >= MaxVariableNameLength)
		{
			Serial.println("Variable too long");
			return;
		}

		strcpy(_variableName, pVariableName);
	}


	char* GetVariableName() { return _variableName;}

	void SetStackLevel(int stackLevel) { _stackLevel = stackLevel; }
	int GetStackLevel() { return _stackLevel; }
};

class VariableCollection
{
	static const int VariableMaxCount = 30;
	static int _serialNumberNext;
	static int _debugVariables;
	
	Variable _undefined;
	Variable _constant;
	Variable* _pVariables;
	int _serialNumber;

public:
	VariableCollection()
	{
		_pVariables = new Variable[VariableMaxCount];
		for (int i = 0; i < VariableMaxCount; i++)
		{
			_pVariables[i].SetStackLevel(-1);	// mark as empty...
		}

		_serialNumber = _serialNumberNext++;
		if (_debugVariables) printf("Create variable collection %d\n", _serialNumber);
	}

	~VariableCollection()
	{
		delete _pVariables;
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

	bool Matches(Variable* pVariable, const char* pVariableName, int stackLevel)
	{
		return strcmp(pVariable->GetVariableName(), pVariableName) == 0 && pVariable->GetStackLevel() == stackLevel;
	}

	void Add(const char* pVariableName, int stackLevel)
	{
		int firstFree = -1;
		for (int i = 0; i < VariableMaxCount; i++)
		{
			if (_pVariables[i].GetStackLevel() == -1 && firstFree == -1)
			{
				firstFree = i;
			}

			if (Matches(_pVariables + i, pVariableName, stackLevel))
			{
				return;
			}
		}

		if (firstFree == -1)
		{
			Serial.println("Too many variables");
			return;
		}

		_pVariables[firstFree].SetVariableName(pVariableName);
		_pVariables[firstFree].SetStackLevel(stackLevel);
		if (_debugVariables) printf("%d Add: %s (%d)\n", _serialNumber, pVariableName, stackLevel);

		return;
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

			if (_debugVariables) printf("%d Delc: %d %s\n", _serialNumber, i, _pVariables[i].GetVariableName());
		}
	}

	Variable* Get(int index)
	{
		if (index < 0 || index > VariableMaxCount)
		{
			return 0;
		}

		return &_pVariables[index];
	}

	Variable* GetWithoutErrorCheck(const char* pVariableName, int stackLevel)
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

	void Delete(const char* pVariableName, int stackLevel)
	{
		for (int i = 0; i < VariableMaxCount; i++)
		{
			if (Matches(_pVariables + i, pVariableName, stackLevel))
			{
				if (_debugVariables) printf("%d DelD: %d %s\n", _serialNumber, i, _pVariables[i].GetVariableName());

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
				if (_debugVariables) printf("%d DelS: %d %s\n", _serialNumber, i, _pVariables[i].GetVariableName());

				_pVariables[i].Clear();
				_pVariables[i].SetStackLevel(-1);
			}
		}
	}

	Variable* Get(const char* pVariableName, int stackLevel, ParseErrors* pParseErrors, int lineNumber)
	{
		Variable* pResult = GetWithoutErrorCheck(pVariableName, stackLevel);

		if (pResult != 0)
		{
			return pResult;
		}

		pParseErrors->AddError("Undeclared variable: ", pVariableName, lineNumber);

		return 0;
	}

	static bool IsAlphaOrNum(char c)
	{
		if (c >= 'a' && c <= 'z') return true;
		if (c >= 'A' && c <= 'Z') return true;
		if (c >= '0' && c <= '9') return true;

		return false;
	}

	static const char* GetVariableName(const char *pCommand, char* pVariableName, int bufferLength)
	{
		const char *pStart = pCommand;

		while (IsAlphaOrNum(*pCommand))
		{
			pCommand++;
		}

		int length = pCommand - pStart;

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

	Variable* Lookup(const char* pCommand, int stackLevel, ParseErrors* pParseErrors, int lineNumber)
	{
		char variableName[64];

		//Serial.print("  Variable name: "); Serial.println(pCommand);
		pCommand = GetVariableName(pCommand, variableName, sizeof(variableName));

		return Get(variableName, stackLevel, pParseErrors, lineNumber);
	}

	void AddAndSet(const char* variableName, Variable* pVariable, int stackLevel)
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

		Add(variableName, stackLevel);
		Variable* pVariableNew = Get(variableName, stackLevel, 0, -1);

		if (pVariable->GetValueCount() != 0)
		{
			for (int i = 0; i < pVariable->GetValueCount(); i++)
			{
				pVariableNew->SetValue(i, pVariable->GetValueFloat(i));
			}
		}
		else
		{
			pVariableNew->SetValue(pVariable->GetValueString());
		}
	}
};

int VariableCollection::_serialNumberNext = 0;
int VariableCollection::_debugVariables = 0;
