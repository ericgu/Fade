class ExecutionFlow
{
	ICommandSource* _pCommandSource;
	ExecutionContext _executionContext;

	LedCommand CommandExecute(CommandResult commandResult)
	{
		return LedCommand(commandResult);
	}

	void CommandEndOfLoop(Command command)
	{
		int serialNumber = _executionContext._stack.GetTopFrame().SerialNumberStart;
		_executionContext._stack.GetTopFrame().SerialNumberEnd = command.GetSerialNumber();

		_pCommandSource->SetCommandToSerialNumber(serialNumber);
	}

	void CommandExitLoopBody()
	{
		int serialNumber = _executionContext._stack.GetTopFrame().SerialNumberEnd;

		_pCommandSource->SetCommandToSerialNumber(serialNumber); // point to endloop
		_executionContext._stack.DestroyFrame();
		_pCommandSource->GetNextCommand(); // skip to next statement...
	}

public:
	ExecutionFlow(ICommandSource* pCommandSource)
	{
		_pCommandSource = pCommandSource;
	}

	ParseErrors* GetParseErrors()
	{
		return &_executionContext._parseErrors;
	}

	LedCommand GetNextLedCommand()
	{
		CommandResult commandResult;

		while (true)
		{
			Command command = _pCommandSource->GetNextCommand();

			if (command.GetSerialNumber() == -1)
			{
				if (_executionContext._stack.GetFrameCount() != 0)
				{
					_executionContext._parseErrors.AddError("Missing loop end", "", -1);
					return LedCommand(commandResult);
				}

				_pCommandSource->Reset();
				command = _pCommandSource->GetNextCommand();
			}

			CommandDecoder::Decode(_executionContext, command, commandResult);
			//Serial.print("Status: ");
			//Serial.println((int) commandResult.GetStatus());

			switch (commandResult.GetStatus())
			{
				case CommandResultStatus::CommandExecute:
					return CommandExecute(commandResult);

				case CommandResultStatus::CommandEndOfLoop:
					CommandEndOfLoop(command);
					break;

				case CommandResultStatus::CommandExitLoopBody:
					CommandExitLoopBody();
					break;

				case CommandResultStatus::CommandLoopMatched:
				case CommandResultStatus::CommandNone:
				case CommandResultStatus::CommandSkipToNext:
				case CommandResultStatus::CommandTargetCountExceeded:
					break;
			}

			if (commandResult.GetTargetCountExceeded())
			{
				commandResult.SetStatus(CommandResultStatus::CommandTargetCountExceeded);
				return CommandExecute(commandResult);
			}
		}
	}
};