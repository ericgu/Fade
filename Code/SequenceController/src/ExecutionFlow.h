class ExecutionFlow : public IExecutionFlow
{
    static const int MaxButtons = 4;

    ICommandSource *_pCommandSource;
    ExecutionContext *_pExecutionContext;
    ParseErrors *_pParseErrors;
    ILedMessageHandler *_pLedMessageHandler;
    CommandResult *_pCommandResult;
    IButton *_pButtons[MaxButtons];
    int _buttonCount;
    bool _breaking;
    IButtonCreator *_pButtonCreator;

public:
    ExecutionFlow(ICommandSource *pCommandSource, ParseErrors *pParseErrors, ILedMessageHandler *pLedMessageHandler, IButtonCreator *pButtonCreator)
    {
        _pCommandSource = pCommandSource;
        _pParseErrors = pParseErrors;
        _pLedMessageHandler = pLedMessageHandler;
        _pExecutionContext = new ExecutionContext();
        _pCommandResult = new CommandResult(16);
        _pButtonCreator = pButtonCreator;
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

    void ConfigureLeds(int ledGroupNumber, const char *pLedType, int ledCount, int pin1, int pin2, int pin3, int pin4)
    {
        _pLedMessageHandler->Configure(ledGroupNumber, pLedType, ledCount, pin1, pin2, pin3, pin4);

        // TODO: get the led count from the message handler...
        delete _pCommandResult;
        _pCommandResult = new CommandResult(ledCount);
    }

    void ConfigureButton(int buttonNumber, const char *pButtonType, int pinNumber, int parameter1)
    {
        if (buttonNumber != _buttonCount)
        {
            return;
        }

        IButton *pButton = _pButtonCreator->Create(pButtonType, pinNumber, parameter1);

        if (pButton != 0)
        {
            _pButtons[_buttonCount] = pButton;
            _buttonCount++;
        }
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

    void AddButton(IButton *pButton)
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