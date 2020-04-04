class IExecutionFlow
{
public:
	virtual CommandResultStatus RunProgram(int runCount = -1) = 0;
	virtual	void ExecuteLedCommand(CommandResult* pCommandResult) = 0;
	virtual CommandResult* GetCommandResult() = 0;
	virtual Command* GetCommand(int commandNumber) = 0;
};