class ExecutionFlow : public IExecutionFlow
{
    static const int MaxButtons = 4;

    ICommandSource *_pCommandSource;
    ExecutionContext *_pExecutionContext;
    ParseErrors *_pParseErrors;
    ILedMessageHandler *_pLedMessageHandler;
    CommandResult *_pCommandResult;
    IButton* _pButtons[MaxButtons];
    int      _buttonCount;
    bool _breaking;

public:
    ExecutionFlow(ICommandSource *pCommandSource, ParseErrors *pParseErrors, ILedMessageHandler *pLedMessageHandler)
    {
        _pCommandSource = pCommandSource;
        _pParseErrors = pParseErrors;
        _pLedMessageHandler = pLedMessageHandler;
        _pExecutionContext = new ExecutionContext();
        _pCommandResult = new CommandResult(16);
        _buttonCount = 0;
        _breaking = false;
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

    void BreakExecution()
    {
        _breaking = true;
    }

    bool IsBreaking()
    {
        return _breaking;
    }

    void ClearBreak()
    {
        _breaking = false;
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

    void AddButton(IButton* pButton)
    {
        if (_buttonCount == 4)
        {
            return;
        }

        _pButtons[_buttonCount] = pButton;
        _buttonCount++;
    }

    virtual bool GetButtonState(int buttonNumber)
    {
        if (buttonNumber < 0 || buttonNumber >= _buttonCount)
        {
            return false;
        }

        return _pButtons[buttonNumber]->GetButtonStatus();
    }

    virtual int GetButtonCount()
    {
        return _buttonCount;
    }

};