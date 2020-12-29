// The result of evaluating an expression...

class ExpressionResult
{
public:
    Variable _variable;

    static ExpressionResult Empty()
    {
      ExpressionResult expressionResult;
      expressionResult._variable = Variable::Empty();
      return expressionResult;
    }
};