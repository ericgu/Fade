

class RDEvaluater
{
	ExpressionTokenSource* _pExpressionTokenSource;
	VariableCollection* _pVariableCollection;
	Stack* _pStack;
	IFunctionCaller* _pFunctionCaller;
	ParseErrors* _pParseErrors;
	int _lineNumber;

	// recursively evaluate function arguments. On the way out of the function we store the arguments. This prevents collisions when the function
	// argument evaluation requires function calls themselves. 
	bool EvaluateFunctionArguments(int argumentNumber)
	{
		if (_pExpressionTokenSource->GetCurrentNode() == 0)
		{
			_pParseErrors->AddError("Missing closing ) in expression", "", _lineNumber);
			return false;
		}

			// if the current token is the closing brace, we are done.
		if (_pExpressionTokenSource->EqualTo(")"))
		{
			_pExpressionTokenSource->Advance();
			_pVariableCollection->AddAndSet("#A", &Variable(argumentNumber), _pStack->GetFrameCount() + 1); // put into frame that will be created soon...
			return true;
		}

		Variable value = EvaluateTop();

		if (_pExpressionTokenSource->EqualTo(","))
		{
			_pExpressionTokenSource->Advance();
		}

		if (!EvaluateFunctionArguments(argumentNumber + 1))
		{
			return false;
		}

		char argumentName[10];
		sprintf(argumentName, "#A%d", argumentNumber);
		_pVariableCollection->AddAndSet(argumentName, &value, _pStack->GetFrameCount() + 1); // put into frame that will be created soon...

		return true;
	}

	Variable EvaluateExpression()
	{
		ExpressionNode* pNode = _pExpressionTokenSource->GetCurrentNode();
		if (pNode == 0)
		{
			_pParseErrors->AddError("Missing value at end of expression", "", _lineNumber);
			return Variable::Empty();
		}

		if (pNode->IsNumber())
		{
			Variable value = Variable::ParseFloat(pNode->_pItem);
			_pExpressionTokenSource->Advance();
			return value;
		}
		else if (pNode->IsIdentifier())
		{
			char identifier[128];
			strcpy(identifier, pNode->_pItem);

			_pExpressionTokenSource->Advance();

			// variable or function call...

			if (_pExpressionTokenSource->EqualTo("("))	// function call...
			{
				_pExpressionTokenSource->Advance();

				bool argumentParseSuccess = EvaluateFunctionArguments(0);

				if (argumentParseSuccess)
				{
					_pStack->CreateFrame();

					Variable returnValue = _pFunctionCaller->Call(identifier, _lineNumber);

					_pVariableCollection->DeleteStackLevel(_pStack->GetFrameCount());
					_pStack->DestroyFrame();

					return returnValue;
				}
			}
			else
			{
				Variable *pVariable = _pVariableCollection->GetWithoutErrorCheck(identifier, _pStack->GetFrameCount());
				if (pVariable)
				{
					Variable returnValue = *pVariable;

					if (_pExpressionTokenSource->EqualTo("++"))
					{
						pVariable->SetValue(pVariable->GetValueFloat(0) + 1);
						_pExpressionTokenSource->Advance();
					}
					else if (_pExpressionTokenSource->EqualTo("--"))
					{
						pVariable->SetValue(pVariable->GetValueFloat(0) - 1);
						_pExpressionTokenSource->Advance();
					}

					return returnValue;
				}
				else
				{
						// Add in a sentinel; this notes that we found a name that we didn't recognize. That means it's either:
						// 1) A variable we are creating in an assignment
						// 2) An undefined variable.
						// We will delete this variable if we end up doing the assignment. 
						// Any variables like this at the end of evaluation were undefined...
					char taggedVariableName[128];
					strcpy(taggedVariableName, "$");
					strcat(taggedVariableName, identifier);
					_pVariableCollection->AddAndSet(taggedVariableName, &Variable(1), _pStack->GetFrameCount());

					Variable returnValue;
					returnValue.SetToNan();
					returnValue.SetVariableName(identifier);

					return returnValue;
				}
			}
		}

		return Variable::Empty();
	}

	Variable EvaluateString()
	{
		if (_pExpressionTokenSource->FirstChar() == '"')
		{
			Variable value;
			value.SetValue(_pExpressionTokenSource->GetCurrentNode()->_pItem + 1);

			_pExpressionTokenSource->Advance();

			return value;
		}

		return EvaluateExpression();
	}

	Variable EvaluateParentheses()
	{
		if (_pExpressionTokenSource->EqualTo("("))
		{
			_pExpressionTokenSource->Advance();

			Variable value = EvaluateTop();

			if (!_pExpressionTokenSource->EqualTo(")"))
			{
				_pParseErrors->AddError("Missing ) in expression", "", _lineNumber);

				return Variable::Empty();
			}
			_pExpressionTokenSource->Advance(); // eat closing paren...

			return value;
		}

		return EvaluateString();
	}

	Variable EvaluateMultiValueNumber()
	{
		if (_pExpressionTokenSource->EqualTo("{"))
		{
			_pExpressionTokenSource->Advance();

			Variable first = EvaluateTop();

			while (_pExpressionTokenSource->EqualTo(","))
			{
				_pExpressionTokenSource->Advance();

				Variable next = EvaluateTop();

				first.SetValue(first.GetValueCount(), next.GetValueFloat(0));	// accumulate to first value
			}

			if (!_pExpressionTokenSource->EqualTo("}"))
			{
				_pParseErrors->AddError("Missing } in expression", "", _lineNumber);

				return Variable::Empty();
			}
			_pExpressionTokenSource->Advance(); // eat closing brace...

			return first;
		}
		
		return EvaluateParentheses();
	}


	Variable EvaluateUnary()
	{
		ExpressionNode* pNode = _pExpressionTokenSource->GetCurrentNode();

		if (_pExpressionTokenSource->EqualTo("+") || _pExpressionTokenSource->EqualTo("-"))
		{
			char op = _pExpressionTokenSource->FirstChar();

			_pExpressionTokenSource->Advance();
			Variable value = EvaluateUnary();

			switch (op)
			{
			case '-':
				return -value.GetValueFloat(0);

			case '+':
				return value;
			}
		}

		return EvaluateMultiValueNumber();
	}

	Variable EvaluateMultiplicative()
	{
		Variable left = EvaluateUnary();

		while (_pExpressionTokenSource->EqualTo("*") || _pExpressionTokenSource->EqualTo("/") || _pExpressionTokenSource->EqualTo("%"))
		{
			char op = _pExpressionTokenSource->FirstChar();

			_pExpressionTokenSource->Advance();
			Variable right = EvaluateUnary();

			switch (op)
			{
			case '*':
				left = Variable(left.GetValueFloat(0) * right.GetValueFloat(0));	// NOTE: how to handle multi-values?
				break;

			case '/':
				left = Variable(left.GetValueFloat(0) / right.GetValueFloat(0));	// NOTE: how to handle multi-values?
				break;

			case '%':
				left = Variable(left.GetValueInt() % right.GetValueInt());
				break;
			}
		}

		return left;
	}

	Variable EvaluateAdditive()
	{
		Variable left = EvaluateMultiplicative();

		while (_pExpressionTokenSource->EqualTo("+") || _pExpressionTokenSource->EqualTo("-"))
		{
			char op = _pExpressionTokenSource->FirstChar();
			_pExpressionTokenSource->Advance();
			Variable right = EvaluateMultiplicative();

			switch (op)
			{
			case '+':
				left = Variable(left.GetValueFloat(0) + right.GetValueFloat(0));	// NOTE: how to handle multi-values?
				break;

			case '-':
				left = Variable(left.GetValueFloat(0) - right.GetValueFloat(0));	// NOTE: how to handle multi-values?
				break;
			}
		}

		return left;
	}

	Variable EvaluateRelational()
	{
		Variable left = EvaluateAdditive();

		while (_pExpressionTokenSource->EqualTo("<") || _pExpressionTokenSource->EqualTo("<=") || 
			   _pExpressionTokenSource->EqualTo(">") || _pExpressionTokenSource->EqualTo(">="))
		{
			char op = _pExpressionTokenSource->FirstChar();
			char andEqual = _pExpressionTokenSource->SecondChar() == '=';

			_pExpressionTokenSource->Advance();
			Variable right = EvaluateAdditive();

			if (op == '<')
			{
				left = andEqual ? Variable(left.GetValueFloat(0) <= right.GetValueFloat(0)) : Variable(left.GetValueFloat(0) < right.GetValueFloat(0));
			}
			else if (op == '>')
			{
				left = andEqual ? Variable(left.GetValueFloat(0) >= right.GetValueFloat(0)) : Variable(left.GetValueFloat(0) > right.GetValueFloat(0));
			}
		}

		return left;
	}

	Variable EvaluateEquality()
	{
		Variable left = EvaluateRelational();

		while (_pExpressionTokenSource->EqualTo("==") || _pExpressionTokenSource->EqualTo("!="))
		{
			char op = _pExpressionTokenSource->FirstChar();

			_pExpressionTokenSource->Advance();
			Variable right = EvaluateRelational();

			if (op == '=')
			{
				left = Variable(left.GetValueFloat(0) == right.GetValueFloat(0));
			}
			else if (op == '!')
			{
				left = Variable(left.GetValueFloat(0) != right.GetValueFloat(0));
			}
		}

		return left;
	}

	Variable EvaluateLogicalAnd()
	{
		Variable left = EvaluateEquality();

		while (_pExpressionTokenSource->EqualTo("&&"))
		{
			_pExpressionTokenSource->Advance();
			Variable right = EvaluateEquality();

			left = Variable(left.GetValueFloat(0) && right.GetValueFloat(0));
		}

		return left;
	}

	Variable EvaluateLogicalOr()
	{
		Variable left = EvaluateLogicalAnd();

		while (_pExpressionTokenSource->EqualTo("||"))
		{
			_pExpressionTokenSource->Advance();
			Variable right = EvaluateLogicalAnd();

			left = Variable(left.GetValueFloat(0) || right.GetValueFloat(0));
		}

		return left;
	}

	Variable EvaluateAssignment()
	{
		Variable left = EvaluateLogicalOr();

		if (_pExpressionTokenSource->EqualTo("="))
		{
			_pExpressionTokenSource->Advance();
			Variable right = EvaluateLogicalOr();

			Variable* pDestination = _pVariableCollection->GetWithoutErrorCheck(left.GetVariableName(), 0);  // stack level...
			if (!pDestination)
			{
				// new variable. 
				_pVariableCollection->AddAndSet(left.GetVariableName(), &Variable(), _pStack->GetFrameCount());
				pDestination = _pVariableCollection->GetWithoutErrorCheck(left.GetVariableName(), _pStack->GetFrameCount());

				// Remove "undefined" sentinel...
				char taggedVariableName[128];
				strcpy(taggedVariableName, "$");
				strcat(taggedVariableName, left.GetVariableName());

				_pVariableCollection->Delete(taggedVariableName, _pStack->GetFrameCount()); 
			}

			for (int i = 0; i < right.GetValueCount(); i++)
			{
				pDestination->SetValue(i, right.GetValueFloat(i));
			}

			left.SetToNan();	// assignments are not l-values in this language to prevent "IF TEST = 5"
		}

		return left;
	}

	Variable EvaluateTop()
	{
		return EvaluateAssignment();
	}
	
public:
	Variable Evaluate(const char* pExpression, VariableCollection* pVariableCollection = 0, Stack* pStack = 0, IFunctionCaller* pFunctionCaller = 0, ParseErrors* pParseErrors = 0, int lineNumber = -100)
	{
		_pExpressionTokenSource = new ExpressionTokenSource(pExpression, pParseErrors);
		_pVariableCollection = pVariableCollection;
		_pStack = pStack;
		_pFunctionCaller = pFunctionCaller;
		_pParseErrors = pParseErrors;
		_lineNumber = lineNumber;

		Variable value = EvaluateTop();

		ExpressionNode* pNode = _pExpressionTokenSource->GetCurrentNode();

		if (pNode != 0)
		{
			_pParseErrors->AddError("Unexpected token remaining after parsing: ", pNode->_pItem, _lineNumber);
		}

		// Check for undefined sentinel variables. 
		if (pVariableCollection)
		{
			for (int i = 0; i < pVariableCollection->GetActiveVariableCount(); i++)
			{
				Variable* pVariable = pVariableCollection->Get(i);

				if (*pVariable->GetVariableName() == '$' && pVariable->GetStackLevel() == pStack->GetFrameCount())
				{
					_pParseErrors->AddError("Undefined variable: ", pVariable->GetVariableName() + 1, _lineNumber); // line number
				}
			}
		}

		return value;
	}
};
