
class ExpressionToken
{
public:
	const char* _pItem;
	int	_pItemLength;
};

class ExpressionTokenizer
{
	static const char* operators[];

	const char* _pExpression;
	ExpressionToken _tokens[128];
	int _tokenCount;

public:
	void Tokenize(const char *pExpression)
	{
		_pExpression = pExpression;
		const char* pCurrent = pExpression;

		while (true)
		{
			_tokens[_tokenCount]._pItem = pCurrent;

			if (strmp())

		}

		_tokenCount = 1;
		_tokens[0]._pItem = pExpression;
		_tokens[0]._pItemLength = strlen(pExpression);
	}

	int GetTokenCount() { return _tokenCount; }

	ExpressionToken* GetToken(int tokenNumber)
	{
		return &_tokens[tokenNumber];
	}
};

const char* ExpressionTokenizer::operators[] = { "+", "-", "*", "/", "==", "!=", ">", ">=", "<=", "<", "&&", "||"};