class ListParser
{
	char* _pBuffer;
	char* _pItems[64];
	int _itemCount;

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
	ListParser(const char* pDelimiters, const char* pString)
	{
		_itemCount = 0;

		int length = strlen(pString) + 1;
		_pBuffer = new char[length];

		//strcpy_s(_pBuffer, length, pString);
		strcpy(_pBuffer, pString);

		char* pCurrent = _pBuffer;

		bool inDelimiter = true;

		while (*pCurrent != '\0')
		{
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

	~ListParser()
	{
		delete _pBuffer;
	}

	int GetCount() { return _itemCount; }

	char* GetItem(int i) { return _pItems[i]; }
};