// Receives Udp commands from another device, sends them to this device.

class LedUdpReceiver : public ILedDevice
{
    int _pixelCount;
    int _universe;
    int _port;
    int _startingChannelToCopy;
    int _channelsToCopy;

    ILedManager *_pLedManager;

    MyUdp _myUdp;

public:
    static int _nextChannelToUse;

    LedUdpReceiver(int pinCount, int pin1, int pin2, int pin3, int pin4, ILedManager *pLedManager)
    {
        Serial.println(">LedUdpReceiver constructor");

        _pixelCount = pinCount;
        _universe = pin1;
        _port = pin2;
        _startingChannelToCopy = pin3;
        _channelsToCopy = pin4;

        _pLedManager = pLedManager;

        _myUdp.Begin(_port);
    }

    ~LedUdpReceiver()
    {
        Serial.println("LedUdpReceiver Free");
    }

    virtual void FreeDevice()
    {
    }

    void UpdateLed(int channel, Variable *pBrightness)
    {
        //Serial.print("UpdateLed");
        //Serial.flush();
    }

    void Show()
    {
        //Serial.print("Show");
        //Serial.flush();

        UdpData *pData = (UdpData *)_myUdp.ReadData();

        if (pData != 0)
        {
            if (pData->_universe != _universe)
            {
                Serial.println("Mismatched universe");
            }

#if fred
            Serial.println("UDP Data: ");
            Serial.print("Name: ");
            Serial.println(pData->_name);
            Serial.print("Universe: ");
            Serial.println(pData->_universe);
            Serial.print("Sequence Number: ");
            Serial.println(pData->_sequenceNumber);
            Serial.print("Channel Count: ");
            Serial.println(pData->_channelCount);
            Serial.print("Values Per Channel: ");
            Serial.println(pData->_valuesPerChannel);
#endif
            for (int channel = _startingChannelToCopy; channel < _startingChannelToCopy + _channelsToCopy; channel++)
            {
                if (channel <= pData->_channelCount)
                {
                    //Serial.print("Channel: ");
                   // Serial.println(channel);

                    Variable variable;
                    for (int index = 0; index < pData->_valuesPerChannel; index++)
                    {
                        //Serial.print(pData->_channelData[channel * pData->_valuesPerChannel + index]);
                        //Serial.print(" ");

                        variable.SetValue(index, pData->_channelData[channel * pData->_valuesPerChannel + index] / 65535.0);
                    }
                    //Serial.println();
                    _pLedManager->SetLedState(channel, &variable);
                    //Serial.print("U: ");
                    //Serial.print(pData->_channelData[0]);
                    //Serial.print(" ");
                }
            }
        }
    }
};
