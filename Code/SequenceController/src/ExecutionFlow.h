class ExecutionFlow: public IExecutionFlow
{
	ICommandSource* _pCommandSource;
	ExecutionContext _executionContext;
	ParseErrors* _pParseErrors;
	CommandResultCallback _callback;

public:
	ExecutionFlow(ICommandSource* pCommandSource, ParseErrors* pParseErrors, CommandResultCallback callback)
	{
		_pCommandSource = pCommandSource;
		_pParseErrors = pParseErrors;
		_callback = callback;
	}

	void ResetProgramState()
	{
		_executionContext.ResetVariablesAndStack();
	}

	void ExecuteLedCommand(CommandResult* pCommandResult)
	{
		_callback(pCommandResult);
	}

	ExecutionContext GetExecutionContext()
	{
		return _executionContext;
	}

	CommandResultStatus RunProgram(int runCount = -1)
	{
		CommandResult commandResult;

		int calls = 0;

		while (true)
		{
			Command command = _pCommandSource->GetCommand(_executionContext._stack.GetTopFrame()->InstructionPointer);
			_executionContext._stack.GetTopFrame()->InstructionPointer++;

			if (_pParseErrors->GetErrorCount() != 0)
			{
				return CommandResultStatus::CommandParseError;
			}

			if (command.GetSerialNumber() == -1)
			{
				if (_executionContext._stack.GetFrameCount() > 1)
				{
					_pParseErrors->AddError("Missing loop end", "", -1);
					return CommandResultStatus::CommandParseError;
				}

				calls++;
				if (calls == runCount && runCount != -1)
				{
					return CommandResultStatus::CommandCompleted;
				}

				_executionContext._stack.GetTopFrame()->InstructionPointer = 0;
			}
			else
			{
				CommandDecoder::Decode(_executionContext, _pParseErrors, &command, commandResult, _callback, this);

				switch (commandResult.GetStatus())
				{
					case CommandResultStatus::CommandExecute:
						if (_callback != 0)
						{
							_callback(&commandResult);
							commandResult = CommandResult();
							break;
						}
						break;

					case CommandResultStatus::CommandEndOfLoop:
					case CommandResultStatus::CommandExitLoopBody:
					case CommandResultStatus::CommandEndOfFunction:
						return commandResult.GetStatus();

					case CommandResultStatus::CommandLoopMatched:
					case CommandResultStatus::CommandNone:
					case CommandResultStatus::CommandSkipToNext:
					case CommandResultStatus::CommandTargetCountExceeded:
					case CommandResultStatus::CommandParseError:
					case CommandResultStatus::CommandCompleted:
						break;
				}

				if (commandResult.GetTargetCountExceeded())
				{
					commandResult.SetStatus(CommandResultStatus::CommandTargetCountExceeded);
					return commandResult.GetStatus();
				}
			}
		}
	}
};