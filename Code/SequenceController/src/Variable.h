class Variable
{
	char _variableName[64];
	int _active;
	float _value;

public:
	Variable()
	{
		_active = 0;
		_value = 0.0;
	}

	Variable(int value)
	{
		_active = true;
		_value = (float) value;
	}

	Variable(float value)
	{
		_active = true;
		_value = value;
	}

	static Variable ParseFloat(const char* pCommand)
	{
		Variable variable;
		variable._value = (float) atof(pCommand);

		return variable;
	}

	void Increment(float increment)
	{
		_value += increment;
        //Serial.println(_value);
	}

	void SetActiveFlag(int active)
	{
		_active = active;
	}

	int GetActiveFlag() { return _active; }

	int GetValueInt() { return (int) _value; }

	float GetValueFloat() { return _value; }
	void SetValue(float value) { _value = value; }

	void SetVariableName(const char* pVariableName) { strcpy(_variableName, pVariableName); }
	char* GetVariableName() { return _variableName;}
};

class VariableCollection
{
	static const int VariableCount = 10;

	Variable _variables[VariableCount];

public:
	void Add(const char* variableName)
	{
		for (int i = 0; i < VariableCount; i++)
		{
			if (_variables[i].GetActiveFlag() == 0)
			{
				_variables[i].SetVariableName(variableName);
				_variables[i].SetActiveFlag(1);
				return;
			}
		}
	}

	Variable& Get(int index)
	{
		if (index < 0 || index > VariableCount)
		{
			return _variables[0];
		}

		return _variables[index];
	}

	Variable& Get(const char* pVariableName)
	{
		for (int i = 0; i < VariableCount; i++)
		{
			if (strcmp(pVariableName, _variables[i].GetVariableName()) == 0)
			{
				return _variables[i];
			}
		}

		return _variables[0];
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

		return pCommand + 1;
	}

	Variable ParseFloatOrVariable(const char* pCommand)
	{
		char variableName[64];

		if (*pCommand >= '0' && *pCommand <= '9')
		{
			return Variable::ParseFloat(pCommand);
		}
		else
		{
			pCommand = GetVariableName(pCommand, variableName);

			return Get(variableName);
		}
	}

	void AddAndSet(const char* variableName, float value)
	{
		Add(variableName);
		Get(variableName).SetValue(value);
	}

};