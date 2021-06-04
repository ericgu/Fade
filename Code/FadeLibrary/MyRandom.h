class MyRandom
{
	static int _firstValue;
	static int _value;
public:
	static void SetFirstValue(int value)
	{
		_firstValue = value;
		_value = value;
	}

	static int GetValue(int min, int max)
	{
		if (_value >= max)
		{
			_value = _firstValue;
		}

		int returnValue = _value;

		_value += 2;

		return returnValue;
	}
};

int MyRandom::_value = 1;
int MyRandom::_firstValue = 1;
