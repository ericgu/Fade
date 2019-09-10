class ListParser
{
	char _buffer[128];
	char* _pItems[32];
	int _itemCount;

	static const char* SkipToCharOrNull(const char* pCommand, char c)
	{
		while (*pCommand != c && *pCommand != '\0')
		{
			pCommand++;
		}

		return pCommand;
	}

public:
	ListParser(char delimiter, const char* pString)
	{
		strcpy_s(_buffer, pString);

		char* pCurrent = _buffer;
		if (*pCurrent == '\0')
		{
			_itemCount = 0;
			return;
		}

		_pItems[0] = pCurrent;
		_itemCount = 1;

		while (*pCurrent != '\0')
		{
			if (*pCurrent == delimiter)
			{
				*pCurrent = '\0';
				pCurrent++;
				_pItems[_itemCount] = pCurrent;
				_itemCount++;
			}
			else
			{
				pCurrent++;
			}
		}
	}

	int GetCount() { return _itemCount; }

	char* GetItem(int i) { return _pItems[i]; }
};