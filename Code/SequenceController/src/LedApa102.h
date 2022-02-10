#include <spi.h>


class LedApa102 : public ILedDevice
{
    int _pixelCount;
    int _spiGroup;
    volatile bool _operating;

    SPIClass * _pSpi = NULL;

    static const int spiClk = 1000000; // 1 MHz    

    int _bufferSize;
    uint8_t *_pBuffer;

public:
    LedApa102(int pixelCount, int pinCount, int pins[])
    {
        Serial.println(">LedApa102 constructor");
        Serial.flush();

        _pixelCount = pixelCount;

        _bufferSize =   4 +                                     // 32 high bits
                        _pixelCount * 4  +                      // 4 bytes per color
                        (_pixelCount / 2 + 8) / 8;              // extra high bytes = pixel count / 2 bits, plus padding. 
        _pBuffer = new uint8_t[_bufferSize];

        Serial.print("Buffer size: ");
        Serial.println(_bufferSize);

        // 32 zero bits
        for (int i = 0; i < 4; i++)
        {
            _pBuffer[i] = 0;
        }

        // 1 bit pads at the end...

        for (int i = 4 + _pixelCount * 4; i < _bufferSize; i++)
        {
            Serial.println(i);
            _pBuffer[i] = 0xFF;
        }

        Serial.println(">LedApa102 constructor - strip created");

        _spiGroup = 0;

        int sclk;
        int miso;
        int mosi;
        int ss; 

        if (pinCount >= 1)
        {
            _spiGroup = pins[0] != 0;
        }

        if (_spiGroup == 0) // VSPI
        {
            Serial.println("VSPI");
            _pSpi = new SPIClass(VSPI);

            sclk = 18;
            miso = 19;
            mosi = 23;
            ss = 5;
        }
        else        // HSPI
        {
            Serial.println("hSPI");
            _pSpi = new SPIClass(HSPI);

            sclk = 14;
            miso = 12;
            mosi = 13;
            ss = 15;
        }

        if (pinCount == 5)  // user-specified pins
        {
            sclk = pins[1];
            miso = pins[2];
            mosi = pins[3];
            ss = pins[4];
        }

        Serial.print("SCLK, MISO, MOSI, SS ");
        Serial.print(sclk);
        Serial.print(", ");
        Serial.print(miso);
        Serial.print(", ");
        Serial.print(mosi);
        Serial.print(", ");
        Serial.println(ss);

        _pSpi->begin(sclk, miso, mosi, ss); 


        //Serial.println((int)_pStrip);
        Serial.flush();
        //_pStrip->Begin();
        Serial.println("<LedApa102 constructor");
        Serial.flush();
        _operating = true;

        SetToBlack();
    }

    ~LedApa102()
    {
        delete _pSpi;
        _pSpi = NULL;
        delete _pBuffer;
    }

    void SetToBlack()
    {
        RgbColor black(0, 0, 0);
        for (int pixel = 0; pixel < _pixelCount; pixel++)
        {
            //_pStrip->SetPixelColor(pixel, black);
        }
        //_pStrip->Show();
    }

    virtual void FreeDevice()
    {
        _operating = false;
        Serial.println("LedApa102 Cleanup1");
        SetToBlack();
        Serial.println("LedApa102 Cleanup2");
        delay(500);
        Serial.println("LedApa102 Cleanup3");
        delete _pSpi;
        _pSpi = NULL;
        Serial.println("LedApa102 Cleanup4");
        Serial.println("LedApa102 Cleanup done");
        Serial.flush();
    }

    void UpdateLed(int channel, Variable *pBrightness)
    {
        if (!_operating)
        {
            return;
        }

        int offset = channel * 4 + 4;

        //Serial.print("update: ");
        //Serial.print(channel);
        //Serial.print(" ");
        //Serial.print(offset);
        //Serial.print(" ");
        //Serial.print(pBrightness->GetValueFloat(0) * 255);
        //Serial.print(" ");
        //Serial.print(pBrightness->GetValueFloat(1) * 255);
        //Serial.print(" ");
        //Serial.print(pBrightness->GetValueFloat(2) * 255);
        //Serial.println();

        _pBuffer[offset] = 0xFF;                                            // full bright
        _pBuffer[offset + 1] = pBrightness->GetValueFloat(2) * 255;         // blue
        _pBuffer[offset + 2] = pBrightness->GetValueFloat(1) * 255;         // green
        _pBuffer[offset + 3] = pBrightness->GetValueFloat(0) * 255;         // red
    }


    void Show()
    {
        if (!_operating)
        {
            return;
        }

        //Serial.println("show");

            //use it as you would the regular arduino SPI API
        _pSpi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
            //digitalWrite(VSPI_SS, LOW); //pull SS slow to prep other end for transfer
        _pSpi->transfer(_pBuffer, _bufferSize); 
            //digitalWrite(VSPI_SS, HIGH); //pull ss high to signify end of data transfer
        _pSpi->endTransaction();
    }
};

