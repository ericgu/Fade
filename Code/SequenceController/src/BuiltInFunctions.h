class BuiltInFunctions
{
	static bool HandleBuiltInRand(const char* pFunctionName, VariableCollection* pVariableCollection, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow, Variable* pReturnValue)
	{
		if (strcmp(pFunctionName, "RAND") == 0)
		{
			//Serial.println("    found random: ");
			Variable* pMinValue = pVariableCollection->GetWithoutErrorCheck("#A0", pStack->GetFrameCount());
			Variable* pMaxValue = pVariableCollection->GetWithoutErrorCheck("#A1", pStack->GetFrameCount());

			pReturnValue->SetValue((float)MyRandom::GetValue(pMinValue->GetValueInt(), pMaxValue->GetValueInt()));
			return true;
		}

		return false;
	}

	static bool HandleBuiltInDirect(const char* pFunctionName, VariableCollection* pVariableCollection, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow, Variable* pReturnValue)
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

	static bool HandleBuiltInSequential(const char* pFunctionName, VariableCollection* pVariableCollection, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow, Variable* pReturnValue)
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

	static bool HandleBuiltInAnimate(const char* pFunctionName, VariableCollection* pVariableCollection, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow, Variable* pReturnValue)
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


	static bool HandleBuiltInPrint(const char* pFunctionName, VariableCollection* pVariableCollection, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow, Variable* pReturnValue)
	{
		if ((strcmp(pFunctionName, "P") == 0 || strcmp(pFunctionName, "PL") == 0))
		{
			char outputString[64];
			outputString[0] = '\0';

			bool newLine = strcmp(pFunctionName, "PL") == 0;

			Variable* pArgument = pVariableCollection->GetWithoutErrorCheck("#A0", pStack->GetFrameCount());

			if (pArgument->GetValueCount() > 1)
			{
				strcat(outputString, "{");

				for (int i = 0; i < pArgument->GetValueCount(); i++)
				{
					if (i != 0)
					{
						strcat(outputString, ", ");
					}
					char buffer[16];
					snprintf(buffer, sizeof(buffer), "%f", pArgument->GetValueFloat(i));
					strcat(outputString, buffer);
				}
				strcat(outputString, "}");

			}
			else if (pArgument->GetValueCount() == 1)
			{
				snprintf(outputString, sizeof(outputString), "%f", pArgument->GetValueFloat(0));
			}
			else
			{
				strcat(outputString, pArgument->GetValueString());
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

	static bool HandleBuiltInAbort(const char* pFunctionName, VariableCollection* pVariableCollection, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow, Variable* pReturnValue)
	{
		if (strcmp(pFunctionName, "ABORT") == 0)
		{
			pExecutionFlow->AbortExecution();
			return true;
		}

		return false;
	}

	static bool HandleBuiltInConfigLed(const char* pFunctionName, VariableCollection* pVariableCollection, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow, Variable* pReturnValue)
	{
		if (strcmp(pFunctionName, "CONFIGLED") == 0)
		{
			Variable* pLedType = pVariableCollection->GetWithoutErrorCheck("#A0", pStack->GetFrameCount());
			Variable* pLedCount = pVariableCollection->GetWithoutErrorCheck("#A1", pStack->GetFrameCount());
			Variable* pLedPin = pVariableCollection->GetWithoutErrorCheck("#A2", pStack->GetFrameCount());

			pExecutionFlow->ConfigureLeds(pLedType->GetValueString(), pLedCount->GetValueInt(), pLedPin->GetValueInt());

			// strings aren't real variables so they aren't repurposed; we need to delete it explicitly. 
			pVariableCollection->Delete("#A0", pStack->GetFrameCount());

			return true;
		}

		return false;
	}



public:
	static bool HandleBuiltInFunctions(const char* pFunctionName, VariableCollection* pVariableCollection, Stack* pStack, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow, Variable* pReturnValue)
	{

		if (HandleBuiltInRand(pFunctionName, pVariableCollection, pStack, pParseErrors, lineNumber, pExecutionFlow, pReturnValue))
		{
			return true;
		}

		if (HandleBuiltInDirect(pFunctionName, pVariableCollection, pStack, pParseErrors, lineNumber, pExecutionFlow, pReturnValue))
		{
			return true;
		}

		if (HandleBuiltInSequential(pFunctionName, pVariableCollection, pStack, pParseErrors, lineNumber, pExecutionFlow, pReturnValue))
		{
			return true;
		}

		if (HandleBuiltInAnimate(pFunctionName, pVariableCollection, pStack, pParseErrors, lineNumber, pExecutionFlow, pReturnValue))
		{
			return true;
		}

		if (HandleBuiltInPrint(pFunctionName, pVariableCollection, pStack, pParseErrors, lineNumber, pExecutionFlow, pReturnValue))
		{
			return true;
		}

		if (HandleBuiltInAbort(pFunctionName, pVariableCollection, pStack, pParseErrors, lineNumber, pExecutionFlow, pReturnValue))
		{
			return true;
		}

		if (HandleBuiltInConfigLed(pFunctionName, pVariableCollection, pStack, pParseErrors, lineNumber, pExecutionFlow, pReturnValue))
		{
			return true;
		}

		return false;
	}
};
