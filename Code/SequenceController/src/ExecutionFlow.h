class ExecutionFlow : public IExecutionFlow
{
    ICommandSource *_pCommandSource;
    ExecutionContext *_pExecutionContext;
    ParseErrors *_pParseErrors;
    ILedMessageHandler *_pLedMessageHandler;
    CommandResult *_pCommandResult;

public:
    ExecutionFlow(ICommandSource *pCommandSource, ParseErrors *pParseErrors, ILedMessageHandler *pLedMessageHandler)
    {
        _pCommandSource = pCommandSource;
        _pParseErrors = pParseErrors;
        _pLedMessageHandler = pLedMessageHandler;
        _pExecutionContext = new ExecutionContext();
        _pCommandResult = new CommandResult(16);
    }

    ~ExecutionFlow()
    {
        delete _pExecutionContext;
        delete _pCommandResult;
    }

    void ResetProgramState()
    {
        _pExecutionContext->ResetVariablesAndStack();
        _pCommandResult->Reset();
    }

    void AbortExecution()
    {
        _pCommandResult->Abort();
    }

    bool IsAborting()
    {
        return _pCommandResult->GetAbort();
    }

    void ClearAbort()
    {
        _pCommandResult->ClearAbort();
    }

    void ExecuteLedCommand(CommandResult *pCommandResult)
    {
        _pLedMessageHandler->ExecuteLedCommandMember(pCommandResult);
    }

    void ConfigureLeds(const char *pLedType, int ledCount, int pin)
    {
        delete _pCommandResult;
        _pCommandResult = new CommandResult(ledCount);
        _pLedMessageHandler->Configure(pLedType, ledCount, pin);
    }

    ExecutionContext *GetExecutionContext()
    {
        return _pExecutionContext;
    }

    CommandResult *GetCommandResult()
    {
        return _pCommandResult;
    }

    Command *GetCommand(int commandNumber)
    {
        return _pCommandSource->GetCommand(commandNumber);
    }

    bool RunProgram(const char *pCommand)
    {
        //Serial.println("ExecutionFlow::RunProgram");

        _pExecutionContext->Evaluate(pCommand, _pParseErrors, 0, this);

        return true;
    }
};