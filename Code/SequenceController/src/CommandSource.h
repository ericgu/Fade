class ICommandSource
{
    public:
        virtual Command* GetCommand(int commandNumber) = 0;
};

class CommandSource : public ICommandSource
{
	//int _serialNumber;

	Command _command;

	const char *_pCommandString;

public:
	void SetCommand(const char* pCommandString)
	{
		_pCommandString = pCommandString;
	}

	const char* SkipToNextLine(const char* pCurrent)
	{
		while (*pCurrent != '\0')
		{
			if (*pCurrent == '\n' || *pCurrent == '\r')
			{
				pCurrent++;
				if (*pCurrent == '\n' || *pCurrent == '\r')
				{
					pCurrent++;
				}

				return pCurrent;
			}
			pCurrent++;
		}

		return pCurrent;
	}

	Command* GetCommand(int commandNumber)
	{
		const char* pCurrent = _pCommandString;

		if (_pCommandString == 0)
		{
			return 0;
		}

		int currentCommandNumber = commandNumber;
		while (currentCommandNumber != 0)
		{
			pCurrent = SkipToNextLine(pCurrent);
			currentCommandNumber--;
		}

		if (*pCurrent == 0)
		{
			return 0;
		}

		while (*pCurrent == ' ' || *pCurrent == '\t')
		{
			pCurrent++; 
		}

		const char* pEnd = pCurrent;
		while (*pEnd != '\r' && *pEnd != '\n' && *pEnd != '\0')
		{
			pEnd++;
		}

		_command = Command(pCurrent, (int) (pEnd - pCurrent), commandNumber);

		return &_command;
	}
};