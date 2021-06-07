class FakeButton: public IButton
{
    int _status = 0;
public:

    virtual bool GetButtonStatus()
    {
        if (_status == 1)
        {
            _status = 0;
            return 1;
        }

        return 0;
    }

    void SetStatus(int status)
    {
        _status = status;
    }
};



class ButtonCreator : public IButtonCreator
{
    static int _buttonCount;
    static FakeButton* _pButtons[10];

    public:
    virtual IButton *Create(const char *pButtonType, int pin, int parameter1)
    {
        FakeButton* pButton = new FakeButton();
        _pButtons[_buttonCount] = pButton;
        _buttonCount++;
        return pButton;
    }

    static void ResetButtons()
    {
        _buttonCount = 0;
    }

    static void PushButton(int buttonNumber)
    {
        if (buttonNumber >= 0 && buttonNumber < _buttonCount)
        {
            _pButtons[buttonNumber]->SetStatus(1);
        }
    }
};

int ButtonCreator::_buttonCount = 0;
FakeButton* ButtonCreator::_pButtons[10];