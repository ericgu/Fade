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
		_variableName[0] = '\0';
	}

	Variable(int value)
	{
		_active = true;
		_value = (float) value;
		_variableName[0] = '\0';
	}

	Variable(float value)
	{
		_active = true;
		_value = value;
		_variableName[0] = '\0';
	}

	static Variable ParseFloat(const char* pCommand)
	{
		Variable variable;
		variable._value = (float) atof(pCommand);

		return variable;
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

	void Add(const char* pVariableName)
	{
		for (int i = 0; i < VariableCount; i++)
		{
			if (strcmp(_variables[i].GetVariableName(), pVariableName) == 0)
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
				return;
			}
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

	Variable* Get(const char* pVariableName, ParseErrors* pParseErrors)
	{
		for (int i = 0; i < VariableCount; i++)
		{
			if (strcmp(pVariableName, _variables[i].GetVariableName()) == 0)
			{
				return &_variables[i];
			}
		}

		pParseErrors->AddError("Undeclared variable: ", pVariableName, -1);

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

	Variable* ParseFloatOrVariable(const char* pCommand, ParseErrors* pParseErrors)
	{
		char variableName[64];

		while (*pCommand == ' ')
		{
			pCommand++;
		}

		if ((*pCommand >= '0' && *pCommand <= '9') || *pCommand == '-')
		{
			_constant = Variable::ParseFloat(pCommand);
			return &_constant;
		}
		else
		{
			//Serial.print("Variable or function call: "); Serial.println(pCommand);

			if (strchr(pCommand, '(') != 0)
			{
				//Serial.print("  found function call: "); Serial.println(pCommand);
				if (strncmp(pCommand, "R(", 2) == 0)
				{
					//Serial.println("    found random: ");
					ListParser listParser(":", pCommand + 2);

					Variable minValue = Variable::ParseFloat(listParser.GetItem(0));
					Variable maxValue = Variable::ParseFloat(listParser.GetItem(1));

					_constant = MyRandom::GetValue(minValue.GetValueInt(), maxValue.GetValueInt());

					return &_constant;
				}
			}
			else
			{
				//Serial.print("  Variable name: "); Serial.println(pCommand);
				pCommand = GetVariableName(pCommand, variableName);
			}

			return Get(variableName, pParseErrors);
		}
	}

	void AddAndSet(const char* variableName, float value)
	{
		Add(variableName);
		Get(variableName, 0)->SetValue(value);
	}

};