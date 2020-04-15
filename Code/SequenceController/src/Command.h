class Command
{
    private:
		char _commandString[128];
		int _serialNumber;

    public:

		Command(const Command &source)
		{
			SafeString::StringCopy(_commandString, source._commandString, sizeof(_commandString));
			_serialNumber = source._serialNumber;
		}

		Command()
		{
			SafeString::StringCopy(_commandString, "<Unknown command>", sizeof(_commandString));
			_serialNumber = -1;
		}

		Command(const char* pCommand, int serialNumber)
		{
			if (strlen(pCommand) >= sizeof(_commandString))
			{
				Serial.println(pCommand);
				Serial.println("Command string too long");
				return;
			}

			if (pCommand)
			{
				SafeString::StringCopy(_commandString, pCommand, sizeof(_commandString));
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