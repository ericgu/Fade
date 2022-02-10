
class LedServoEsp32 : public ILedDevice
{
    const int PwmFrequency = 50;
    const int PwmBits = 14;
    const int PwmMax = 16384;

    const int ServoMaxCount = 1638;
    const int ServoRange = 819;

    int _pinCount;
    int _pinNumbers[16];
    int _channelNumbers[16];
    int _servoMin;
    int _servoMax;

    // static int _nextChannelToUse; // Shared with LedPwmEsp32 since they use the same hardware;

public:
    LedServoEsp32(int pinCount, int *pins)
    {
        Serial.println(">LedServo constructor");

        _servoMin = pins[0];
        _servoMax = pins[1];

        _pinCount = pinCount - 2;
        Serial.print("Pins: ");
        Serial.println(_pinCount);

        for (int pin = 0; pin < _pinCount; pin++)
        {
            _pinNumbers[pin] = pins[pin + 2];
        }

        for (int i = 0; i < _pinCount; i++)
        {
            _channelNumbers[i] = LedPwmEsp32::_nextChannelToUse + i;
            Serial.print("Assigning pin: ");
            Serial.print(_pinNumbers[i]);
            Serial.print(" to channel ");
            Serial.println(_channelNumbers[i]);

            if (_pinNumbers[i] != -1)
            {
                ledcSetup(_channelNumbers[i], PwmFrequency, PwmBits);
                ledcAttachPin(_pinNumbers[i], _channelNumbers[i]);
            }
        }
        LedPwmEsp32::_nextChannelToUse += pinCount;

        Serial.println("<LedServer constructor");
    }

    ~LedServoEsp32()
    {
        Serial.println("LedServo Free");
    }

    virtual void FreeDevice()
    {
    }

    void UpdateLed(int channel, Variable *pBrightness)
    {
        // Servo PWM runs at 50 Hz, or 20 mS per cycle.
        //
        // The first two parameters passed in are the min and max servo values.
        // They are mapped backwards - the min servo value is the maximum clockwise rotation and the max is the maximum
        // counter-clockwise rotation.
        //
        // We will map the brightness value into that range, inverting it to convert to clockwise = bigger.

        int servoPwmValue = _servoMax - pBrightness->GetValueFloat(0) * (_servoMax - _servoMin);

        // Translate the global channel number to the group channel number...

        ledcWrite(_channelNumbers[channel], servoPwmValue);
        //if (channel == 100)
        {
            //Serial.print("Servo: ");
            //Serial.print(_channelNumbers[channel]);
            //Serial.print(" ");
            //Serial.print(pBrightness->GetValueFloat(0));
            //Serial.print(": ");
            //Serial.print (servoPwmValue);
            //Serial.print(" ");
        }
    }

    void Show()
    {
    }
};
