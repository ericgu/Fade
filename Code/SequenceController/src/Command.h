class Command
{
    private:
		int _count;
		char _commandString[128];
		int _serialNumber;

    public:

		Command(const Command &source)
		{
			_count = source._count;
			strcpy_s(_commandString, source._commandString);
			_serialNumber = source._serialNumber;
		}

		Command()
		{
			strcpy_s(_commandString, "<Unknown command>");
			_serialNumber = -1;
		}

		Command(int count, const char* pCommand, int CommandLength, int serialNumber)
		{
			_count = count;
			strncpy_s(_commandString, pCommand, CommandLength);
			_serialNumber = serialNumber;
		}

		Command(int count, const char* pCommand, int serialNumber)
		{
			_count = count;
			strcpy_s(_commandString, pCommand);
			_serialNumber = serialNumber;
		}

        char* GetString()
        {
            return _commandString;
        }

		int GetCount()
		{
			return _count;
		}

		int GetSerialNumber()
		{
			return _serialNumber;
		}
};