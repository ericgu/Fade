enum class CommandResultStatus
{
	CommandNone = 0,
	CommandExecute = 1,
	CommandSkipToNext = 2,
	CommandEndOfLoop = 3,
	CommandExitLoopBody = 4,
	CommandLoopMatched = 5,
	CommandTargetCountExceeded = 6,
	CommandParseError = 7,
	CommandCompleted = 8,
	CommandEndOfFunction = 9,
	CommandEndIf = 10,
	CommandElse = 11,
	CommandElseIf = 12
};

class CommandResult
{
	LedState *_targets = 0;
	int _targetAllocation;
	int _targetCount = 0;
	volatile CommandResultStatus _status;
	int _cycleCount = 0;
	bool _targetCountExceeded = false;
	volatile bool _aborting = false;

public:
	CommandResult() : CommandResult(16)
	{
	}

	CommandResult(int targetAllocation)
	{
		_targetAllocation = targetAllocation;
		_targets = new LedState[targetAllocation];

		Reset();
	}

	void Cleanup()
	{
		if (_targets)
		{
			delete[] _targets;
			_targets = 0;
		}
	}

	~CommandResult()
	{
		Cleanup();
	}

	void DeepCopy(const CommandResult &old)
	{
		Cleanup();

		_targetAllocation = old._targetAllocation;
		_targets = new LedState[_targetAllocation];

		_targetCount = old._targetCount;
		for (int i = 0; i < _targetAllocation; i++)
		{
			_targets[i] = old._targets[i];
		}
		_status = old._status;
		_cycleCount = old._cycleCount;
		_targetCountExceeded = old._targetCountExceeded;
		_aborting = old._aborting;
	}

	CommandResult(const CommandResult &old)
	{
		DeepCopy(old);
	}

	CommandResult &operator=(const CommandResult &source)
	{
		if (this == &source)
		{
			return *this;
		}

		DeepCopy(source);

		return *this;
	}

	void Abort()
	{
		_aborting = true;
	}

	void ClearAbort()
	{
		_aborting = false;
	}

	bool GetAbort()
	{
		return _aborting;
	}

	void Reset()
	{
		_status = CommandResultStatus::CommandNone;
		_targetCount = 0;
		_cycleCount = 0;
		_targetCountExceeded = false;
	}

	void AddTarget(LedState ledState)
	{
		if (_targetCount == _targetAllocation)
		{
			_targetCountExceeded = true;
			Serial.println("LED index beyond configured length referenced.");
			Serial.println(_targetCount);
			Serial.println(_targetAllocation);
			//*((int *)0) = 15; // force a crash

			return;
		}

		_targets[_targetCount] = ledState;
		_targetCount++;
	}

	LedState GetTarget(int index)
	{
		return _targets[index];
	}

	int GetCount() { return _targetCount; }

	CommandResultStatus GetStatus() { return _status; }
	void SetStatus(CommandResultStatus status) { _status = status; }

	int HasStatus() { return _status != CommandResultStatus::CommandNone; }

	int GetCycleCount() { return _cycleCount; }
	void SetCycleCount(int cycleCount) { _cycleCount = cycleCount; }

	bool GetTargetCountExceeded() { return _targetCountExceeded; }
};

typedef void (*CommandResultCallback)(CommandResult *pCommandResult);
