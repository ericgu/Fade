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

    ExpressionResult(Variable result)
    {
      _arrayIndex = -1;
      _variable = result;
    }

    ExpressionResult(const ExpressionResult &other)
    {
        _variable = other._variable;
        _arrayIndex = other._arrayIndex;
    }

    ExpressionResult &operator=(const ExpressionResult &other)
    {
        _variable = other._variable;
        _arrayIndex = other._arrayIndex;

        return *this;
    }

    ~ExpressionResult()
    {

    }

    static ExpressionResult Empty()
    {
      ExpressionResult expressionResult;
      expressionResult._variable = Variable::Empty();
      return expressionResult;
    }
};