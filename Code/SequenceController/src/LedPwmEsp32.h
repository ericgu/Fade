
class LedPwmEsp32: public ILedPwm
{
    const int ledPins[16] = { 2,  4,  5, 18, 19, 21, 22, 23,   // main row
                             14, 27, 26, 25, 33, 32, 12, 13};  // extension row
    
    const int PwmFrequency = 500;
    const int PwmBits = 8;
    const int PwmMax = 256;

    public:
        LedPwmEsp32()
        {
            for (int i = 0; i < 16; i++)
            {
                ledcSetup(i, PwmFrequency + i, PwmBits);
                ledcAttachPin(ledPins[i], i);
            }
        }

        void UpdateLed(LedState ledState)
        {
            int brightnessPwmValue = PwmMax * ledState.GetBrightness();

            ledcWrite(ledState.GetChannel(), brightnessPwmValue);
        }
};