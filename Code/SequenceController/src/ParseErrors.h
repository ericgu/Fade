#define MaxErrorCount 16

class ParseError
{
public:
	ParseError()
	{
		_errorText[0] = '\0';
		_lineNumber = -1;
	}

	char _errorText[128];
	int _lineNumber;
};

class ParseErrors
{
	ParseError _parseErrors[MaxErrorCount];
	int _errorCount;
	char _buffer[4096];

public:
	ParseErrors()
	{
		_errorCount = 0;
		_buffer[0] = '\0';
	}

	ParseError GetError(int index)
	{
		if (index < 0 || index >= _errorCount)
		{
			return ParseError();
		}

		return _parseErrors[index];
	}

	void AddError(const char* pErrorCode, const char* pErrorText, int lineNumber)
	{
		if (_errorCount == MaxErrorCount)
		{
			return;
		}

		for (int errorNumber = 0; errorNumber < _errorCount; errorNumber++)
		{
			if (_parseErrors[errorNumber]._lineNumber == lineNumber)
			{
				return;
			}
		}

        //Serial.print("Error: "); Serial.print(pErrorCode); Serial.print(pErrorText); Serial.print(" => "); Serial.println(lineNumber);

		strcpy(_parseErrors[_errorCount]._errorText, pErrorCode);
		strcat(_parseErrors[_errorCount]._errorText, pErrorText);
		_parseErrors[_errorCount]._lineNumber = lineNumber;
		_errorCount++;
	}
	
	void Clear()
	{
		_errorCount = 0;
	}

	int GetErrorCount()
	{
		return _errorCount;
	}

	char* FormatErrors()
	{
		*_buffer = '\0';
		// find max line number...
		int maxLineNumber = -1;

		for (int i = 0; i < _errorCount; i++)
		{
			if (_parseErrors[i]._lineNumber > maxLineNumber)
			{
				maxLineNumber = _parseErrors[i]._lineNumber;
			}
		}

		for (int lineNumber = 0; lineNumber <= maxLineNumber; lineNumber++)
		{
			for (int i = 0; i < _errorCount; i++)
			{
				if (_parseErrors[i]._lineNumber == lineNumber)
				{
					strcat(_buffer, _parseErrors[i]._errorText);
				}
			}
			strcat(_buffer, "\n");
		}

		for (int i = 0; i < _errorCount; i++)
		{
			if (_parseErrors[i]._lineNumber == -1)
			{
				strcat(_buffer, _parseErrors[i]._errorText);
			}
		}

		return _buffer;
	}
};