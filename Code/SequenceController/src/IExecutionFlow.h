class IExecutionFlow
{
public:
    //virtual CommandResultStatus RunProgram(int runCount) = 0;
    virtual	void ExecuteLedCommand(CommandResult* pCommandResult) = 0;
    virtual CommandResult* GetCommandResult() = 0;
    virtual Command* GetCommand(int commandNumber) = 0;
    virtual void AbortExecution() = 0;
    virtual bool IsAborting() = 0;

    virtual bool ConfigureLeds(int ledGroupNumber, const char* pLedType, int ledCount, int pin1, int pin2, int pin3, int pin4) = 0;
    virtual void ConfigureButton(int buttonNumber, const char* pButtonType, int pinNumber, int parameter) = 0;
    virtual bool GetButtonState(int buttonNumber) = 0;
    virtual int GetButtonCount() = 0;

    virtual void BreakExecution() = 0;
    virtual bool IsBreaking() = 0;
    virtual void ClearBreak() = 0;
};