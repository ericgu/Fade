
class LedPwmEsp32 : public ILedDevice
{
    //const int ledPins[16] = { 2,  4,  5, 18, 19, 21, 22, 23,   // main row
    //                         14, 27, 26, 25, 33, 32, 12, 13};  // extension row

    const int PwmFrequency = 500;
    const int PwmBits = 8;
    const int PwmMax = 256;

    int _pinCount;
    int _pinNumbers[4];

public:
    LedPwmEsp32(int pinCount, int pin1, int pin2, int pin3, int pin4)
    {
        _pinCount = pinCount;
        _pinNumbers[0] = pin1;
        _pinNumbers[1] = pin2;
        _pinNumbers[2] = pin3;
        _pinNumbers[3] = pin4;

        for (int i = 0; i < pinCount; i++)
        {
            ledcSetup(i, PwmFrequency + i, PwmBits);
            ledcAttachPin(_pinNumbers[i], i);
        }
    }

    ~LedPwmEsp32()
    {
    }

    void UpdateLed(LedState ledState)
    {
        int brightnessPwmValue = PwmMax * ledState.GetBrightness()->GetValueFloat(0);

        ledcWrite(ledState.GetChannel(), brightnessPwmValue);
        if (ledState.GetChannel() == 100)
        {
            Serial.print("PWM: ");
            Serial.print(ledState.GetChannel());
            Serial.print(" ");
            Serial.print(ledState.GetBrightness()->GetValueFloat(0));
            Serial.print(": ");
            Serial.println(brightnessPwmValue);
        }
    }

    void Show()
    {
    }
};