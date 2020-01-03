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

	Variable Evaluate(const char* pCommand, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow)
	{
		return _expression.Evaluate(pCommand, &_variables, &_functionStore, &_stack, pParseErrors, lineNumber, pExecutionFlow);
	}

	void ResetVariablesAndStack()
	{
		_variables.Clear();
		_stack.Clear();
		_stack.CreateFrame();
	}
};