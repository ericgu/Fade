class ExpressionBuiltInFunctions
{
	static bool HandleBuiltInRand(const char* pFunctionName, ExpressionNode* pNode, ExpressionTokenizer* pExpressionTokenizer, int i, VariableCollection* pVariableCollection, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow)
	{
		if (strcmp(pFunctionName, "RAND") == 0)
		{
			//Serial.println("    found random: ");
			Variable* pMinValue = pVariableCollection->GetWithoutErrorCheck("#A0", pStack->GetFrameCount());
			Variable* pMaxValue = pVariableCollection->GetWithoutErrorCheck("#A1", pStack->GetFrameCount());

			pNode->_value = MyRandom::GetValue(pMinValue->GetValueInt(), pMaxValue->GetValueInt());
			pExpressionTokenizer->SetNodeEmpty(i + 2);

			return true;
		}

		return false;
	}

	static bool HandleBuiltInDirect(const char* pFunctionName, ExpressionNode* pNode, ExpressionTokenizer* pExpressionTokenizer, int i, VariableCollection* pVariableCollection, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow)
	{
		if ((strcmp(pFunctionName, "D") == 0 || strcmp(pFunctionName, "DI") == 0))
		{
			bool immediateMode;
			if (*(pFunctionName + 1) == 'I')
			{
				pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandExecute);
				immediateMode = true;
			}
			else
			{
				pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandNone);
				immediateMode = false;
			}

			Variable* pArgumentCount = pVariableCollection->GetWithoutErrorCheck("#A", pStack->GetFrameCount());

			Variable* pCycleCount = pVariableCollection->GetWithoutErrorCheck("#A0", pStack->GetFrameCount());

			if (immediateMode)
			{
				pExecutionFlow->GetCommandResult()->SetCycleCount(pCycleCount->GetValueInt());
			}

			// even number of arguments is a problem...
			if (pArgumentCount->GetValueInt() % 2 == 0)
			{
				pParseErrors->AddError("Invalid D command: ", "missing brightness target", lineNumber);
				return true;
			}

			for (int i = 1; i < pArgumentCount->GetValueInt(); i += 2)
			{
				char argumentName[10];

				sprintf(argumentName, "#A%d", i);
				Variable* pChannel = pVariableCollection->GetWithoutErrorCheck(argumentName, pStack->GetFrameCount());

				sprintf(argumentName, "#A%d", i + 1);
				Variable* pBrightness = pVariableCollection->GetWithoutErrorCheck(argumentName, pStack->GetFrameCount());

				pExecutionFlow->GetCommandResult()->AddTarget(LedState(pChannel->GetValueInt(), pBrightness, pCycleCount->GetValueInt()));
			}

			return true;
		}

		return false;
	}

	static bool HandleBuiltInSequential(const char* pFunctionName, ExpressionNode* pNode, ExpressionTokenizer* pExpressionTokenizer, int i, VariableCollection* pVariableCollection, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow)
	{
		if ((strcmp(pFunctionName, "S") == 0 || strcmp(pFunctionName, "SI") == 0))
		{
			bool immediateMode;
			if (*(pFunctionName + 1) == 'I')
			{
				pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandExecute);
				immediateMode = true;
			}
			else
			{
				pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandNone);
				immediateMode = false;
			}

			Variable* pArgumentCount = pVariableCollection->GetWithoutErrorCheck("#A", pStack->GetFrameCount());
			if (pArgumentCount->GetValueInt() == 0)
			{
				pParseErrors->AddError("Invalid S command: ", "expected cycle count after (", lineNumber);
				return true;
			}

			Variable* pCycleCount = pVariableCollection->GetWithoutErrorCheck("#A0", pStack->GetFrameCount());

			if (immediateMode)
			{
				pExecutionFlow->GetCommandResult()->SetCycleCount(pCycleCount->GetValueInt());
			}

			for (int channel = 1; channel < pArgumentCount->GetValueInt(); channel++)
			{
				char argumentName[10];

				sprintf(argumentName, "#A%d", channel);
				Variable* pBrightness = pVariableCollection->GetWithoutErrorCheck(argumentName, pStack->GetFrameCount());

				pExecutionFlow->GetCommandResult()->AddTarget(LedState(channel - 1, pBrightness, pCycleCount->GetValueInt()));
			}

			return true;
		}

		return false;
	}

	static bool HandleBuiltInAnimate(const char* pFunctionName, ExpressionNode* pNode, ExpressionTokenizer* pExpressionTokenizer, int i, VariableCollection* pVariableCollection, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow)
	{
		if (strcmp(pFunctionName, "A") == 0)
		{
			Variable* pArgumentCount = pVariableCollection->GetWithoutErrorCheck("#A", pStack->GetFrameCount());
			if (pArgumentCount->GetValueInt() == 0)
			{
				pParseErrors->AddError("Invalid A command: ", "expected cycle count", lineNumber);
				return true;
			}

			Variable* pCycleCount = pVariableCollection->GetWithoutErrorCheck("#A0", pStack->GetFrameCount());

			pExecutionFlow->GetCommandResult()->SetCycleCount(pCycleCount->GetValueInt());
			pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandExecute);

			return true;
		}

		return false;
	}


	static bool HandleBuiltInPrint(const char* pFunctionName, ExpressionNode* pNode, ExpressionTokenizer* pExpressionTokenizer, int i, VariableCollection* pVariableCollection, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow)
	{
		if ((strcmp(pFunctionName, "P") == 0 || strcmp(pFunctionName, "PL") == 0))
		{
			char outputString[64];
			outputString[0] = '\0';

			bool newLine = strcmp(pFunctionName, "PL") == 0;

			if (pExpressionTokenizer->GetNodeCount() > 3)
			{
				ExpressionNode* pArgument = pExpressionTokenizer->GetNode(2);

				if (pArgument->StartsWith('"'))
				{
					const char* pString = pArgument->_pItem;

					const char* pEnd = strchr(pString + 1, '"');

					if (pEnd != 0)
					{
						int length = pEnd - pString - 1;
						strncpy(outputString, pString + 1, length);
						outputString[length] = '\0';
					}
				}
				else
				{
					Variable* pArgument = pVariableCollection->GetWithoutErrorCheck("#A0", pStack->GetFrameCount());
					//char variableName[64];
					//pCommandString = VariableCollection::GetVariableName(pCommandString, variableName);

					//Variable variable = executionContext.Evaluate(variableName, pParseErrors, pCommand->GetSerialNumber(), pExecutionFlow);

					snprintf(outputString, sizeof(outputString), "%f", pArgument->GetValueFloat(0));
				}
			}

			if (newLine)
			{
				strcat(outputString, "\n");
			}
			Serial.print(outputString);

			return true;
		}

		return false;
	}

	static bool HandleBuiltInAbort(const char* pFunctionName, ExpressionNode* pNode, ExpressionTokenizer* pExpressionTokenizer, int i, VariableCollection* pVariableCollection, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow)
	{
		if (strcmp(pFunctionName, "ABORT") == 0)
		{
			pExecutionFlow->AbortExecution();
			return true;
		}

		return false;
	}

	static bool HandleBuiltInConfigLed(const char* pFunctionName, ExpressionNode* pNode, ExpressionTokenizer* pExpressionTokenizer, int i, VariableCollection* pVariableCollection, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow)
	{
		if (strcmp(pFunctionName, "CONFIGLED") == 0)
		{
			ExpressionNode* pLedType = pExpressionTokenizer->GetNode(2);
			char buffer[64];
			strcpy(buffer, pLedType->_pItem + 1);
			buffer[pLedType->_itemLength - 2] = 0;

			Variable* pLedCount = pVariableCollection->GetWithoutErrorCheck("#A1", pStack->GetFrameCount());
			Variable* pLedPin = pVariableCollection->GetWithoutErrorCheck("#A2", pStack->GetFrameCount());

			pExecutionFlow->ConfigureLeds(buffer, pLedCount->GetValueInt(), pLedPin->GetValueInt());

			return true;
		}

		return false;
	}



public:
	static bool HandleBuiltInFunctions(const char* pFunctionName, ExpressionNode* pNode, ExpressionTokenizer* pExpressionTokenizer, int i, VariableCollection* pVariableCollection, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow)
	{
		bool handled = false;

		if (HandleBuiltInRand(pFunctionName, pNode, pExpressionTokenizer, i, pVariableCollection, pStack, pParseErrors, lineNumber, pExecutionFlow))
		{
			return true;
		}

		if (HandleBuiltInDirect(pFunctionName, pNode, pExpressionTokenizer, i, pVariableCollection, pStack, pParseErrors, lineNumber, pExecutionFlow))
		{
			return true;
		}

		if (HandleBuiltInSequential(pFunctionName, pNode, pExpressionTokenizer, i, pVariableCollection, pStack, pParseErrors, lineNumber, pExecutionFlow))
		{
			return true;
		}

		if (HandleBuiltInAnimate(pFunctionName, pNode, pExpressionTokenizer, i, pVariableCollection, pStack, pParseErrors, lineNumber, pExecutionFlow))
		{
			return true;
		}

		if (HandleBuiltInPrint(pFunctionName, pNode, pExpressionTokenizer, i, pVariableCollection, pStack, pParseErrors, lineNumber, pExecutionFlow))
		{
			return true;
		}

		if (HandleBuiltInAbort(pFunctionName, pNode, pExpressionTokenizer, i, pVariableCollection, pStack, pParseErrors, lineNumber, pExecutionFlow))
		{
			return true;
		}

		if (HandleBuiltInConfigLed(pFunctionName, pNode, pExpressionTokenizer, i, pVariableCollection, pStack, pParseErrors, lineNumber, pExecutionFlow))
		{
			return true;
		}

		return false;
	}
};