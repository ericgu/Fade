class ExecutionContext
{
public:
	VariableCollection _variables;

	Stack _stack;

	FunctionStore _functionStore;

	ExecutionContext()
	{
		_stack.CreateFrame();
	}

	Variable Evaluate(const char* pCommand, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow)
	{
		StackWatcher::Log("ExecutionContext::Evaluate");

		FunctionCaller functionCaller(&_functionStore, &_stack, &_variables, pParseErrors, pExecutionFlow);

		RDEvaluater rdEvaluator;
		return rdEvaluator.Evaluate(pCommand, &_variables, &_stack, &functionCaller, pParseErrors, lineNumber);
	}

	void ResetVariablesAndStack()
	{
		_variables.Clear();
		_stack.Clear();
		_stack.CreateFrame();
	}
};