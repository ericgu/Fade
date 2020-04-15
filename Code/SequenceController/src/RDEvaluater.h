

class RDEvaluater
{
	ExpressionTokenSource* _pExpressionTokenSource;
	VariableCollection* _pVariableCollection;
	Stack* _pStack;
	IFunctionCaller* _pFunctionCaller;
	ParseErrors* _pParseErrors;
	int _lineNumber;
	char _temporaryBuffer[1024];

	// recursively evaluate function arguments. On the way out of the function we store the arguments. This prevents collisions when the function
	// argument evaluation requires function calls themselves. 
	bool EvaluateFunctionArguments(int argumentNumber)
	{
		PROLOGUE

		if (_pExpressionTokenSource->GetCurrentNode() == 0)
		{
			_pParseErrors->AddError("Missing closing ) in expression", "", _lineNumber);
			return false;
		}

			// if the current token is the closing brace, we are done.
		if (_pExpressionTokenSource->EqualTo(")"))
		{
			_pExpressionTokenSource->Advance();
			Variable argument(argumentNumber);
			_pVariableCollection->AddAndSet("#A", &argument, _pStack->GetFrameCount() + 1); // put into frame that will be created soon...
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

		_pVariableCollection->AddAndSet(FunctionCaller::GenerateArgumentName(argumentNumber), &value, _pStack->GetFrameCount() + 1); // put into frame that will be created soon...

		return true;
	}

	Variable EvaluateExpression()
	{
		PROLOGUE
			
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
			SafeString::StringCopy(identifier, pNode->_pItem, sizeof(identifier));

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
					SafeString::StringCopy(_temporaryBuffer, "$", sizeof(_temporaryBuffer));
					SafeString::StringCat(_temporaryBuffer, identifier, sizeof(_temporaryBuffer));
					Variable var = Variable::Empty();
					_pVariableCollection->AddAndSet(_temporaryBuffer, &var, _pStack->GetFrameCount());

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
		PROLOGUE
			
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
		PROLOGUE
			
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
		PROLOGUE
			
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
		PROLOGUE

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
		PROLOGUE

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
		PROLOGUE

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
		PROLOGUE

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
		PROLOGUE

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
		PROLOGUE

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
		PROLOGUE

		Variable left = EvaluateLogicalOr();

		if (_pExpressionTokenSource->EqualTo("="))
		{
			_pExpressionTokenSource->Advance();
			Variable right = EvaluateLogicalOr();

			Variable* pDestination = _pVariableCollection->GetWithoutErrorCheck(left.GetVariableName(), 0);  // stack level...
			if (!pDestination)
			{
				// new variable. 
				Variable futureValue = Variable::Empty();
				_pVariableCollection->AddAndSet(left.GetVariableName(), &futureValue, _pStack->GetFrameCount());
				pDestination = _pVariableCollection->GetWithoutErrorCheck(left.GetVariableName(), _pStack->GetFrameCount());

				// Remove "undefined" sentinel...
				SafeString::StringCopy(_temporaryBuffer, "$", sizeof(_temporaryBuffer));
				SafeString::StringCat(_temporaryBuffer, left.GetVariableName(), sizeof(_temporaryBuffer));

				_pVariableCollection->Delete(_temporaryBuffer, _pStack->GetFrameCount());
			}

			for (int i = 0; i < right.GetValueCount(); i++)
			{
				pDestination->SetValue(i, right.GetValueFloat(i));
			}

			left.SetToNan();	// assignments are not l-values in this language to prevent "IF TEST = 5"
		}

		return left;
	}

	Variable EvaluateEmpty()
	{
		PROLOGUE

		RETURN(EvaluateAssignment());
	}

	Variable EvaluateTop()
	{
		PROLOGUE

		if (_pExpressionTokenSource->GetCurrentNode() == 0)
		{
			RETURN(Variable::Empty());
		}

		RETURN(EvaluateEmpty());
	}
	
public:
	Variable Evaluate(const char* pExpression, VariableCollection* pVariableCollection = 0, Stack* pStack = 0, IFunctionCaller* pFunctionCaller = 0, ParseErrors* pParseErrors = 0, int lineNumber = -100)
	{
		PROLOGUE

		//Serial.print("Evaluating: "); Serial.println(pExpression);

		ExpressionTokenSource expressionTokenSource(pExpression, pParseErrors);
		_pExpressionTokenSource = &expressionTokenSource;
		_pStack = pStack;
		_pFunctionCaller = pFunctionCaller;
		_pParseErrors = pParseErrors;
		_lineNumber = lineNumber;
		_pVariableCollection = pVariableCollection;

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

		RETURN(value);
	}
};
