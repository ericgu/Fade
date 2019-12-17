#include "Loop.h"

class CommandDecoder
{
	static bool DecodeDirect(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command* pCommand, CommandResult& commandResult)
	{
		if (!pCommand->StartsWith("D")) { return false; }

		char* pCommandString = pCommand->GetString() + 1;
		
		bool immediateMode;
		if (*pCommandString == 'I')
		{
			pCommandString++;
			commandResult.SetStatus(CommandResultStatus::CommandExecute);
			immediateMode = true;
		}
		else if (*pCommandString == '(')
		{
			commandResult.SetStatus(CommandResultStatus::CommandNone);
			immediateMode = false;
		}
		else
		{
			pParseErrors->AddError("Invalid D command: ", "expected I or (", pCommand->GetSerialNumber());
			return true;
		}
		pCommandString++;		// skip (

		ListParser listParser(",", pCommandString);

		if (listParser.GetCount() == 0)
		{
			pParseErrors->AddError("Invalid D command: ", "expected cycle count after (", pCommand->GetSerialNumber());
			return true;
		}

		Variable cycleCount = *executionContext.Parse(listParser.GetItem(0), pParseErrors, pCommand->GetSerialNumber());

		if (immediateMode)
		{
			commandResult.SetCycleCount(cycleCount.GetValueInt());
		}
		
		// even number of arguments is a problem...
		if (listParser.GetCount() %2 == 0)
		{
			pParseErrors->AddError("Invalid D command: ", "missing brightness target", pCommand->GetSerialNumber());
			return true;
		}

		for (int i = 1; i < listParser.GetCount(); i += 2)
		{
			Variable channel = *executionContext.Parse(listParser.GetItem(i), pParseErrors, pCommand->GetSerialNumber());

			Variable brightness = *executionContext.Parse(listParser.GetItem(i + 1), pParseErrors, pCommand->GetSerialNumber());

			commandResult.AddTarget(LedState(channel.GetValueInt(), brightness.GetValueFloat(), cycleCount.GetValueInt()));
		}

		return true;
	}

	static bool DecodeSequential(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command* pCommand, CommandResult& commandResult)
	{
		if (!pCommand->StartsWith("S")) { return false; }

		char* pCommandString = pCommand->GetString() + 1;
		bool immediateMode = false;

		if (*pCommandString == 'I')
		{
			commandResult.SetStatus(CommandResultStatus::CommandExecute);
			pCommandString++;
			immediateMode = true;
		}

		if (*pCommandString != '(')
		{
			pParseErrors->AddError("Invalid S command: ", "expected I or (", pCommand->GetSerialNumber());
			return true;
		}

		pCommandString++;		// skip (

		ListParser listParser(",", pCommandString);
		if (listParser.GetCount() == 0)
		{
			pParseErrors->AddError("Invalid S command: ", "expected cycle count after (", pCommand->GetSerialNumber());
			return true;
		}

		Variable cycleCount = *executionContext.Parse(listParser.GetItem(0), pParseErrors, pCommand->GetSerialNumber());
		if (immediateMode)
		{
			commandResult.SetCycleCount(cycleCount.GetValueInt());
		}

		for (int channel = 1; channel < listParser.GetCount(); channel++)
		{
			Variable brightness = *executionContext.Parse(listParser.GetItem(channel), pParseErrors, pCommand->GetSerialNumber());
			commandResult.AddTarget(LedState(channel - 1, brightness.GetValueFloat(), cycleCount.GetValueInt()));
		}

		return true;
	}

	static bool DecodeAnimate(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command* pCommand, CommandResult& commandResult)
	{
		if (!pCommand->StartsWith("A")) { return false; }

		char* pCommandString = pCommand->GetString();
		pCommandString++;

		if (*pCommandString != '(')
		{
			pParseErrors->AddError("Invalid A command: ", "expected (", pCommand->GetSerialNumber());
			return true;
		}
		pCommandString++;

		if (*pCommandString == '\0')
		{
			pParseErrors->AddError("Invalid A command: ", "expected cycle count", pCommand->GetSerialNumber());
			return true;
		}

		Variable cycleCount = *executionContext.Parse(pCommandString, pParseErrors, pCommand->GetSerialNumber());
		commandResult.SetCycleCount(cycleCount.GetValueInt());
		commandResult.SetStatus(CommandResultStatus::CommandExecute);

		return true;
	}
	
	static bool DecodeLoopStart(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command* pCommand, CommandResult& commandResult)
	{
		Loop loop = Loop::Parse(pCommand->GetString(), executionContext, pParseErrors, pCommand->GetSerialNumber());

		if (!loop.GetMatch())
		{
			return false;
		}

		// See if the variable we're using is defined. If it is, we're in process for this loop
		Variable* pLoopVariable = executionContext._variables.GetWithoutErrorCheck(loop.GetVariableName());
		
		if (pLoopVariable != 0)
		{
			pLoopVariable->Increment(loop.GetVariableInc());

			if (!loop.GetIsInRange(pLoopVariable->GetValueFloat()))
			{
				pLoopVariable->Clear();
				commandResult.SetStatus(CommandResultStatus::CommandExitLoopBody);
				return true;
			}

			commandResult.SetStatus(CommandResultStatus::CommandSkipToNext);
		}
		else // first time
		{
			executionContext._variables.AddAndSet(loop.GetVariableName(), loop.GetVariableStart().GetValueFloat());

			executionContext._stack.CreateFrame();
			executionContext._stack.GetTopFrame()->SerialNumberStart = pCommand->GetSerialNumber();
			executionContext._stack.GetTopFrame()->InstructionPointer = pCommand->GetSerialNumber() + 1;

			commandResult.SetStatus(CommandResultStatus::CommandSkipToNext);
		}

		return true;
	}

	static bool DecodeLoopEnd(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command* pCommand, CommandResult& commandResult)
	{
		if (pCommand->StartsWith("ENDFOR"))
		{
			commandResult.SetStatus(CommandResultStatus::CommandEndOfLoop);
			return true;
		}
		else
		{
			commandResult.SetStatus(CommandResultStatus::CommandNone);
			return false;
		}
	}

	static bool DecodeFunction(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command* pCommand, CommandResult& commandResult)
	{
		if (pCommand->StartsWith("ENDFUNC"))
		{
			if (executionContext._functionStore.GetIsCurrentlyParsingFunction())
			{
				executionContext._functionStore.DefineEnd(pCommand->GetSerialNumber());
			}
			else
			{
				// done with function call, restore stack etc. Figure out how to handle return values here...
				executionContext._stack.DestroyFrame();
			}

			return true;
		}

		if (executionContext._functionStore.GetIsCurrentlyParsingFunction())
		{
			return true;
		}

		if (pCommand->StartsWith("FUNC"))
		{
			const char* pCommandString = pCommand->GetString() + 4;

			while (*pCommandString == ' ' || *pCommandString == '\t')
			{
				pCommandString++;
			}

			FunctionDefinition functionDefinition;
			strcpy(functionDefinition.Name, pCommandString);
			functionDefinition.SerialNumberStart = pCommand->GetSerialNumber();

			executionContext._functionStore.DefineStart(functionDefinition);

			return true;
		}

		return false;
	}

	static bool DecodeAssignment(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command* pCommand, CommandResult& commandResult)
	{
		if (strchr(pCommand->GetString(), '=') != 0)
		{
			char variableName[64];

			const char* pCommandString = pCommand->GetString();
			//Serial.print("In Assignment1: "); Serial.println(pCommand);

			pCommandString = VariableCollection::GetVariableName(pCommandString, variableName);
			//Serial.print("In Assignment2: "); Serial.println(pCommand);

			while ((*pCommandString == '=' || *pCommandString == ' ') && *pCommandString != '\0')
			{
				pCommandString++;
			}

			//Serial.print("In Assignment3: "); Serial.println(pCommand);
			Variable variable = *executionContext.Parse(pCommandString, pParseErrors, pCommand->GetSerialNumber());

			executionContext._variables.AddAndSet(variableName, variable.GetValueFloat());

			commandResult.SetStatus(CommandResultStatus::CommandNone);
			return true;
		}

		return false;
	}

	static bool DecodeFunctionCall(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command* pCommand, CommandResult& commandResult)
	{
		char* pCommandString = pCommand->GetString();
		char* open = strchr(pCommandString, '(');
		char* closed = strchr(pCommandString, ')');

		if (open != 0 && closed != 0 && closed > open)
		{
			Variable* pResult = executionContext.Parse(pCommandString, pParseErrors, pCommand->GetSerialNumber());

			if (pResult != 0)
			{
				return true;
			}
		}

		return false;
	}

	// P("text")
	// P(<var-name>)
	static bool DecodePrint(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command* pCommand, CommandResult& commandResult)
	{
		if (pCommand->StartsWith("P"))
		{
			char outputString[64];
			outputString[0] = '\0';

			const char* pCommandString = pCommand->GetString();

			pCommandString++;
			bool newLine = false;

			if (*pCommandString == 'L')
			{
				newLine = true;
				pCommandString++;
			}

			pCommandString++; // move to parameter

			if (*pCommandString == '"')
			{
				pCommandString++;

				char* pEnd = strchr((char*)pCommandString, '"');

				if (pEnd != 0)
				{
					int length = pEnd - pCommandString;
					strncpy(outputString, pCommandString, length);
					outputString[length] = '\0';
				}
			}
			else
			{
				char variableName[64];
				pCommandString = VariableCollection::GetVariableName(pCommandString, variableName);

				Variable variable = *executionContext.Parse(variableName, pParseErrors, pCommand->GetSerialNumber());

				snprintf(outputString, sizeof(outputString), "%f", variable.GetValueFloat());
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

	static bool DecodeWhitespaceOrCommentCommand(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command* pCommand, CommandResult& commandResult)
	{
		char* pCommandString = pCommand->GetString();

		if (pCommand->StartsWith("//"))
		{
			return true;
		}

		while (*pCommandString != '\0')
		{
			if (*pCommandString != ' ' && *pCommandString != '\t')
			{
				return false;
			}

			pCommandString++;
		}

		return true;
	}

	static bool DecodeUnrecognizedCommand(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command* pCommand, CommandResult& commandResult)
	{
		pParseErrors->AddError("Unrecognized command: ", pCommand->GetString(), pCommand->GetSerialNumber());
		return true;
	}

    public:
    static void Decode(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command* pCommand, CommandResult& commandResult)
    {
		//Serial.println(pCommand->GetString());

		if (DecodeFunction(executionContext, pParseErrors, pCommand, commandResult)) { return; }

		if (DecodeLoopStart(executionContext, pParseErrors, pCommand, commandResult)) { return; }

		if (DecodeLoopEnd(executionContext, pParseErrors, pCommand, commandResult)) { return; }

		if (DecodePrint(executionContext, pParseErrors, pCommand, commandResult)) { return; }

		if (DecodeAssignment(executionContext, pParseErrors, pCommand, commandResult)) { return; }

		if (DecodeAnimate(executionContext, pParseErrors, pCommand, commandResult)) { return; }

		if (DecodeDirect(executionContext, pParseErrors, pCommand, commandResult)) { return; }

		if (DecodeSequential(executionContext, pParseErrors, pCommand, commandResult)) { return; }

		if (DecodeFunctionCall(executionContext, pParseErrors, pCommand, commandResult)) { return; }

		if (DecodeWhitespaceOrCommentCommand(executionContext, pParseErrors, pCommand, commandResult)) { return; }

		if (DecodeUnrecognizedCommand(executionContext, pParseErrors, pCommand, commandResult)) { return; }
	}
};