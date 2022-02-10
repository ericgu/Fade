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
        Serial.print("Interrupt on pin: ");
        Serial.println(_pin);
        _touchTrue = 1;
    }

    const int touchPinNumbers[10] = {4, 0, 2, 15, 13, 12, 14, 27, 33, 32}; // T0 through T9

public:
    static void InterruptHandler0()
    {
        _pInstances[0]->InterruptHandler();
    }

    static void InterruptHandler1()
    {
        _pInstances[1]->InterruptHandler();
    }

    static void InterruptHandler2()
    {
        _pInstances[2]->InterruptHandler();
    }

    static void InterruptHandler3()
    {
        _pInstances[3]->InterruptHandler();
    }

    static void InterruptHandler4()
    {
        _pInstances[4]->InterruptHandler();
    }

    static void InterruptHandler5()
    {
        _pInstances[5]->InterruptHandler();
    }

    static void InterruptHandler6()
    {
        _pInstances[6]->InterruptHandler();
    }

    static void InterruptHandler7()
    {
        _pInstances[7]->InterruptHandler();
    }

    static void InterruptHandler8()
    {
        _pInstances[8]->InterruptHandler();
    }

    static void InterruptHandler9()
    {
        _pInstances[9]->InterruptHandler();
    }

    TouchButton(int pin, int threshold)
    {
        _pin = pin;
        _threshold = threshold;
        _cycleCount = 0;
        _hitCount = 0;

        _pInstances[_instanceCount] = this;

#if fred
        switch (_instanceCount)
        {
        case 0:
            touchAttachInterrupt(pin, InterruptHandler0, threshold);
            break;
        case 1:
            touchAttachInterrupt(pin, InterruptHandler1, threshold);
            break;
        case 2:
            touchAttachInterrupt(pin, InterruptHandler2, threshold);
            break;
        case 3:
            touchAttachInterrupt(pin, InterruptHandler3, threshold);
            break;
        case 4:
            touchAttachInterrupt(pin, InterruptHandler4, threshold);
            break;
        case 5:
            touchAttachInterrupt(pin, InterruptHandler5, threshold);
            break;
        case 6:
            touchAttachInterrupt(pin, InterruptHandler6, threshold);
            break;
        case 7:
            touchAttachInterrupt(pin, InterruptHandler7, threshold);
            break;
        case 8:
            touchAttachInterrupt(pin, InterruptHandler8, threshold);
            break;
        case 9:
            touchAttachInterrupt(pin, InterruptHandler9, threshold);
            break;
        }
#endif

        _instanceCount++;
    }

    virtual void PressButton()
    {
        _touchTrue = 1;
        _trueCount = 2;
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
        if (Environment.DebugLogTouchButtonValues)
        {
            for (int button = 0; button < 10; button++)
            {
                int value = touchRead(touchPinNumbers[button]);
                Serial.print(value);
                Serial.print(" ");
            }
            Serial.println();
        }

        int value = touchRead(_pin);
        //Serial.print("Touch value on pin: ");
        //Serial.print(_pin);
        //Serial.print(" ");
        //Serial.println(value);

        if (value < _threshold)
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