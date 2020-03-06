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
		if (pNext == 0 || !pNext->Is("("))
		{
			return false;
		}

		int j = pExpressionTokenizer->FindMatchingParen(i + 1);

		if (j < 0)
		{
			pParseErrors->AddError("Invalid expression: ", "missing \")\"", lineNumber);
			return false;
		}

		// get rid of parens...
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
					if (pArgument->Is("(") || pArgument->Is("{"))
					{
						parenCount++;
					}
					else if (pArgument->Is(")") || pArgument->Is("}"))
					{
						parenCount--;
					}
					else if ((pArgument->IsEmpty() || pArgument->Is(",")) && parenCount == 0)
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

				argumentCollection.AddAndSet(argumentName, &argument, 0); // copy all values across...

				argumentIndex = argumentIndexEnd + 1;
			}
		}

		pStack->CreateFrame();

		for (int i = 0; i < argumentCount; i++)
		{
			char argumentName[10];
			sprintf(argumentName, "#A%d", i);

			Variable* pArgument = argumentCollection.GetWithoutErrorCheck(argumentName, 0);
			pVariableCollection->AddAndSet(argumentName, pArgument, pStack->GetFrameCount());
		}

		Variable argumentCountVariable(argumentCount);

		pVariableCollection->AddAndSet("#A", &argumentCountVariable, pStack->GetFrameCount());

		if (pFunctionStore)
		{
			FunctionDefinition* pFunction = pFunctionStore->Lookup(pFunctionName);
			if (pFunction != 0)
			{
				pNode->_value = (*functionCallHandler)(pFunction, pVariableCollection, pFunctionStore, pStack, pParseErrors, lineNumber, pExecutionFlow);

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

		// Detect assignments...
		if (nodeCount > 2)
		{
			ExpressionNode* pIdentifier = expressionTokenizer.GetNode(0);
			ExpressionNode* pEquals = expressionTokenizer.GetNode(1);

			if (pEquals->Is("="))
			{
				if (pIdentifier->IsIdentifier())
				{
					Variable result = Evaluate(&expressionTokenizer, 2, nodeCount - 1, pVariableCollection, pFunctionStore, pStack, pParseErrors, lineNumber, functionCallHandler, pExecutionFlow);
					char identifier[128];
					strncpy(identifier, pIdentifier->_pItem, pIdentifier->_itemLength);
					identifier[pIdentifier->_itemLength] = '\0';

					pVariableCollection->AddAndSet(identifier, &result, pStack->GetFrameCount());

					return result;
				}
			}
		}
		else if (nodeCount == 2)
		{
			ExpressionNode* pIdentifier = expressionTokenizer.GetNode(0);
			ExpressionNode* pSecond = expressionTokenizer.GetNode(1);

			int delta = 0;

			if (pSecond->Is("++"))
			{
				delta = 1;
			}
			else if (pSecond->Is("--"))
			{
				delta = -1;
			}

			if (delta != 0)
			{
				// might be a variable or a function call. 
				char identifier[128];
				strncpy(identifier, pIdentifier->_pItem, pIdentifier->_itemLength);
				identifier[pIdentifier->_itemLength] = '\0';

				Variable* pVariable = pVariableCollection->GetWithoutErrorCheck(identifier, pStack->GetFrameCount());
				if (pVariable != 0)
				{
					float value = pVariable->GetValueFloat(0);
					pVariable->SetValue(0, value + delta);
					return value;
				}
			}
		}

		return Evaluate(&expressionTokenizer, 0, nodeCount - 1, pVariableCollection, pFunctionStore, pStack, pParseErrors, lineNumber, functionCallHandler, pExecutionFlow);
	}
	
	Variable Evaluate(ExpressionTokenizer* pExpressionTokenizer, int start, int max, VariableCollection* pVariableCollection, FunctionStore* pFunctionStore, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, FunctionCallHandler functionCallHandler = 0, IExecutionFlow* pExecutionFlow = 0)
	{
		// Replace literals with their values, look up variables...

		for (int i = start; i <= max; i++)
		{
			ExpressionNode* pNode = pExpressionTokenizer->GetNode(i);

			if (pNode->IsEmpty() || !pNode->_value.IsNan())
			{
				continue;
			}

			if (pNode->IsNumber())
			{
				pNode->_value = Variable::ParseFloat(pNode->_pItem);
				pNode->_pItem = 0;
			}
			else if (pNode->IsIdentifier())
			{
				// might be a variable or a function call. 
				char identifier[128];
				strncpy(identifier, pNode->_pItem, pNode->_itemLength);
				identifier[pNode->_itemLength] = '\0';

				Variable* pValue = pVariableCollection->GetWithoutErrorCheck(identifier, pStack->GetFrameCount());
				ExpressionNode* pNext = pExpressionTokenizer->GetNode(i + 1);

				// implement ++ and --
				int delta = 0;

				if (pNext->_pItem != 0 && strcmp(pNext->_pItem, "++") == 0)
				{
					delta = 1;
				}
				else if (pNext->_pItem != 0 && strcmp(pNext->_pItem, "--") == 0)
				{
					delta = -1;
				}

				if (delta != 0)
				{
					float value = pValue->GetValueFloat(0);
					pValue->SetValue(0, value + delta);
					return value;
				}

				// if the next node is a '(', this is a function call even if there is a matching variable name. 
				bool nextNodeParen = pNext != 0 && pNext->Is("(");

				if (pValue && !nextNodeParen)
				{
					pNode->_value = *pValue;
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

			if (pNode->Is("("))
			{
				int j = pExpressionTokenizer->FindMatchingParen(i);

				pExpressionTokenizer->SetNodeEmpty(i);
				pExpressionTokenizer->SetNodeEmpty(j);

				Evaluate(pExpressionTokenizer, i + 1, j - 1, pVariableCollection, pFunctionStore, pStack, pParseErrors, lineNumber);

				i -= 1; // reevaluate the current node
			}
		}
	}

	void EvaluateList(ExpressionTokenizer* pExpressionTokenizer, int start, int max, VariableCollection* pVariableCollection, FunctionStore* pFunctionStore, Stack* pStack, ParseErrors* pParseErrors, int lineNumber)
	{
		for (int i = start; i <= max; i++)
		{
			ExpressionNode* pNode = pExpressionTokenizer->GetNode(i);

			if (pNode->Is("{"))
			{
				int j;
				for (j = i + 1; j <= max; j++)
				{
					if (pExpressionTokenizer->GetNode(j)->Is("}"))
					{
						break;
					}
				}

				// evaluate each of the sub-expressions...
				int start = i + 1;

				int end;
				for (end = start + 1; end <= j; end++)
				{
					if (pExpressionTokenizer->GetNode(end)->Is(","))
					{
						Evaluate(pExpressionTokenizer, start, end - 1, pVariableCollection, pFunctionStore, pStack, pParseErrors, lineNumber);
						pExpressionTokenizer->SetNodeEmpty(end);
						start = end + 1;
					}
				}

				Evaluate(pExpressionTokenizer, start, j - 1, pVariableCollection, pFunctionStore, pStack, pParseErrors, lineNumber);

				int itemIndex = 1;
				ExpressionNode* pDataNode = 0;
				for (int index = i + 1; index < j; index++)
				{
					ExpressionNode* pListNode = pExpressionTokenizer->GetNode(index);

					if (!pListNode->IsEmpty())
					{
						if (pDataNode == 0)
						{
							pDataNode = pListNode;	// first number node is where we accumulate the values...
						}
						else
						{
							pDataNode->_value.SetValue(itemIndex, pListNode->_value.GetValueFloat(0));
							itemIndex++;
							pExpressionTokenizer->SetNodeEmpty(index);
						}
					}
				}

				pExpressionTokenizer->SetNodeEmpty(i);
				pExpressionTokenizer->SetNodeEmpty(j);

				i -= 1; // reevaluate the current node
			}
		}
	}

	Variable EvaluateRest(ExpressionTokenizer* pExpressionTokenizer, int start, int max, VariableCollection* pVariableCollection, FunctionStore* pFunctionStore, Stack* pStack, ParseErrors* pParseErrors, int lineNumber)
	{
		// Handle parens...
		EvaluateList(pExpressionTokenizer, start, max, pVariableCollection, pFunctionStore, pStack, pParseErrors, lineNumber);

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

		return pExpressionTokenizer->FindFirstValue(start, max);
	}
};