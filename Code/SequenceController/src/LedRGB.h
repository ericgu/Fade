#include <NeoPixelBus.h>

class LedRGB : public ILedDevice
{
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_pStrip;
    int _pixelCount;
    int _pixelPin;
    volatile bool _operating;
    LedType _ledType;

public:
    LedRGB(LedType ledType, int pixelCount, int pixelPin)
    {
        _ledType = ledType;
        Serial.println(">LedRGB constructor");
        Serial.flush();
        _pixelCount = pixelCount;
        _pixelPin = pixelPin;
        _pStrip = new NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>(_pixelCount, _pixelPin);
        Serial.println(">LedRGB constructor - strip created");
        Serial.println((int)_pStrip);
        Serial.flush();
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

    RgbColor GetRgbColor(Variable *pBrightness)
    {
        int r = pBrightness->GetValueFloat(0) * 255;
        int g = pBrightness->GetValueFloat(1) * 255;
        int b = pBrightness->GetValueFloat(2) * 255;

        RgbColor color;

        switch (_ledType.Get())
        {
        case LedType::WS2812RGB:
            return RgbColor(r, g, b);

        case LedType::WS2812RBG:
            return RgbColor(r, b, g);

        case LedType::WS2812GRB:
            return RgbColor(g, r, b);

        case LedType::WS2812GBR:
            return RgbColor(g, b, r);

        case LedType::WS2812BRG:
            return RgbColor(b, r, g);

        case LedType::WS2812BGR:
            return RgbColor(b, g, r);
        }

        return RgbColor(r, g, b);
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

        _pStrip->SetPixelColor(channel, GetRgbColor(pBrightness));
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