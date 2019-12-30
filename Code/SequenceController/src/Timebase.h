

class Timebase
{
    ILedManager* _pLedManager;
    int _currentCount;
	ExecutionFlow _executionFlow;
	ParseErrors* _pParseErrors;
	static Timebase* _pTimebase;

    public:
        Timebase(ICommandSource* pCommandSource, ILedManager* pLedManager, ParseErrors* pParseErrors) :
			_executionFlow(pCommandSource, pParseErrors, ExecuteLedCommand)
        {
            _pLedManager = pLedManager;
			_pParseErrors = pParseErrors;
			_currentCount = 0;
			_pTimebase = this;
        }

		// Callback when there is an animation command to execute.
		static void ExecuteLedCommand(CommandResult* pCommandResult)
		{
			_pTimebase->ExecuteLedCommandMember(pCommandResult);
		}

		void ExecuteLedCommandMember(CommandResult* pCommandResult)
		{
			_currentCount = pCommandResult->GetCycleCount();
			_pLedManager->SetDelta(*pCommandResult);

			while (_currentCount > 0)
			{
				_pLedManager->Tick();
				_currentCount--;
			}
		}

        void RunProgram(int runCount)
        {
			_executionFlow.ResetProgramState();

			CommandResultStatus commandResultStatus = _executionFlow.RunProgram(runCount);
			if (commandResultStatus == CommandResultStatus::CommandTargetCountExceeded)
			{
				_pParseErrors->AddError(">> Target count exceeded: Did you forget an animate command? <<", "", 0);
			}

			if (_pParseErrors->GetErrorCount() != 0)
			{
				return;
			}
        }

		void ResetExecutionState()
		{
			_executionFlow.ResetProgramState();
			_pLedManager->ResetState();
		}
};

Timebase* Timebase::_pTimebase;