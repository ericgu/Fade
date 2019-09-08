class Loop
{
	int _variableNumber;
	int _variableMin;
	int _variableMax;
	int _match;
	int _errorOffset;

    public:
		Loop()
		{
			_variableNumber = 0;
			_variableMin = 0;
			_variableMax = 0;
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

		loop._variableMin = atoi(pCommand);

		while (*pCommand != ':' && *pCommand != '\0')
		{
			pCommand++;
		}

		if (*pCommand == '\0')
		{
			loop._errorOffset = pCommand - pCommandStart;
			return loop;
		}

		pCommand++;

		loop._variableMax = atoi(pCommand);

		loop._match = 1;

		return loop;
    }

    int GetVariableNumber()
    {
		return _variableNumber;
    }

    int GetVariableMin()
    {
		return _variableMin;
    }

	int GetVariableMax()
	{
		return _variableMax;
	}

	int GetMatch()
	{
		return _match;
	}

	int GetErrorOffset()
	{
		return _errorOffset;
	}
};