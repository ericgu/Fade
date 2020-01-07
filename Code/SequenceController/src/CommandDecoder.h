#include "Loop.h"

class CommandDecoder
{
	static bool DecodeLoopStart(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command* pCommand, IExecutionFlow* pExecutionFlow)
	{
		Loop loop = Loop::Parse(pCommand->GetString(), executionContext, pParseErrors, pCommand->GetSerialNumber(), pExecutionFlow);

		if (!loop.GetMatch())
		{
			return false;
		}

		executionContext._variables.AddAndSet(loop.GetVariableName(), loop.GetVariableStart().GetValueFloat(), executionContext._stack.GetFrameCount());
		Variable* pLoopVariable = executionContext._variables.GetWithoutErrorCheck(loop.GetVariableName(), executionContext._stack.GetFrameCount());

		while (true)
		{
			executionContext._stack.GetTopFrame()->InstructionPointer = pCommand->GetSerialNumber() + 1;

			CommandResultStatus commandResultStatus = pExecutionFlow->RunProgram(1);
			if (commandResultStatus != CommandResultStatus::CommandEndOfLoop)
			{
				pParseErrors->AddError("Missing loop end", "", -1);
				return true;
			}
			int instructionAfterEnd = executionContext._stack.GetTopFrame()->InstructionPointer;

			pLoopVariable->Increment(loop.GetVariableInc());

			if (!loop.GetIsInRange(pLoopVariable->GetValueFloat()))
			{
				pLoopVariable->Clear();
				executionContext._stack.GetTopFrame()->InstructionPointer = instructionAfterEnd;
				pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandNone);
				return true;
			}
		}

		return true;
	}

	static bool DecodeLoopEnd(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command* pCommand, IExecutionFlow* pExecutionFlow)
	{
		if (pCommand->StartsWith("ENDFOR"))
		{
			executionContext._stack.GetTopFrame()->SerialNumberEnd = pCommand->GetSerialNumber();

			pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandEndOfLoop);
			return true;
		}
		else
		{
			pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandNone);
			return false;
		}
	}

	static void AddFunctionCallError(ParseErrors* pParseErrors, const char* pFunctionName, int parameterCount, int argumentCount, int callingLineNumber)
	{
		char buffer[256];
		sprintf(buffer, "Function %s requires %d parameter(s) but was called with %d argument(s).", pFunctionName, parameterCount, argumentCount);
		pParseErrors->AddError("Mismatched ", buffer, callingLineNumber);
	}

	static bool DecodeFunction(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command* pCommand, IExecutionFlow* pExecutionFlow)
	{
		if (pCommand->StartsWith("ENDFUNC"))
		{
			if (executionContext._functionStore.GetIsCurrentlyParsingFunction())
			{
				executionContext._functionStore.DefineEnd(pCommand->GetSerialNumber());
			}
			else
			{
				pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandEndOfFunction);
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

			ListParser parts("()", pCommandString);	// parse to remove parameters...

			// Having a #A variable means that this is a function call rather than a function definition...

			Variable* pArgumentCount = executionContext._variables.GetWithoutErrorCheck("#A", executionContext._stack.GetFrameCount());
			if (pArgumentCount != 0)
			{
				// function call - rename arguments to local variable names...

				if (parts.GetCount() > 1)
				{
					ListParser parameters(", \t", parts.GetItem(1));

					if (parameters.GetCount() != pArgumentCount->GetValueInt())
					{
						AddFunctionCallError(pParseErrors, parts.GetItem(0), parameters.GetCount(), pArgumentCount->GetValueInt(), executionContext._stack.GetCallingFrame()->InstructionPointer);
						return true;
					}

					for (int i = 0; i < parameters.GetCount(); i++)
					{
						char argumentName[10];
						sprintf(argumentName, "#A%d", i);
						Variable* pArgument = executionContext._variables.GetWithoutErrorCheck(argumentName, executionContext._stack.GetFrameCount());
						if (pArgument != 0)
						{
							pArgument->SetVariableName(parameters.GetItem(i));
						}
					}
				}
				else if (pArgumentCount->GetValueInt() != 0)
				{
					AddFunctionCallError(pParseErrors, parts.GetItem(0), 0, pArgumentCount->GetValueInt(), executionContext._stack.GetCallingFrame()->InstructionPointer);
					return true;
				}
				executionContext._variables.Delete("#A", executionContext._stack.GetFrameCount());
			}
			else
			{
				FunctionDefinition functionDefinition;
				strcpy(functionDefinition.Name, parts.GetItem(0));
				functionDefinition.SerialNumberStart = pCommand->GetSerialNumber();

				executionContext._functionStore.DefineStart(functionDefinition);
			}

			return true;
		}

		return false;
	}

	static bool DecodeAssignment(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command* pCommand, IExecutionFlow* pExecutionFlow)
	{
		if (strchr(pCommand->GetString(), '=') != 0)
		{
			char variableName[64];
			const char* pCommandString = pCommand->GetString();

			pCommandString = VariableCollection::GetVariableName(pCommandString, variableName);

			while ((*pCommandString == '=' || *pCommandString == ' ') && *pCommandString != '\0')
			{
				pCommandString++;
			}

			Variable value = executionContext.Evaluate(pCommandString, pParseErrors, pCommand->GetSerialNumber(), pExecutionFlow);

			executionContext._variables.AddAndSet(variableName, value.GetValueFloat(), executionContext._stack.GetFrameCount());
			pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandNone);

			return true;
		}

		return false;
	}

	static bool DecodeFunctionCall(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command* pCommand, IExecutionFlow* pExecutionFlow)
	{
		char* pCommandString = pCommand->GetString();
		char* open = strchr(pCommandString, '(');
		char* closed = strchr(pCommandString, ')');

		if (open != 0 && closed != 0 && closed > open)
		{
			Variable result = executionContext.Evaluate(pCommandString, pParseErrors, pCommand->GetSerialNumber(), pExecutionFlow);
			Variable* pResult = &result;

			if (pResult != 0)
			{
				return true;
			}
		}

		return false;
	}

	static bool DecodeReturn(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command* pCommand, IExecutionFlow* pExecutionFlow)
	{
		if (pCommand->StartsWith("RETURN"))
		{
			const char* pCommandString = pCommand->GetString() + 7;

			Variable result = executionContext.Evaluate(pCommandString, pParseErrors, pCommand->GetSerialNumber(), pExecutionFlow);

			executionContext._variables.AddAndSet("<ReturnValue>", result.GetValueFloat(), executionContext._stack.GetFrameCount());

			return true;
		}

		return false;
	}

	// P("text")
	// P(<var-name>)
	static bool DecodePrint(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command* pCommand, IExecutionFlow* pExecutionFlow)
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

				Variable variable = executionContext.Evaluate(variableName, pParseErrors, pCommand->GetSerialNumber(), pExecutionFlow);

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

	static bool DecodeWhitespaceOrCommentCommand(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command* pCommand, IExecutionFlow* pExecutionFlow)
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

	static bool DecodeUnrecognizedCommand(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command* pCommand, IExecutionFlow* pExecutionFlow)
	{
		pParseErrors->AddError("Unrecognized command: ", pCommand->GetString(), pCommand->GetSerialNumber());
		return true;
	}

    public:
    static void Decode(ExecutionContext& executionContext, ParseErrors* pParseErrors, Command* pCommand, IExecutionFlow* pExecutionFlow = 0)
    {
		//Serial.println(pCommand->GetString());

		if (DecodeFunction(executionContext, pParseErrors, pCommand, pExecutionFlow)) { return; }

		if (DecodeLoopStart(executionContext, pParseErrors, pCommand, pExecutionFlow)) { return; }

		if (DecodeLoopEnd(executionContext, pParseErrors, pCommand, pExecutionFlow)) { return; }

		if (DecodePrint(executionContext, pParseErrors, pCommand, pExecutionFlow)) { return; }

		if (DecodeAssignment(executionContext, pParseErrors, pCommand, pExecutionFlow)) { return; }

		if (DecodeFunctionCall(executionContext, pParseErrors, pCommand, pExecutionFlow)) { return; }

		if (DecodeReturn(executionContext, pParseErrors, pCommand, pExecutionFlow)) { return; }

		if (DecodeWhitespaceOrCommentCommand(executionContext, pParseErrors, pCommand, pExecutionFlow)) { return; }

		if (DecodeUnrecognizedCommand(executionContext, pParseErrors, pCommand, pExecutionFlow)) { return; }
	}
};