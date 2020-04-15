class ExpressionNode
{
public:
	const char* _pItem;
	int	_itemLength;
	Variable _value;

	ExpressionNode()
	{
		_value.SetToNan();
	}

	bool StartsWith(char character)
	{
		return _pItem != 0 && *_pItem == character;
	}

	bool EqualTo(const char* pString)
	{
		return this != 0 && _itemLength == strlen(pString) && strncmp(pString, _pItem, _itemLength) == 0;
	}

	bool IsEmpty() { return _itemLength == -1; }

	bool IsNumber() { return *_pItem >= '0' && *_pItem <= '9';  }

	bool IsIdentifier() { return (*_pItem >= 'a' && *_pItem <= 'z') || (*_pItem >= 'A' && *_pItem <= 'Z'); }

	bool Is(const char *pTest) 
	{ 
		if (_pItem == 0)
		{
			return false;
		}

		return strncmp(_pItem, pTest, _itemLength) == 0; 
	}
};