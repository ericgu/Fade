typedef Variable(*FunctionCallHandler)(FunctionDefinition* pFunctionDefinition, VariableCollection* pVariableCollection, FunctionStore* pFunctionStore, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow);

class Expression
{
	Variable _result;

public:
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
		if (pNext == 0 || pNext->_pItem == 0 || *pNext->_pItem != '(')
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

		int argumentCount = 0;
		VariableCollection argumentCollection;


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

				argumentCollection.AddAndSet(argumentName, argument.GetValueFloat(), 0);

				argumentIndex = argumentIndexEnd + 1;
			}
		}

		pStack->CreateFrame();

		for (int i = 0; i < argumentCount; i++)
		{
			char argumentName[10];
			sprintf(argumentName, "#A%d", i);

			Variable* pArgument = argumentCollection.GetWithoutErrorCheck(argumentName, 0);
			pVariableCollection->AddAndSet(argumentName, pArgument->GetValueFloat(), pStack->GetFrameCount());
		}

		pVariableCollection->AddAndSet("#A", (float)argumentCount, pStack->GetFrameCount());

		if (pFunctionStore)
		{
			FunctionDefinition* pFunction = pFunctionStore->Lookup(pFunctionName);
			if (pFunction != 0)
			{
				pNode->_pValue = (*functionCallHandler)(pFunction, pVariableCollection, pFunctionStore, pStack, pParseErrors, lineNumber, pExecutionFlow);

				handled = true;
			}
		}

		if (!handled)
		{
			handled = ExpressionBuiltInFunctions::HandleBuiltInFunctions(pFunctionName, pNode, pExpressionTokenizer, i, pVariableCollection, pStack, pParseErrors, lineNumber, pExecutionFlow);
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
				char identifier[128];
				strncpy(identifier, pNode->_pItem, pNode->_pItemLength);
				identifier[pNode->_pItemLength] = '\0';

				Variable* pValue = pVariableCollection->GetWithoutErrorCheck(identifier, pStack->GetFrameCount());
				ExpressionNode* pNext = pExpressionTokenizer->GetNode(i + 1);

					// if the next node is a '(', this is a function call even if there is a matching variable name. 
				bool nextNodeParen = pNext != 0 && pNext->_pItem != 0 && *pNext->_pItem == '(';

				if (pValue && !nextNodeParen)
				{
					pNode->_pValue = *pValue;
				}
				else
				{
					if (!HandleFunctionCall(identifier, pExpressionTokenizer, i, pVariableCollection, pFunctionStore, pStack, pParseErrors, lineNumber, functionCallHandler, pExecutionFlow))
					{
						pParseErrors->AddError("Unrecognized identifier: ", identifier, lineNumber);
						return Variable();
					}
				}
			}
		}

		return EvaluateRest(pExpressionTokenizer, start, max, pVariableCollection, pFunctionStore, pStack, pParseErrors, lineNumber);
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
		ExpressionOperators::EvaluateUnaryMatches(pExpressionTokenizer, start, max);

		// evaluate *,/,%
		ExpressionOperators::EvaluateBinaryMatches(pExpressionTokenizer, start, max, "*/%");

		// evaluate +,-
		ExpressionOperators::EvaluateBinaryMatches(pExpressionTokenizer, start, max, "+-");

		// evaluate ==, !=, >, <, >=, <=
		ExpressionOperators::EvaluateBinaryMatches(pExpressionTokenizer, start, max, "=!><");

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