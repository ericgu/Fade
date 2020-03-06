#include "Loop.h"

class CommandDecoder
{
	static bool DecodeFor(ExecutionContext* pExecutionContext, ParseErrors* pParseErrors, Command* pCommand, IExecutionFlow* pExecutionFlow)
	{
		Loop loop = Loop::Parse(pCommand->GetString(), pExecutionContext, pParseErrors, pCommand->GetSerialNumber(), pExecutionFlow);

		if (!loop.GetMatch())
		{
			return false;
		}

		Variable startValue = loop.GetVariableStart();
		pExecutionContext->_variables.AddAndSet(loop.GetVariableName(), &startValue, pExecutionContext->_stack.GetFrameCount());
		Variable* pLoopVariable = pExecutionContext->_variables.GetWithoutErrorCheck(loop.GetVariableName(), pExecutionContext->_stack.GetFrameCount());

		while (true)
		{
			pExecutionContext->_stack.GetTopFrame()->InstructionPointer = pCommand->GetSerialNumber() + 1;

			CommandResultStatus commandResultStatus = pExecutionFlow->RunProgram(1);
			if (commandResultStatus != CommandResultStatus::CommandEndOfLoop)
			{
				pParseErrors->AddError("Missing loop end", "", -1);
				return true;
			}
			int instructionAfterEnd = pExecutionContext->_stack.GetTopFrame()->InstructionPointer;

			pLoopVariable->Increment(loop.GetVariableInc());

			if (!loop.GetIsInRange(pLoopVariable->GetValueFloat(0)))
			{
				pExecutionContext->_variables.Delete(loop.GetVariableName(), pExecutionContext->_stack.GetFrameCount());
				pExecutionContext->_stack.GetTopFrame()->InstructionPointer = instructionAfterEnd;
				pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandNone);
				return true;
			}
		}

		return true;
	}

	static bool DecodeEndFor(ExecutionContext* pExecutionContext, ParseErrors* pParseErrors, Command* pCommand, IExecutionFlow* pExecutionFlow)
	{
		if (pCommand->StartsWith("ENDFOR"))
		{
			pExecutionContext->_stack.GetTopFrame()->SerialNumberEnd = pCommand->GetSerialNumber();

			pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandEndOfLoop);
			return true;
		}
		else
		{
			pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandNone);
			return false;
		}
	}

	static int FindTrueIfClause(ExecutionContext* pExecutionContext, ParseErrors* pParseErrors, IExecutionFlow* pExecutionFlow)
	{
		int statementIndex = pExecutionContext->_stack.GetTopFrame()->InstructionPointer;

		int ifCount = 0;

		while (true)
		{
			Command nextCommand = pExecutionFlow->GetCommand(statementIndex);
			if (nextCommand.GetSerialNumber() == -1)
			{
				return -1;
			}

			if (nextCommand.StartsWith("IF"))
			{
				ifCount++;
			}

			if (ifCount == 1)
			{
				if (nextCommand.StartsWith("IF") || nextCommand.StartsWith("ELSEIF"))
				{
					int offset = nextCommand.StartsWith("ELSEIF") ? 7 : 3;

					Variable condition = pExecutionContext->_expression.Evaluate(
						nextCommand.GetString() + offset,
						&pExecutionContext->_variables,
						&pExecutionContext->_functionStore,
						&pExecutionContext->_stack,
						pParseErrors,
						pExecutionContext->_stack.GetTopFrame()->InstructionPointer,
						pExecutionFlow);

					if (condition.GetValueInt() != 0)
					{
						return statementIndex;
					}
				}
				else if (nextCommand.StartsWith("ELSE"))
				{
					return statementIndex;
				}
				else if (nextCommand.StartsWith("ENDIF"))
				{
					return -1;
				}
			}

			if (nextCommand.StartsWith("ENDIF"))
			{
				ifCount--;
			}

			statementIndex++;
		}
	}

	static int FindEndif(ExecutionContext* pExecutionContext, IExecutionFlow* pExecutionFlow, int ifStatementIndex)
	{
		int statementIndex = ifStatementIndex;
		int ifCount = 0;

		while (true)
		{
			Command nextCommand = pExecutionFlow->GetCommand(statementIndex);
			if (nextCommand.GetSerialNumber() == -1)
			{
				return -1;
			}

			if (nextCommand.StartsWith("IF"))
			{
				ifCount++;
			}
			else if (nextCommand.StartsWith("ENDIF"))
			{
				ifCount--;

				if (ifCount == 0)
				{
					return statementIndex;
				}
			}

			statementIndex++;
		}
	}

	static bool DecodeIf(ExecutionContext* pExecutionContext, ParseErrors* pParseErrors, Command* pCommand, IExecutionFlow* pExecutionFlow)
	{
		if (pCommand->StartsWith("IF"))
		{
			int ifStatementIndex = pExecutionContext->_stack.GetTopFrame()->InstructionPointer;

			int statementIndex = FindTrueIfClause(pExecutionContext, pParseErrors, pExecutionFlow);

			if (statementIndex != -1)
			{
				pExecutionContext->_stack.GetTopFrame()->InstructionPointer = statementIndex + 1;
				pExecutionFlow->RunProgram(1);
			}

			int endIfIndex = FindEndif(pExecutionContext, pExecutionFlow, ifStatementIndex);
			if (endIfIndex == -1)
			{
				pParseErrors->AddError("Missing ENDIF", "", pCommand->GetSerialNumber());
				return true;
			}

			pExecutionContext->_stack.GetTopFrame()->InstructionPointer = endIfIndex;

			return true;
		}

		if (pCommand->StartsWith("ENDIF"))
		{
			pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandEndIf);
			return true;
		}

		if (pCommand->StartsWith("ELSEIF"))
		{
			pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandElseIf);
			return true;
		}

		if (pCommand->StartsWith("ELSE"))
		{
			pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandElse);
			return true;
		}

		return false;
	}


	static void AddFunctionCallError(ParseErrors* pParseErrors, const char* pFunctionName, int parameterCount, int argumentCount, int callingLineNumber)
	{
		char buffer[256];
		sprintf(buffer, "Function %s requires %d parameter(s) but was called with %d argument(s).", pFunctionName, parameterCount, argumentCount);
		pParseErrors->AddError("Mismatched ", buffer, callingLineNumber);
	}

	static bool DecodeFunction(ExecutionContext* pExecutionContext, ParseErrors* pParseErrors, Command* pCommand, IExecutionFlow* pExecutionFlow)
	{
		if (pCommand->StartsWith("ENDFUNC"))
		{
			if (pExecutionContext->_functionStore.GetIsCurrentlyParsingFunction())
			{
				pExecutionContext->_functionStore.DefineEnd(pCommand->GetSerialNumber());
			}
			else
			{
				pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandEndOfFunction);
			}

			return true;
		}

		if (pExecutionContext->_functionStore.GetIsCurrentlyParsingFunction())
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

			Variable* pArgumentCount = pExecutionContext->_variables.GetWithoutErrorCheck("#A", pExecutionContext->_stack.GetFrameCount());
			if (pArgumentCount != 0)
			{
				// function call - rename arguments to local variable names...

				if (parts.GetCount() > 1)
				{
					ListParser parameters(", \t", parts.GetItem(1));

					if (parameters.GetCount() != pArgumentCount->GetValueInt())
					{
						AddFunctionCallError(pParseErrors, parts.GetItem(0), parameters.GetCount(), pArgumentCount->GetValueInt(), pExecutionContext->_stack.GetCallingFrame()->InstructionPointer);
						return true;
					}

					for (int i = 0; i < parameters.GetCount(); i++)
					{
						char argumentName[10];
						sprintf(argumentName, "#A%d", i);
						Variable* pArgument = pExecutionContext->_variables.GetWithoutErrorCheck(argumentName, pExecutionContext->_stack.GetFrameCount());
						if (pArgument != 0)
						{
							pArgument->SetVariableName(parameters.GetItem(i));
						}
					}
				}
				else if (pArgumentCount->GetValueInt() != 0)
				{
					AddFunctionCallError(pParseErrors, parts.GetItem(0), 0, pArgumentCount->GetValueInt(), pExecutionContext->_stack.GetCallingFrame()->InstructionPointer);
					return true;
				}
				pExecutionContext->_variables.Delete("#A", pExecutionContext->_stack.GetFrameCount());
			}
			else
			{
				pExecutionContext->_functionStore.DefineStart(parts.GetItem(0), pCommand->GetSerialNumber());
			}

			return true;
		}

		return false;
	}

	static bool DecodeExpression(ExecutionContext* pExecutionContext, ParseErrors* pParseErrors, Command* pCommand, IExecutionFlow* pExecutionFlow)
	{
		pExecutionContext->Evaluate(pCommand->GetString(), pParseErrors, pCommand->GetSerialNumber(), pExecutionFlow);

		return true;
	}

	static bool DecodeReturn(ExecutionContext* pExecutionContext, ParseErrors* pParseErrors, Command* pCommand, IExecutionFlow* pExecutionFlow)
	{
		if (pCommand->StartsWith("RETURN"))
		{
			const char* pCommandString = pCommand->GetString() + 7;

			Variable result = pExecutionContext->Evaluate(pCommandString, pParseErrors, pCommand->GetSerialNumber(), pExecutionFlow);

			pExecutionContext->_variables.AddAndSet("<ReturnValue>", &result, pExecutionContext->_stack.GetFrameCount());

			return true;
		}

		return false;
	}

	static bool DecodeWhitespaceOrCommentCommand(ExecutionContext* pExecutionContext, ParseErrors* pParseErrors, Command* pCommand, IExecutionFlow* pExecutionFlow)
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

    public:
    static void Decode(ExecutionContext* pExecutionContext, ParseErrors* pParseErrors, Command* pCommand, IExecutionFlow* pExecutionFlow = 0)
    {
		//Serial.println(pCommand->GetString());

		if (DecodeWhitespaceOrCommentCommand(pExecutionContext, pParseErrors, pCommand, pExecutionFlow)) { return; }

		if (DecodeFunction(pExecutionContext, pParseErrors, pCommand, pExecutionFlow)) { return; }

		if (DecodeFor(pExecutionContext, pParseErrors, pCommand, pExecutionFlow)) { return; }

		if (DecodeEndFor(pExecutionContext, pParseErrors, pCommand, pExecutionFlow)) { return; }

		if (DecodeIf(pExecutionContext, pParseErrors, pCommand, pExecutionFlow)) { return; }

		if (DecodeReturn(pExecutionContext, pParseErrors, pCommand, pExecutionFlow)) { return; }

		if (DecodeExpression(pExecutionContext, pParseErrors, pCommand, pExecutionFlow)) { return; }
	}
};