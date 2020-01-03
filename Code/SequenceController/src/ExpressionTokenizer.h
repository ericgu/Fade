
class ExpressionNode
{
public:
	const char* _pItem;
	int	_pItemLength;
	Variable _pValue;

	ExpressionNode()
	{
		_pValue.SetToNan();
	}

	bool StartsWith(char character)
	{
		return _pItem != 0 && *_pItem == character;
	}

	bool IsEmpty() { return _pItemLength == -1; }
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

		while (*pCurrent != 0)
		{
			bool found = false;
			for (int i = 0; i < _operatorCount; i++)
			{
				if (strncmp(pCurrent, _operators[i], strlen(_operators[i])) == 0)
				{
					if (_nodes[_nodeCount]._pItem < pCurrent)
					{
						_nodes[_nodeCount]._pItemLength = pCurrent - _nodes[_nodeCount]._pItem;
						_nodeCount++;
					}
					_nodes[_nodeCount]._pItem = _operators[i]; 
					_nodes[_nodeCount]._pItemLength = strlen(_operators[i]);
					_nodeCount++;

					pCurrent += strlen(_operators[i]);
					_nodes[_nodeCount]._pItem = pCurrent;
					found = true;
					break;
				}
			}

			if (!found)
			{
				pCurrent++;
			}
		}

		if (pCurrent - _nodes[_nodeCount]._pItem > 0)
		{
			_nodes[_nodeCount]._pItemLength = pCurrent - _nodes[_nodeCount]._pItem;
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
			_nodes[i]._pValue.SetToNan();
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
		_nodes[index]._pItemLength = -1;
		_nodes[index]._pItem = 0;
		_nodes[index]._pValue.SetToNan();
	}
};

const char* ExpressionTokenizer::_operators[] = { " ", "\t", "+", "-", "*", "/", "%", "!=", "==", ">=", ">", "<=", "<", "&&", "||", "(", ")", "!", ","};
const int ExpressionTokenizer::_operatorCount = 19;
