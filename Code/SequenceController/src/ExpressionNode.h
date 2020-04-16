class ExpressionNode
{
public:
	const char* _pItem;

	bool IsNumber() { return *_pItem >= '0' && *_pItem <= '9';  }

	bool IsIdentifier() { return (*_pItem >= 'a' && *_pItem <= 'z') || (*_pItem >= 'A' && *_pItem <= 'Z'); }
};