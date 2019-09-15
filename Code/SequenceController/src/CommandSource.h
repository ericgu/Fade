class ICommandSource
{
    public:
        virtual Command GetNextCommand(int autoReset = 0) = 0;
		virtual void SetCommandToSerialNumber(int serialNumber) = 0;
};

class CommandSource: public ICommandSource
{
    const char* _pCommandString;
	const char* _pCurrent;
	int _serialNumber;

    public:
		void SetCommand(const char* pCommandString)
		{
			_pCommandString = pCommandString;
			_pCurrent = _pCommandString;
			_serialNumber = 0;
		}

		Command GetNextCommand(int autoReset = 1)
		{
			const char* _pNext = _pCurrent;

			// Points to '$' at the start of a command...
			_pNext++;
			const char *pCommandStart = _pNext;

			// end of this command is $, null, or newline

			while (*_pNext != '$' && *_pNext != '\0' && *_pNext != '\n')
			{
				_pNext++;
			}

			int commandLength = _pNext - pCommandStart;

			if (*_pNext == '\n')
			{
				_pNext++;
			}

			Command command = Command(pCommandStart, commandLength, _serialNumber);
			_serialNumber++;

			// reset if we have no more commands...
			_pCurrent = _pNext;
			if (*_pCurrent == '\0')
			{
				if (autoReset == 1)
				{
					_pCurrent = _pCommandString;
					_serialNumber = 0;
				}
				else
				{
					return Command(pCommandStart, commandLength, -1);
				}
			}

			return command;
		}

		void SetCommandToSerialNumber(int serialNumber)
		{
			_pCurrent = _pCommandString;
			_serialNumber = 0;

			while (_serialNumber != serialNumber)
			{
				GetNextCommand();
			}
		}

};