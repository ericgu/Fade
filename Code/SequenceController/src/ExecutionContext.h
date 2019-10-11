class ExecutionContext
{
public:
	VariableCollection _variables;

	Stack _stack;

	ParseErrors _parseErrors;

	Variable* ParseFloatOrVariable(const char* pCommand, int lineNumber)
	{
		return _variables.ParseFloatOrVariable(pCommand, &_parseErrors, lineNumber);
	}
};