

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
            if (_currentCount == 0)
            {
				LedCommand ledCommand = _executionFlow.GetNextLedCommand();

				_currentCount = ledCommand._commandResult.GetCycleCount();
				_pLedManager->SetDelta(ledCommand._commandResult);
	        }

			_pLedManager->Tick();
			_currentCount--;
        }
};