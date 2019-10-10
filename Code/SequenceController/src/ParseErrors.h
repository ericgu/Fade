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

public:
	ParseErrors()
	{
		_errorCount = 0;
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

		strcpy(_parseErrors[_errorCount]._errorText, pErrorCode);
		strcat(_parseErrors[_errorCount]._errorText, pErrorText);
		_parseErrors[_errorCount]._lineNumber = lineNumber;
		_errorCount++;
	}
	
	int GetErrorCount()
	{
		return _errorCount;
	}
};