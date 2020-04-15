class IExecutionContext
{
public:
	virtual Variable Evaluate(const char* pCommand, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow) = 0;
	virtual void ResetVariablesAndStack() = 0;
	virtual Variable* GetVariableWithoutErrorCheck(const char* pVariableName) = 0;
	virtual void AddVariableAndSet(const char* pVariableName, Variable* pVariable, int stackLevel = -1) = 0;
	virtual void DeleteVariable(const char* pVariableName, int stackLevel = -1) = 0;
	virtual VariableCollection* Variables() = 0;
	virtual Stack* GetStack() = 0;
	virtual StackFrame* StackTopFrame() = 0;
	virtual StackFrame* StackCallingFrame() = 0;
};