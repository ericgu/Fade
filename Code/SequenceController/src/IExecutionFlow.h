class IExecutionFlow
{
public:
	virtual CommandResultStatus RunProgram(int runCount) = 0;
	virtual	void ExecuteLedCommand(CommandResult* pCommandResult) = 0;
	virtual CommandResult* GetCommandResult() = 0;
	virtual Command* GetCommand(int commandNumber) = 0;
	virtual void AbortExecution() = 0;
	virtual bool IsAborting() = 0;

	virtual void ConfigureLeds(const char* pLedType, int ledCount, int pin) = 0;
};