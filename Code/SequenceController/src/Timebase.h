
typedef void(*TimebaseCallback)();

class Timebase
{
	static const int UpdateRateInMicroseconds = 10000;		// 100 Hz

    ILedManager* _pLedManager;
    int _currentCount;
	ExecutionFlow _executionFlow;
	ParseErrors* _pParseErrors;
	static Timebase* _pTimebase;
	TimebaseCallback _timebaseCallback;
	TickSource _tickSource;
	Delayer* _pDelayer;
	int _lastMicros;

    public:
        Timebase(ICommandSource* pCommandSource, ILedManager* pLedManager, ParseErrors* pParseErrors, TimebaseCallback timebaseCallback) :
			_executionFlow(pCommandSource, pParseErrors, ExecuteLedCommand)
        {
            _pLedManager = pLedManager;
			_pParseErrors = pParseErrors;
			_timebaseCallback = timebaseCallback;
			_currentCount = 0;
			_pTimebase = this;
			_pDelayer = 0;
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
				if (_timebaseCallback)
				{
					(*_timebaseCallback)();
					int mf = micros();
					_pDelayer->Wait();
					_pDelayer->Snapshot(UpdateRateInMicroseconds);
					int m = micros();
					Serial.print("Micros: "); Serial.print(m - _lastMicros); Serial.print(" w "); Serial.println(m-mf);
					_lastMicros = m;
				}
			}
		}

        void RunProgram(int runCount)
        {
			if (_pDelayer == 0)
			{
				_pDelayer = new Delayer(&_tickSource);
				_pDelayer->Snapshot(UpdateRateInMicroseconds);
			}

			
			Serial.println("> Timebase::RunProgram");

  			StackWatcher::Log("Timebase::RunProgram");			
			_executionFlow.ResetProgramState();

			CommandResultStatus commandResultStatus = _executionFlow.RunProgram(runCount);
			if (commandResultStatus == CommandResultStatus::CommandTargetCountExceeded)
			{
				_pParseErrors->AddError(">> Target count exceeded: Did you forget an animate command? <<", "", 0);
			}
			Serial.println("< Timebase::RunProgram");

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