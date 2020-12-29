class BuiltInFunctions
{
	static bool HandleBuiltInRand(const char *pFunctionName, IExecutionContext *pExecutionContext, ParseErrors *pParseErrors, ExpressionTokenSource *pExpressionTokenSource, IExecutionFlow *pExecutionFlow, ExpressionResult *pExpressionResult)
	{
		if (strcmp(pFunctionName, "RAND") == 0)
		{
			//Serial.println("    found random: ");
			Variable *pMinValue = pExecutionContext->GetVariableWithoutErrorCheck("#A0");
			Variable *pMaxValue = pExecutionContext->GetVariableWithoutErrorCheck("#A1");

			pExpressionResult->_variable.SetValue(0, (float)MyRandom::GetValue(pMinValue->GetValueInt(), pMaxValue->GetValueInt()));
			return true;
		}

		return false;
	}

	static bool HandleBuiltInDirect(const char *pFunctionName, IExecutionContext *pExecutionContext, ParseErrors *pParseErrors, ExpressionTokenSource *pExpressionTokenSource, IExecutionFlow *pExecutionFlow, ExpressionResult *pExpressionResult)
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

			Variable *pArgumentCount = pExecutionContext->GetVariableWithoutErrorCheck("#A");

			Variable *pCycleCount = pExecutionContext->GetVariableWithoutErrorCheck("#A0");

			if (immediateMode)
			{
				pExecutionFlow->GetCommandResult()->SetCycleCount(pCycleCount->GetValueInt());
			}

			// even number of arguments is a problem...
			if (pArgumentCount->GetValueInt() % 2 == 0)
			{
				pParseErrors->AddError("Invalid D command: ", "missing brightness target", pExpressionTokenSource->GetLineNumber());
				return true;
			}

			for (int i = 1; i < pArgumentCount->GetValueInt(); i += 2)
			{
				char argumentName[10];

				snprintf(argumentName, sizeof(argumentName) / sizeof(char), "#A%d", i);
				Variable *pChannel = pExecutionContext->GetVariableWithoutErrorCheck(argumentName);

				snprintf(argumentName, sizeof(argumentName) / sizeof(char), "#A%d", i + 1);
				Variable *pBrightness = pExecutionContext->GetVariableWithoutErrorCheck(argumentName);

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

	static bool HandleBuiltInSequential(const char *pFunctionName, IExecutionContext *pExecutionContext, ParseErrors *pParseErrors, ExpressionTokenSource *pExpressionTokenSource, IExecutionFlow *pExecutionFlow, ExpressionResult *pExpressionResult)
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

			Variable *pArgumentCount = pExecutionContext->GetVariableWithoutErrorCheck("#A");
			if (pArgumentCount->GetValueInt() == 0)
			{
				pParseErrors->AddError("Invalid S command: ", "expected cycle count after (", pExpressionTokenSource->GetLineNumber());
				return true;
			}

			Variable *pCycleCount = pExecutionContext->GetVariableWithoutErrorCheck("#A0");

			if (immediateMode)
			{
				pExecutionFlow->GetCommandResult()->SetCycleCount(pCycleCount->GetValueInt());
			}

			for (int channel = 1; channel < pArgumentCount->GetValueInt(); channel++)
			{
				char argumentName[10];

				snprintf(argumentName, sizeof(argumentName) / sizeof(char), "#A%d", channel);
				Variable *pBrightness = pExecutionContext->GetVariableWithoutErrorCheck(argumentName);

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

	static bool HandleBuiltInAnimate(const char *pFunctionName, IExecutionContext *pExecutionContext, ParseErrors *pParseErrors, ExpressionTokenSource *pExpressionTokenSource, IExecutionFlow *pExecutionFlow, ExpressionResult *pExpressionResult)
	{
		if (strcmp(pFunctionName, "A") == 0)
		{
			Variable *pArgumentCount = pExecutionContext->GetVariableWithoutErrorCheck("#A");
			if (pArgumentCount->GetValueInt() == 0)
			{
				pParseErrors->AddError("Invalid A command: ", "expected cycle count", pExpressionTokenSource->GetLineNumber());
				return true;
			}

			Variable *pCycleCount = pExecutionContext->GetVariableWithoutErrorCheck("#A0");

			pExecutionFlow->GetCommandResult()->SetCycleCount(pCycleCount->GetValueInt());
			pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandExecute);

			pExecutionFlow->ExecuteLedCommand(pExecutionFlow->GetCommandResult());

			return true;
		}

		return false;
	}

	static bool HandleBuiltInPrint(const char *pFunctionName, IExecutionContext *pExecutionContext, ParseErrors *pParseErrors, ExpressionTokenSource *pExpressionTokenSource, IExecutionFlow *pExecutionFlow, ExpressionResult *pExpressionResult)
	{
		if ((strcmp(pFunctionName, "P") == 0 || strcmp(pFunctionName, "PL") == 0))
		{
			char outputString[64];
			outputString[0] = '\0';

			bool newLine = strcmp(pFunctionName, "PL") == 0;

			Variable *pArgument = pExecutionContext->GetVariableWithoutErrorCheck("#A0");

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

	static bool HandleBuiltInAbort(const char *pFunctionName, IExecutionContext *pExecutionContext, ParseErrors *pParseErrors, ExpressionTokenSource *pExpressionTokenSource, IExecutionFlow *pExecutionFlow, ExpressionResult *pExpressionResult)
	{
		if (strcmp(pFunctionName, "ABORT") == 0)
		{
			pExecutionFlow->AbortExecution();
			pParseErrors->AddError("Aborting: ", "ABORT", pExpressionTokenSource->GetLineNumber());

			return true;
		}

		return false;
	}

	static int GetVariableOrNegativeOne(IExecutionContext *pExecutionContext, const char *pVariableName)
	{
		Variable *pValue = pExecutionContext->GetVariableWithoutErrorCheck(pVariableName);

		if (pValue != 0)
		{
			return pValue->GetValueInt();
		}
		else
		{
			return -1;
		}
	}

	static bool HandleBuiltInConfigLed(const char *pFunctionName, IExecutionContext *pExecutionContext, ParseErrors *pParseErrors, ExpressionTokenSource *pExpressionTokenSource, IExecutionFlow *pExecutionFlow, ExpressionResult *pExpressionResult)
	{
		if (strcmp(pFunctionName, "CONFIGLED") == 0)
		{
			Variable *pLedGroupNumber = pExecutionContext->GetVariableWithoutErrorCheck("#A0");
			Variable *pLedType = pExecutionContext->GetVariableWithoutErrorCheck("#A1");
			Variable *pLedCount = pExecutionContext->GetVariableWithoutErrorCheck("#A2");
			int pin1 = GetVariableOrNegativeOne(pExecutionContext, "#A3");
			int pin2 = GetVariableOrNegativeOne(pExecutionContext, "#A4");
			int pin3 = GetVariableOrNegativeOne(pExecutionContext, "#A5");
			int pin4 = GetVariableOrNegativeOne(pExecutionContext, "#A6");

			pExecutionFlow->ConfigureLeds(pLedGroupNumber->GetValueInt(), pLedType->GetValueString(), pLedCount->GetValueInt(), pin1, pin2, pin3, pin4);

			return true;
		}

		return false;
	}

	static bool HandleBuiltInConfigButton(const char *pFunctionName, IExecutionContext *pExecutionContext, ParseErrors *pParseErrors, ExpressionTokenSource *pExpressionTokenSource, IExecutionFlow *pExecutionFlow, ExpressionResult *pExpressionResult)
	{
		if (strcmp(pFunctionName, "CONFIGBUTTON") == 0)
		{
			Variable *pButtonNumber = pExecutionContext->GetVariableWithoutErrorCheck("#A0");
			Variable *pButtonType = pExecutionContext->GetVariableWithoutErrorCheck("#A1");
			Variable *pPin = pExecutionContext->GetVariableWithoutErrorCheck("#A2");
			Variable *pParameter1 = pExecutionContext->GetVariableWithoutErrorCheck("#A3");

			pExecutionFlow->ConfigureButton(pButtonNumber->GetValueInt(), pButtonType->GetValueString(), pPin->GetValueInt(), pParameter1->GetValueInt());

			return true;
		}

		return false;
	}

	static bool HandleBuiltInReadButton(const char *pFunctionName, IExecutionContext *pExecutionContext, ParseErrors *pParseErrors, ExpressionTokenSource *pExpressionTokenSource, IExecutionFlow *pExecutionFlow, ExpressionResult *pExpressionResult)
	{
		if (strcmp(pFunctionName, "READBUTTON") == 0)
		{
			Variable *pButtonNumber = pExecutionContext->GetVariableWithoutErrorCheck("#A0");

			int buttonNumber = pButtonNumber->GetValueInt();

			if (buttonNumber < 0)
			{
				pParseErrors->AddError("Invalid Button Number: ", "button numbers must positive", pExpressionTokenSource->GetLineNumber());
			}
			else if (buttonNumber >= pExecutionFlow->GetButtonCount())
			{
				pParseErrors->AddError("Invalid Button Number: ", "button number is not defined", pExpressionTokenSource->GetLineNumber());
			}
			else
			{
				bool buttonState = pExecutionFlow->GetButtonState(buttonNumber);
				//Serial.print("ReadButton: ");
				//Serial.println(buttonState);

				pExpressionResult->_variable.SetValue(0, (float)buttonState);
			}
			return true;
		}

		return false;
	}

	static bool HandleBuiltInDebug(const char *pFunctionName, IExecutionContext *pExecutionContext, ParseErrors *pParseErrors, ExpressionTokenSource *pExpressionTokenSource, IExecutionFlow *pExecutionFlow, ExpressionResult *pExpressionResult)
	{
		if (strcmp(pFunctionName, "DEBUG") == 0)
		{
			Variable *pIdentifier = pExecutionContext->GetVariableWithoutErrorCheck("#A0");

			if (strcmp(pIdentifier->GetValueString(), "LogStatements") == 0)
			{
				Variable *pValue = pExecutionContext->GetVariableWithoutErrorCheck("#A1");
				Environment.DebugLogStatements = pValue->GetValueInt();
				return true;
			}
			else if (strcmp(pIdentifier->GetValueString(), "LogHeapFreeOnAllocation") == 0)
			{
				Variable *pValue = pExecutionContext->GetVariableWithoutErrorCheck("#A1");
				Environment.DebugLogHeapFreeOnAllocation = pValue->GetValueInt();
				return true;
			}
		}

		return false;
	}

public:
	static bool HandleBuiltInFunctions(const char *pFunctionName, IExecutionContext *pExecutionContext, ParseErrors *pParseErrors, ExpressionTokenSource *pExpressionTokenSource, IExecutionFlow *pExecutionFlow, ExpressionResult *pExpressionResult)
	{
		if (HandleBuiltInRand(pFunctionName, pExecutionContext, pParseErrors, pExpressionTokenSource, pExecutionFlow, pExpressionResult))
		{
			return true;
		}

		if (HandleBuiltInDirect(pFunctionName, pExecutionContext, pParseErrors, pExpressionTokenSource, pExecutionFlow, pExpressionResult))
		{
			return true;
		}

		if (HandleBuiltInSequential(pFunctionName, pExecutionContext, pParseErrors, pExpressionTokenSource, pExecutionFlow, pExpressionResult))
		{
			return true;
		}

		if (HandleBuiltInAnimate(pFunctionName, pExecutionContext, pParseErrors, pExpressionTokenSource, pExecutionFlow, pExpressionResult))
		{
			return true;
		}

		if (HandleBuiltInPrint(pFunctionName, pExecutionContext, pParseErrors, pExpressionTokenSource, pExecutionFlow, pExpressionResult))
		{
			return true;
		}

		if (HandleBuiltInAbort(pFunctionName, pExecutionContext, pParseErrors, pExpressionTokenSource, pExecutionFlow, pExpressionResult))
		{
			return true;
		}

		if (HandleBuiltInConfigLed(pFunctionName, pExecutionContext, pParseErrors, pExpressionTokenSource, pExecutionFlow, pExpressionResult))
		{
			return true;
		}

		if (HandleBuiltInConfigButton(pFunctionName, pExecutionContext, pParseErrors, pExpressionTokenSource, pExecutionFlow, pExpressionResult))
		{
			return true;
		}

		if (HandleBuiltInReadButton(pFunctionName, pExecutionContext, pParseErrors, pExpressionTokenSource, pExecutionFlow, pExpressionResult))
		{
			return true;
		}

		if (HandleBuiltInDebug(pFunctionName, pExecutionContext, pParseErrors, pExpressionTokenSource, pExecutionFlow, pExpressionResult))
		{
			return true;
		}

		return false;
	}
};
