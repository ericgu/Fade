class ExecutionFlow
{
	ICommandSource* _pCommandSource;
	ExecutionContext _executionContext;

public:
	ExecutionFlow(ICommandSource* pCommandSource)
	{
		_pCommandSource = pCommandSource;
	}

	LedCommand GetNextLedCommand()
	{
		_executionContext._brightnessTarget.Clear();

		while (true)
		{
			Command command = _pCommandSource->GetNextCommand();
			//Serial.println(command.GetString());

			CommandDecoder::Decode(_executionContext, command);

			if (_executionContext._commandResult == CommandResult::CommandExecute)
			{
				return LedCommand(_executionContext._brightnessTarget, command.GetCount());
			}
			else if (_executionContext._commandResult == CommandResult::CommandEndOfLoop)
			{
				int serialNumber = _executionContext._stack.GetTopFrame().SerialNumberStart;
				_executionContext._stack.GetTopFrame().SerialNumberEnd = command.GetSerialNumber();

				_pCommandSource->SetCommandToSerialNumber(serialNumber);
			}
			else if (_executionContext._commandResult == CommandResult::CommandExitLoopBody)
			{
				int serialNumber = _executionContext._stack.GetTopFrame().SerialNumberEnd;

				_pCommandSource->SetCommandToSerialNumber(serialNumber); // point to endloop
				_executionContext._stack.DestroyFrame();
				_pCommandSource->GetNextCommand(); // skip to next statement...
			}
		}
	}
};