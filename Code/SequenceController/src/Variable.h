class Variable
{
	int _active;
	int _valueInt;
	float _valueFloat;

public:
	Variable()
	{
		_active = 0;
		_valueInt = 0;
		_valueFloat = 0.0;
	}

	Variable(int value)
	{
		_active = true;
		_valueInt = value;
		_valueFloat = (float) value;
	}

	Variable(float value)
	{
		_active = true;
		_valueInt = -1;
		_valueFloat = value;
	}

	static Variable ParseInt(const char* pCommand)
	{
		Variable variable; 

		variable._valueInt = atoi(pCommand);
		variable._valueFloat = (float) variable._valueInt;

		return variable;
	}

	static Variable ParseFloat(const char* pCommand)
	{
		Variable variable;
		variable._valueFloat = (float) atof(pCommand);

		return variable;
	}

	void Increment()
	{
		_valueInt++;
		_valueFloat++;
	}

	void SetActiveFlag(int active)
	{
		_active = active;
	}

	int GetActiveFlag() { return _active; }

	int GetValueInt() { return _valueInt; }
	void SetValueInt(int value) { _valueInt = value; }

	float GetValueFloat() { return _valueFloat; }
	void SetValueFloat(float value) { _valueFloat = value; }
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

};