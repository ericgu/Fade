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

	LedCommand GetNextLedCommand()
	{
		while (true)
		{
			Command command = _pCommandSource->GetNextCommand();

			CommandResult commandResult = CommandDecoder::Decode(_executionContext, command);

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
					break;
			}
		}
	}
};