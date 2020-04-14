#include <NeoPixelBus.h>

class LedRGB: public ILedDevice
{
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>* _pStrip;
    int _pixelCount;
    int _pixelPin;

    public:
        LedRGB(int pixelCount, int pixelPin)
        {
             Serial.println(">LegRGB constructor"); Serial.flush();
            _pixelCount = pixelCount;
            _pixelPin = pixelPin;
             _pStrip = new NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>(_pixelCount, _pixelPin);
             _pStrip->Begin();
             Serial.println("<LegRGB constructor"); Serial.flush();
             
             RgbColor black(0, 0, 0);
             for (int pixel = 0; pixel < pixelCount; pixel++)
             {
                _pStrip->SetPixelColor(pixel, black);
             }

             Show();
        }

        ~LedRGB()
        {
            delete _pStrip;
        }

        void UpdateLed(LedState ledState)
        {
            int channel = ledState.GetChannel();
            Variable* pBrightness = ledState.GetBrightness();

            RgbColor color( pBrightness->GetValueFloat(0) * 255,
                            pBrightness->GetValueFloat(1) * 255,
                            pBrightness->GetValueFloat(2) * 255);

            _pStrip->SetPixelColor(channel, color);
        }

        void Show()
        {
            _pStrip->Show();
        }
};