#include "Loop.h"

class CommandDecoder
{
	ListParser* _pListParser;
	ListParser* _pParameters;

	Loop* _pLoop;


public:
	CommandDecoder()
	{
		_pListParser = new ListParser(1020, 1024);
		_pParameters = new ListParser(1021, 1024);
		_pLoop = new Loop();
	}

	~CommandDecoder()
	{
		delete _pListParser;
		delete _pParameters;
		delete _pLoop;
	}

    

	bool DecodeFor(ExecutionContext* pExecutionContext, ParseErrors* pParseErrors, Command* pCommand, IExecutionFlow* pExecutionFlow)
	{
        Profiler.Start("DecodeFor");

        //bool result = RDDecodeFor(pExecutionContext, pCommand, pParseErrors, pExecutionFlow);
        //return result;

		if (!_pLoop->Parse(pCommand->GetString(), pExecutionContext, pParseErrors, pCommand->GetLineNumber(), pExecutionFlow))
		{
			return false;
		}
		StackWatcher::Log("CommandDecoder::DecodeFor a");

		Profiler.Start("DecodeFor-Loop");

		Variable startValue = _pLoop->GetVariableStart();
		//Variable endValue = _pLoop->GetVariableEnd();
		float endValue = _pLoop->GetVariableEnd().GetValueFloat(0);

		float increment = _pLoop->GetVariableInc().GetValueFloat(0);
		pExecutionContext->AddVariableAndSet(_pLoop->GetVariableName(), &startValue);
		Variable* pLoopVariable = pExecutionContext->GetVariableWithoutErrorCheck(_pLoop->GetVariableName());

		StackWatcher::Log("CommandDecoder::DecodeFor b");

		int forLoopLineNumber = pCommand->GetLineNumber();

		while (true)
		{
			pExecutionContext->StackTopFrame()->SetInstructionPointer(forLoopLineNumber + 1, "DecodeFor");

			//Serial.print(pLoopVariable->GetVariableName()); Serial.print(" "); Serial.println(pLoopVariable->GetValueFloat(0));
			CommandResultStatus commandResultStatus = pExecutionFlow->RunProgram(1);
			StackWatcher::Log("CommandDecoder::DecodeFor c");
			if (pExecutionFlow->IsAborting())
			{
				return true;
			}

			if (commandResultStatus != CommandResultStatus::CommandEndOfLoop)
			{
				pParseErrors->AddError("Missing loop end", "", -1);
				return true;
			}
			int instructionAfterEnd = pExecutionContext->StackTopFrame()->GetInstructionPointer();

			pLoopVariable->Increment(increment);

			if (!Loop::GetIsInRange(startValue.GetValueFloat(0), endValue, pLoopVariable->GetValueFloat(0)))
			{
				pExecutionContext->DeleteVariable(pLoopVariable->GetVariableName());
				pExecutionContext->StackTopFrame()->SetInstructionPointer(instructionAfterEnd, "DecodeFor");
				pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandNone);
				return true;
			}
		}

		return true;
	}

	bool DecodeEndFor(ExecutionContext* pExecutionContext, ParseErrors* pParseErrors, Command* pCommand, IExecutionFlow* pExecutionFlow)
	{
		if (pCommand->StartsWith("ENDFOR"))
		{
			StackWatcher::Log("CommandDecoder::DecodeEndFor");

			pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandEndOfLoop);
			return true;
		}
		else
		{
			pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandNone);
			return false;
		}
	}

	int FindTrueIfClause(ExecutionContext* pExecutionContext, ParseErrors* pParseErrors, IExecutionFlow* pExecutionFlow)
	{
		PROLOGUE

		int statementIndex = pExecutionContext->StackTopFrame()->GetInstructionPointer();

		int ifCount = 0;

		while (true)
		{
			Command* pNextCommand = pExecutionFlow->GetCommand(statementIndex);
			//Serial.print("Scan => ");
			//Serial.println(pNextCommand->GetString());
			if (pNextCommand->GetLineNumber() == -1)
			{
				return -1;
			}

			if (pNextCommand->StartsWith("IF"))
			{
				ifCount++;
			}

			if (ifCount == 1)
			{
				if (pNextCommand->StartsWith("IF") || pNextCommand->StartsWith("ELSEIF"))
				{
					int offset = pNextCommand->StartsWith("ELSEIF") ? 7 : 3;

					Variable condition = pExecutionContext->Evaluate(
						pNextCommand->GetString() + offset,
						pParseErrors,
						pNextCommand->GetLineNumber(),
						pExecutionFlow);

					if (condition.GetValueInt() != 0)
					{
						return statementIndex;
					}
				}
				else if (pNextCommand->StartsWith("ELSE"))
				{
					return statementIndex;
				}
				else if (pNextCommand->StartsWith("ENDIF"))
				{
					return -1;
				}
			}

			if (pNextCommand->StartsWith("ENDIF"))
			{
				ifCount--;
			}

			statementIndex++;
		}
	}

	int FindEndif(ExecutionContext* pExecutionContext, IExecutionFlow* pExecutionFlow, int ifStatementIndex)
	{
		int statementIndex = ifStatementIndex;
		int ifCount = 0;

		while (true)
		{
			Command* pNextCommand = pExecutionFlow->GetCommand(statementIndex);
			if (pNextCommand->GetLineNumber() == -1)
			{
				return -1;
			}

			if (pNextCommand->StartsWith("IF"))
			{
				ifCount++;
			}
			else if (pNextCommand->StartsWith("ENDIF"))
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

	bool DecodeIf(ExecutionContext* pExecutionContext, ParseErrors* pParseErrors, Command* pCommand, IExecutionFlow* pExecutionFlow)
	{
		Profiler.Start("DecodeIf");
		if (pCommand->StartsWith("IF"))
		{
			StackWatcher::Log("CommandDecoder::DecodeIf");
			int ifStatementIndex = pExecutionContext->StackTopFrame()->GetInstructionPointer();

			int statementIndex = FindTrueIfClause(pExecutionContext, pParseErrors, pExecutionFlow);

			if (statementIndex != -1)
			{
				pExecutionContext->StackTopFrame()->SetInstructionPointer(statementIndex + 1, "DecodeIf");
				pExecutionFlow->RunProgram(1);
			}

			int endIfIndex = FindEndif(pExecutionContext, pExecutionFlow, ifStatementIndex);
			if (endIfIndex == -1)
			{
				pParseErrors->AddError("Missing ENDIF", "", pCommand->GetLineNumber());
				return true;
			}

			pExecutionContext->StackTopFrame()->SetInstructionPointer(endIfIndex, "DecodeIf");

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


	void AddFunctionCallError(ParseErrors* pParseErrors, const char* pFunctionName, int parameterCount, int argumentCount, int callingLineNumber)
	{
		char buffer[256];
		snprintf(buffer, sizeof(buffer) / sizeof(char), "Function %s requires %d parameter(s) but was called with %d argument(s).", pFunctionName, parameterCount, argumentCount);
		pParseErrors->AddError("Mismatched ", buffer, callingLineNumber);
	}

	bool DecodeFunction(ExecutionContext* pExecutionContext, ParseErrors* pParseErrors, Command* pCommand, IExecutionFlow* pExecutionFlow)
	{
		Profiler.Start("DecodeFunction");
		if (pCommand->StartsWith("ENDFUNC"))
		{
			if (pExecutionContext->_functionStore.GetIsCurrentlyParsingFunction())
			{
				pExecutionContext->_functionStore.DefineEnd(pCommand->GetLineNumber());
			}
			else
			{
				pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandEndOfFunction);
			}

			return true;
		}

		if (pExecutionContext->_functionStore.GetIsCurrentlyParsingFunction())
		{
			//Serial.print("Skipping: "); Serial.println(pCommand->GetString());
			return true;
		}

		if (pCommand->StartsWith("FUNC"))
		{
			StackWatcher::Log("CommandDecoder::DecodeFunction");
			const char* pCommandString = pCommand->GetString() + 4;

			while (*pCommandString == ' ' || *pCommandString == '\t')
			{
				pCommandString++;
			}

			_pListParser->Parse("()", pCommandString);	// parse to remove parameters...

			// Having a #A variable means that this is a function call rather than a function definition...

			Variable* pArgumentCount = pExecutionContext->GetVariableWithoutErrorCheck("#A");
			if (pArgumentCount != 0)
			{
				// function call - rename arguments to local variable names...

				if (_pListParser->GetCount() > 1)
				{
					_pParameters->Parse(", \t", _pListParser->GetItem(1));

					if (_pParameters->GetCount() != pArgumentCount->GetValueInt())
					{
						AddFunctionCallError(pParseErrors, _pListParser->GetItem(0), _pParameters->GetCount(), pArgumentCount->GetValueInt(), pExecutionContext->StackCallingFrame()->GetInstructionPointer());
						return true;
					}

					for (int i = 0; i < _pParameters->GetCount(); i++)
					{
						Variable* pArgument = pExecutionContext->GetVariableWithoutErrorCheck(FunctionCaller::GenerateArgumentName(i));
						if (pArgument != 0)
						{
							pArgument->SetVariableName(_pParameters->GetItem(i));
						}
					}
				}
				else if (pArgumentCount->GetValueInt() != 0)
				{
					AddFunctionCallError(pParseErrors, _pListParser->GetItem(0), 0, pArgumentCount->GetValueInt(), pExecutionContext->StackCallingFrame()->GetInstructionPointer());
					return true;
				}
				pExecutionContext->DeleteVariable("#A");
			}
			else
			{
				pExecutionContext->_functionStore.DefineStart(_pListParser->GetItem(0), pParseErrors, pCommand->GetLineNumber());
			}

			return true;
		}

		return false;
	}

	bool DecodeExpression(ExecutionContext* pExecutionContext, ParseErrors* pParseErrors, Command* pCommand, IExecutionFlow* pExecutionFlow)
	{
		StackWatcher::Log("CommandDecoder::DecodeExpression");
		Profiler.Start("DecodeExpression");
		pExecutionContext->Evaluate(pCommand->GetString(), pParseErrors, pCommand->GetLineNumber(), pExecutionFlow);

		return true;
	}

	bool DecodeReturn(ExecutionContext* pExecutionContext, ParseErrors* pParseErrors, Command* pCommand, IExecutionFlow* pExecutionFlow)
	{
		Profiler.Start("DecodeReturn");
		if (pCommand->StartsWith("RETURN"))
		{
			const char* pCommandString = pCommand->GetString() + 7;

			Variable result = pExecutionContext->Evaluate(pCommandString, pParseErrors, pCommand->GetLineNumber(), pExecutionFlow);

			pExecutionContext->AddVariableAndSet("<ReturnValue>", &result);

			return true;
		}

		return false;
	}

	bool DecodeWhitespaceOrCommentCommand(ExecutionContext* pExecutionContext, ParseErrors* pParseErrors, Command* pCommand, IExecutionFlow* pExecutionFlow)
	{
		Profiler.Start("DecodeWhitespaceOrCommentCommand");

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
    void Decode(ExecutionContext* pExecutionContext, ParseErrors* pParseErrors, Command* pCommand, IExecutionFlow* pExecutionFlow = 0)
    {
		//Serial.print(pCommand->GetLineNumber()); Serial.print(" "); Serial.println(pCommand->GetString());

		if (DecodeWhitespaceOrCommentCommand(pExecutionContext, pParseErrors, pCommand, pExecutionFlow)) { return; }

		if (DecodeFunction(pExecutionContext, pParseErrors, pCommand, pExecutionFlow)) { return; }

		if (DecodeFor(pExecutionContext, pParseErrors, pCommand, pExecutionFlow)) { return; }

		if (DecodeEndFor(pExecutionContext, pParseErrors, pCommand, pExecutionFlow)) { return; }

		if (DecodeIf(pExecutionContext, pParseErrors, pCommand, pExecutionFlow)) { return; }

		if (DecodeReturn(pExecutionContext, pParseErrors, pCommand, pExecutionFlow)) { return; }

		if (DecodeExpression(pExecutionContext, pParseErrors, pCommand, pExecutionFlow)) { return; }
	}
};