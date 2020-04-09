class LedDeviceCreator: public ILedDeviceCreator
{
    char _ledType[64];
    int _ledCount;
    int _pin;
    ILedDevice* _pCurrentLedDevice;

public:
    ILedDevice* Create(const char* pLedType, int ledCount, int pin)
    {
        if (strcmp(pLedType, _ledType) == 0 && ledCount == _ledCount && pin == _pin)
        {
            return 0;   // same device as before...
        }

        Serial.print("Creating: "); Serial.print(pLedType); Serial.print(" "); Serial.print(ledCount); Serial.print(" "); Serial.print(pin);
        strcpy(_ledType, pLedType);
        _ledCount = ledCount;
        _pin = pin; 

        delete _pCurrentLedDevice;

        if (strcmp(pLedType, "RGB") == 0)
        {
            _pCurrentLedDevice = new LedRGB(ledCount, pin);
            return _pCurrentLedDevice;
        }
        else if (strcmp(pLedType, "PWM") == 0)
        {
            _pCurrentLedDevice = new LedPwmEsp32();
            return _pCurrentLedDevice;
        }

        return 0;
    }
};