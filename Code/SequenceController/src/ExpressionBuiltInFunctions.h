class ExpressionBuiltInFunctions
{
public:
	static bool HandleBuiltInFunctions(const char* pFunctionName, ExpressionNode* pNode, ExpressionTokenizer* pExpressionTokenizer, int i, VariableCollection* pVariableCollection, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow)
	{
		bool handled = false;

		if (!handled && strcmp(pFunctionName, "RAND") == 0)
		{
			//Serial.println("    found random: ");
			Variable* pMinValue = pVariableCollection->GetWithoutErrorCheck("#A0", pStack->GetFrameCount());
			Variable* pMaxValue = pVariableCollection->GetWithoutErrorCheck("#A1", pStack->GetFrameCount());

			pNode->_pValue = MyRandom::GetValue(pMinValue->GetValueInt(), pMaxValue->GetValueInt());
			pExpressionTokenizer->SetNodeEmpty(i + 2);

			handled = true;
		}

		if (!handled && (strcmp(pFunctionName, "D") == 0 || strcmp(pFunctionName, "DI") == 0))
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

				pExecutionFlow->GetCommandResult()->AddTarget(LedState(pChannel->GetValueInt(), pBrightness->GetValueFloat(), pCycleCount->GetValueInt()));
			}

			handled = true;
		}

		if (!handled && (strcmp(pFunctionName, "S") == 0 || strcmp(pFunctionName, "SI") == 0))
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

				pExecutionFlow->GetCommandResult()->AddTarget(LedState(channel - 1, pBrightness->GetValueFloat(), pCycleCount->GetValueInt()));
			}

			handled = true;
		}

		if (!handled && strcmp(pFunctionName, "A") == 0)
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

			handled = true;
		}

		return handled;
	}
};