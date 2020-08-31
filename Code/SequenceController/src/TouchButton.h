class TouchButton : public IButton
{
    int _pin;
    int _threshold;

public:
    TouchButton(int pin, int threshold)
    {
        _pin = pin;
        _threshold = threshold;
    }

    virtual bool GetButtonStatus()
    {
        int value = touchRead(_pin);
        if (value < _threshold)
        {
            Serial.print("Button: ");
            Serial.println(value);
        }
        return touchRead(_pin) < _threshold;
    }
};