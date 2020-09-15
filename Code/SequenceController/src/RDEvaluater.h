

class RDEvaluater
{
	ExpressionTokenSource* _pExpressionTokenSource;
	IExecutionContext* _pExecutionContext;
	//IFunctionCaller* _pFunctionCaller;
	ParseErrors* _pParseErrors;
    IExecutionFlow* _pExecutionFlow;
	char _temporaryBuffer[1024];

    const char* GenerateArgumentName(int argumentNumber)
    {
        snprintf(_temporaryBuffer, sizeof(_temporaryBuffer), "#A%d", argumentNumber);

        return _temporaryBuffer;
    }

    void ReportError(const char* pString, const char* pValue, int lineNumber = -10)
    {
        if (lineNumber == -10)
        {
            lineNumber = _pExpressionTokenSource->GetLineNumber();
        }
        _pParseErrors->AddError(pString, pValue, lineNumber);
    }

    bool IsAborting(const char* pAbortingMessage, int lineNumber = -10)
    {
        if (_pExecutionFlow != 0 && _pExecutionFlow->IsAborting())
        {
            if (pAbortingMessage != 0)
            {
                ReportError("Aborting: ", pAbortingMessage, lineNumber);
            }
            return true;
        }

        if (_pExecutionFlow != 0 && _pExecutionFlow->IsBreaking())
        {
            return true;
        }

        if (_pParseErrors != 0 &&_pParseErrors->GetErrorCount() != 0)
        {
            return true;
        }

        return false;
    }


	// recursively evaluate function arguments. On the way out of the function we store the arguments. This prevents collisions when the function
	// argument evaluation requires function calls themselves. 
	bool EvaluateFunctionArguments(int argumentNumber)
	{
        PROLOGUE;

		if (_pExpressionTokenSource->AtEnd())
		{
            ReportError("Missing closing ) in expression", "");
			return false;
		}

			// if the current token is the closing brace, we are done.
		if (_pExpressionTokenSource->EqualTo(")"))
		{
			_pExpressionTokenSource->Advance();
			Variable argument(argumentNumber);
			_pExecutionContext->Variables()->AddAndSet("#A", &argument, _pExecutionContext->GetStack()->GetFrameCount() + 1); // put into frame that will be created soon...
            RETURN(true);
		}

		Variable value = EvaluateTop();
        if (!(ValidateHasValue(&value)))
        {
            return false;
        }

		if (_pExpressionTokenSource->EqualTo(","))
		{
			_pExpressionTokenSource->Advance();
		}

		if (!EvaluateFunctionArguments(argumentNumber + 1))
		{
			return false;
		}

		_pExecutionContext->Variables()->AddAndSet(GenerateArgumentName(argumentNumber), &value, _pExecutionContext->GetStack()->GetFrameCount() + 1); // put into frame that will be created soon...

        RETURN(true);
	}

    // recursively evaluate function arguments. On the way out of the function we store the arguments. This prevents collisions when the function
    // argument evaluation requires function calls themselves. 
    bool AssignFunctionParameters(const char* pFunctionName, int parameterNumber)
    {
        PROLOGUE;

            if (_pExpressionTokenSource->AtEnd())
            {
                ReportError("Missing closing ) in function call ", pFunctionName);
                return false;
            }

        // if the current token is the closing brace, we are done.
        if (_pExpressionTokenSource->EqualTo(")"))
        {
            _pExpressionTokenSource->Advance();
            Variable* pArgumentCount = _pExecutionContext->GetVariableWithoutErrorCheck("#A");
            if (parameterNumber != pArgumentCount->GetValueInt())
            {
                ReportError("Extra arguments passed to function ", pFunctionName);
                return false;
            }

            RETURN(true);
        }

        Variable parameterName = EvaluateTop();

        Variable* pArgument = _pExecutionContext->GetVariableWithoutErrorCheck(GenerateArgumentName(parameterNumber));
        if (pArgument == 0)
        {
            // missing argument...
            ReportError("Missing argument in function call for parameter: ", parameterName.GetVariableName());
            return false;
        }

        pArgument->SetVariableName(parameterName.GetVariableName());    // rename argument to parameter

        if (_pExpressionTokenSource->EqualTo(","))
        {
            _pExpressionTokenSource->Advance();
        }

        if (!AssignFunctionParameters(pFunctionName, parameterNumber + 1))
        {
            return false;
        }

        RETURN(true);
    }

    Variable HandleFunctionCall(const char* pFunctionName)
    {
        PROLOGUE;
            
        Variable returnValue = Variable::Empty();

        FunctionDefinition* pFunctionDefinition = _pExecutionContext->Functions()->Lookup(pFunctionName);
        if (pFunctionDefinition != 0)
        {
            // save current location to return to it after the function call. 
            int parseLocation = _pExpressionTokenSource->GetParseLocation();
            _pExpressionTokenSource->SetParseLocation(pFunctionDefinition->LineNumberStart); // point to function...

            // rename variables...

            if (_pExpressionTokenSource->EqualTo("FUNC"))
            {
                _pExpressionTokenSource->Advance();
            }
            else
            {
                Serial.print("Error1: "); Serial.println(_pExpressionTokenSource->GetCurrentToken());
            }

            if (_pExpressionTokenSource->EqualTo(pFunctionName))
            {
                _pExpressionTokenSource->Advance();
            }
            else
            {
                Serial.print("Error2: "); Serial.println(_pExpressionTokenSource->GetCurrentToken());
            }

            if (_pExpressionTokenSource->EqualTo("("))
            {
                _pExpressionTokenSource->Advance();
            }
            else
            {
                Serial.print("Error3: "); Serial.println(_pExpressionTokenSource->GetCurrentToken());
            }

            if (!AssignFunctionParameters(pFunctionName, 0))
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

                    RETURN(returnValue);
                }
                else
                {
                    EvaluateStatement();

                    if (IsAborting("Function"))
                    {
                        return returnValue;
                    }
                }
            }

            // error case; missing ENDFUNC


        }
        else if (!BuiltInFunctions::HandleBuiltInFunctions(pFunctionName, _pExecutionContext, _pParseErrors, _pExpressionTokenSource, _pExecutionFlow, &returnValue))
        {
            ReportError("Unrecognized function: ", pFunctionName);

            return Variable::Empty();
        }

        return returnValue;
    }


    Variable EvaluateFunctionCall(const char* functionName)
    {
        PROLOGUE;
            
        _pExpressionTokenSource->Advance();

        bool argumentParseSuccess = EvaluateFunctionArguments(0);

        if (argumentParseSuccess)
        {
            _pExecutionContext->GetStack()->CreateFrame();

            Variable returnValue = HandleFunctionCall(functionName);

            _pExecutionContext->Variables()->DeleteStackLevel(_pExecutionContext->GetStack()->GetFrameCount());
            _pExecutionContext->GetStack()->DestroyFrame();

            RETURN(returnValue);
        }
        else
        {
            return Variable::Empty();
        }
    }

	Variable EvaluateExpression()
	{
        PROLOGUE;
			
		if (_pExpressionTokenSource->AtEnd())
		{
			ReportError("Missing value at end of expression", "");
			return Variable::Empty();
		}

        if (_pExpressionTokenSource->IsNumber())
		{
			Variable value = Variable::ParseFloat(_pExpressionTokenSource->GetCurrentToken());
			_pExpressionTokenSource->Advance();
            RETURN(value);
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
						pVariable->SetValue(0, pVariable->GetValueFloat(0) + 1);
						_pExpressionTokenSource->Advance();
					}
					else if (_pExpressionTokenSource->EqualTo("--"))
					{
						pVariable->SetValue(0, pVariable->GetValueFloat(0) - 1);
						_pExpressionTokenSource->Advance();
					}

                    RETURN(returnValue);
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

                    RETURN(returnValue);
				}
			}
		}

		return Variable::Empty();
	}

	Variable EvaluateString()
	{
        PROLOGUE;

        //Serial.println("EvaluateString");
        //Serial.println(_pExpressionTokenSource->GetCurrentToken());
        
        if (_pExpressionTokenSource->FirstChar() == '"')
        {
			Variable value;
			value.SetValue(_pExpressionTokenSource->GetCurrentToken() + 1);

			_pExpressionTokenSource->Advance();

			return value;
		}

		Variable value = EvaluateExpression();
        RETURN(value);
	}

	Variable EvaluateParentheses()
	{
        PROLOGUE;
			
		if (_pExpressionTokenSource->EqualTo("("))
		{
			_pExpressionTokenSource->Advance();

			Variable value = EvaluateTop();

			if (!_pExpressionTokenSource->EqualTo(")"))
			{
				ReportError("Missing ) in expression", "");

				return Variable::Empty();
			}
			_pExpressionTokenSource->Advance(); // eat closing paren...

			return value;
		}

		Variable value = EvaluateString();
        RETURN(value);
	}

    void RemoveUndefinedSentinel(const char* pVariableName)
    {
        if (strlen(pVariableName) != 0)
        {
            SafeString::StringCopy(_temporaryBuffer, "$", sizeof(_temporaryBuffer));
            SafeString::StringCat(_temporaryBuffer, pVariableName, sizeof(_temporaryBuffer));
            _pExecutionContext->DeleteVariable(_temporaryBuffer);
        }
    }

    bool ValidateHasValue(Variable* pVariable)
    {
        if (*pVariable->GetValueString() != '\0' || !pVariable->IsNan())
        {
            return true;
        }

        ReportError("Undefined variable: ", pVariable->GetVariableName());
        RemoveUndefinedSentinel(pVariable->GetVariableName());

        return false;
    }

	Variable EvaluateMultiValueNumber()
	{
        PROLOGUE;
			
		if (_pExpressionTokenSource->EqualTo("{"))
		{
			_pExpressionTokenSource->Advance();

			Variable first = EvaluateTop();
            if (!(ValidateHasValue(&first)))
            {
                return Variable::Empty();
            }

			while (_pExpressionTokenSource->EqualTo(","))
			{
				_pExpressionTokenSource->Advance();

				Variable next = EvaluateTop();
                if (!(ValidateHasValue(&next)))
                {
                    return Variable::Empty();
                }

				first.SetValue(first.GetValueCount(), next.GetValueFloat(0));	// accumulate to first value
			}

			if (!_pExpressionTokenSource->EqualTo("}"))
			{
				ReportError("Missing } in expression", "");

				return Variable::Empty();
			}
			_pExpressionTokenSource->Advance(); // eat closing brace...

			return first;
		}
		
		Variable value = EvaluateParentheses();
        RETURN(value);
	}


	Variable EvaluateUnary()
	{
        PROLOGUE;
			
        Variable value;
        if (_pExpressionTokenSource->EqualTo("+") || _pExpressionTokenSource->EqualTo("-"))
		{
			char op = _pExpressionTokenSource->FirstChar();

			_pExpressionTokenSource->Advance();
			value = EvaluateUnary();
            if (!(ValidateHasValue(&value)))
            {
                return Variable::Empty();
            }

			switch (op)
			{
			case '-':
                value = -value.GetValueFloat(0);

			case '+':
                break;
			}
            
		}
        else
        {
            value = EvaluateMultiValueNumber();
        }
        RETURN(value);
	}

	Variable EvaluateMultiplicative()
	{
        PROLOGUE;

		Variable left = EvaluateUnary();

		while (_pExpressionTokenSource->EqualTo("*") || _pExpressionTokenSource->EqualTo("/") || _pExpressionTokenSource->EqualTo("%"))
		{
            if (!(ValidateHasValue(&left)))
            {
                return Variable::Empty();
            }

			char op = _pExpressionTokenSource->FirstChar();

			_pExpressionTokenSource->Advance();
			Variable right = EvaluateUnary();
            if (!(ValidateHasValue(&right)))
            {
                return Variable::Empty();
            }

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

        RETURN(left);
	}

	Variable EvaluateAdditive()
	{
        PROLOGUE;

        Variable left = EvaluateMultiplicative();

		while (_pExpressionTokenSource->EqualTo("+") || _pExpressionTokenSource->EqualTo("-"))
		{
            if (!(ValidateHasValue(&left)))
            {
                return Variable::Empty();
            }

			char op = _pExpressionTokenSource->FirstChar();
			_pExpressionTokenSource->Advance();
			Variable right = EvaluateMultiplicative();
            if (!(ValidateHasValue(&right)))
            {
                return Variable::Empty();
            }

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

        RETURN(left);
	}


	Variable EvaluateRelational()
	{
        PROLOGUE;

		Variable left = EvaluateAdditive();

		while (_pExpressionTokenSource->EqualTo("<") || _pExpressionTokenSource->EqualTo("<=") || 
			   _pExpressionTokenSource->EqualTo(">") || _pExpressionTokenSource->EqualTo(">="))
		{
            if (!(ValidateHasValue(&left)))
            {
                return Variable::Empty();
            }

			char op = _pExpressionTokenSource->FirstChar();
			char andEqual = _pExpressionTokenSource->SecondChar() == '=';

			_pExpressionTokenSource->Advance();
			Variable right = EvaluateAdditive();
            if (!(ValidateHasValue(&right)))
            {
                return Variable::Empty();
            }

			if (op == '<')
			{
				left = andEqual ? Variable(left.GetValueFloat(0) <= right.GetValueFloat(0)) : Variable(left.GetValueFloat(0) < right.GetValueFloat(0));
			}
			else if (op == '>')
			{
				left = andEqual ? Variable(left.GetValueFloat(0) >= right.GetValueFloat(0)) : Variable(left.GetValueFloat(0) > right.GetValueFloat(0));
			}
		}

        RETURN(left);
	}

	Variable EvaluateEquality()
	{
        PROLOGUE;

		Variable left = EvaluateRelational();

		while (_pExpressionTokenSource->EqualTo("==") || _pExpressionTokenSource->EqualTo("!="))
		{
            if (!(ValidateHasValue(&left)))
            {
                return Variable::Empty();
            }
			char op = _pExpressionTokenSource->FirstChar();

			_pExpressionTokenSource->Advance();
			Variable right = EvaluateRelational();
            if (!(ValidateHasValue(&right)))
            {
                return Variable::Empty();
            }

			if (op == '=')
			{
				left = Variable(left.GetValueFloat(0) == right.GetValueFloat(0));
			}
			else if (op == '!')
			{
				left = Variable(left.GetValueFloat(0) != right.GetValueFloat(0));
			}
		}

        RETURN(left);
	}

	Variable EvaluateLogicalAnd()
	{
        PROLOGUE;

		Variable left = EvaluateEquality();

		while (_pExpressionTokenSource->EqualTo("&&"))
		{
            if (!(ValidateHasValue(&left)))
            {
                return Variable::Empty();
            }

			_pExpressionTokenSource->Advance();
			Variable right = EvaluateEquality();
            if (!(ValidateHasValue(&right)))
            {
                return Variable::Empty();
            }

			left = Variable(left.GetValueFloat(0) && right.GetValueFloat(0));
		}

        RETURN(left);
	}

	Variable EvaluateLogicalOr()
	{
        PROLOGUE;

		Variable left = EvaluateLogicalAnd();

		while (_pExpressionTokenSource->EqualTo("||"))
		{
            if (!(ValidateHasValue(&left)))
            {
                return Variable::Empty();
            }
			_pExpressionTokenSource->Advance();
			Variable right = EvaluateLogicalAnd();
            if (!(ValidateHasValue(&right)))
            {
                return Variable::Empty();
            }

			left = Variable(left.GetValueFloat(0) || right.GetValueFloat(0));
		}

        RETURN(left);
	}

	Variable EvaluateAssignment()
	{
        PROLOGUE;

		Variable left = EvaluateLogicalOr();

		if (_pExpressionTokenSource->EqualTo("="))
		{
			_pExpressionTokenSource->Advance();
			Variable right = EvaluateLogicalOr();
            if (!(ValidateHasValue(&right)))
            {
                return Variable::Empty();
            }

			Variable* pDestination = _pExecutionContext->GetVariableWithoutErrorCheck(left.GetVariableName());
			if (!pDestination)
			{
				// new variable. 
				Variable futureValue = Variable::Empty();
				_pExecutionContext->AddVariableAndSet(left.GetVariableName(), &futureValue);
				pDestination = _pExecutionContext->GetVariableWithoutErrorCheck(left.GetVariableName());

                RemoveUndefinedSentinel(left.GetVariableName());
			}

			for (int i = 0; i < right.GetValueCount(); i++)
			{
				pDestination->SetValue(i, right.GetValueFloat(i));
			}

			left.SetToNan();	// assignments are not l-values in this language to prevent "IF TEST = 5"
		}

        RETURN(left);
	}

	Variable EvaluateEmpty()
	{
        PROLOGUE;
			 
		RETURN(EvaluateAssignment());
	}

	Variable EvaluateTop()
	{
        PROLOGUE;

		if (_pExpressionTokenSource->AtEnd())
		{
			RETURN(Variable::Empty());
		}

        Variable value = EvaluateEmpty();

		RETURN(value);
	}

    void HandleIf(bool active)
    {
        PROLOGUE;

        // Just skip until we find endif
        if (!active)
        {
            while (!_pExpressionTokenSource->AtEnd())
            {
                _pExpressionTokenSource->AdvanceToNewLine();

                if (_pExpressionTokenSource->EqualTo("IF"))
                {
                    HandleIf(false);
                }
                else if (_pExpressionTokenSource->EqualTo("ENDIF"))
                {
                    _pExpressionTokenSource->AdvanceToNewLine();
                    return;
                }
            }

            ReportError("Missing ENDIF", "");
        }

        // we are on an if statement that is active. Evaluate it, and decide what to do next...
        bool conditionMatched = false;
        bool executing = false;

        _pExpressionTokenSource->Advance();
        Variable condition = EvaluateTop();

        if (condition.GetValueInt() != 0)
        {
            conditionMatched = true;
            executing = true;
        }
        _pExpressionTokenSource->AdvanceToNewLine();

        while (!_pExpressionTokenSource->AtEnd())
        {
            if (_pExpressionTokenSource->EqualTo("IF"))
            {
                HandleIf(executing);
            }
            else if (_pExpressionTokenSource->EqualTo("ELSEIF"))
            {
                executing = false;

                _pExpressionTokenSource->Advance();

                if (conditionMatched == false)
                {
                    Variable condition = EvaluateTop();

                    if (condition.GetValueInt() != 0)
                    {
                        conditionMatched = true;
                        executing = true;
                    }
                }

                _pExpressionTokenSource->AdvanceToNewLine();
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
                EPILOGUE;
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

        ReportError("Missing ENDIF", "");

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
        PROLOGUE;

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
            RemoveUndefinedSentinel(identifier.GetVariableName());

            Variable* pLoopVariable = _pExecutionContext->GetVariableWithoutErrorCheck(identifier.GetVariableName());
            _pExpressionTokenSource->AdvanceToNewLine();

            int firstLineOfForLoop = _pExpressionTokenSource->GetParseLocation();

            bool breaking = false; 
            while (!_pExpressionTokenSource->AtEnd())
            {
                if (_pExpressionTokenSource->EqualTo("ENDFOR"))
                {
                    if (IsAborting("FOR", -2) && !breaking)
                    {
                        return false;
                    }

                    pLoopVariable->Increment(increment);

                    if (!GetIsInRange(startValue.GetValueFloat(0), endValue.GetValueFloat(0), pLoopVariable->GetValueFloat(0)) || breaking)
                    {
                        _pExecutionFlow->ClearBreak();
                        _pExecutionContext->DeleteVariable(identifier.GetVariableName());
                        _pExpressionTokenSource->AdvanceToNewLine();

                        RETURN(true);
                    }

                    _pExpressionTokenSource->SetParseLocation(firstLineOfForLoop);
                }
                else
                {
                    if (!breaking)
                    {
                        EvaluateStatement();
                        if (_pExecutionFlow->IsBreaking())
                        {
                            breaking = true;
                        }
                        else if (IsAborting("FOR", -2))
                        {
                            return false;
                        }
                    }
                    else
                    {
                        _pExpressionTokenSource->AdvanceToNewLine();
                    }
                }
            }

            ReportError("Missing ENDFOR", "");

            return true;

        }
        return false;
    }

    bool HandleFunctionDefinition()
    {
        PROLOGUE;

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

                    RETURN(true);
                }
                else if (_pExpressionTokenSource->EqualTo("FUNC"))
                {
                    ReportError("Missing ENDFUNC for function: ", identifier);
                    return true;
                }
                else
                {
                    _pExpressionTokenSource->AdvanceToNewLine();
                }
            }

            ReportError("Missing ENDFUNC for function: ", identifier);
            return true;

        }
        RETURN(false);
    }

    bool HandleReturn()
    {
        PROLOGUE;

        if (_pExpressionTokenSource->EqualTo("RETURN"))
        {
            _pExpressionTokenSource->Advance();
            Variable returnValue = EvaluateTop();

            if (returnValue.GetValueCount() == 0)
            {
                ReportError("Missing value in RETURN statement", "");
                returnValue = Variable::Empty();
            }

            _pExecutionContext->AddVariableAndSet("<ReturnValue>", &returnValue);

            RETURN(true);
        }
        RETURN(false);
    }

    Variable EvaluateStatement()
    {
        PROLOGUE;

        if (DebugFlags.LogStatements)
        {
            Serial.println(_pExpressionTokenSource->GetCurrentToken());
        }

        Variable lastValue;
        lastValue.SetToNan();
        if (_pExpressionTokenSource->EqualTo("IF"))
        {
            HandleIf(true);
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
        else if (_pExpressionTokenSource->EqualTo("BREAK"))
        {
            _pExecutionFlow->BreakExecution();
            _pExpressionTokenSource->AdvanceToNewLine();
        }
        else
        {
            lastValue = EvaluateTop();

            if (!_pExpressionTokenSource->AtEnd() != 0 && !_pExpressionTokenSource->EqualTo("\n"))
            {
                ReportError("Unexpected token remaining after parsing: ", _pExpressionTokenSource->GetCurrentToken());
                _pExpressionTokenSource->AdvanceToNewLine();
            }
        }

        if (_pExpressionTokenSource->EqualTo("\n"))
        {
            _pExpressionTokenSource->Advance();
        }

        RETURN(lastValue);
    }

    Variable EvaluateStatements()
    {
        PROLOGUE;

        Variable lastValue;
        lastValue.SetToNan();

        while (!_pExpressionTokenSource->AtEnd())
        {
            lastValue = EvaluateStatement();
            if (IsAborting("STATEMENT", -3))
            {
                return false;
            }
        }

        RETURN(lastValue);
    }
	
public:

    Variable EvaluateInExistingParse(ExpressionTokenSource* pExpressionTokenSource, IExecutionContext* pExecutionContext = 0, ParseErrors* pParseErrors = 0, IExecutionFlow* pExecutionFlow = 0)
    {

        _pExpressionTokenSource = pExpressionTokenSource;
        _pExecutionContext = pExecutionContext;
        //_pFunctionCaller = pFunctionCaller;
        _pParseErrors = pParseErrors;
        _pExecutionFlow = pExecutionFlow;

        FunctionTimesInstance.Clear();

        Variable value = EvaluateStatements();
        FunctionTimesInstance.Dump();

        return value;
    }


	Variable Evaluate(const char* pExpression, IExecutionContext* pExecutionContext = 0, ParseErrors* pParseErrors = 0, IExecutionFlow* pExecutionFlow = 0)
	{
        PROLOGUE;

        ExpressionTokenSource expressionTokenSource(pExpression, pParseErrors);
        Variable value = EvaluateInExistingParse(&expressionTokenSource, pExecutionContext, pParseErrors, pExecutionFlow);

        if (IsAborting(0))
        {
            RETURN(value);
        }

		if (!_pExpressionTokenSource->AtEnd())
		{
			ReportError("Unexpected token remaining after parsing: ", _pExpressionTokenSource->GetCurrentToken(), -1);
		}

		// Check for undefined sentinel variables. 
		if (pExecutionContext)
		{

			for (int i = 0; i <_pExecutionContext->Variables()->GetActiveVariableCount(); i++)
			{
				Variable* pVariable = _pExecutionContext->Variables()->Get(i);

				if (*pVariable->GetVariableName() == '$' && pVariable->GetStackLevel() == _pExecutionContext->GetStack()->GetFrameCount())
				{
					ReportError("Undefined variable: ", pVariable->GetVariableName() + 1, -1);
				}
			}
		}

		RETURN(value);
	}
};
