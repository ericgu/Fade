class Loop
{
	int _variableNumber;
	float _variableStart;
	float _variableEnd;
	float _variableInc;
	int _match;
	int _errorOffset;

	static const char* FindColonOrEnd(const char *pCommand)
	{
		while (*pCommand != ':' && *pCommand != '\0')
		{
			pCommand++;
		}
		return pCommand;
	}

    public:
		Loop()
		{
			_variableNumber = 0;
			_variableStart = 0;
			_variableEnd = 0;
			_variableInc = 1;
			_match = 0;
			_errorOffset = 0;
		}

    static Loop Parse(const char* pCommand)
    {
		const char *pCommandStart = pCommand;

		Loop loop;

		if (strncmp(pCommand, "LOOP", 4) != 0)
		{
			loop._errorOffset = pCommand - pCommandStart;
			return loop;
		}

		pCommand += 5;	// skip 'LOOP '

		if (*pCommand != '%')
		{
			loop._errorOffset = pCommand - pCommandStart;
			return loop;
		}
		pCommand++;

		char varName = *pCommand;
		loop._variableNumber = varName - 'A';
		if (loop._variableNumber < 0 || loop._variableNumber > 9)
		{
			loop._errorOffset = pCommand - pCommandStart;
			return loop;
		}

		pCommand += 2; // skip to number...

		loop._variableStart = (float) atof(pCommand);


		pCommand = FindColonOrEnd(pCommand);

		if (*pCommand == '\0')
		{
			loop._errorOffset = pCommand - pCommandStart;
			return loop;
		}

		pCommand++;

		loop._variableEnd = (float) atof(pCommand);

		pCommand = FindColonOrEnd(pCommand);
		if (*pCommand != '\0')	// has optional increment
		{
			pCommand++;
			loop._variableInc = (float)atof(pCommand);
		}

		loop._match = 1;

		return loop;
    }

    int GetVariableNumber()
    {
		return _variableNumber;
    }

	float GetVariableStart()
    {
		return _variableStart;
    }

	float GetVariableEnd()
	{
		return _variableEnd;
	}

	float GetVariableInc()
	{
		return _variableInc;
	}

	int GetMatch()
	{
		return _match;
	}

	int GetErrorOffset()
	{
		return _errorOffset;
	}

	int GetIsInRange(float value)
	{
		float min = _variableStart < _variableEnd ? _variableStart : _variableEnd;
		float max = _variableStart < _variableEnd ? _variableEnd : _variableStart;

		return value >= min && value <= max;
	}
};