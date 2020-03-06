#include <math.h>

class Variable
{
	static const int ValuesPerVariable = 4;

	char _variableName[64];
	float _value[ValuesPerVariable];
	int _valueCount;
	int _stackLevel;

public:
	Variable()
	{
		_value[0] = 0.0;
		_valueCount = 0;
		_stackLevel = 0;
		_variableName[0] = '\0';
	}

	Variable(int value)
	{
		_value[0] = (float) value;
		_valueCount = 1;
		_stackLevel = 0;
		_variableName[0] = '\0';
	}

	Variable(float value)
	{
		_value[0] = value;
		_valueCount = 1;
		_stackLevel = 0;
		_variableName[0] = '\0';
	}

	static Variable ParseFloat(const char* pCommand)
	{
		Variable variable;
		variable._value[0] = (float) atof(pCommand);
		variable._valueCount = 1;
		variable._stackLevel = 0;

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

	int GetValueInt() { return (int) _value[0]; }

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

	void SetVariableName(const char* pVariableName) { strcpy(_variableName, pVariableName); }
	char* GetVariableName() { return _variableName;}

	void SetStackLevel(int stackLevel) { _stackLevel = stackLevel; }
	int GetStackLevel() { return _stackLevel; }
};

class VariableCollection
{
	static const int VariableCount = 30;
	static int _serialNumberNext;
	static int _debugVariables;
	
	Variable _undefined;
	Variable _constant;
	Variable _variables[VariableCount];
	int _activeCount;
	int _serialNumber;

public:
	VariableCollection()
	{
		_activeCount = 0;
		_serialNumber = _serialNumberNext++;
		if (_debugVariables) printf("Create variable collection %d\n", _serialNumber);
	}

	int GetActiveVariableCount()
	{
		return _activeCount;
	}

	bool Matches(Variable* pVariable, const char* pVariableName, int stackLevel)
	{
		return strcmp(pVariable->GetVariableName(), pVariableName) == 0 && pVariable->GetStackLevel() == stackLevel;
	}

	void Add(const char* pVariableName, int stackLevel)
	{
		for (int i = 0; i < _activeCount; i++)
		{
			if (Matches(_variables + i, pVariableName, stackLevel))
			{
				return;
			}
		}

		_variables[_activeCount].SetVariableName(pVariableName);
		_variables[_activeCount].SetStackLevel(stackLevel);
		_activeCount++;
		if (_debugVariables) printf("%d Add: %d %s (%d) active=%d\n", _serialNumber, _activeCount - 1, pVariableName, stackLevel, _activeCount);

		return;
	}

	void Clear()
	{
		for (int i = 0; i < _activeCount; i++)
		{
			if (_debugVariables) printf("%d Delc: %d %s\n", _serialNumber, i, _variables[i].GetVariableName());
		}
		_activeCount = 0;
	}

	Variable* Get(int index)
	{
		if (index < 0 || index > _activeCount)
		{
			return 0;
		}

		return &_variables[index];
	}

	Variable* GetWithoutErrorCheck(const char* pVariableName, int stackLevel)
	{
		for (int i = _activeCount - 1; i >= 0; i--)
		{
			if (Matches(_variables + i, pVariableName, stackLevel))
			{
				return _variables + i;
			}
		}

		return 0;
	}

	void Delete(const char* pVariableName, int stackLevel)
	{
		for (int i = 0; i < _activeCount; i++)
		{
			if (Matches(_variables + i, pVariableName, stackLevel))
			{
				if (_debugVariables) printf("%d DelD: %d %s\n", _serialNumber, i, _variables[i].GetVariableName());

				_variables[i].Clear();
				_activeCount--;
				return;
			}
		}
	}

	void DeleteStackLevel(int stackLevel)
	{
		for (int i = _activeCount - 1; i += 0; i--)
		{
			if (_variables[i].GetStackLevel() == stackLevel)
			{
				if (_debugVariables) printf("%d DelS: %d %s\n", _serialNumber, i, _variables[i].GetVariableName());

				_variables[i].Clear();
				_activeCount--;
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

	static const char* GetVariableName(const char *pCommand, char* pVariableName)
	{
		const char *pStart = pCommand;

		while (IsAlphaOrNum(*pCommand))
		{
			pCommand++;
		}

		int length = pCommand - pStart;
		strncpy(pVariableName, pStart, length);
		*(pVariableName + length) = '\0';
		 
		return pCommand;
	}

	Variable* Lookup(const char* pCommand, int stackLevel, ParseErrors* pParseErrors, int lineNumber)
	{
		char variableName[64];

		//Serial.print("  Variable name: "); Serial.println(pCommand);
		pCommand = GetVariableName(pCommand, variableName);

		return Get(variableName, stackLevel, pParseErrors, lineNumber);
	}

	void AddAndSet(const char* variableName, Variable* pVariable, int stackLevel)
	{
		if (_debugVariables) printf("%d AddAndSet: %s %f %d\n", _serialNumber, variableName, pVariable->GetValueFloat(0), stackLevel);

		Add(variableName, stackLevel);
		Variable* pVariableNew = Get(variableName, stackLevel, 0, -1);
		
		for (int i = 0; i < pVariable->GetValueCount(); i++)
		{
			pVariableNew->SetValue(i, pVariable->GetValueFloat(i));
		}
	}
};

int VariableCollection::_serialNumberNext = 0;
int VariableCollection::_debugVariables = 0;
