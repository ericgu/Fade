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
		StackWatcher::Log("ExecutionContext::Evaluate");

		FunctionCaller functionCaller(&_functionStore, &_stack, &_variables, pParseErrors, pExecutionFlow);
//			FunctionCaller(FunctionStore* pFunctionStore, Stack* pStack, VariableCollection* pVariableCollection, ParseErrors* pParseErrors, IExecutionFlow* pExecutionFlow)


		RDEvaluater rdEvaluator;
		return rdEvaluator.Evaluate(pCommand, &_variables, &_stack, &functionCaller, pParseErrors, lineNumber);
//			Variable Evaluate(const char* pExpression, VariableCollection* pVariableCollection = 0, Stack* pStack = 0, IFunctionCaller* pFunctionCaller = 0, ParseErrors* pParseErrors = 0, int lineNumber = -100)

		//return _expression.Evaluate(pCommand, &_variables, &_functionStore, &_stack, pParseErrors, lineNumber, pExecutionFlow);
	}

	void ResetVariablesAndStack()
	{
		_variables.Clear();
		_stack.Clear();
		_stack.CreateFrame();
	}
};