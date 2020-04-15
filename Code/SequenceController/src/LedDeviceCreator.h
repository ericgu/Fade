class LedDeviceCreator: public ILedDeviceCreator
{
    char _ledType[64];
    int _ledCount;
    int _pin;
    ILedDevice* _pCurrentLedDevice = 0;

public:
    ILedDevice* Create(const char* pLedType, int ledCount, int pin)
    {
        if (strcmp(pLedType, _ledType) == 0 && ledCount == _ledCount && pin == _pin)
        {
            //Serial.println("Same device");
            return 0;   // same device as before...
        }

        //Serial.print("Creating: "); Serial.flush();
        //Serial.print(pLedType);  Serial.flush();
        //Serial.print(" ");  Serial.flush();
        //Serial.print(ledCount);  Serial.flush();
        //Serial.print(" ");  Serial.flush();
        //Serial.print(pin); Serial.flush();

        SafeString::StringCopy(_ledType, pLedType, sizeof(_ledType));
        _ledCount = ledCount;
        _pin = pin; 

        if (_pCurrentLedDevice)
        {
            //Serial.println("delete"); Serial.flush();
            delete _pCurrentLedDevice;
        }

        if (strcmp(pLedType, "RGB") == 0)
        {
            Serial.println("Creating RGB device");
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