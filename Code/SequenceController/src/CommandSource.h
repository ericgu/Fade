class ICommandSource
{
    public:
        virtual Command GetNextCommand() = 0;
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

		Command GetNextCommand()
        {
			const char* _pNext = _pCurrent;

			// Points to '$' at the start of a command...
			_pNext++;
			const char *pCountStart = _pNext;

			while (*_pNext != '$')
			{
				_pNext++;
			}

			int countLength = _pNext - pCountStart;

			// points to second '$' in command
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

			Command command = Command(pCountStart, countLength, pCommandStart, commandLength, _serialNumber);
			_serialNumber++;

			// reset if we have no more commands...
			_pCurrent = _pNext;
			if (*_pCurrent == '\0')
			{
				_pCurrent = _pCommandString;
				_serialNumber = 0;
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