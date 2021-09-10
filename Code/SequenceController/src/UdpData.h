struct UdpData
{
    char _name[16]; // "FADE1.0"
    unsigned short _size;
    unsigned short _universe;         // support multiple animations on one network
    unsigned short _sequenceNumber;   // incrmental message sequenceNumber
    unsigned short _channelCount;     // # of channels included in this message
    unsigned short _valuesPerChannel; // # of data values per channel
    unsigned short _channelData[0];   // array of channel data.

    static UdpData *AllocateBuffer(int channelCount, int valuesPerChannel, int universe)
    {
        int size = sizeof(UdpData) + channelCount * valuesPerChannel * sizeof(unsigned short);
        //Serial.print("UdpData buffer size: ");
        //Serial.println(size);
        //Serial.flush();

        UdpData *pData = (UdpData *)new char[size];
        memset(pData, 0, size);

        strcpy(pData->_name, "FADE1.0");

        pData->_size = size;
        pData->_universe = universe;
        pData->_channelCount = channelCount;
        pData->_valuesPerChannel = valuesPerChannel;
        pData->_sequenceNumber = 0;

        return pData;
    }

    void SetValueForChannel(int channelNumber, int valueInChannel, unsigned short value)
    {
        unsigned short *pValue = _channelData + channelNumber * _valuesPerChannel + valueInChannel;
        //Serial.print("Channel: ");
        //Serial.print(channelNumber);
        //Serial.print(" value in channel: ");
        ///Serial.print(valueInChannel);
        //Serial.print(" offset");
        //Serial.println(pValue - _channelData);

        *pValue = value;
    }

    void IncrementSequenceNumber()
    {
        _sequenceNumber++;
    }
};
