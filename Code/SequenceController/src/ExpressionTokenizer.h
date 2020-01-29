
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
	static const char* _operators[];
	static const int _operatorCount;

	const char* _pExpression;
	ExpressionNode _nodes[128];
	int _nodeCount;

public:
	void Tokenize(const char *pExpression)
	{
		_pExpression = pExpression;
		const char* pCurrent = pExpression;

		_nodeCount = 0;
		_nodes[_nodeCount]._pItem = pCurrent;
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
						if (_nodes[_nodeCount]._pItem < pCurrent)
						{
							_nodes[_nodeCount]._itemLength = pCurrent - _nodes[_nodeCount]._pItem;
							_nodeCount++;
						}
						_nodes[_nodeCount]._pItem = _operators[i];
						_nodes[_nodeCount]._itemLength = strlen(_operators[i]);
						_nodeCount++;

						pCurrent += strlen(_operators[i]);
						_nodes[_nodeCount]._pItem = pCurrent;
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

		if (pCurrent - _nodes[_nodeCount]._pItem > 0)
		{
			_nodes[_nodeCount]._itemLength = pCurrent - _nodes[_nodeCount]._pItem;
			_nodeCount++;
		}

		// Get rid of the whitespace nodes...
		int destination = 0;
		for (int i = 0; i < _nodeCount; i++)
		{
			if (*_nodes[i]._pItem != ' ' && *_nodes[i]._pItem != '\t')
			{
				_nodes[destination] = _nodes[i];
				destination++;
			}
		}

		for (int i = destination; i < _nodeCount; i++)
		{
			_nodes[i]._pItem = 0;
			_nodes[i]._value.SetToNan();
		}
		_nodeCount = destination;

	}

	int GetNodeCount() { return _nodeCount; }

	ExpressionNode* GetNode(int nodeNumber)
	{
		return &_nodes[nodeNumber];
	}

	void SetNodeEmpty(int index)
	{
		_nodes[index]._itemLength = -1;
		_nodes[index]._pItem = 0;
		_nodes[index]._value.SetToNan();
	}

	int FindMatchingParen(int parenIndex)
	{
		if (*_nodes[parenIndex]._pItem != '(')
		{
			return -1;
		}

		int openParenCount = 0;
		for (int endParenIndex = parenIndex + 1; endParenIndex < _nodeCount; endParenIndex++)
		{
			ExpressionNode* pNode = _nodes + endParenIndex;
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
			ExpressionNode* pNode = _nodes + i;

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

const char* ExpressionTokenizer::_operators[] = { " ", "\t", "++", "+", "--", "-", "*", "/", "%", "!=", "==", ">=", ">", "<=", "<", "&&", "||", "(", ")", "!", ",", "="};
const int ExpressionTokenizer::_operatorCount = 22;
