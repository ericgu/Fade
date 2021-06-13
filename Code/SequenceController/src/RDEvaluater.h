

class RDEvaluater
{
	ExpressionTokenSource* _pExpressionTokenSource;
	IExecutionContext* _pExecutionContext;
	//ifunctionCaller* _pFunctionCaller;
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
        if (_pExecutionFlow != nullptr && _pExecutionFlow->IsAborting())
        {
            if (pAbortingMessage != nullptr)
            {
                ReportError("Aborting: ", pAbortingMessage, lineNumber);
            }
            return true;
        }

        if (_pExecutionFlow != nullptr && _pExecutionFlow->IsBreaking())
        {
            return true;
        }

        if (_pParseErrors != nullptr &&_pParseErrors->GetErrorCount() != 0)
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

      ExpressionResult value = EvaluateTop();
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

      _pExecutionContext->Variables()->AddAndSet(GenerateArgumentName(argumentNumber), &value._variable, _pExecutionContext->GetStack()->GetFrameCount() + 1); // put into frame that will be created soon...

      RETURN(true);
    }

    // recursively evaluate function arguments. On the way out of the function we store the arguments. This prevents collisions when the function
    // argument evaluation requires function calls themselves. 
    bool AssignFunctionParameters(const char* pFunctionName, int parameterNumber)
    {
        PROLOGUE;

            if (_pExpressionTokenSource->AtEnd() || _pExpressionTokenSource->FirstChar() == '\n')
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

        Variable parameterName = EvaluateTop()._variable;

        Variable* pArgument = _pExecutionContext->GetVariableWithoutErrorCheck(GenerateArgumentName(parameterNumber));
        if (pArgument == nullptr)
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

    ExpressionResult HandleFunctionCall(const char* pFunctionName)
    {
        PROLOGUE;
            
        ExpressionResult expressionResult = ExpressionResult::Empty();

        FunctionDefinition* pFunctionDefinition = _pExecutionContext->Functions()->Lookup(pFunctionName);
        if (pFunctionDefinition != nullptr)
        {
            // save current location to return to it after the function call. 
            int parseLocation = _pExpressionTokenSource->GetParseLocation();
            _pExpressionTokenSource->SetParseLocation(pFunctionDefinition->LineNumberStart); // point to function...

            // rename variables...

            if (_pExpressionTokenSource->EqualTo("func"))
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
                return expressionResult;
            }

            _pExpressionTokenSource->AdvanceToNewLine();

            while (!_pExpressionTokenSource->AtEnd())
            {
                if (_pExpressionTokenSource->EqualTo("endfunc"))
                {
                    _pExpressionTokenSource->SetParseLocation(parseLocation);

                    Variable* pReturnValue = _pExecutionContext->GetVariableWithoutErrorCheck("<ReturnValue>");
                    if (pReturnValue)
                    {
                      expressionResult._variable = *pReturnValue;
                    }

                    RETURN(expressionResult);
                }
                else
                {
                    EvaluateStatement();

                    if (IsAborting("Function"))
                    {
                        return expressionResult;
                    }
                }
            }

            // error case; missing endfunc


        }
        else if (!BuiltInFunctions::HandleBuiltInFunctions(pFunctionName, _pExecutionContext, _pParseErrors, _pExpressionTokenSource, _pExecutionFlow, &expressionResult))
        {
            ReportError("Unrecognized function: ", pFunctionName);

            return expressionResult;
        }

        return expressionResult;
    }


    ExpressionResult EvaluateFunctionCall(const char* functionName)
    {
        PROLOGUE;
            
        _pExpressionTokenSource->Advance();

        bool argumentParseSuccess = EvaluateFunctionArguments(0);

        if (argumentParseSuccess)
        {
            _pExecutionContext->GetStack()->CreateFrame();

            ExpressionResult expressionResult = HandleFunctionCall(functionName);

            _pExecutionContext->Variables()->DeleteStackLevel(_pExecutionContext->GetStack()->GetFrameCount());
            _pExecutionContext->GetStack()->DestroyFrame();

            RETURN(expressionResult);
        }
        else
        {
          return ExpressionResult::Empty();
        }
    }

    ExpressionResult EvaluateExpression()
    {
      PROLOGUE;

      if (_pExpressionTokenSource->AtEnd())
      {
        ReportError("Missing value at end of expression", "");
        return ExpressionResult::Empty();
      }

      if (_pExpressionTokenSource->IsNumber())
      {
        ExpressionResult expressionResult;
        expressionResult._variable = Variable::ParseFloat(_pExpressionTokenSource->GetCurrentToken());
        _pExpressionTokenSource->Advance();
        RETURN(expressionResult);
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
            Variable currentValue = *pVariable;
            Variable index;
            bool arrayReference = false;

            if (_pExpressionTokenSource->EqualTo("["))
            {
              _pExpressionTokenSource->Advance();
              index = EvaluateTop()._variable;

              if (!_pExpressionTokenSource->EqualTo("]"))
              {
                  ReportError("Missing closing bracket", "");
                  return ExpressionResult::Empty();
              }
              _pExpressionTokenSource->Advance();
              arrayReference = true;
            }

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

            ExpressionResult expressionResult;
            Variable returnValue;
            if (arrayReference)
            {
              returnValue = currentValue.GetValueFloat(index.GetValueInt());
              expressionResult._arrayIndex = index.GetValueInt();
              returnValue.SetVariableName(currentValue.GetVariableName());
            }
            else
            {
                returnValue = currentValue;
            }

            expressionResult._variable = returnValue;
            RETURN(expressionResult);
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

            Variable index;
            bool arrayReference = false;

            if (_pExpressionTokenSource->EqualTo("["))
            {
                _pExpressionTokenSource->Advance();
                index = EvaluateTop()._variable;

                if (!_pExpressionTokenSource->EqualTo("]"))
                {
                    ReportError("Missing closing bracket", "");
                    return ExpressionResult::Empty();
                }
                _pExpressionTokenSource->Advance();
                arrayReference = true;
            }

            ExpressionResult expressionResult;
            expressionResult._variable.SetToNan();
            expressionResult._variable.SetVariableName(identifier);

            if (arrayReference)
            {
                expressionResult._arrayIndex = index.GetValueInt();
            }

            RETURN(expressionResult);
          }
        }
      }

      RETURN(ExpressionResult::Empty());
    }

    ExpressionResult EvaluateString()
    {
      PROLOGUE;

      //Serial.println("EvaluateString");
      //Serial.println(_pExpressionTokenSource->GetCurrentToken());

      if (_pExpressionTokenSource->FirstChar() == '"')
      {
        ExpressionResult expressionResult;
        expressionResult._variable.SetValue(_pExpressionTokenSource->GetCurrentToken() + 1);

        _pExpressionTokenSource->Advance();

        return expressionResult;
      }

      ExpressionResult expressionResult = EvaluateExpression();
      RETURN(expressionResult);
    }

    ExpressionResult EvaluateParentheses()
    {
      PROLOGUE;

      if (_pExpressionTokenSource->EqualTo("("))
      {
        _pExpressionTokenSource->Advance();

        Variable value = EvaluateTop()._variable;

        if (!_pExpressionTokenSource->EqualTo(")"))
        {
          ReportError("Missing ) in expression", "");

          return ExpressionResult::Empty();
        }
        _pExpressionTokenSource->Advance(); // eat closing paren...

        ExpressionResult expressionResult;
        expressionResult._variable = value;
        return expressionResult;
      }
      RETURN(EvaluateString());
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

    bool ValidateHasValue(ExpressionResult* pExpressionResult)
    {
        if (*pExpressionResult->_variable.GetValueString() != '\0' || !pExpressionResult->_variable.IsNan())
        {
            return true;
        }

        ReportError("Undefined variable: ", pExpressionResult->_variable.GetVariableName());
        RemoveUndefinedSentinel(pExpressionResult->_variable.GetVariableName());

        return false;
    }

    ExpressionResult EvaluateMultiValueNumber()
    {
      PROLOGUE;

      if (_pExpressionTokenSource->EqualTo("{"))
      {
        _pExpressionTokenSource->Advance();

        ExpressionResult first = EvaluateTop();
        if (!(ValidateHasValue(&first)))
        {
          return ExpressionResult::Empty();
        }

        while (_pExpressionTokenSource->EqualTo(","))
        {
          _pExpressionTokenSource->Advance();

          ExpressionResult next = EvaluateTop();
          if (!(ValidateHasValue(&next)))
          {
            return ExpressionResult::Empty();
          }

          first._variable.SetValue(first._variable.GetValueCount(), next._variable.GetValueFloat(0));	// accumulate to first value
        }

        if (!_pExpressionTokenSource->EqualTo("}"))
        {
          ReportError("Missing } in expression", "");

          return ExpressionResult::Empty();
        }
        _pExpressionTokenSource->Advance(); // eat closing brace...

        ExpressionResult expressionResult;
        expressionResult._variable = first._variable;
        return expressionResult;
      }

      RETURN(EvaluateParentheses());
    }


    ExpressionResult EvaluateUnary()
    {
      PROLOGUE;

      ExpressionResult value;
      if (_pExpressionTokenSource->EqualTo("+") || _pExpressionTokenSource->EqualTo("-"))
      {
        char op = _pExpressionTokenSource->FirstChar();

        _pExpressionTokenSource->Advance();
        value = EvaluateUnary();
        if (!(ValidateHasValue(&value)))
        {
          return ExpressionResult::Empty();
        }

        switch (op)
        {
        case '-':
          value._variable = -value._variable.GetValueFloat(0);

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

    ExpressionResult EvaluateMultiplicative()
    {
      PROLOGUE;

      ExpressionResult left = EvaluateUnary();

      while (_pExpressionTokenSource->EqualTo("*") || _pExpressionTokenSource->EqualTo("/") || _pExpressionTokenSource->EqualTo("%"))
      {
        if (!(ValidateHasValue(&left)))
        {
          return ExpressionResult::Empty();
        }

        char op = _pExpressionTokenSource->FirstChar();

        _pExpressionTokenSource->Advance();
        ExpressionResult right = EvaluateUnary();
        if (!(ValidateHasValue(&right)))
        {
          return ExpressionResult::Empty();
        }

        switch (op)
        {
        case '*':
          left._variable = Variable(left._variable.GetValueFloat(0) * right._variable.GetValueFloat(0));	// NOTE: how to handle multi-values?
          break;

        case '/':
          left._variable = Variable(left._variable.GetValueFloat(0) / right._variable.GetValueFloat(0));	// NOTE: how to handle multi-values?
          break;

        case '%':
          left._variable = Variable(left._variable.GetValueInt() % right._variable.GetValueInt());
          break;
        }
      }

      RETURN(left);
    }

    ExpressionResult EvaluateAdditive()
    {
      PROLOGUE;

      ExpressionResult left = EvaluateMultiplicative();

      while (_pExpressionTokenSource->EqualTo("+") || _pExpressionTokenSource->EqualTo("-"))
      {
        if (!(ValidateHasValue(&left)))
        {
          return ExpressionResult::Empty();
        }

        char op = _pExpressionTokenSource->FirstChar();
        _pExpressionTokenSource->Advance();
        ExpressionResult right = EvaluateMultiplicative();
        if (!(ValidateHasValue(&right)))
        {
          return ExpressionResult::Empty();
        }

        switch (op)
        {
        case '+':
          left._variable = Variable(left._variable.GetValueFloat(0) + right._variable.GetValueFloat(0));	// NOTE: how to handle multi-values?
          break;

        case '-':
          left._variable = Variable(left._variable.GetValueFloat(0) - right._variable.GetValueFloat(0));	// NOTE: how to handle multi-values?
          break;
        }
      }

      RETURN(left);
    }


    ExpressionResult EvaluateRelational()
    {
      PROLOGUE;

      ExpressionResult left = EvaluateAdditive();

      while (_pExpressionTokenSource->EqualTo("<") || _pExpressionTokenSource->EqualTo("<=") ||
        _pExpressionTokenSource->EqualTo(">") || _pExpressionTokenSource->EqualTo(">="))
      {
        if (!(ValidateHasValue(&left)))
        {
          return ExpressionResult::Empty();
        }

        char op = _pExpressionTokenSource->FirstChar();
        char andEqual = _pExpressionTokenSource->SecondChar() == '=';

        _pExpressionTokenSource->Advance();
        ExpressionResult right = EvaluateAdditive();
        if (!(ValidateHasValue(&right)))
        {
          return ExpressionResult::Empty();
        }

        if (op == '<')
        {
          left._variable = andEqual ? Variable(left._variable.GetValueFloat(0) <= right._variable.GetValueFloat(0)) : Variable(left._variable.GetValueFloat(0) < right._variable.GetValueFloat(0));
        }
        else if (op == '>')
        {
          left._variable = andEqual ? Variable(left._variable.GetValueFloat(0) >= right._variable.GetValueFloat(0)) : Variable(left._variable.GetValueFloat(0) > right._variable.GetValueFloat(0));
        }
      }

      RETURN(left);
    }

    ExpressionResult EvaluateEquality()
    {
      PROLOGUE;

      ExpressionResult left = EvaluateRelational();

      while (_pExpressionTokenSource->EqualTo("==") || _pExpressionTokenSource->EqualTo("!="))
      {
        if (!(ValidateHasValue(&left)))
        {
          return ExpressionResult::Empty();
        }
        char op = _pExpressionTokenSource->FirstChar();

        _pExpressionTokenSource->Advance();
        ExpressionResult right = EvaluateRelational();
        if (!(ValidateHasValue(&right)))
        {
          return ExpressionResult::Empty();
        }

        if (op == '=')
        {
          left._variable = Variable(left._variable.GetValueFloat(0) == right._variable.GetValueFloat(0));
        }
        else if (op == '!')
        {
          left._variable = Variable(left._variable.GetValueFloat(0) != right._variable.GetValueFloat(0));
        }
      }

      RETURN(left);
    }

    ExpressionResult EvaluateLogicalAnd()
    {
      PROLOGUE;

      ExpressionResult left = EvaluateEquality();

      while (_pExpressionTokenSource->EqualTo("&&"))
      {
        if (!(ValidateHasValue(&left)))
        {
          return ExpressionResult::Empty();
        }

        _pExpressionTokenSource->Advance();
        ExpressionResult right = EvaluateEquality();
        if (!(ValidateHasValue(&right)))
        {
          return ExpressionResult::Empty();
        }

        left._variable = Variable(left._variable.GetValueFloat(0) && right._variable.GetValueFloat(0));
      }

      RETURN(left);
    }

    ExpressionResult EvaluateLogicalOr()
    {
      PROLOGUE;

      ExpressionResult left = EvaluateLogicalAnd();

      while (_pExpressionTokenSource->EqualTo("||"))
      {
        if (!(ValidateHasValue(&left)))
        {
          return ExpressionResult::Empty();
        }
        _pExpressionTokenSource->Advance();
        ExpressionResult right = EvaluateLogicalAnd();
        if (!(ValidateHasValue(&right)))
        {
          return ExpressionResult::Empty();
        }

        left._variable = Variable(left._variable.GetValueFloat(0) || right._variable.GetValueFloat(0));
      }

      RETURN(left);
    }

    ExpressionResult EvaluateAssignment()
    {
      PROLOGUE;

      ExpressionResult left = EvaluateLogicalOr();

      if (_pExpressionTokenSource->EqualTo("="))
      {
        _pExpressionTokenSource->Advance();
        ExpressionResult right = EvaluateLogicalOr();
        if (!(ValidateHasValue(&right)))
        {
          return ExpressionResult::Empty();
        }

        Variable* pDestination = _pExecutionContext->GetVariableWithoutErrorCheck(left._variable.GetVariableName());
        if (!pDestination)
        {
          // new variable. 
          Variable futureValue = Variable::Empty();
          _pExecutionContext->AddVariableAndSet(left._variable.GetVariableName(), &futureValue);
          pDestination = _pExecutionContext->GetVariableWithoutErrorCheck(left._variable.GetVariableName());

          RemoveUndefinedSentinel(left._variable.GetVariableName());
        }

        if (left._arrayIndex != -1)
        {
            pDestination->SetValue(left._arrayIndex, right._variable.GetValueFloat(0));
        }
        else
        {
            for (int i = 0; i < right._variable.GetValueCount(); i++)
            {
                pDestination->SetValue(i, right._variable.GetValueFloat(i));
            }
        }

        left._variable.SetToNan();	// assignments are not l-values in this language to prevent "if TEST = 5"
      }

      RETURN(left);
    }

    ExpressionResult EvaluateEmpty()
    {
      PROLOGUE;

      RETURN(EvaluateAssignment());
    }

    ExpressionResult EvaluateTop()
    {
      PROLOGUE;

      if (_pExpressionTokenSource->AtEnd())
      {
        RETURN(ExpressionResult::Empty());
      }

      ExpressionResult expressionResult = EvaluateEmpty();

      RETURN(expressionResult);
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

                if (_pExpressionTokenSource->EqualTo("if"))
                {
                    HandleIf(false);
                }
                else if (_pExpressionTokenSource->EqualTo("endif"))
                {
                    _pExpressionTokenSource->AdvanceToNewLine();
                    return;
                }
            }

            ReportError("Missing endif", "");
        }

        // we are on an if statement that is active. Evaluate it, and decide what to do next...
        bool conditionMatched = false;
        bool executing = false;

        _pExpressionTokenSource->Advance();
        Variable condition = EvaluateTop()._variable;

        if (!condition.IsNan() && condition.GetValueInt() != 0)
        {
            conditionMatched = true;
            executing = true;
        }
        _pExpressionTokenSource->AdvanceToNewLine();

        while (!_pExpressionTokenSource->AtEnd())
        {
            if (_pExpressionTokenSource->EqualTo("if"))
            {
                HandleIf(executing);
            }
            else if (_pExpressionTokenSource->EqualTo("elseif"))
            {
                executing = false;

                _pExpressionTokenSource->Advance();

                if (conditionMatched == false)
                {
                    Variable condition = EvaluateTop()._variable;

                    if (condition.GetValueInt() != 0)
                    {
                        conditionMatched = true;
                        executing = true;
                    }
                }

                _pExpressionTokenSource->AdvanceToNewLine();
            }
            else if (_pExpressionTokenSource->EqualTo("else"))
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
            else if (_pExpressionTokenSource->EqualTo("endif"))
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

        ReportError("Missing endif", "");

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

        if (_pExpressionTokenSource->EqualTo("for"))
        {
            _pExpressionTokenSource->Advance();

            Variable identifier = EvaluateTop()._variable;    // get loop control variable name

            Variable startValue = EvaluateTop()._variable;

            //expressionTokenSource.Advance();
            if (_pExpressionTokenSource->FirstChar() != ':')
            {
                // expected ":" error...
            }
            _pExpressionTokenSource->Advance();

            Variable endValue = EvaluateTop()._variable;

            Variable increment(1.0F);
            if (_pExpressionTokenSource->FirstChar() == ':')
            {
                _pExpressionTokenSource->Advance();
                increment = EvaluateTop()._variable;
            }

            if (!_pExpressionTokenSource->AtEnd() && _pExpressionTokenSource->FirstChar() != '\n')
            {
                // error - unexpected token at the end of for statement
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
                if (_pExpressionTokenSource->EqualTo("endfor"))
                {
                    if (IsAborting("for", -2) && !breaking)
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
                        else if (IsAborting("for", -2))
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

            ReportError("Missing endfor", "");

            return true;

        }
        return false;
    }

    bool HandleFunctionDefinition()
    {
        PROLOGUE;

        if (_pExpressionTokenSource->EqualTo("func"))
        {
            int startOfFunction = _pExpressionTokenSource->GetParseLocation();

            _pExpressionTokenSource->Advance();

            char identifier[128];
            SafeString::StringCopy(identifier, _pExpressionTokenSource->GetCurrentToken(), sizeof(identifier));

            _pExpressionTokenSource->AdvanceToNewLine();

            while (!_pExpressionTokenSource->AtEnd())
            {
                if (_pExpressionTokenSource->EqualTo("endfunc"))
                {
                    FunctionDefinition* pFunctionDefinition = _pExecutionContext->Functions()->Lookup(identifier);
                    
                    if (pFunctionDefinition == nullptr)
                    {
                        _pExecutionContext->Functions()->DefineStart(identifier, _pParseErrors, startOfFunction);
                    }

                    _pExpressionTokenSource->AdvanceToNewLine();

                    RETURN(true);
                }
                else if (_pExpressionTokenSource->EqualTo("func"))
                {
                    ReportError("Missing endfunc for function: ", identifier);
                    return true;
                }
                else
                {
                    _pExpressionTokenSource->AdvanceToNewLine();
                }
            }

            ReportError("Missing endfunc for function: ", identifier);
            return true;

        }
        RETURN(false);
    }

    bool HandleReturn()
    {
        PROLOGUE;

        if (_pExpressionTokenSource->EqualTo("return"))
        {
            _pExpressionTokenSource->Advance();
            Variable returnValue = EvaluateTop()._variable;

            if (returnValue.GetValueCount() == 0)
            {
                ReportError("Missing value in return statement", "");
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

        if (Environment.DebugLogStatements)
        {
            Serial.println(_pExpressionTokenSource->GetCurrentToken());
        }

        Variable lastValue;
        lastValue.SetToNan();
        if (_pExpressionTokenSource->EqualTo("if"))
        {
            HandleIf(true);
        }
        else if (_pExpressionTokenSource->EqualTo("for"))
        {
            HandleFor();
        }
        else if (_pExpressionTokenSource->EqualTo("func"))
        {
            HandleFunctionDefinition();
        }
        else if (_pExpressionTokenSource->EqualTo("return"))
        {
            HandleReturn();
        }
        else if (_pExpressionTokenSource->EqualTo("break"))
        {
            _pExecutionFlow->BreakExecution();
            _pExpressionTokenSource->AdvanceToNewLine();
        }
        else
        {
            lastValue = EvaluateTop()._variable;

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

    Variable EvaluateInExistingParse(ExpressionTokenSource* pExpressionTokenSource, IExecutionContext* pExecutionContext = nullptr, ParseErrors* pParseErrors = nullptr, IExecutionFlow* pExecutionFlow = nullptr)
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


	Variable Evaluate(const char* pExpression, IExecutionContext* pExecutionContext = nullptr, ParseErrors* pParseErrors = nullptr, IExecutionFlow* pExecutionFlow = nullptr)
	{
        PROLOGUE;

        ExpressionTokenSource expressionTokenSource(pExpression, pParseErrors);
        Variable value = EvaluateInExistingParse(&expressionTokenSource, pExecutionContext, pParseErrors, pExecutionFlow);

        if (IsAborting(nullptr))
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
