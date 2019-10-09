enum class CommandResultStatus
{
	CommandNone = 0,
	CommandExecute = 1,
	CommandSkipToNext = 2,
	CommandEndOfLoop = 3,
	CommandExitLoopBody = 4,
	CommandLoopMatched = 5,
	CommandTargetCountExceeded = 6
};

#define TargetMax 16

class CommandResult
{
    LedState _targets[TargetMax];
    int _targetCount = 0;
	CommandResultStatus _status;
	int _cycleCount = 0;
	bool _targetCountExceeded = false;

    public:
		CommandResult()
		{
			_status = CommandResultStatus::CommandNone;
		}
 
        void AddTarget(LedState ledState)
        {
			if (_targetCount == TargetMax)
			{
				_targetCountExceeded = true;
				return;
			}
				
			_targets[_targetCount] = ledState;
            _targetCount++;
        }

        LedState GetTarget(int index)
        {
            return _targets[index];
        }

        int GetCount() { return _targetCount;}

		CommandResultStatus GetStatus() { return _status; }
		void SetStatus(CommandResultStatus status) { _status = status; }

		int HasStatus() { return _status != CommandResultStatus::CommandNone; }

		int GetCycleCount() { return _cycleCount; }
		void SetCycleCount(int cycleCount) { _cycleCount = cycleCount; }

		bool GetTargetCountExceeded() { return _targetCountExceeded; }
};