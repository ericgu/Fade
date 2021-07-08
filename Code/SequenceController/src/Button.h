class Button : public IButton
{
    int _pin;
    int _threshold;

    volatile int _pressedTrue = 0;
    bool _activeHigh = 0;

public:
    Button(int pin, bool activeHigh)
    {
        _pin = pin;
        _activeHigh = activeHigh;

        pinMode(pin, _activeHigh ? INPUT_PULLDOWN : INPUT_PULLUP);
    }

    virtual void PressButton()
    {
        _pressedTrue = 1;
    }

    // NOTE: Active high doesn't seem to work, but active low does.
    virtual bool GetButtonStatus()
    {
        if (_pressedTrue != 0 || digitalRead(_pin) == _activeHigh ? HIGH : LOW)
        {
            _pressedTrue = 0;
            return true;
        }

        return false;
    }
};
