class ExecutionContext
{
public:
	VariableCollection _variables;

	Stack _stack;

	//ParseErrors _parseErrors;

	Variable* ParseFloatOrVariable(const char* pCommand, ParseErrors* pParseErrors, int lineNumber)
	{
		return _variables.ParseFloatOrVariable(pCommand, pParseErrors, lineNumber);
	}

	void ResetVariablesAndStack()
	{
		_variables.Clear();
		_stack.Clear();
	}
};