class LedDeviceCreator : public ILedDeviceCreator
{
    char _ledType[64];
    int _ledCount;
    int _pin1;
    int _pin2;
    int _pin3;
    int _pin4;
    ILedDevice *_pCurrentLedDevice = 0;

public:
    ILedDevice *Create(const char *pLedType, int ledCount, int pin1, int pin2, int pin3, int pin4)
    {
        if (strcmp(pLedType, _ledType) == 0 && ledCount == _ledCount && pin1 == _pin1 && pin2 == _pin2 && pin3 == _pin3 && pin4 == _pin4)
        {
            //Serial.println("Same device");
            return 0; // same device as before...
        }

        //Serial.print("Creating: "); Serial.flush();
        //Serial.print(pLedType);  Serial.flush();
        //Serial.print(" ");  Serial.flush();
        //Serial.print(ledCount);  Serial.flush();
        //Serial.print(" ");  Serial.flush();
        //Serial.print(pin); Serial.flush();

        SafeString::StringCopy(_ledType, pLedType, sizeof(_ledType));
        _ledCount = ledCount;
        _pin1 = pin1;
        _pin2 = pin2;
        _pin3 = pin3;
        _pin4 = pin4;

        if (_pCurrentLedDevice)
        {
            //Serial.println("delete"); Serial.flush();
            delete _pCurrentLedDevice;
        }

        if (strcmp(pLedType, "RGB") == 0)
        {
            Serial.println("Creating RGB device");
            _pCurrentLedDevice = new LedRGB(ledCount, pin1);
            return _pCurrentLedDevice;
        }
        else if (strcmp(pLedType, "PWM") == 0)
        {
            _pCurrentLedDevice = new LedPwmEsp32(ledCount, pin1, pin2, pin3, pin4);
            return _pCurrentLedDevice;
        }

        return 0;
    }
};