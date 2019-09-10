class Variable
{
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
};

class VariableCollection
{
	static const int VariableCount = 10;

	Variable _variables[VariableCount];

public:
	Variable& Get(int index)
	{
		if (index < 0 || index > VariableCount)
		{
			return _variables[0];
		}

		return _variables[index];
	}

	Variable ParseFloatOrVariable(const char* pCommand)
	{
		if (*pCommand == '%')
		{
			pCommand++;

			int variableNumber = *pCommand - 'A';

			return _variables[variableNumber];
		}
		else
		{
			return Variable::ParseFloat(pCommand);
		}
	}

};