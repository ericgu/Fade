#include <math.h>

class Variable
{
	char _variableName[64];
	int _active;
	float _value;
	int _stackLevel;

public:
	Variable()
	{
		_active = 0;
		_value = 0.0;
		_stackLevel = 0;
		_variableName[0] = '\0';
	}

	Variable(int value)
	{
		_active = true;
		_value = (float) value;
		_stackLevel = 0;
		_variableName[0] = '\0';
	}

	Variable(float value)
	{
		_active = true;
		_value = value;
		_stackLevel = 0;
		_variableName[0] = '\0';
	}

	static Variable ParseFloat(const char* pCommand)
	{
		Variable variable;
		variable._value = (float) atof(pCommand);
		variable._stackLevel = 0;

		return variable;
	}

	void SetToNan()
	{
		_value = NAN;
	}

	bool IsNan()
	{
		return isnan(_value);
	}

	void Increment(Variable increment)
	{
		_value += increment.GetValueFloat();
        //Serial.println(_value);
	}

	void SetActiveFlag(int active)
	{
		_active = active;
	}

	void Clear()
	{
		_active = false;
		_value = 0.0;
		_variableName[0] = '\0';
	}

	int GetActiveFlag() { return _active; }

	int GetValueInt() { return (int) _value; }

	float GetValueFloat() { return _value; }
	void SetValue(float value) { _value = value; }

	void SetVariableName(const char* pVariableName) { strcpy(_variableName, pVariableName); }
	char* GetVariableName() { return _variableName;}

	void SetStackLevel(int stackLevel) { _stackLevel = stackLevel; }
	int GetStackLevel() { return _stackLevel; }
};

class VariableCollection
{
	static const int VariableCount = 30;
	
	Variable _undefined;
	Variable _constant;
	Variable _variables[VariableCount];

public:
	int GetActiveVariableCount()
	{
		int count = 0;

		for (int i = 0; i < VariableCount; i++)
		{
			if (_variables[i].GetActiveFlag() == 1)
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
		for (int i = 0; i < VariableCount; i++)
		{
			if (Matches(_variables + i, pVariableName, stackLevel))
			{
				return;
			}
		}

		for (int i = 0; i < VariableCount; i++)
		{
			if (_variables[i].GetActiveFlag() == 0)
			{
				_variables[i].SetVariableName(pVariableName);
				_variables[i].SetActiveFlag(1);
				_variables[i].SetStackLevel(stackLevel);
				return;
			}
		}
	}

	void Clear()
	{
		for (int i = 0; i < VariableCount; i++)
		{
			_variables[i].SetActiveFlag(0);
		}
	}

	Variable* Get(int index)
	{
		if (index < 0 || index > VariableCount)
		{
			return &_undefined;
		}

		return &_variables[index];
	}

	Variable* GetWithoutErrorCheck(const char* pVariableName, int stackLevel)
	{
		for (int i = VariableCount - 1; i >= 0; i--)
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
		for (int i = 0; i < VariableCount; i++)
		{
			if (Matches(_variables + i, pVariableName, stackLevel))
			{
				_variables[i].Clear();
			}
		}
	}

	void DeleteStackLevel(int stackLevel)
	{
		for (int i = 0; i < VariableCount; i++)
		{
			if (_variables[i].GetStackLevel() == stackLevel)
			{
				_variables[i].Clear();
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

		return &_undefined;
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

	void AddAndSet(const char* variableName, float value, int stackLevel)
	{
		Add(variableName, stackLevel);
		Get(variableName, stackLevel, 0, -1)->SetValue(value);
	}
};