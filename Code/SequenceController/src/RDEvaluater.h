

class RDEvaluater
{
	ExpressionTokenSource* _pExpressionTokenSource;
	IExecutionContext* _pExecutionContext;
	IFunctionCaller* _pFunctionCaller;
	ParseErrors* _pParseErrors;
    IExecutionFlow* _pExecutionFlow;
	int _lineNumber;
	char _temporaryBuffer[1024];

	// recursively evaluate function arguments. On the way out of the function we store the arguments. This prevents collisions when the function
	// argument evaluation requires function calls themselves. 
	bool EvaluateFunctionArguments(int argumentNumber)
	{
		PROLOGUE

		if (_pExpressionTokenSource->AtEnd())
		{
			_pParseErrors->AddError("Missing closing ) in expression", "", _lineNumber);
			return false;
		}

			// if the current token is the closing brace, we are done.
		if (_pExpressionTokenSource->EqualTo(")"))
		{
			_pExpressionTokenSource->Advance();
			Variable argument(argumentNumber);
			_pExecutionContext->Variables()->AddAndSet("#A", &argument, _pExecutionContext->GetStack()->GetFrameCount() + 1); // put into frame that will be created soon...
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

		_pExecutionContext->Variables()->AddAndSet(FunctionCaller::GenerateArgumentName(argumentNumber), &value, _pExecutionContext->GetStack()->GetFrameCount() + 1); // put into frame that will be created soon...

		return true;
	}

    // recursively evaluate function arguments. On the way out of the function we store the arguments. This prevents collisions when the function
    // argument evaluation requires function calls themselves. 
    bool AssignFunctionParameters(int parameterNumber)
    {
        PROLOGUE

            if (_pExpressionTokenSource->AtEnd())
            {
                _pParseErrors->AddError("Missing closing ) in function call", "", _lineNumber);
                return false;
            }

        // if the current token is the closing brace, we are done.
        if (_pExpressionTokenSource->EqualTo(")"))
        {
            _pExpressionTokenSource->Advance();
            Variable argument(parameterNumber);

            Variable* pArgumentCount = _pExecutionContext->GetVariableWithoutErrorCheck("#A");
            if (parameterNumber != pArgumentCount->GetValueInt())
            {
                _pParseErrors->AddError("Extra arguments passed to function", "", _lineNumber);
                return false;
            }

            return true;
        }

        Variable parameterName = EvaluateTop();

        Variable* pArgument = _pExecutionContext->GetVariableWithoutErrorCheck(FunctionCaller::GenerateArgumentName(parameterNumber));
        if (pArgument == 0)
        {
            // missing argument...
            _pParseErrors->AddError("Missing argument in function call for parameter: ", parameterName.GetVariableName(), _lineNumber);
            return false;
        }

        pArgument->SetVariableName(parameterName.GetVariableName());    // rename argument to parameter

        if (_pExpressionTokenSource->EqualTo(","))
        {
            _pExpressionTokenSource->Advance();
        }

        if (!AssignFunctionParameters(parameterNumber + 1))
        {
            return false;
        }

        return true;
    }

    Variable HandleFunctionCall(const char* pFunctionName)
    {
        Variable returnValue = Variable::Empty();

        FunctionDefinition* pFunctionDefinition = _pExecutionContext->Functions()->Lookup(pFunctionName);
        if (pFunctionDefinition != 0)
        {
            // save current location to return to it after the function call. 
            int parseLocation = _pExpressionTokenSource->GetParseLocation();
            _pExpressionTokenSource->SetParseLocation(pFunctionDefinition->LineNumberStart); // point to function...

            // rename variables...

            _pExpressionTokenSource->Advance(); // skip "FUNC"
            _pExpressionTokenSource->Advance(); // skip name
            _pExpressionTokenSource->Advance(); // skip intro '('

            if (!AssignFunctionParameters(0))
            {
                _pExpressionTokenSource->SetParseLocation(parseLocation);
                return Variable::Empty();
            }

            _pExpressionTokenSource->AdvanceToNewLine();

            while (!_pExpressionTokenSource->AtEnd())
            {
                if (_pExpressionTokenSource->EqualTo("ENDFUNC"))
                {
                    _pExpressionTokenSource->SetParseLocation(parseLocation);

                    Variable* pReturnValue = _pExecutionContext->GetVariableWithoutErrorCheck("<ReturnValue>");
                    if (pReturnValue)
                    {
                        returnValue = *pReturnValue;
                    }

                    return returnValue;
                }
                else
                {
                    EvaluateStatement();

                    // handle abort 
#if fred
                    if (_pExecutionFlow->GetCommandResult()->GetAbort())
                    {
                        return returnValue;
                    }
#endif
                }
            }

            // error case; missing ENDFUNC


        }
        else if (!BuiltInFunctions::HandleBuiltInFunctions(pFunctionName, _pExecutionContext, _pParseErrors, _lineNumber, _pExecutionFlow, &returnValue))
        {
            _pParseErrors->AddError("Unrecognized function: ", pFunctionName, _lineNumber);

            return Variable::Empty();
        }

        return returnValue;
    }


    Variable EvaluateFunctionCall(const char* functionName)
    {
        _pExpressionTokenSource->Advance();

        bool argumentParseSuccess = EvaluateFunctionArguments(0);

        if (argumentParseSuccess)
        {
            _pExecutionContext->GetStack()->CreateFrame();

            Variable returnValue = HandleFunctionCall(functionName);

            _pExecutionContext->Variables()->DeleteStackLevel(_pExecutionContext->GetStack()->GetFrameCount());
            _pExecutionContext->GetStack()->DestroyFrame();

            return returnValue;
        }
        else
        {
            return Variable::Empty();
        }
    }

	Variable EvaluateExpression()
	{
		PROLOGUE
			
		if (_pExpressionTokenSource->AtEnd())
		{
			_pParseErrors->AddError("Missing value at end of expression", "", _lineNumber);
			return Variable::Empty();
		}

        if (_pExpressionTokenSource->IsNumber())
		{
			Variable value = Variable::ParseFloat(_pExpressionTokenSource->GetCurrentToken());
			_pExpressionTokenSource->Advance();
			return value;
		}
		else if (_pExpressionTokenSource->IsIdentifier())
		{
			char identifier[128];
			SafeString::StringCopy(identifier, _pExpressionTokenSource->GetCurrentToken(), sizeof(identifier));

			_pExpressionTokenSource->Advance();

			// variable or function call...

			if (_pExpressionTokenSource->EqualTo("("))	// function call...
			{
                return(EvaluateFunctionCall(identifier));
			}
			else
			{
				Variable *pVariable = _pExecutionContext->GetVariableWithoutErrorCheck(identifier);
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
					_pExecutionContext->AddVariableAndSet(_temporaryBuffer, &var);

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
			value.SetValue(_pExpressionTokenSource->GetCurrentToken() + 1);

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

			Variable* pDestination = _pExecutionContext->GetVariableWithoutErrorCheck(left.GetVariableName());
			if (!pDestination)
			{
				// new variable. 
				Variable futureValue = Variable::Empty();
				_pExecutionContext->AddVariableAndSet(left.GetVariableName(), &futureValue);
				pDestination = _pExecutionContext->GetVariableWithoutErrorCheck(left.GetVariableName());

				// Remove "undefined" sentinel...
				SafeString::StringCopy(_temporaryBuffer, "$", sizeof(_temporaryBuffer));
				SafeString::StringCat(_temporaryBuffer, left.GetVariableName(), sizeof(_temporaryBuffer));

				_pExecutionContext->DeleteVariable(_temporaryBuffer);
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

		if (_pExpressionTokenSource->AtEnd())
		{
			RETURN(Variable::Empty());
		}

        Variable value = EvaluateEmpty();

		RETURN(value);
	}

    void HandleIf()
    {
        bool conditionMatched = false;
        bool executing = false;

        while (!_pExpressionTokenSource->AtEnd())
        {
            if (_pExpressionTokenSource->EqualTo("IF") ||
                _pExpressionTokenSource->EqualTo("ELSEIF"))
            {
                if (_pExpressionTokenSource->EqualTo("IF") && executing)
                {
                    HandleIf();
                }
                else
                {
                    _pExpressionTokenSource->Advance();

                    if (conditionMatched == false)
                    {
                        executing = false;

                        Variable condition = EvaluateTop();

                        if (condition.GetValueInt() != 0)
                        {
                            conditionMatched = true;
                            executing = true;
                        }
                    }
                    else
                    {
                        executing = false;
                    }

                    _pExpressionTokenSource->AdvanceToNewLine();
                }
            }
            else if (_pExpressionTokenSource->EqualTo("ELSE"))
            {
                if (conditionMatched == false)
                {
                    conditionMatched = true;
                    executing = true;
                }
                else
                {
                    executing = false;
                }
                _pExpressionTokenSource->AdvanceToNewLine();
            }
            else if (_pExpressionTokenSource->EqualTo("ENDIF"))
            {
                _pExpressionTokenSource->AdvanceToNewLine();
                return;
            }
            else
            {
                if (executing)
                {
                    EvaluateStatement();
                }
                else
                {
                    _pExpressionTokenSource->AdvanceToNewLine();
                }
            }
        }

        _pParseErrors->AddError("Missing ENDIF", "", _lineNumber);

        return;
    }

    int GetIsInRange(float variableStart, float variableEnd, float value)
    {
        float min = variableStart < variableEnd ? variableStart : variableEnd;
        float max = variableStart < variableEnd ? variableEnd : variableStart;

        return value >= min && value <= max;
    }

    bool HandleFor()
    {
        if (_pExpressionTokenSource->EqualTo("FOR"))
        {
            _pExpressionTokenSource->Advance();

            Variable identifier = EvaluateTop();    // get loop control variable name

            Variable startValue = EvaluateTop();

            //expressionTokenSource.Advance();
            if (_pExpressionTokenSource->FirstChar() != ':')
            {
                // expected ":" error...
            }
            _pExpressionTokenSource->Advance();

            Variable endValue = EvaluateTop();

            Variable increment(1.0F);
            if (_pExpressionTokenSource->FirstChar() == ':')
            {
                _pExpressionTokenSource->Advance();
                increment = EvaluateTop();
            }

            if (!_pExpressionTokenSource->AtEnd() && _pExpressionTokenSource->FirstChar() != '\n')
            {
                // error - unexpected token at the end of FOR statement
            }

            _pExecutionContext->AddVariableAndSet(identifier.GetVariableName(), &startValue);
            // Remove "undefined" sentinel...
            SafeString::StringCopy(_temporaryBuffer, "$", sizeof(_temporaryBuffer));
            SafeString::StringCat(_temporaryBuffer, identifier.GetVariableName(), sizeof(_temporaryBuffer));
            _pExecutionContext->DeleteVariable(_temporaryBuffer);

            Variable* pLoopVariable = _pExecutionContext->GetVariableWithoutErrorCheck(identifier.GetVariableName());
            _pExpressionTokenSource->AdvanceToNewLine();

            int firstLineOfForLoop = _pExpressionTokenSource->GetParseLocation();

            while (!_pExpressionTokenSource->AtEnd())
            {
                if (_pExpressionTokenSource->EqualTo("ENDFOR"))
                {
                    pLoopVariable->Increment(increment);

                    if (!GetIsInRange(startValue.GetValueFloat(0), endValue.GetValueFloat(0), pLoopVariable->GetValueFloat(0)))
                    {
                        _pExecutionContext->DeleteVariable(identifier.GetVariableName());
                        _pExpressionTokenSource->AdvanceToNewLine();

                        return true;
                    }

                    _pExpressionTokenSource->SetParseLocation(firstLineOfForLoop);
                }
                else
                {
                    EvaluateStatement();
                }
            }

            _pParseErrors->AddError("Missing ENDFOR", "", _lineNumber);

            return true;

        }
        return false;
    }

    bool HandleFunctionDefinition()
    {
        if (_pExpressionTokenSource->EqualTo("FUNC"))
        {
            int startOfFunction = _pExpressionTokenSource->GetParseLocation();

            _pExpressionTokenSource->Advance();

            char identifier[128];
            SafeString::StringCopy(identifier, _pExpressionTokenSource->GetCurrentToken(), sizeof(identifier));

            _pExpressionTokenSource->AdvanceToNewLine();

            while (!_pExpressionTokenSource->AtEnd())
            {
                if (_pExpressionTokenSource->EqualTo("ENDFUNC"))
                {
                    FunctionDefinition* pFunctionDefinition = _pExecutionContext->Functions()->Lookup(identifier);
                    
                    if (pFunctionDefinition == 0)
                    {
                        _pExecutionContext->Functions()->DefineStart(identifier, _pParseErrors, startOfFunction);
                    }

                    _pExpressionTokenSource->AdvanceToNewLine();

                    return true;
                }
                else
                {
                    _pExpressionTokenSource->AdvanceToNewLine();
                }
            }

            _pParseErrors->AddError("Missing ENDFUNC for function: ", identifier, _lineNumber);
            return true;

        }
        return false;
    }

    bool HandleReturn()
    {
        if (_pExpressionTokenSource->EqualTo("RETURN"))
        {
            _pExpressionTokenSource->Advance();
            Variable returnValue = EvaluateTop();

            if (returnValue.GetValueCount() == 0)
            {
                _pParseErrors->AddError("Missing value in RETURN statement", "", _lineNumber);
                returnValue = Variable::Empty();
            }

            _pExecutionContext->AddVariableAndSet("<ReturnValue>", &returnValue);

            return true;
        }
        return false;
    }

    Variable EvaluateStatement()
    {
        Variable lastValue;
        lastValue.SetToNan();
        if (_pExpressionTokenSource->EqualTo("IF"))
        {
            HandleIf();
        }
        else if (_pExpressionTokenSource->EqualTo("FOR"))
        {
            HandleFor();
        }
        else if (_pExpressionTokenSource->EqualTo("FUNC"))
        {
            HandleFunctionDefinition();
        }
        else if (_pExpressionTokenSource->EqualTo("RETURN"))
        {
            HandleReturn();
        }
        else
        {
            lastValue = EvaluateTop();

            if (!_pExpressionTokenSource->AtEnd() != 0 && !_pExpressionTokenSource->EqualTo("\n"))
            {
                _pParseErrors->AddError("Unexpected token remaining after parsing: ", _pExpressionTokenSource->GetCurrentToken(), _lineNumber);
                _pExpressionTokenSource->AdvanceToNewLine();
            }
        }

        if (_pExpressionTokenSource->EqualTo("\n"))
        {
            _pExpressionTokenSource->Advance();
        }

        return lastValue;
    }

    Variable EvaluateStatements()
    {
        Variable lastValue;
        lastValue.SetToNan();

        while (!_pExpressionTokenSource->AtEnd())
        {
            lastValue = EvaluateStatement();
        }

        return lastValue;
    }
	
public:
    Variable EvaluateInExistingParse(ExpressionTokenSource* pExpressionTokenSource, IExecutionContext* pExecutionContext = 0, IFunctionCaller* pFunctionCaller = 0, ParseErrors* pParseErrors = 0, int lineNumber = -100, IExecutionFlow* pExecutionFlow = 0)
    {
        PROLOGUE

        _pExpressionTokenSource = pExpressionTokenSource;
        _pExecutionContext = pExecutionContext;
        _pFunctionCaller = pFunctionCaller;
        _pParseErrors = pParseErrors;
        _lineNumber = lineNumber;
        _pExecutionFlow = pExecutionFlow;

        return EvaluateStatements();
    }


	Variable Evaluate(const char* pExpression, IExecutionContext* pExecutionContext = 0, IFunctionCaller* pFunctionCaller = 0, ParseErrors* pParseErrors = 0, int lineNumber = -100, IExecutionFlow* pExecutionFlow = 0)
	{
		PROLOGUE

        ExpressionTokenSource expressionTokenSource(pExpression, pParseErrors);
        Variable value = EvaluateInExistingParse(&expressionTokenSource, pExecutionContext, pFunctionCaller, pParseErrors, lineNumber, pExecutionFlow);

		if (!_pExpressionTokenSource->AtEnd())
		{
			_pParseErrors->AddError("Unexpected token remaining after parsing: ", _pExpressionTokenSource->GetCurrentToken(), _lineNumber);
		}

		// Check for undefined sentinel variables. 
		if (pExecutionContext)
		{

			for (int i = 0; i <_pExecutionContext->Variables()->GetActiveVariableCount(); i++)
			{
				Variable* pVariable = _pExecutionContext->Variables()->Get(i);

				if (*pVariable->GetVariableName() == '$' && pVariable->GetStackLevel() == _pExecutionContext->GetStack()->GetFrameCount())
				{
					_pParseErrors->AddError("Undefined variable: ", pVariable->GetVariableName() + 1, _lineNumber); // line number
				}
			}
		}

		RETURN(value);
	}
};
