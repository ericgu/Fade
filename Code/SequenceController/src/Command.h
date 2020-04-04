class Command
{
    private:
		char _commandString[128];
		int _serialNumber;

    public:

		Command(const Command &source)
		{
			strcpy_s(_commandString, source._commandString);
			_serialNumber = source._serialNumber;
		}

		Command()
		{
			strcpy_s(_commandString, "<Unknown command>");
			_serialNumber = -1;
		}

		Command(const char* pCommand, int commandLength, int serialNumber)
		{
			if (commandLength >= sizeof(_commandString))
			{
				Serial.println("Command string too long");
				return;
			}

			strncpy_s(_commandString, pCommand, commandLength);
			*(_commandString + commandLength) = '\0';
			_serialNumber = serialNumber;
		}

		Command(const char* pCommand, int serialNumber)
		{
			if (strlen(pCommand) >= sizeof(_commandString))
			{
				Serial.println("Command string too long");
				return;
			}

			if (pCommand)
			{
				strcpy_s(_commandString, pCommand);
			}
			else
			{
				_commandString[0] = '\0';
			}
			_serialNumber = serialNumber;
		}

        char* GetString()
        {
            return _commandString;
        }

		int GetSerialNumber()
		{
			return _serialNumber;
		}

		void SetSerialNumber(int serialNumber)
		{
			_serialNumber = serialNumber;
		}

		int StartsWith(const char* pPrefix)
		{
			return (strncmp(_commandString, pPrefix, strlen(pPrefix)) == 0);
		}
};