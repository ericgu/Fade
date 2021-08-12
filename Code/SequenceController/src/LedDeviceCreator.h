class LedDeviceCreator : public ILedDeviceCreator
{
    char _ledType[64];
    int _ledCount;
    int _pin1;
    int _pin2;
    int _pin3;
    int _pin4;

public:
    ILedDevice *Create(const char *pLedType, int ledCount, int pin1, int pin2, int pin3, int pin4)
    {
        Serial.print("Creating: ");
        Serial.flush();
        Serial.print(pLedType);
        Serial.flush();
        Serial.print(" ");
        Serial.flush();
        Serial.print(ledCount);
        Serial.flush();
        Serial.print(" ");
        Serial.flush();
        Serial.print(pin1);
        Serial.flush();

        SafeString::StringCopy(_ledType, pLedType, sizeof(_ledType));
        _ledCount = ledCount;
        _pin1 = pin1;
        _pin2 = pin2;
        _pin3 = pin3;
        _pin4 = pin4;

        if (strcmp(pLedType, "RGB") == 0 || strcmp(pLedType, "WS2812"))
        {
            Serial.println("Creating RGB device");
            return new LedRGB(ledCount, pin1);
        }
        else if (strcmp(pLedType, "PWM") == 0)
        {
            Serial.println("Creating PWM device");
            return new LedPwmEsp32(ledCount, pin1, pin2, pin3, pin4);
        }

        return 0;
    }
};