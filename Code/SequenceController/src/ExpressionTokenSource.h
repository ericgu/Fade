class ExpressionTokenSource
{
	ExpressionNode _node;
	ExpressionNode *_pCurrentNode;

	ParseErrors *_pParseErrors;
	int _lineNumber;

	const char *_pExpression;

	const char *_pCurrent;
	const char *_pCurrentPreviousValue;
	char _value[512];

public:
    static bool LogTokens;

    ExpressionTokenSource(const char *pExpression, ParseErrors *pParseErrors = 0, int lineNumber = 0)
	{
		PROLOGUE;

		//		SafeString::StringCopy(_expression, pExpression, sizeof(_expression));

		_pExpression = pExpression;
		_pCurrent = _pExpression;
		_pParseErrors = pParseErrors;
		_lineNumber = lineNumber;

		_pCurrentNode = &_node;
		_value[0] = '\0';
		_node._pItem = _value;

		Advance();

		EPILOGUE;
	}

	bool EqualTo(const char *pString)
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

	const char *GetCurrentToken()
	{
		return _pCurrentNode->_pItem;
	}

	int GetLineNumber()
	{
		// save current location, and then walk through from the beginning,
		// counting newlines until we get to the current location...

		int currentLocation = GetParseLocation();

		SetParseLocation(0);

		int lineNumber = 0;
		while (GetParseLocation() < currentLocation)
		{
			if (EqualTo("\n"))
			{
				lineNumber++;
			}
			Advance();
		}

		SetParseLocation(currentLocation);

		return lineNumber;
	}

	bool AtEnd()
	{
		return _pCurrentNode == 0;
	}

//	bool IsNumber(char first) { return first >= '0' && first <= '9'; }

//	bool IsIdentifier(char first) { return (first >= 'a' && first <= 'z') || (first >= 'A' && first <= 'Z'); }

    bool IsNumber(char first) { return CharacterClassifier::IsDigit(first); }

    bool IsIdentifier(char first) { return CharacterClassifier::IsAlpha(first); }

	bool IsNumber()
	{
		if (_pCurrentNode == 0)
		{
			return false;
		}

		return IsNumber(*_pCurrentNode->_pItem);
	}

	bool IsIdentifier()
	{
		if (_pCurrentNode == 0)
		{
			return false;
		}

		return IsIdentifier(*_pCurrentNode->_pItem);
	}

	char PeekChar()
	{
		if (_pCurrent == 0)
		{
			return 0;
		}

		return *_pCurrent;
	}

	char AdvanceChar()
	{
		return *_pCurrent++;
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

		while (index < sizeof(_value))
		{
			char c = AdvanceChar();
			if (c == 0)
			{
				_pParseErrors->AddError("Missing closing \" in string", "", 55);
				return;
			}

			if (c == '"')
			{
				_value[index] = '\0'; // string is copied with beginning " but without closing "
				return;
			}

			_value[index] = c;
			index++;
		}
	}

	void CopyIdentifierToToken(char c)
	{
		char *pValue = _value;
		*pValue = c;
		pValue++;
		int index = 1;

		while (index < sizeof(_value))
		{
			char c = PeekChar();

			if (IsIdentifierCharacter(c))
			{
				*pValue = c;
				pValue++;
				//_value[index] = c;
				index++;
				AdvanceChar();
			}
			else
			{
				*pValue = '\0';
				//_value[index] = '\0';
				return;
			}
		}
	}

	void CopyNumberToToken(char c)
	{
		_value[0] = c;
		int index = 1;

		while (index < sizeof(_value))
		{
			char c = PeekChar();

			if (CharacterClassifier::IsNumber(c))
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
		return CharacterClassifier::IsDigit(c);
	}

	bool IsAlpha(char c)
	{
		return CharacterClassifier::IsAlpha(c);
	}

	bool IsIdentifierCharacter(char c)
	{
		return CharacterClassifier::IsIdentifier(c);
	}

	void CopyToToken(char value)
	{
		*_value = value;
        *(_value + 1) = '\0';
	}

	void CopyToToken(const char *pValue)
	{
		SafeString::StringCopy(_value, pValue, sizeof(_value));
	}

	void EatComment()
	{
		while (*_pCurrent != 0 && *_pCurrent != '\n' && *_pCurrent != '\r')
		{
			_pCurrent++;
		}

		if (*_pCurrent == 0)
		{
			_pCurrentNode = 0;
			return;
		}

		Advance();
	}

	void Advance()
	{
        if (LogTokens)
        {
            Serial.print("Last token: ");
            if (*GetCurrentToken() == '\n')
            {
                Serial.println("<\\n>");
            }
            else if (*GetCurrentToken() == '\r')
            {
                Serial.println("<\\r>");
            }
            else
            {
                Serial.println(GetCurrentToken());
            }
        }

        _pCurrentPreviousValue = _pCurrent;

		while (true)
		{
			char c = AdvanceChar();
			//Serial.print("  ");
			//Serial.println((int)c);

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
			case '%':
      case ':':
      case '[':
      case ']':
        CopyToToken(c);
				return;

			case '-':
				CopyToToken(Match('-') ? "--" : "-");
				return;
			case '+':
				CopyToToken(Match('+') ? "++" : "+");
				return;
			case '!':
				CopyToToken(Match('=') ? "!=" : "!");
				return;
			case '=':
				CopyToToken(Match('=') ? "==" : "=");
				return;
			case '>':
				CopyToToken(Match('=') ? ">=" : ">");
				return;
			case '<':
				CopyToToken(Match('=') ? "<=" : "<");
				return;

			case '/':
				if (Match('/'))
				{
					EatComment();
				}
				else
				{
					CopyToToken("/");
				}
				return;

			case '&':
				if (Match('&'))
				{
					CopyToToken("&&");
				}
				return;
			case '|':
				if (Match('|'))
				{
					CopyToToken("||");
				}
				return;

			case '\t':
				break;

			case '\n':
				if (PeekChar() == '\r')
				{
					AdvanceChar();
				}
				*_value = '\n';
				_value[1] = '\0';
				return;

			case '\r':
				if (PeekChar() == '\n')
				{
					AdvanceChar();
				}
				_value[0] = '\n';
				_value[1] = '\0';
				return;

			case ' ':
				// Ignore
				break;

			case '"':
				CopyStringToToken();
				return;

			default:
                if (IsAlpha(c))
                {
                    CopyIdentifierToToken(c);
                    return;
                }
                else if (IsDigit(c))
				{
					CopyNumberToToken(c);
					return;
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
				while (_pCurrentNode != 0 && *_pCurrentNode->_pItem == '\n')
				{
					Advance();
				}
				return;
			}
			Advance();
		}
	}

	int GetParseLocation()
	{
		return _pCurrentPreviousValue - _pExpression;
	}

	void SetParseLocation(int newParseLocation)
	{
		_pCurrentNode = &_node;
		_pCurrent = _pExpression + newParseLocation;

		Advance();
	}
};

bool ExpressionTokenSource::LogTokens = false;
