class ICommandSource
{
    public:
        virtual Command GetNextCommand(int autoReset = 0) = 0;
		virtual void SetCommandToSerialNumber(int serialNumber) = 0;
};

class CommandSource: public ICommandSource
{
	int _serialNumber;

	ListParser* _pListParser;

    public:
		void SetCommand(const char* pCommandString)
		{
			_serialNumber = 0;
			_pListParser = new ListParser("\n\r", pCommandString);
		}

		Command GetNextCommand(int autoReset = 1)
		{
			const char *pCommandStart = _pListParser->GetItem(_serialNumber);

			while (*pCommandStart == ' ')
			{
				pCommandStart++;
			}

			const char* _pNext = pCommandStart;

			// end of this command is $, null, or newline

			while (*_pNext != '\0' && *_pNext != '\n')
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

			if (_serialNumber == _pListParser->GetCount())
			{
				if (autoReset == 1)
				{
					_serialNumber = 0;
					//Serial.println((char*) "Autoreset");
				}
				else
				{
					return Command(pCommandStart, commandLength, -1);
				}
			}

			//Serial.println(command.GetString());

			return command;
		}

		void SetCommandToSerialNumber(int serialNumber)
		{
			_serialNumber = serialNumber;
		}
};