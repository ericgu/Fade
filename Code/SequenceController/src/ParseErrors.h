class ParseError
{
	static const int ErrorTextLength = 128;

public:
	ParseError()
	{
		_errorText[0] = '\0';
		_lineNumber = -1;
	}

	void SetError(const char* pErrorCode, const char* pErrorText, int lineNumber)
	{
		int remainingSpace = ErrorTextLength - 1;

		char* pDest = _errorText;
		const char* pSource = pErrorCode;
		
		while (remainingSpace != 0 && *pSource != 0)
		{
			*pDest = *pSource;
			pDest++;
			pSource++;
			remainingSpace--;
		}

		pSource = pErrorText;
		while (remainingSpace != 0 && *pSource != 0)
		{
			*pDest = *pSource;
			pDest++;
			pSource++;
			remainingSpace--;
		}

		*pDest = 0;

		_lineNumber = lineNumber;
	}

	char _errorText[ErrorTextLength];
	int _lineNumber;
};

class ParseErrors
{
	static const int MaxErrorCount = 16;
	static const int TextErrorBufferSize = 4096;

	ParseError _parseErrors[MaxErrorCount];
	int _errorCount;
	char _buffer[TextErrorBufferSize];
	int _bufferUsed;

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

		_parseErrors[_errorCount].SetError(pErrorCode, pErrorText, lineNumber);
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

	void AddToBuffer(const char* pString)
	{
		int newLength = strlen(pString);

		if (_bufferUsed + newLength > TextErrorBufferSize - 1)
		{
			return;
		}

		strcat(_buffer, pString);
		_bufferUsed += newLength;
	}

	char* FormatErrors()
	{
		*_buffer = '\0';
		_bufferUsed = 0;

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
					AddToBuffer(_parseErrors[i]._errorText);
				}
			}
			AddToBuffer("\n");
		}

		for (int i = 0; i < _errorCount; i++)
		{
			if (_parseErrors[i]._lineNumber == -1)
			{
				AddToBuffer(_parseErrors[i]._errorText);
			}
		}

		return _buffer;
	}
};