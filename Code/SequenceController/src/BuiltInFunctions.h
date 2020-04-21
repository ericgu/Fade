class BuiltInFunctions
{
	static bool HandleBuiltInRand(const char* pFunctionName, IExecutionContext* pExecutionContext, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow, Variable* pReturnValue)
	{
		if (strcmp(pFunctionName, "RAND") == 0)
		{
			//Serial.println("    found random: ");
			Variable* pMinValue = pExecutionContext->GetVariableWithoutErrorCheck("#A0");
			Variable* pMaxValue = pExecutionContext->GetVariableWithoutErrorCheck("#A1");

			pReturnValue->SetValue((float)MyRandom::GetValue(pMinValue->GetValueInt(), pMaxValue->GetValueInt()));
			return true;
		}

		return false;
	}

	static bool HandleBuiltInDirect(const char* pFunctionName, IExecutionContext* pExecutionContext, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow, Variable* pReturnValue)
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

			Variable* pArgumentCount = pExecutionContext->GetVariableWithoutErrorCheck("#A");

			Variable* pCycleCount = pExecutionContext->GetVariableWithoutErrorCheck("#A0");

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

				snprintf(argumentName, sizeof(argumentName) / sizeof(char), "#A%d", i);
				Variable* pChannel = pExecutionContext->GetVariableWithoutErrorCheck(argumentName);

				snprintf(argumentName, sizeof(argumentName) / sizeof(char), "#A%d", i + 1);
				Variable* pBrightness = pExecutionContext->GetVariableWithoutErrorCheck(argumentName);

				pExecutionFlow->GetCommandResult()->AddTarget(LedState(pChannel->GetValueInt(), pBrightness, pCycleCount->GetValueInt()));
			}

            if (immediateMode)
            {
                pExecutionFlow->ExecuteLedCommand(pExecutionFlow->GetCommandResult());
            }

			return true;
		}

		return false;
	}

	static bool HandleBuiltInSequential(const char* pFunctionName, IExecutionContext* pExecutionContext, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow, Variable* pReturnValue)
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

			Variable* pArgumentCount = pExecutionContext->GetVariableWithoutErrorCheck("#A");
			if (pArgumentCount->GetValueInt() == 0)
			{
				pParseErrors->AddError("Invalid S command: ", "expected cycle count after (", lineNumber);
				return true;
			}

			Variable* pCycleCount = pExecutionContext->GetVariableWithoutErrorCheck("#A0");

			if (immediateMode)
			{
				pExecutionFlow->GetCommandResult()->SetCycleCount(pCycleCount->GetValueInt());
			}

			for (int channel = 1; channel < pArgumentCount->GetValueInt(); channel++)
			{
				char argumentName[10];

				snprintf(argumentName, sizeof(argumentName) / sizeof(char), "#A%d", channel);
				Variable* pBrightness = pExecutionContext->GetVariableWithoutErrorCheck(argumentName);

				pExecutionFlow->GetCommandResult()->AddTarget(LedState(channel - 1, pBrightness, pCycleCount->GetValueInt()));
			}

            if (immediateMode)
            {
                pExecutionFlow->ExecuteLedCommand(pExecutionFlow->GetCommandResult());
            }

			return true;
		}

		return false;
	}

	static bool HandleBuiltInAnimate(const char* pFunctionName, IExecutionContext* pExecutionContext, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow, Variable* pReturnValue)
	{
		if (strcmp(pFunctionName, "A") == 0)
		{
			Variable* pArgumentCount = pExecutionContext->GetVariableWithoutErrorCheck("#A");
			if (pArgumentCount->GetValueInt() == 0)
			{
				pParseErrors->AddError("Invalid A command: ", "expected cycle count", lineNumber);
				return true;
			}

			Variable* pCycleCount = pExecutionContext->GetVariableWithoutErrorCheck("#A0");

			pExecutionFlow->GetCommandResult()->SetCycleCount(pCycleCount->GetValueInt());
			pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandExecute);

            pExecutionFlow->ExecuteLedCommand(pExecutionFlow->GetCommandResult());

			return true;
		}

		return false;
	}


	static bool HandleBuiltInPrint(const char* pFunctionName, IExecutionContext* pExecutionContext, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow, Variable* pReturnValue)
	{
		if ((strcmp(pFunctionName, "P") == 0 || strcmp(pFunctionName, "PL") == 0))
		{
			char outputString[64];
			outputString[0] = '\0';

			bool newLine = strcmp(pFunctionName, "PL") == 0;

			Variable* pArgument = pExecutionContext->GetVariableWithoutErrorCheck("#A0");

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

	static bool HandleBuiltInAbort(const char* pFunctionName, IExecutionContext* pExecutionContext, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow, Variable* pReturnValue)
	{
		if (strcmp(pFunctionName, "ABORT") == 0)
		{
			pExecutionFlow->AbortExecution();
            pParseErrors->AddError("Aborting: ", "ABORT", lineNumber);

			return true;
		}

		return false;
	}

	static bool HandleBuiltInConfigLed(const char* pFunctionName, IExecutionContext* pExecutionContext, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow, Variable* pReturnValue)
	{
		if (strcmp(pFunctionName, "CONFIGLED") == 0)
		{
			Variable* pLedType = pExecutionContext->GetVariableWithoutErrorCheck("#A0");
			Variable* pLedCount = pExecutionContext->GetVariableWithoutErrorCheck("#A1");
			Variable* pLedPin = pExecutionContext->GetVariableWithoutErrorCheck("#A2");

			pExecutionFlow->ConfigureLeds(pLedType->GetValueString(), pLedCount->GetValueInt(), pLedPin->GetValueInt());

			return true;
		}

		return false;
	}

public:
	static bool HandleBuiltInFunctions(const char* pFunctionName, IExecutionContext* pExecutionContext, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow, Variable* pReturnValue)
	{
		if (HandleBuiltInRand(pFunctionName, pExecutionContext, pParseErrors, lineNumber, pExecutionFlow, pReturnValue))
		{
			return true;
		}

		if (HandleBuiltInDirect(pFunctionName, pExecutionContext, pParseErrors, lineNumber, pExecutionFlow, pReturnValue))
		{
			return true;
		}

		if (HandleBuiltInSequential(pFunctionName, pExecutionContext, pParseErrors, lineNumber, pExecutionFlow, pReturnValue))
		{
			return true;
		}

		if (HandleBuiltInAnimate(pFunctionName, pExecutionContext, pParseErrors, lineNumber, pExecutionFlow, pReturnValue))
		{
			return true;
		}

		if (HandleBuiltInPrint(pFunctionName, pExecutionContext, pParseErrors, lineNumber, pExecutionFlow, pReturnValue))
		{
			return true;
		}

		if (HandleBuiltInAbort(pFunctionName, pExecutionContext, pParseErrors, lineNumber, pExecutionFlow, pReturnValue))
		{
			return true;
		}

		if (HandleBuiltInConfigLed(pFunctionName, pExecutionContext, pParseErrors, lineNumber, pExecutionFlow, pReturnValue))
		{
			return true;
		}

		return false;
	}
};
