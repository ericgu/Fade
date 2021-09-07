// Acts like an LED Device, but sends out the commands via UDP...

class LedUdpSender : public ILedDevice
{
    int _pixelCount;
    int _valuesPerPixel;

    UdpData *_pDataBuffer;
    MyUdp *_pMyUdp;

public:
    static int _nextChannelToUse;

    LedUdpSender(int pinCount, int pin1, int pin2, int pin3, int pin4, MyUdp *pMyUdp)
    {
        Serial.println(">LedUdpRemote constructor");

        _pixelCount = pinCount;
        _valuesPerPixel = pin1;

        _pDataBuffer = UdpData::AllocateBuffer(_pixelCount, _valuesPerPixel, pin2);
    }

    ~LedUdpSender()
    {
        Serial.println("LedUdpRemote Free");
    }

    virtual void FreeDevice()
    {
        delete _pDataBuffer;
    }

    void UpdateLed(int channel, Variable *pBrightness)
    {
        for (int valueIndex = 0; valueIndex < _valuesPerPixel; valueIndex++)
        {
            _pDataBuffer->SetValueForChannel(channel, valueIndex, (int)65535 * pBrightness->GetValueFloat(valueIndex));
        }
    }

    void Show()
    {
        _pMyUdp->Send((uint8_t *)_pDataBuffer, _pDataBuffer->_size);
    }
};
