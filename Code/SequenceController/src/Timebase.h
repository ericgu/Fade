

class Timebase
{
    ILedManager* _pLedManager;
    int _currentCount;
	ExecutionFlow _executionFlow;

    public:
        Timebase(ICommandSource* pCommandSource, ILedManager* pLedManager) :
			_executionFlow(pCommandSource)
        {
            _pLedManager = pLedManager;
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
					_executionFlow.GetParseErrors()->AddError(">> Target count exceeded: Did you forget an animate command? <<", "", 0);
				}

				if (_executionFlow.GetParseErrors()->GetErrorCount() != 0)
				{
					return;
				}

				_currentCount = ledCommand._commandResult.GetCycleCount();
				_pLedManager->SetDelta(ledCommand._commandResult);
	        }

			_pLedManager->Tick();
			_currentCount--;
        }

		ParseErrors* GetParseErrors()
		{
			return _executionFlow.GetParseErrors();
		}
};