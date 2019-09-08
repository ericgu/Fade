enum class CommandResult
{
	CommandNone = 0,
	CommandExecute = 1,
	CommandSkipToNext = 2,
	CommandEndOfLoop = 3,
	CommandExitLoopBody = 4
};

class ExecutionContext
{
public:
	ExecutionContext()
	{
		_commandResult = CommandResult::CommandNone;
	}

	CommandResult _commandResult;

	BrightnessTarget _brightnessTarget;

	VariableCollection _variables;

	Stack _stack;
};