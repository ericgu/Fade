class ExecutionContext
{
public:
	VariableCollection _variables;

	Stack _stack;

	ParseErrors _parseErrors;

	Variable* ParseFloatOrVariable(const char* pCommand)
	{
		return _variables.ParseFloatOrVariable(pCommand, &_parseErrors);
	}
};