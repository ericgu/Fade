#pragma once

class LedFadeLibrary : public ILedDevice
{
    int _ledGroupNumber;
    LedType _ledType;
    int _ledCount;
    int _pin1;
    int _pin2;
    int _pin3;
    int _pin4;

    int _pixelCount;
    int _pixelPin;

public:
    LedFadeLibrary(LedType ledType, int ledCount, int pin1, int pin2, int pin3, int pin4)
    {
        _ledType = ledType;
        _ledCount = ledCount;
        _pin1 = pin1;
        _pin2 = pin2;
        _pin3 = pin3;
        _pin4 = pin4;
        _ledGroupNumber = -1;
    }

    void SetLedGroupNumber(int ledGroupNumber)
    {
        _ledGroupNumber = ledGroupNumber;
    }

    void UpdateLed(int channel, Variable* pBrightness)
    {
        if (_pLedUpdatedCallback)
        {
            _pLedUpdatedCallback(
                _ledGroupNumber,
                channel,
                pBrightness->GetValueCount(),
                pBrightness->GetValueFloat(0),
                pBrightness->GetValueFloat(1),
                pBrightness->GetValueFloat(2),
                pBrightness->GetValueFloat(3));

            if (Environment.DebugLogLedUpdates)
            {
                Serial.print("Led: ");

                Serial.print(_ledGroupNumber);
                Serial.print(", ");
                Serial.print(channel);
                Serial.print(", ");
                Serial.print(pBrightness->GetValueCount());
                Serial.print(", ");
                Serial.print(pBrightness->GetValueFloat(0));
                Serial.print(", ");
                Serial.print(pBrightness->GetValueFloat(1));
                Serial.print(", ");
                Serial.print(pBrightness->GetValueFloat(2));
                Serial.print(", ");
                Serial.print(pBrightness->GetValueFloat(3));
                Serial.println();
            }
        }
    }

    void Show()
    {
    }

    virtual void FreeDevice()
    {
    }
};