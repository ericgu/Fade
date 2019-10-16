#include "Loop.h"

class CommandDecoder
{
	static bool DecodeDirect(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command command, CommandResult& commandResult)
	{
		if (!command.StartsWith("D")) { return false; }

		char* pCommand = command.GetString() + 1;
		
		bool immediateMode;
		if (*pCommand == 'I')
		{
			pCommand++;
			commandResult.SetStatus(CommandResultStatus::CommandExecute);
			immediateMode = true;
		}
		else if (*pCommand == '(')
		{
			commandResult.SetStatus(CommandResultStatus::CommandNone);
			immediateMode = false;
		}
		else
		{
			pParseErrors->AddError("Invalid D command: ", "expected I or (", command.GetSerialNumber());
			return true;
		}
		pCommand++;		// skip (

		ListParser listParser(",", pCommand);

		if (listParser.GetCount() == 0)
		{
			pParseErrors->AddError("Invalid D command: ", "expected cycle count after (", command.GetSerialNumber());
			return true;
		}

		Variable cycleCount = *executionContext.ParseFloatOrVariable(listParser.GetItem(0), pParseErrors, command.GetSerialNumber());

		if (immediateMode)
		{
			commandResult.SetCycleCount(cycleCount.GetValueInt());
		}
		
		// even number of arguments is a problem...
		if (listParser.GetCount() %2 == 0)
		{
			pParseErrors->AddError("Invalid D command: ", "missing brightness target", command.GetSerialNumber());
			return true;
		}

		for (int i = 1; i < listParser.GetCount(); i += 2)
		{
			Variable channel = *executionContext.ParseFloatOrVariable(listParser.GetItem(i), pParseErrors, command.GetSerialNumber());

			Variable brightness = *executionContext.ParseFloatOrVariable(listParser.GetItem(i + 1), pParseErrors, command.GetSerialNumber());

			commandResult.AddTarget(LedState(channel.GetValueInt(), brightness.GetValueFloat(), cycleCount.GetValueInt()));
		}

		return true;
	}

	static bool DecodeSequential(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command command, CommandResult& commandResult)
	{
		if (!command.StartsWith("S")) { return false; }

		char* pCommand = command.GetString() + 1;
		bool immediateMode = false;

		if (*pCommand == 'I')
		{
			commandResult.SetStatus(CommandResultStatus::CommandExecute);
			pCommand++;
			immediateMode = true;
		}

		if (*pCommand != '(')
		{
			pParseErrors->AddError("Invalid S command: ", "expected I or (", command.GetSerialNumber());
			return true;
		}

		pCommand++;		// skip (

		ListParser listParser(",", pCommand);
		if (listParser.GetCount() == 0)
		{
			pParseErrors->AddError("Invalid S command: ", "expected cycle count after (", command.GetSerialNumber());
			return true;
		}

		Variable cycleCount = *executionContext.ParseFloatOrVariable(listParser.GetItem(0), pParseErrors, command.GetSerialNumber());
		if (immediateMode)
		{
			commandResult.SetCycleCount(cycleCount.GetValueInt());
		}

		for (int channel = 1; channel < listParser.GetCount(); channel++)
		{
			Variable brightness = *executionContext.ParseFloatOrVariable(listParser.GetItem(channel), pParseErrors, command.GetSerialNumber());
			commandResult.AddTarget(LedState(channel - 1, brightness.GetValueFloat(), cycleCount.GetValueInt()));
		}

		return true;
	}

	static bool DecodeAnimate(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command command, CommandResult& commandResult)
	{
		if (!command.StartsWith("A")) { return false; }

		char* pCommand = command.GetString();
		pCommand++;

		if (*pCommand != '(')
		{
			pParseErrors->AddError("Invalid A command: ", "expected (", command.GetSerialNumber());
			return true;
		}
		pCommand++;

		if (*pCommand == '\0')
		{
			pParseErrors->AddError("Invalid A command: ", "expected cycle count", command.GetSerialNumber());
			return true;
		}

		Variable cycleCount = *executionContext.ParseFloatOrVariable(pCommand, pParseErrors, command.GetSerialNumber());
		commandResult.SetCycleCount(cycleCount.GetValueInt());
		commandResult.SetStatus(CommandResultStatus::CommandExecute);

		return true;
	}
	
	static bool DecodeLoopStart(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command command, CommandResult& commandResult)
	{
		Loop loop = Loop::Parse(command.GetString(), executionContext, pParseErrors, command.GetSerialNumber());

		if (!loop.GetMatch())
		{
			return false;
		}

		// check top of stack to see if we're in process for this loop
		if (executionContext._stack.GetFrameCount() != 0 &&
			executionContext._stack.GetTopFrame().SerialNumberStart == command.GetSerialNumber())
		{
			executionContext._variables.Get(loop.GetVariableName(), pParseErrors, command.GetSerialNumber())->Increment(loop.GetVariableInc());

			if (!loop.GetIsInRange(executionContext._variables.Get(loop.GetVariableName(), pParseErrors, command.GetSerialNumber())->GetValueFloat()))
			{
				executionContext._variables.Get(loop.GetVariableName(), pParseErrors, command.GetSerialNumber())->SetActiveFlag(false);
				commandResult.SetStatus(CommandResultStatus::CommandExitLoopBody);
				return true;
			}

			commandResult.SetStatus(CommandResultStatus::CommandSkipToNext);
		}
		else // first time
		{
			executionContext._variables.AddAndSet(loop.GetVariableName(), loop.GetVariableStart().GetValueFloat());

			executionContext._stack.CreateFrame();
			executionContext._stack.GetTopFrame().SerialNumberStart = command.GetSerialNumber();

			commandResult.SetStatus(CommandResultStatus::CommandSkipToNext);
		}

		return true;
	}

	static bool DecodeLoopEnd(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command command, CommandResult& commandResult)
	{
		if (command.StartsWith("ENDFOR"))
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

	static bool DecodeAssignment(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command command, CommandResult& commandResult)
	{
		if (strchr(command.GetString(), '=') != 0)
		{
			char variableName[64];

			const char* pCommand = command.GetString();
			//Serial.print("In Assignment1: "); Serial.println(pCommand);

			pCommand = VariableCollection::GetVariableName(pCommand, variableName);
			//Serial.print("In Assignment2: "); Serial.println(pCommand);

			while ((*pCommand == '=' || *pCommand == ' ') && *pCommand != '\0')
			{
				pCommand++;
			}

			//Serial.print("In Assignment3: "); Serial.println(pCommand);
			Variable variable = *executionContext.ParseFloatOrVariable(pCommand, pParseErrors, command.GetSerialNumber());

			executionContext._variables.AddAndSet(variableName, variable.GetValueFloat());

			commandResult.SetStatus(CommandResultStatus::CommandNone);
			return true;
		}

		return false;
	}

	// P("text")
	// P(<var-name>)
	static bool DecodePrint(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command command, CommandResult& commandResult)
	{
		if (command.StartsWith("P"))
		{
			char outputString[64];
			outputString[0] = '\0';

			const char* pCommand = command.GetString();

			pCommand++;
			bool newLine = false;

			if (*pCommand == 'L')
			{
				newLine = true;
				pCommand++;
			}

			pCommand++; // move to parameter

			if (*pCommand == '"')
			{
				pCommand++;

				char* pEnd = strchr((char*) pCommand, '"');

				if (pEnd != 0)
				{
					int length = pEnd - pCommand;
					strncpy(outputString, pCommand, length);
					outputString[length] = '\0';
				}
			}
			else
			{
				char variableName[64];
				pCommand = VariableCollection::GetVariableName(pCommand, variableName);

				Variable variable = *executionContext.ParseFloatOrVariable(variableName, pParseErrors, command.GetSerialNumber());

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

	static bool DecodeWhitespaceOrCommentCommand(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command command, CommandResult& commandResult)
	{
		char* pCommand = command.GetString();

		if (command.StartsWith("//"))
		{
			return true;
		}

		while (*pCommand != '\0')
		{
			if (*pCommand != ' ' && *pCommand != '\t')
			{
				return false;
			}

			pCommand++;
		}

		return true;
	}

	static bool DecodeUnrecognizedCommand(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command command, CommandResult& commandResult)
	{
		pParseErrors->AddError("Unrecognized command: ", command.GetString(), command.GetSerialNumber());
		return true;
	}

    public:
    static void Decode(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command command, CommandResult& commandResult)
    {
		//Serial.println(command.GetString());

		if (DecodeLoopStart(executionContext, pParseErrors, command, commandResult)) { return; }

		if (DecodeLoopEnd(executionContext, pParseErrors, command, commandResult)) { return; }

		if (DecodePrint(executionContext, pParseErrors, command, commandResult)) { return; }

		if (DecodeAssignment(executionContext, pParseErrors, command, commandResult)) { return; }

		if (DecodeAnimate(executionContext, pParseErrors, command, commandResult)) { return; }

		if (DecodeDirect(executionContext, pParseErrors, command, commandResult)) { return; }

		if (DecodeSequential(executionContext, pParseErrors, command, commandResult)) { return; }

		if (DecodeWhitespaceOrCommentCommand(executionContext, pParseErrors, command, commandResult)) { return; }

		if (DecodeUnrecognizedCommand(executionContext, pParseErrors, command, commandResult)) { return; }
	}
};