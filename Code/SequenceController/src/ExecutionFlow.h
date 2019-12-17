class ExecutionFlow
{
	ICommandSource* _pCommandSource;
	ExecutionContext _executionContext;
	ParseErrors* _pParseErrors;

	LedCommand CommandExecute(CommandResult commandResult)
	{
		return LedCommand(commandResult);
	}

	void CommandEndOfLoop(Command command)
	{
		int serialNumber = _executionContext._stack.GetTopFrame()->SerialNumberStart;
		_executionContext._stack.GetTopFrame()->SerialNumberEnd = command.GetSerialNumber();

		_executionContext._stack.GetTopFrame()->InstructionPointer = serialNumber;
	}

	void CommandExitLoopBody()
	{
		int serialNumber = _executionContext._stack.GetTopFrame()->SerialNumberEnd;
		//Serial.print("Exit loop body: "); Serial.println(serialNumber);

		_executionContext._stack.DestroyFrame();
		_executionContext._stack.GetTopFrame()->InstructionPointer = serialNumber + 1;
	}

public:
	ExecutionFlow(ICommandSource* pCommandSource, ParseErrors* pParseErrors)
	{
		_pCommandSource = pCommandSource;
		_pParseErrors = pParseErrors;
	}

	void ResetProgramState()
	{
		_executionContext.ResetVariablesAndStack();
	}

	ExecutionContext GetExecutionContext()
	{
		return _executionContext;
	}

	LedCommand GetNextLedCommand()
	{
		CommandResult commandResult;

		while (true)
		{
			Command command = _pCommandSource->GetCommand(_executionContext._stack.GetTopFrame()->InstructionPointer);
			_executionContext._stack.GetTopFrame()->InstructionPointer++;

			if (_pParseErrors->GetErrorCount() != 0)
			{
				return LedCommand(commandResult);
			}

			if (command.GetSerialNumber() == -1)
			{
				if (_executionContext._stack.GetFrameCount() > 1)
				{
					//Serial.println("Stack not empty at end of program");
					//Serial.println(_executionContext._stack.GetFrameCount());
					//StackFrame frame = _executionContext._stack.GetTopFrame();
					//Serial.println(frame.SerialNumberStart);
					//Serial.println(frame.SerialNumberEnd);

					_pParseErrors->AddError("Missing loop end", "", -1);
					return LedCommand(commandResult);
				}

				_executionContext._stack.GetTopFrame()->InstructionPointer = 0;
				command = _pCommandSource->GetCommand(_executionContext._stack.GetTopFrame()->InstructionPointer);
			}

			CommandDecoder::Decode(_executionContext, _pParseErrors, &command, commandResult);
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