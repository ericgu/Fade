
typedef void(*TimebaseCallback)();

class Timebase: public ILedMessageHandler
{
	static const int UpdateRateInMicroseconds = 10000;		// 10 mS => 100 Hz

    ILedManager* _pLedManager;
    int _currentCount;
	ExecutionFlow _executionFlow;
	ParseErrors* _pParseErrors;
	static Timebase* _pTimebase;
	TimebaseCallback _timebaseCallback;
	TimeServices _timeServices;
	Delayer* _pDelayer;
	int _lastMicros;
	volatile long int _executionCount = 0;


    public:
        Timebase(ICommandSource* pCommandSource, ILedManager* pLedManager, ParseErrors* pParseErrors, TimebaseCallback timebaseCallback) :
			_executionFlow(pCommandSource, pParseErrors, this)
        {
            _pLedManager = pLedManager;
			_pParseErrors = pParseErrors;
			_timebaseCallback = timebaseCallback;
			_currentCount = 0;
			_pTimebase = this;
			_pDelayer = 0;
        }

		virtual ~Timebase()
		{

		}

		void Abort()
		{
			_executionFlow.AbortExecution();
		}

		long int GetExecutionCount()
		{
			return _executionCount;
		}

		// Callback when there is an animation command to execute.
		static void ExecuteLedCommand(CommandResult* pCommandResult)
		{
			Profiler.Start("Timebase::ExecuteLedCommand");
			Profiler.StartBig("ExecuteLedCommandMember");
			_pTimebase->ExecuteLedCommandMember(pCommandResult);
			Profiler.StartBig("Other");

		}

		void Configure(const char* pLedType, int ledTypeLength, int ledCount)
		{
			_pLedManager->Configure(pLedType, ledTypeLength, ledCount);
		}

		void TaskDelay()
		{
			_timeServices.TaskDelay(1);
		}

		void ExecuteLedCommandMember(CommandResult* pCommandResult)
		{
			_currentCount = pCommandResult->GetCycleCount();
			_pLedManager->SetDelta(pCommandResult);

			while (_currentCount > 0)
			{
				_timeServices.TaskDelay(1); // this allows other tasks to run and keeps the watchdog timer happy.
				_executionCount++;
				_pLedManager->Tick();
				_currentCount--;
				if (_timebaseCallback)
				{
					(*_timebaseCallback)();
					//int mf = _timeServices.GetTicks();

					while (!_pDelayer->CheckIfDone(_timeServices.GetTicks()))
					{
						_timeServices.DelayMicroseconds(50);
					}

					_pDelayer->Snapshot(_timeServices.GetTicks(), UpdateRateInMicroseconds);
					//int m = _timeServices.GetTicks();
					//Serial.print("Micros: "); Serial.print(m - _lastMicros); Serial.print(" w "); Serial.println(m-mf);
					//_lastMicros = m;
				}
			}
		}

        void RunProgram(const char* pCommand)
        {
            if (_pDelayer == 0)
            {
                _pDelayer = new Delayer();
                _pDelayer->Snapshot(_timeServices.GetTicks(), UpdateRateInMicroseconds);
            }

            _executionFlow.RunProgram(pCommand);
        }

#if fred
        void RunProgram(int runCount)
        {
			if (_pDelayer == 0)
			{
				_pDelayer = new Delayer();
				_pDelayer->Snapshot(_timeServices.GetTicks(), UpdateRateInMicroseconds);
			}

  			StackWatcher::Log("Timebase::RunProgram");			
			_executionFlow.ResetProgramState();

			CommandResultStatus commandResultStatus = _executionFlow.RunProgram(runCount);
			if (_executionFlow.IsAborting())
			{
				_pParseErrors->AddError(">> Aborted <<", "", 0);	// tells supervisor to stop executing...
				_executionFlow.ResetProgramState();
				_executionFlow.GetCommandResult()->SetStatus(CommandResultStatus::CommandNone);
				Serial.println("Timebase::RunProgram -> Abort");
				return;
			}

			if (commandResultStatus == CommandResultStatus::CommandTargetCountExceeded)
			{
				_pParseErrors->AddError(">> Target count exceeded: Did you forget an animate command? <<", "", 0);
			}

			if (_pParseErrors->GetErrorCount() != 0)
			{
				return;
			}
        }
#endif

		void ResetExecutionState()
		{
			_executionFlow.ResetProgramState();
			_pLedManager->ResetState();
			_executionCount = 0;
			_executionFlow.ClearAbort();
		}
};

Timebase* Timebase::_pTimebase;