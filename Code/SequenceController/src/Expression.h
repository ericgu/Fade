typedef Variable(*FunctionCallHandler)(FunctionDefinition* pFunctionDefinition, VariableCollection* pVariableCollection, FunctionStore* pFunctionStore, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow);

class Expression
{
	Variable _result;

public:

	bool EvaluateBinary(ExpressionTokenizer* pExpressionTokenizer, int index)
	{
		ExpressionNode* pNode = pExpressionTokenizer->GetNode(index);

		int leftOperandIndex = index - 1;
		while (pExpressionTokenizer->GetNode(leftOperandIndex)->IsEmpty())
		{
			leftOperandIndex--;
		}

		int rightOperandIndex = index + 1;
		while (pExpressionTokenizer->GetNode(rightOperandIndex)->IsEmpty())
		{
			rightOperandIndex++;
		}

		ExpressionNode* pOperandLeft = pExpressionTokenizer->GetNode(leftOperandIndex);
		ExpressionNode* pOperandRight = pExpressionTokenizer->GetNode(rightOperandIndex);

		float valueLeft = pOperandLeft->_pValue.GetValueFloat();
		float valueRight = pOperandRight->_pValue.GetValueFloat();

		switch (*pNode->_pItem)
		{
		case '*':
			pNode->_pValue = valueLeft * valueRight;
			break;

		case '/':
			pNode->_pValue = valueLeft / valueRight;
			break;

		case '%':
			pNode->_pValue = (int) valueLeft % (int) valueRight;
			break;

		case '+':
			pNode->_pValue = valueLeft + valueRight;
			break;

		case '-':
			pNode->_pValue = valueLeft - valueRight;
			break;

		case '>':
			if (*(pNode->_pItem + 1) == '=')
			{
				pNode->_pValue = valueLeft >= valueRight;
			}
			else
			{
				pNode->_pValue = valueLeft > valueRight;
			}
			break;

		case '<':
			if (*(pNode->_pItem + 1) == '=')
			{
				pNode->_pValue = valueLeft <= valueRight;
			}
			else
			{
				pNode->_pValue = valueLeft < valueRight;
			}
			break;

		case '!':
			if (*(pNode->_pItem + 1) == '=')
			{
				pNode->_pValue = valueLeft != valueRight;
			}
			break;

		case '=':
			if (*(pNode->_pItem + 1) == '=')
			{
				pNode->_pValue = valueLeft == valueRight;
			}
			break;

		default:
			// handle not-matched here with direct return
			return false;
		}

		pNode->_pItem = 0;
		pExpressionTokenizer->SetNodeEmpty(leftOperandIndex);
		pExpressionTokenizer->SetNodeEmpty(rightOperandIndex);

		return true;
	}

	static Variable DoFunctionCall(FunctionDefinition* pFunctionDefinition, VariableCollection* pVariableCollection, FunctionStore* pFunctionStore, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow)
	{
		StackFrame* pStackFrame = pStack->GetTopFrame();
		pStackFrame->SerialNumberStart = pFunctionDefinition->SerialNumberStart;
		pStackFrame->SerialNumberEnd = pFunctionDefinition->SerialNumberEnd;
		pStackFrame->InstructionPointer = pFunctionDefinition->SerialNumberStart;

		pExecutionFlow->RunProgram(1);

		Variable* pReturnValue = pVariableCollection->GetWithoutErrorCheck("<ReturnValue>", pStack->GetFrameCount());
		if (pReturnValue != 0)
		{
			return *pReturnValue;
		}
		else
		{
			return Variable();
		}
	}

	bool HandleFunctionCall(const char* pFunctionName, ExpressionTokenizer* pExpressionTokenizer, int i, VariableCollection* pVariableCollection, FunctionStore* pFunctionStore, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, FunctionCallHandler functionCallHandler, IExecutionFlow* pExecutionFlow)
	{
		ExpressionNode* pNode = pExpressionTokenizer->GetNode(i);

		if (i + 1 == pExpressionTokenizer->GetNodeCount())
		{
			return false;
		}
		
		ExpressionNode* pNext = pExpressionTokenizer->GetNode(i + 1);
		if (*pNext->_pItem != '(')
		{
			return false;
		}

		// find matching close brace. 
		int j;
		int openParenCount = 0;
		for (j = i + 1; j < pExpressionTokenizer->GetNodeCount(); j++)
		{
			ExpressionNode* pClosed = pExpressionTokenizer->GetNode(j);

			if (*pClosed->_pItem == '(')
			{
				openParenCount++;
			}

			if (*pClosed->_pItem == ')')
			{
				if (openParenCount == 1)
				{
					break;
				}
				openParenCount--;
			}
		}

		// terminator
		pExpressionTokenizer->SetNodeEmpty(i + 1);
		pExpressionTokenizer->SetNodeEmpty(j);

		bool handled = false;
		pStack->CreateFrame();

		int argumentCount = 0;

		if (j > i + 2)	// there are arguments
		{
			int argumentIndex = i + 2;
			while (argumentIndex < j)
			{
				int argumentIndexEnd = argumentIndex;
				int parenCount = 0;
				while (argumentIndexEnd < j)	
				{
					ExpressionNode *pArgument = pExpressionTokenizer->GetNode(argumentIndexEnd);
					if (*pArgument->_pItem == '(')
					{
						parenCount++;
					}
					else if (*pArgument->_pItem == ')')
					{
						parenCount--;
					}
					else if (pArgument->IsEmpty() || (*pArgument->_pItem == ',') && parenCount == 0)
					{
						break;
					}

					argumentIndexEnd++;
				}

				// we have an argument list that runs from argumentIndex to argumentIndexEnd - 1. Evaluate it
				

				Variable argument = Evaluate(pExpressionTokenizer, argumentIndex, argumentIndexEnd, pVariableCollection, pFunctionStore, pStack, pParseErrors, lineNumber, functionCallHandler, pExecutionFlow);
				char argumentName[10];
				sprintf(argumentName, "#A%d", argumentCount);
				argumentCount++;

				// NOTE: Add check that argumentIndex + 1 is a comma...

				pVariableCollection->AddAndSet(argumentName, argument.GetValueFloat(), pStack->GetFrameCount());

				argumentIndex = argumentIndexEnd + 1;
			}
		}
		pVariableCollection->AddAndSet("#A", (float)argumentCount, pStack->GetFrameCount());	// marks a function call. 

		if (pFunctionStore)
		{
			FunctionDefinition* pFunction = pFunctionStore->Lookup(pFunctionName);
			if (pFunction != 0)
			{
				pNode->_pValue = (*functionCallHandler)(pFunction, pVariableCollection, pFunctionStore, pStack, pParseErrors, lineNumber, pExecutionFlow);

				handled = true;
			}
		}

		if (!handled && strcmp(pFunctionName, "RAND") == 0)
		{
			//Serial.println("    found random: ");
			Variable* pMinValue = pVariableCollection->GetWithoutErrorCheck("#A0", pStack->GetFrameCount());
			Variable* pMaxValue = pVariableCollection->GetWithoutErrorCheck("#A1", pStack->GetFrameCount());

			pNode->_pValue = MyRandom::GetValue(pMinValue->GetValueInt(), pMaxValue->GetValueInt());
			pExpressionTokenizer->SetNodeEmpty(i + 2);

			handled = true;
		}
		
		if (!handled && *pFunctionName == 'E')
		{


			pNode->_pValue = Variable(333);
			handled = true;
		}


		pVariableCollection->DeleteStackLevel(pStack->GetFrameCount());
		pStack->DestroyFrame();

		return handled;
	}

	// one to use for the real call. 
	Variable Evaluate(const char* pCommand, VariableCollection* pVariableCollection, FunctionStore* pFunctionStore, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow)
	{
		return Evaluate(pCommand, pVariableCollection, pFunctionStore, pStack, pParseErrors, lineNumber, DoFunctionCall, pExecutionFlow);
	}

	Variable Evaluate(const char* pCommand, VariableCollection* pVariableCollection, FunctionStore* pFunctionStore, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, FunctionCallHandler functionCallHandler = 0, IExecutionFlow* pExecutionFlow = 0)
	{
		ExpressionTokenizer expressionTokenizer;

		expressionTokenizer.Tokenize(pCommand);
		int nodeCount = expressionTokenizer.GetNodeCount();

		return Evaluate(&expressionTokenizer, 0, nodeCount - 1, pVariableCollection, pFunctionStore, pStack, pParseErrors, lineNumber, functionCallHandler, pExecutionFlow);
	}
	
	Variable Evaluate(ExpressionTokenizer* pExpressionTokenizer, int start, int max, VariableCollection* pVariableCollection, FunctionStore* pFunctionStore, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, FunctionCallHandler functionCallHandler = 0, IExecutionFlow* pExecutionFlow = 0)
	{
		// Replace literals with their values, look up variables...

		for (int i = start; i <= max; i++)
		{
			ExpressionNode* pNode = pExpressionTokenizer->GetNode(i);

			if (pNode->IsEmpty() || !pNode->_pValue.IsNan())
			{
				continue;
			}

			if (*pNode->_pItem >= '0' && *pNode->_pItem <= '9')
			{
				pNode->_pValue = Variable::ParseFloat(pNode->_pItem);
				pNode->_pItem = 0;
			}
			else if ((*pNode->_pItem >= 'a' && *pNode->_pItem <= 'z') || (*pNode->_pItem >= 'A' && *pNode->_pItem <= 'Z'))
			{
				// might be a variable or a function call. 
				char functionName[128];
				strncpy(functionName, pNode->_pItem, pNode->_pItemLength);
				functionName[pNode->_pItemLength] = '\0';

				if (!HandleFunctionCall(functionName, pExpressionTokenizer, i, pVariableCollection, pFunctionStore, pStack, pParseErrors, lineNumber, functionCallHandler, pExecutionFlow))
				{
					pNode->_pValue = *pVariableCollection->Lookup(pNode->_pItem, pStack->GetFrameCount(), pParseErrors, lineNumber);
					pNode->_pItem = 0;
				}
			}
		}

		return EvaluateRest(pExpressionTokenizer, start, max, pVariableCollection, pFunctionStore, pStack, pParseErrors, lineNumber);
	}

	void EvaluateUnaryMatches(ExpressionTokenizer* pExpressionTokenizer, int start, int max)
	{
		const char *pMatchesTemplate = "+-!";

		for (int i = start; i <= max; i++)
		{
			ExpressionNode* pNode = pExpressionTokenizer->GetNode(i);

			if (pNode->_pItem != 0)
			{
				const char *pMatches = pMatchesTemplate;

				while (*pMatches != '\0')
				{
					if (pNode->StartsWith(*pMatches))
					{
						// Unaries are matched by having a non-value (start or operator) on the left and a value on the right.

						if (i == start || pExpressionTokenizer->GetNode(i - 1)->_pValue.IsNan())
						{
							ExpressionNode* pNext = pExpressionTokenizer->GetNode(i + 1);
							if (!pNext->_pValue.IsNan())
							{
								Variable newValue;
								switch (*pMatches)
								{
								case '-':
									newValue = -pNext->_pValue.GetValueFloat();
									break;

								case '+':
									newValue = pNext->_pValue.GetValueFloat();
									break;

								case '!':
									newValue = !pNext->_pValue.GetValueFloat();
									break;

								}
								pNext->_pValue.SetValue(newValue.GetValueFloat());
								pExpressionTokenizer->SetNodeEmpty(i);
							}
						}
						break;
					}

					pMatches++;
				}
			}
		}
	}

	void EvaluateBinaryMatches(ExpressionTokenizer* pExpressionTokenizer, int start, int max, const char *pMatchesTemplate)
	{
		for (int i = start; i <= max; i++)
		{
			ExpressionNode* pNode = pExpressionTokenizer->GetNode(i);

			if (pNode->_pItem != 0)
			{
				const char *pMatches = pMatchesTemplate;

				while (*pMatches != '\0')
				{
					if (pNode->StartsWith(*pMatches))
					{
						if (EvaluateBinary(pExpressionTokenizer, i))
						{
							i -= 2;
						}
						break;
					}

					pMatches++;
				}
			}
		}
	}

	void EvaluateParens(ExpressionTokenizer* pExpressionTokenizer, int start, int max, VariableCollection* pVariableCollection, FunctionStore* pFunctionStore, Stack* pStack, ParseErrors* pParseErrors, int lineNumber)
	{
		for (int i = start; i <= max; i++)
		{
			ExpressionNode* pNode = pExpressionTokenizer->GetNode(i);

			if (pNode->StartsWith('('))
			{
				int openParams = 0;

				for (int j = i + 1; j <= max; j++)
				{
					ExpressionNode* pEndNode = pExpressionTokenizer->GetNode(j);

					// Keep open param count here, decrement with ')'. It must be zero to do the evaluation...

					if (pEndNode->StartsWith(')'))
					{
						if (openParams == 0)
						{
							pExpressionTokenizer->SetNodeEmpty(i);
							pExpressionTokenizer->SetNodeEmpty(j);

							Evaluate(pExpressionTokenizer, i + 1, j - 1, pVariableCollection, pFunctionStore, pStack, pParseErrors, lineNumber);

							i -= 1; // reevaluate the current node
							break;
						}
						else
						{
							openParams--;
						}
					}
					else if (pEndNode->StartsWith('('))
					{
						openParams++;
					}
				}
			}
		}
	}

	Variable EvaluateRest(ExpressionTokenizer* pExpressionTokenizer, int start, int max, VariableCollection* pVariableCollection, FunctionStore* pFunctionStore, Stack* pStack, ParseErrors* pParseErrors, int lineNumber)
	{
		// Handle parens...
		EvaluateParens(pExpressionTokenizer, start, max, pVariableCollection, pFunctionStore, pStack, pParseErrors, lineNumber);

		// evaluate unary +,-,!
		EvaluateUnaryMatches(pExpressionTokenizer, start, max);

		// evaluate *,/,%
		EvaluateBinaryMatches(pExpressionTokenizer, start, max, "*/%");

		// evaluate +,-
		EvaluateBinaryMatches(pExpressionTokenizer, start, max, "+-");

		// evaluate ==, !=, >, <, >=, <=
		EvaluateBinaryMatches(pExpressionTokenizer, start, max, "=!><");

		for (int i = start; i <= max; i++)
		{
			ExpressionNode* pNode = pExpressionTokenizer->GetNode(i);

			if (!pNode->IsEmpty())
			{
				return pNode->_pValue;
			}
		}

		Variable badValue;
		badValue.SetToNan();

		return badValue;;
	}
};