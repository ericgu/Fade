
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

class ExpressionTokenizer
{
	static const int NodeCountAllocated = 128;

	static const char* _operators[];
	static const int _operatorCount;

	const char* _pExpression;
	ExpressionNode _pNodes[NodeCountAllocated];
	int _nodeCount;
	int _nodeCountAllocated;

public:
	ExpressionTokenizer()
	{
	}

	void Tokenize(const char *pExpression, ParseErrors* pParseErrors, int lineNumber)
	{
		_pExpression = pExpression;
		const char* pCurrent = pExpression;

		_nodeCount = 0;
		_pNodes[_nodeCount]._pItem = pCurrent;
		bool inQuotedString = false;

		while (*pCurrent != 0)
		{
			if (*pCurrent == '"')
			{
				inQuotedString = !inQuotedString;
			}

			bool found = false;
			if (!inQuotedString)
			{
				for (int i = 0; i < _operatorCount; i++)
				{
					if (strncmp(pCurrent, _operators[i], strlen(_operators[i])) == 0)
					{
						if (_pNodes[_nodeCount]._pItem < pCurrent)
						{
							_pNodes[_nodeCount]._itemLength = pCurrent - _pNodes[_nodeCount]._pItem;
							_nodeCount++;
						}
						_pNodes[_nodeCount]._pItem = _operators[i];
						_pNodes[_nodeCount]._itemLength = strlen(_operators[i]);
						_nodeCount++;
						if (_nodeCount == NodeCountAllocated)
						{
							pParseErrors->AddError("Expression: ", "Too many nodes in expression", lineNumber);
							return;
						}

						pCurrent += strlen(_operators[i]);
						_pNodes[_nodeCount]._pItem = pCurrent;
						found = true;
						break;
					}
				}
			}

			if (!found)
			{
				pCurrent++;
			}
		}

		if (pCurrent - _pNodes[_nodeCount]._pItem > 0)
		{
			_pNodes[_nodeCount]._itemLength = pCurrent - _pNodes[_nodeCount]._pItem;
			_nodeCount++;

			if (_nodeCount == NodeCountAllocated)
			{
				pParseErrors->AddError("Expression: ", "Too many nodes in expression", lineNumber);
				return;
			}
		}

		// Get rid of the whitespace nodes...
		int destination = 0;
		for (int i = 0; i < _nodeCount; i++)
		{
			if (*_pNodes[i]._pItem != ' ' && *_pNodes[i]._pItem != '\t')
			{
				_pNodes[destination] = _pNodes[i];
				destination++;
			}
		}

		for (int i = destination; i < _nodeCount; i++)
		{
			_pNodes[i]._pItem = 0;
			_pNodes[i]._value.SetToNan();
		}
		_nodeCount = destination;

	}

	int GetNodeCount() { return _nodeCount; }

	ExpressionNode* GetNode(int nodeNumber)
	{
		if (nodeNumber < 0 || nodeNumber >= _nodeCount)
		{
			return 0;
		}

		return &_pNodes[nodeNumber];
	}

	void SetNodeEmpty(int index)
	{
		_pNodes[index]._itemLength = -1;
		_pNodes[index]._pItem = 0;
		_pNodes[index]._value.SetToNan();
	}

	int FindMatchingParen(int parenIndex)
	{
		if (*_pNodes[parenIndex]._pItem != '(')
		{
			return -1;
		}

		int openParenCount = 0;
		for (int endParenIndex = parenIndex + 1; endParenIndex < _nodeCount; endParenIndex++)
		{
			ExpressionNode* pNode = _pNodes + endParenIndex;
			if (pNode->_pItem == 0)
			{
				continue;
			}

			if (*pNode->_pItem == '(')
			{
				openParenCount++;
			}

			if (*pNode->_pItem == ')')
			{
				if (openParenCount == 0)
				{
					return endParenIndex;
				}
				openParenCount--;
			}
		}

		return -2;
	}

	Variable FindFirstValue(int start, int max)
	{
		for (int i = start; i <= max; i++)
		{
			ExpressionNode* pNode = _pNodes + i;

			if (!pNode->IsEmpty())
			{
				return pNode->_value;
			}
		}

		Variable nan = Variable();
		nan.SetToNan();
		return nan;
	}
};

const char* ExpressionTokenizer::_operators[] = { " ", "\t", "++", "+", "--", "-", "*", "/", "%", "!=", "==", ">=", ">", "<=", "<", "&&", "||", "(", ")", "!", ",", "=", "{", "}"};
const int ExpressionTokenizer::_operatorCount = 24;
