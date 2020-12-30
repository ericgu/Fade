// The result of evaluating an expression...

class ExpressionResult
{
public:
    Variable _variable;
    int _arrayIndex;

    ExpressionResult()
    {
        _arrayIndex = -1;
    }

    static ExpressionResult Empty()
    {
      ExpressionResult expressionResult;
      expressionResult._variable = Variable::Empty();
      return expressionResult;
    }
};