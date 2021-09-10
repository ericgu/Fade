// Acts like an LED Device, but sends out the commands via UDP...

class LedUdpSender : public ILedDevice
{
    int _pixelCount;
    int _valuesPerPixel;
    int _universe;
    int _port;

    UdpData *_pDataBuffer;
    //MyUdp *_pMyUdp;

    MyUdp _myUdp;

public:
    static int _nextChannelToUse;

    LedUdpSender(int pinCount, int pin1, int pin2, int pin3, int pin4)
    {
        Serial.println(">LedUdpSender constructor");

        _pixelCount = pinCount;
        _valuesPerPixel = pin1;
        _universe = pin2;
        _port = pin3;

        _myUdp.Begin(_port);

        _pDataBuffer = UdpData::AllocateBuffer(_pixelCount, _valuesPerPixel, _universe);
    }

    ~LedUdpSender()
    {
        Serial.println("LedUdpSender Free");
    }

    virtual void FreeDevice()
    {
        delete _pDataBuffer;
    }

    void UpdateLed(int channel, Variable *pBrightness)
    {
        //Serial.print("UpdateLed");
        //Serial.flush();
        for (int valueIndex = 0; valueIndex < _valuesPerPixel; valueIndex++)
        {
            _pDataBuffer->SetValueForChannel(channel, valueIndex, (int)65535 * pBrightness->GetValueFloat(valueIndex));
        }
    }

    void Show()
    {
        //Serial.print("Show");
        //Serial.flush();
        _myUdp.Send(_port, (uint8_t *)_pDataBuffer, _pDataBuffer->_size);
        _pDataBuffer->IncrementSequenceNumber();
    }
};
