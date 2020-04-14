class ExecutionFlow: public IExecutionFlow
{
	ICommandSource* _pCommandSource;
	ExecutionContext* _pExecutionContext;
	ParseErrors* _pParseErrors;
	ILedMessageHandler* _pLedMessageHandler;
	CommandResult* _pCommandResult;
	CommandDecoder* _pCommandDecoder;

public:
	ExecutionFlow(ICommandSource* pCommandSource, ParseErrors* pParseErrors, ILedMessageHandler* pLedMessageHandler)
	{
		_pCommandSource = pCommandSource;
		_pParseErrors = pParseErrors;
		_pLedMessageHandler = pLedMessageHandler;
		_pExecutionContext = new ExecutionContext();
		_pCommandResult = new CommandResult();
		_pCommandDecoder = new CommandDecoder();
	}

	~ExecutionFlow()
	{
		delete _pExecutionContext;
		delete _pCommandResult;
		delete _pCommandDecoder;
	}

	void ResetProgramState()
	{
		_pExecutionContext->ResetVariablesAndStack();
		_pCommandResult->SetStatus(CommandResultStatus::CommandNone);
	}

	void AbortExecution()
	{
		_pCommandResult->Abort();
	}

	bool IsAborting()
	{
		return _pCommandResult->GetAbort();
	}

	void ClearAbort()
	{
		_pCommandResult->ClearAbort();
	}

	void ExecuteLedCommand(CommandResult* pCommandResult)
	{
		_pLedMessageHandler->ExecuteLedCommandMember(pCommandResult);
	}

	void ConfigureLeds(const char* pLedType, int ledCount, int pin)
	{
		_pLedMessageHandler->Configure(pLedType, ledCount, pin);
	}

	ExecutionContext* GetExecutionContext()
	{
		return _pExecutionContext;
	}

	CommandResult* GetCommandResult()
	{
		return _pCommandResult;
	}

	Command* GetCommand(int commandNumber)
	{
		return _pCommandSource->GetCommand(commandNumber);
	}

	CommandResultStatus RunProgram(int runCount)
	{
		Profiler.Start("RunProgram");

		//StackWatcher::Log("ExecutionFlow::RunPrograma");
		int calls = 0;

		while (true)
		{
			Profiler.Start("GetCommand");
			//Serial.print("IP = "); Serial.println(_pExecutionContext->_stack.GetTopFrame()->GetInstructionPointer());
			Command* pCommand = GetCommand(_pExecutionContext->_stack.GetTopFrame()->GetInstructionPointer());
			StackWatcher::Log("ExecutionFlow::RunProgramb");

			if (_pParseErrors->GetErrorCount() != 0)
			{
				return CommandResultStatus::CommandParseError;
			}

			if (pCommand == 0)
			{
				if (_pExecutionContext->_stack.GetFrameCount() > 1)
				{
					_pParseErrors->AddError("Missing loop end", "", -1);
					return CommandResultStatus::CommandParseError;
				}

				calls++;
				if (calls == runCount && runCount != -1)
				{
					return CommandResultStatus::CommandCompleted;
				}

				_pExecutionContext->_stack.GetTopFrame()->SetInstructionPointer(0, "RunProgram reset to 0");
			}
			else
			{
				//Serial.println(pCommand->GetString()); Serial.flush();
				StackWatcher::Log("ExecutionFlow::RunProgramc");
				_pCommandDecoder->Decode(_pExecutionContext, _pParseErrors, pCommand, this);

				//_pExecutionContext->_variables.Dump();

				if (_pCommandResult->GetAbort())
				{
					return CommandResultStatus::CommandNone;
				}

				CommandResultStatus status = _pCommandResult->GetStatus();
				switch (status)
				{
					case CommandResultStatus::CommandExecute:
						if (_pLedMessageHandler != 0)
						{
							_pLedMessageHandler->ExecuteLedCommandMember(_pCommandResult);
							_pCommandResult->Reset();
							break;
						}
						break;

					case CommandResultStatus::CommandEndOfLoop:
					case CommandResultStatus::CommandExitLoopBody:
					case CommandResultStatus::CommandEndOfFunction:
					case CommandResultStatus::CommandElseIf:
					case CommandResultStatus::CommandElse:
					case CommandResultStatus::CommandEndIf:
						_pCommandResult->SetStatus(CommandResultStatus::CommandNone);
						return status;

					case CommandResultStatus::CommandLoopMatched:
					case CommandResultStatus::CommandNone:
					case CommandResultStatus::CommandSkipToNext:
					case CommandResultStatus::CommandTargetCountExceeded:
					case CommandResultStatus::CommandParseError:
					case CommandResultStatus::CommandCompleted:
						break;
				}

				if (_pCommandResult->GetTargetCountExceeded())
				{
					_pCommandResult->SetStatus(CommandResultStatus::CommandTargetCountExceeded);
					return _pCommandResult->GetStatus();
				}

				_pExecutionContext->_stack.GetTopFrame()->IncrementInstructionPointer("Run program increment");
			}
		}
	}
};