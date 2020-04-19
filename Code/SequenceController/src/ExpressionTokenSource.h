class ExpressionTokenSource
{
	ExpressionNode _node;
	ExpressionNode* _pCurrentNode;
    int _advanceCount;

	ParseErrors* _pParseErrors;
	int _lineNumber;

	char _expression[512];

	const char* _pCurrent;
	char _value[512];

public:
	ExpressionTokenSource(const char* pExpression, ParseErrors* pParseErrors = 0, int lineNumber = 0)
	{
        PROLOGUE

		SafeString::StringCopy(_expression, pExpression, sizeof(_expression));
		_pCurrent = _expression;
		_pParseErrors = pParseErrors;
		_lineNumber = lineNumber;

		_pCurrentNode = &_node;
        _advanceCount = 0;
		_value[0] = '\0';
		_node._pItem = _value;

		Advance();
	}

	bool EqualTo(const char* pString)
	{
		if (_pCurrentNode == 0)
		{
			return 0;
		}

		return strcmp(pString, _pCurrentNode->_pItem) == 0;
	}

	char FirstChar()
	{
		if (_pCurrentNode == 0)
		{
			return 0;
		}

		return *_pCurrentNode->_pItem;
	}

	char SecondChar()
	{
		return *(_pCurrentNode->_pItem + 1);
	}

	ExpressionNode * GetCurrentNode()
	{
        PROLOGUE
		return _pCurrentNode;
	}

    bool AtEnd()
    {
        return _pCurrentNode == 0;
    }

	bool IsIdentifier()
	{
		if (_pCurrentNode == 0)
		{
			return false;
		}

		if (!_pCurrentNode->IsIdentifier())
		{
			return false;
		}

		return true;
	}

	char PeekChar()
	{
		return *_pCurrent;
	}

	char AdvanceChar()
	{
		char c = *_pCurrent;
		_pCurrent++;
		return c;
	}

	bool Match(char c)
	{
		if (*_pCurrent == '\0' || *_pCurrent != c)
		{
			return false;
		}

		_pCurrent++;

		return true;
	}

	void CopyStringToToken()
	{
		_value[0] = '"';
		int index = 1;

		while (true)
		{
			char c = AdvanceChar();
			if (c == 0)
			{
				_pParseErrors->AddError("Missing closing \" in string", "", 55);
				return;	
			}

			if (c == '"')
			{
				_value[index] = '\0';		// string is copied with beginning " but without closing "
				return;
			}

			_value[index] = c;
			index++;
		}
	}

	void CopyIdentifierToToken(char c)
	{
		_value[0] = c;
		int index = 1;

		while (true)
		{
			char c = PeekChar();

			if (IsIdentifierCharacter(c))
			{
				_value[index] = c;
				index++;
				AdvanceChar();
			}
			else
			{
				_value[index] = '\0';
				return;
			}
		}
	}

	void CopyNumberToToken(char c)
	{
		_value[0] = c;
		int index = 1;

		while (true)
		{
			char c = PeekChar();

			if (IsDigit(c) || c == '.')
			{
				_value[index] = c;
				index++;
				AdvanceChar();
			}
			else
			{
				_value[index] = '\0';
				return;
			}
		}
	}

	bool IsDigit(char c)
	{
		return c >= '0' && c <= '9';
	}

	bool IsAlpha(char c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
	}

	bool IsIdentifierCharacter(char c)
	{
		return IsDigit(c) || IsAlpha(c) || c == '_';
	}

	void CopyToToken(char value)
	{
		_value[0] = value;
		_value[1] = '\0';
	}

	void CopyToToken(const char* pValue)
	{
		SafeString::StringCopy(_value, pValue, sizeof(_value));
	}

	void Advance()
	{
        _advanceCount++;

		while (true)
		{
			char c = AdvanceChar();

			if (c == 0)
			{
				_pCurrentNode = 0;
				return;
			}

			switch (c)
			{
			case '(': 
			case ')': 
			case '{': 
			case '}': 
			case ',': 
			case '*': 
			case '/': 
			case '%': 
            case ':': CopyToToken(c); return;

			case '-': CopyToToken(Match('-') ? "--" : "-"); return;
			case '+': CopyToToken(Match('+') ? "++" : "+"); return;
			case '!': CopyToToken(Match('=') ? "!=" : "!"); return;
			case '=': CopyToToken(Match('=') ? "==" : "="); return;
			case '>': CopyToToken(Match('=') ? ">=" : ">"); return;
			case '<': CopyToToken(Match('=') ? "<=" : "<"); return;

			case '&': if (Match('&')) { CopyToToken("&&"); } return;
			case '|': if (Match('|')) { CopyToToken("||"); } return;

            case '\t':
                break;

            case '\n':
            case '\r':
                while (PeekChar() == '\n' || PeekChar() == '\r')
                {
                    AdvanceChar();
                }
                _value[0] = '\n';
                _value[1] = '\0';
                return;
            
            case ' ':
				// Ignore
				break;

			case '"': CopyStringToToken(); return;

			default:
				if (IsDigit(c))
				{
					CopyNumberToToken(c); return;
				}
				else if (IsAlpha(c))
				{
					CopyIdentifierToToken(c); return;
				}
				
				char s[2];
				s[0] = c;
				s[1] = 0;
				_pParseErrors->AddError("Unrecognized character: ", s, _lineNumber);
			}
		}
	}

    void AdvanceToNewLine()
    {
        while (true)
        {
            if (_pCurrentNode == 0)
            {
                return;
            }
            if (*_pCurrentNode->_pItem == '\n')
            {
                Advance();
                return;
            }
            Advance();
        }
    }

    int GetParseLocation()
    {
        return _advanceCount;
    }

    void SetParseLocation(int newParseLocation)
    {
        _pCurrent = _expression;
        _advanceCount = 0;

        for (int i = 0; i < newParseLocation; i++)
        {
            Advance();
        }
    }

};
