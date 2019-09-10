class Command
{
    private:
		char _countString[32];
		char _commandString[128];
		int _serialNumber;

    public:

		Command(const Command &source)
		{
			strcpy_s(_countString, source._countString);
			strcpy_s(_commandString, source._commandString);
			_serialNumber = source._serialNumber;
		}

		Command()
		{
			strcpy_s(_commandString, "<Unknown command>");
			_serialNumber = -1;
		}

		Command(const char* pCountString, int countLength, const char* pCommand, int commandLength, int serialNumber)
		{
			strncpy_s(_countString, pCountString, countLength);
			*(_countString + countLength) = '\0';
			strncpy_s(_commandString, pCommand, commandLength);
			*(_commandString + commandLength) = '\0';
			_serialNumber = serialNumber;
		}

		Command(const char* pCountString, const char* pCommand, int serialNumber)
		{
			strcpy_s(_countString, pCountString);
			strcpy_s(_commandString, pCommand);
			_serialNumber = serialNumber;
		}

        char* GetString()
        {
            return _commandString;
        }

		char* GetCount()
		{
			return _countString;
		}

		int GetSerialNumber()
		{
			return _serialNumber;
		}

		int StartsWith(const char* pPrefix)
		{
			return (strncmp(_commandString, pPrefix, strlen(pPrefix)) == 0);
		}
};