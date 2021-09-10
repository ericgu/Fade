
class LedServoEsp32 : public ILedDevice
{
    const int PwmFrequency = 50;
    const int PwmBits = 14;
    const int PwmMax = 16384;

    const int ServoMaxCount = 1638;
    const int ServoRange = 819;

    int _pinCount;
    int _pinNumbers[4];
    int _channelNumbers[4];

    // static int _nextChannelToUse; // Shared with LedPwmEsp32 since they use the same hardware;

public:
    LedServoEsp32(int pinCount, int pins[16])
    {
        Serial.println(">LedServo constructor");

        _pinCount = pinCount;

        for (int pin = 0; pin < _pinCount; pin++)
        {
            _pinNumbers[pin] = pins[pin];
        }

        for (int i = 0; i < pinCount; i++)
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
        // For servos, the pulse size ranges from 1 mS at one end to 2 mS at the other end.
        // That corresponds to a pulse width of 1/20 to 1/10th, or at 14 bits of resolution,
        // 16384 / 20 = 819 to 16384 / 10 = 1638.
        // That gives 819 steps, where each step is 0.44 degrees.
        //
        // Just to make things more fun, the mapping is backwards = a longer pulse means counter-clockwise and a short pulse means clockwise.
        //
        // We will map the brightness value into that range, inverting it to convert to clockwise = bigger.

        int servoPwmValue = ServoMaxCount - pBrightness->GetValueFloat(0) * ServoRange;

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
