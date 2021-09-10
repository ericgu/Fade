#pragma once

class LedFadeLibrary : public ILedDevice
{
    int _ledGroupNumber;
    LedType _ledType;
    int _ledCount;

    int _pinCount;
    int _pins[16];

    int _pin1;
    int _pin2;
    int _pin3;
    int _pin4;

    int _pixelCount;
    int _pixelPin;

public:
    LedFadeLibrary(LedType ledType, int ledCount, int pinCount, int pins[16])
    {
        _ledType = ledType;
        _ledCount = ledCount;

        _pinCount = pinCount;

        for (int pin = 0; pin < pinCount; pin++)
        {
            _pins[pin] = pins[pin];
        }
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