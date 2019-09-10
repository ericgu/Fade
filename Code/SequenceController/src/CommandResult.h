enum class CommandResultStatus
{
	CommandNone = 0,
	CommandExecute = 1,
	CommandSkipToNext = 2,
	CommandEndOfLoop = 3,
	CommandExitLoopBody = 4,
	CommandLoopMatched = 5
};

class CommandResult
{
    LedState _targets[16];
    int _targetCount = 0;
	CommandResultStatus _status;

    public:
		CommandResult()
		{
			_status = CommandResultStatus::CommandNone;
		}
 
        void AddTarget(LedState ledState)
        {
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
};