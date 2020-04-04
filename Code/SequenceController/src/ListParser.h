class ListParser
{
	char* _pBuffer;
	int _bufferSize;
	char** _pItems;
	int _itemCount;
	int _itemCountAllocated;

	static const char* SkipToCharOrNull(const char* pCommand, char c)
	{
		while (*pCommand != c && *pCommand != '\0')
		{
			pCommand++;
		}

		return pCommand;
	}

	bool IsDelimiter(const char* pDelimiters, const char* pCurrent)
	{
		bool delimiterCharacter = false;
		const char* pDelimiter = pDelimiters;
		while (*pDelimiter != '\0')
		{
			if (*pCurrent == *pDelimiter)
			{
				delimiterCharacter = true;
			}
			pDelimiter++;
		}

		return delimiterCharacter;
	}

public:
	ListParser(int stringSize, int itemCountAllocated)
	{
		_bufferSize = stringSize;
		_pBuffer = new char[stringSize];
		_pItems = new char*[itemCountAllocated];
		_itemCountAllocated = itemCountAllocated;
	}

	~ListParser()
	{
		delete _pBuffer;
		delete _pItems;
	}

	void Parse(const char* pDelimiters, const char* pString)
	{
		_itemCount = 0;

		if (strlen(pString) > _bufferSize)
		{
			Serial.println("too big"); Serial.flush();
		}

		//strcpy_s(_pBuffer, length, pString);
		strcpy(_pBuffer, pString);

		char* pCurrent = _pBuffer;

		bool inDelimiter = true;

		while (*pCurrent != '\0')
		{
			if (_itemCount == _itemCountAllocated)
			{
				Serial.print("List to parse had too many items...");
				return;
			}

			bool isDelimiter = IsDelimiter(pDelimiters, pCurrent);

			if (inDelimiter)
			{
				if (!isDelimiter)
				{
					_pItems[_itemCount] = pCurrent;
					_itemCount++;
					inDelimiter = false;
				}
			}
			else
			{
				if (isDelimiter)
				{
					inDelimiter = true;
				}
			}

			if (isDelimiter)
			{
				*pCurrent = '\0';
			}

			pCurrent++;
		}
	}

	int GetCount() { return _itemCount; }

	char* GetItem(int i) { return _pItems[i]; }
};