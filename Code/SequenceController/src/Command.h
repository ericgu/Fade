class Command
{
private:
	char _commandString[1024];
	int _lineNumber;

public:
	Command(const Command &source)
	{
		SafeString::StringCopy(_commandString, source._commandString, sizeof(_commandString));
		_lineNumber = source._lineNumber;
	}

	Command()
	{
		SafeString::StringCopy(_commandString, "<Unknown command>", sizeof(_commandString));
		_lineNumber = -1;
	}

	Command(const char *pCommand, int lineNumber)
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
		_lineNumber = lineNumber;
	}

	Command(const char *pCommand, int length, int lineNumber)
	{
		if (length >= sizeof(_commandString))
		{
			Serial.println(pCommand);
			Serial.println("Command string too long");
			return;
		}

		if (pCommand)
		{
			SafeString::StringCopyCount(_commandString, pCommand, sizeof(_commandString), length);
		}
		else
		{
			_commandString[0] = '\0';
		}
		_lineNumber = lineNumber;
	}

	char *GetString()
	{
		return _commandString;
	}

	int GetLineNumber()
	{
		return _lineNumber;
	}

	void SetLineNumber(int lineNumber)
	{
		_lineNumber = lineNumber;
	}

	int StartsWith(const char *pPrefix)
	{
		return (strncmp(_commandString, pPrefix, strlen(pPrefix)) == 0);
	}
};