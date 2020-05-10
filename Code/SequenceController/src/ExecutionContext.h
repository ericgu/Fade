class ExecutionContext : public IExecutionContext
{
	VariableCollection _variables;
	Stack _stack;

public:
	FunctionStore _functionStore;

	ExecutionContext()
	{
		_stack.CreateFrame();
	}

	virtual ~ExecutionContext()
	{
	}

	Variable Evaluate(ExpressionTokenSource *pExpressionTokenSource, ParseErrors *pParseErrors, IExecutionFlow *pExecutionFlow)
	{
		StackWatcher::Log("ExecutionContext::Evaluate");

		//FunctionCaller functionCaller(&_functionStore, this, pParseErrors, pExecutionFlow);

		RDEvaluater rdEvaluator;
		return rdEvaluator.EvaluateInExistingParse(pExpressionTokenSource, this, pParseErrors, pExecutionFlow);
	}

	Variable Evaluate(const char *pCommand, ParseErrors *pParseErrors, int lineNumber, IExecutionFlow *pExecutionFlow)
	{
		StackWatcher::Log("ExecutionContext::Evaluate");

		//FunctionCaller functionCaller(&_functionStore, this, pParseErrors, pExecutionFlow);

		RDEvaluater rdEvaluator;
		return rdEvaluator.Evaluate(pCommand, this, pParseErrors, pExecutionFlow);
	}

	void ResetVariablesAndStack()
	{
		_variables.Clear();
		_stack.Clear();
		_stack.CreateFrame();
		_functionStore.Clear();
	}

	Variable *GetVariableWithoutErrorCheck(const char *pVariableName)
	{
		return _variables.GetWithoutErrorCheck(pVariableName, _stack.GetFrameCount());
	}

	void AddVariableAndSet(const char *pVariableName, Variable *pVariable, int stackLevel = -1)
	{
		if (stackLevel == -1)
		{
			stackLevel = _stack.GetFrameCount();
		}
		_variables.AddAndSet(pVariableName, pVariable, stackLevel);
	}

	void DeleteVariable(const char *pVariableName, int stackLevel = -1)
	{
		if (stackLevel == -1)
		{
			stackLevel = _stack.GetFrameCount();
		}
		_variables.Delete(pVariableName, stackLevel);
	}

	VariableCollection *Variables()
	{
		return &_variables;
	}

	Stack *GetStack()
	{
		return &_stack;
	}

	StackFrame *StackTopFrame()
	{
		return _stack.GetTopFrame();
	}

	StackFrame *StackCallingFrame()
	{
		return _stack.GetCallingFrame();
	}

	FunctionStore *Functions()
	{
		return &_functionStore;
	}
};