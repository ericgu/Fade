class TouchButton : public IButton
{
    int _pin;
    int _threshold;

    int _cycleCount;
    int _hitCount;

    volatile int _touchTrue = 0;
    int _trueCount = 0;
    int _falseCount = 0;

    static TouchButton *_pInstances[10];
    static int _instanceCount;

    // member function...
    void InterruptHandler()
    {
        _touchTrue = 1;
    }

public:
    static void InterruptHandler0()
    {
        _pInstances[0]->InterruptHandler();
    }

    TouchButton(int pin, int threshold)
    {
        _pin = pin;
        _threshold = threshold;
        _cycleCount = 0;
        _hitCount = 0;

        _pInstances[_instanceCount] = this;
        _instanceCount++;
        touchAttachInterrupt(pin, InterruptHandler0, threshold);
    }

    virtual bool GetButtonStatus()
    {
        // behavior
        // on interrupt, go into "touched" mode.
        // If touch count == n, result is true.
        // stay in touch mode until interrupt touch value is false.

        //Serial.print("Interrupt: ");
        //Serial.print(_touchTrue);
        //Serial.print(" ");
        //Serial.print(_trueCount);
        //Serial.print(" ");
        //Serial.println(_falseCount);

        if (_touchTrue)
        {
            _touchTrue = false;
            _trueCount++;
            _falseCount = 0;

            if (_trueCount == 3)
            {
                return true;
            }
        }
        else
        {
            _falseCount++;

            if (_falseCount == 3)
            {
                _trueCount = 0;
            }
        }

        // int v = touchRead(_pin);
        // int value = touchRead(_pin);
        // Serial.print("Button: ");
        // Serial.print(v);
        // Serial.print(" ");
        // Serial.println(value);

        return false;

        //if (value < _threshold)

        //if (_interruptCount >= 5)
        //{
        //_interruptCount = 0;
        //return 1;
        //Serial.print(_hitCount);
        // Serial.print(" ");
        //Serial.println(_cycleCount);
        //_hitCount++;
        //_cycleCount = 0;
        //Serial.print("Button: ");
        //Serial.println(value);
        //}
        //_cycleCount++;
        return 0;
        //return value < _threshold;
    }
};

int TouchButton::_instanceCount = 0;
TouchButton *TouchButton::_pInstances[10];