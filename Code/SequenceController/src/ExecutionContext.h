class ExecutionContext
{
public:
	VariableCollection _variables;

	Stack _stack;

	FunctionStore _functionStore;
	Expression _expression;

	ExecutionContext()
	{
		_stack.CreateFrame();
	}

	Variable* Parse(const char* pCommand, ParseErrors* pParseErrors, int lineNumber)
	{
		return _expression.Parse(pCommand, &_variables, &_functionStore, &_stack, pParseErrors, lineNumber);
	}

	void ResetVariablesAndStack()
	{
		_variables.Clear();
		_stack.Clear();
	}
};