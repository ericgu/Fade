
typedef void (*TimebaseCallback)();

class Timebase : public ILedMessageHandler
{
	static const int UpdateRateInMicroseconds = 10000; // 10 mS => 100 Hz

	ILedManager *_pLedManager;
	int _currentCount;
	ExecutionFlow _executionFlow;
	ParseErrors *_pParseErrors;
	static Timebase *_pTimebase;
	TimebaseCallback _timebaseCallback;
	TimeServices _timeServices;
	Delayer *_pDelayer;
	int _lastMicros;
	volatile long int _executionCount = 0;

public:
	Timebase(ICommandSource *pCommandSource, ILedManager *pLedManager, ParseErrors *pParseErrors, TimebaseCallback timebaseCallback) : _executionFlow(pCommandSource, pParseErrors, this)
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
	static void ExecuteLedCommand(CommandResult *pCommandResult)
	{
		Profiler.Start("Timebase::ExecuteLedCommand");
		Profiler.StartBig("ExecuteLedCommandMember");
		_pTimebase->ExecuteLedCommandMember(pCommandResult);
		Profiler.StartBig("Other");
	}

	void Configure(const char *pLedType, int ledTypeLength, int ledCount)
	{
		_pLedManager->Configure(pLedType, ledTypeLength, ledCount);
	}

	void TaskDelay()
	{
		_timeServices.TaskDelay(1);
	}

	int _lastCycleStart = -1;
	int _longestCycleTime = -1;

	void ExecuteLedCommandMember(CommandResult *pCommandResult)
	{
		//return;
		if (_lastCycleStart != -1)
		{
			int now = _timeServices.GetTicks();
			int delta = now - _lastCycleStart;
			if (delta > _longestCycleTime)
			{
				_longestCycleTime = delta;
				//Serial.print("Delta: ");
				//Serial.println(_longestCycleTime);
			}
		}

		_currentCount = pCommandResult->GetCycleCount();
		_pLedManager->SetDelta(pCommandResult);

		pCommandResult->Reset();

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

		_lastCycleStart = _timeServices.GetTicks();
	}

	bool RunProgram(const char *pCommand)
	{
		if (_pDelayer == 0)
		{
			_pDelayer = new Delayer();
			_pDelayer->Snapshot(_timeServices.GetTicks(), UpdateRateInMicroseconds);
		}

		_executionFlow.RunProgram(pCommand);
		if (_executionFlow.IsAborting())
		{
			_pParseErrors->AddError(">> Aborted <<", "", 0); // tells supervisor to stop executing...
			_executionFlow.ResetProgramState();
			_executionFlow.GetCommandResult()->SetStatus(CommandResultStatus::CommandNone);
			Serial.println("Timebase::RunProgram -> Abort");
			return false;
		}

		return true;
	}

	void ResetExecutionState()
	{
		_executionFlow.ResetProgramState();
		_pLedManager->ResetState();
		_executionCount = 0;
		_executionFlow.ClearAbort();
		_lastCycleStart = -1;
		_longestCycleTime = -1;
	}
};

Timebase *Timebase::_pTimebase;