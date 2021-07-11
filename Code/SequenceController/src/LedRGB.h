#include <NeoPixelBus.h>

class LedRGB : public ILedDevice
{
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_pStrip;
    int _pixelCount;
    int _pixelPin;
    volatile bool _operating;

public:
    LedRGB(int pixelCount, int pixelPin)
    {
        Serial.println(">LedRGB constructor");
        Serial.flush();
        _pixelCount = pixelCount;
        _pixelPin = pixelPin;
        _pStrip = new NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>(_pixelCount, _pixelPin);
        _pStrip->Begin();
        Serial.println("<LedRGB constructor");
        Serial.flush();
        _operating = true;

        SetToBlack();
    }

    ~LedRGB()
    {
    }

    void SetToBlack()
    {
        RgbColor black(0, 0, 0);
        for (int pixel = 0; pixel < _pixelCount; pixel++)
        {
            _pStrip->SetPixelColor(pixel, black);
        }
        _pStrip->Show();
    }

    virtual void FreeDevice()
    {
        _operating = false;
        Serial.println("LedRGB Cleanup1");
        SetToBlack();
        Serial.println("LedRGB Cleanup2");
        delay(500);
        Serial.println("LedRGB Cleanup3");
        delete _pStrip;
        Serial.println("LedRGB Cleanup4");
        gpio_matrix_out(_pixelPin, 0x100, false, false);
        Serial.println("LedRGB Cleanup done");
        Serial.flush();
    }

    void UpdateLed(int channel, Variable *pBrightness)
    {
        if (!_operating)
        {
            return;
        }

        //char temp[128];
        //sprintf(temp, "%d %f %f %f", channel, pBrightness->GetValueFloat(0), pBrightness->GetValueFloat(1), pBrightness->GetValueFloat(2));

        //Serial.println(temp);

        RgbColor color(pBrightness->GetValueFloat(0) * 255,
                       pBrightness->GetValueFloat(1) * 255,
                       pBrightness->GetValueFloat(2) * 255);

        _pStrip->SetPixelColor(channel, color);
    }
#if fred
    void UpdateLed(LedState *pLedState)
    {
        int channel = pLedState->GetChannel();
        Variable *pBrightness = pLedState->GetBrightness();

        //char temp[128];
        //sprintf(temp, "%d %f %f %f", channel, pBrightness->GetValueFloat(0), pBrightness->GetValueFloat(1), pBrightness->GetValueFloat(2));

        //Serial.println(temp);

        RgbColor color(pBrightness->GetValueFloat(0) * 255,
                       pBrightness->GetValueFloat(1) * 255,
                       pBrightness->GetValueFloat(2) * 255);

        _pStrip->SetPixelColor(channel, color);
    }
#endif
    void Show()
    {
        if (!_operating)
        {
            return;
        }

        //Serial.println("show");
        _pStrip->Show();
    }
};