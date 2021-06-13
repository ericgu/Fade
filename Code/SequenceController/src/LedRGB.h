#include <NeoPixelBus.h>

class LedRGB : public ILedDevice
{
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_pStrip;
    int _pixelCount;
    int _pixelPin;

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

        RgbColor black(0, 0, 0);
        for (int pixel = 0; pixel < pixelCount; pixel++)
        {
            _pStrip->SetPixelColor(pixel, black);
        }

        Show();
    }

    ~LedRGB()
    {
        Serial.println("LedRGB Free");
        delete _pStrip;
    }

    void UpdateLed(int channel, Variable *pBrightness)
    {
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
        //Serial.println("show");
        _pStrip->Show();
    }
};