

class Timebase
{
    ILedManager* _pLedManager;
    int _currentCount;
	ExecutionFlow _executionFlow;
	ParseErrors* _pParseErrors;

    public:
        Timebase(ICommandSource* pCommandSource, ILedManager* pLedManager, ParseErrors* pParseErrors) :
			_executionFlow(pCommandSource, pParseErrors)
        {
            _pLedManager = pLedManager;
			_pParseErrors = pParseErrors;
			_currentCount = 0;
        }

        void DoTick()
        {
			//Serial.println("Tick");
            if (_currentCount == 0)
            {
				LedCommand ledCommand = _executionFlow.GetNextLedCommand();
				if (ledCommand._commandResult.GetStatus() == CommandResultStatus::CommandTargetCountExceeded)
				{
					_pParseErrors->AddError(">> Target count exceeded: Did you forget an animate command? <<", "", 0);
				}

				if (_pParseErrors->GetErrorCount() != 0)
				{
					return;
				}

				_currentCount = ledCommand._commandResult.GetCycleCount();
				_pLedManager->SetDelta(ledCommand._commandResult);
	        }

			_pLedManager->Tick();
			_currentCount--;
        }

		void ResetExecutionState()
		{
			_executionFlow.ResetProgramState();
			_pLedManager->ResetState();
		}
};