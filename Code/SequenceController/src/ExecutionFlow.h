class ExecutionFlow: public IExecutionFlow
{
	ICommandSource* _pCommandSource;
	ExecutionContext _executionContext;
	ParseErrors* _pParseErrors;
	CommandResultCallback _callback;
	CommandResult _commandResult;

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

	CommandResult* GetCommandResult()
	{
		return &_commandResult;
	}


	CommandResultStatus RunProgram(int runCount = -1)
	{
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
				CommandDecoder::Decode(_executionContext, _pParseErrors, &command, this);

				CommandResultStatus status = _commandResult.GetStatus();
				switch (status)
				{
					case CommandResultStatus::CommandExecute:
						if (_callback != 0)
						{
							_callback(&_commandResult);
							_commandResult = CommandResult();
							break;
						}
						break;

					case CommandResultStatus::CommandEndOfLoop:
					case CommandResultStatus::CommandExitLoopBody:
					case CommandResultStatus::CommandEndOfFunction:
						_commandResult.SetStatus(CommandResultStatus::CommandNone);
						return status;

					case CommandResultStatus::CommandLoopMatched:
					case CommandResultStatus::CommandNone:
					case CommandResultStatus::CommandSkipToNext:
					case CommandResultStatus::CommandTargetCountExceeded:
					case CommandResultStatus::CommandParseError:
					case CommandResultStatus::CommandCompleted:
						break;
				}

				if (_commandResult.GetTargetCountExceeded())
				{
					_commandResult.SetStatus(CommandResultStatus::CommandTargetCountExceeded);
					return _commandResult.GetStatus();
				}
			}
		}
	}
};