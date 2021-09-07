
class MyUdp
{
    //static MyWebServer *pMyWebServerInstance;

    WiFiUDP *_pWifiUdp;

    char *_pProgramBuffer;
    char *_pPageBuffer;

    int _port;
    char _buffer[1024];

public:
    MyUdp(int port)
    {
        _port = port;

        _pWifiUdp = new WiFiUDP();
        _pWifiUdp->begin(_port);
    }

    void HandleClient()
    {
        int packetSize = _pWifiUdp->parsePacket();
        if (packetSize)
        {
            // receive incoming UDP packets
            //Serial.printf("Received %d bytes from %s, port %d\n", packetSize, _udp.remoteIP().toString().c_str(), _udp.remotePort());

            int len = _pWifiUdp->read(_buffer, sizeof(_buffer) - 1);
            if (len > 0)
            {
                _buffer[len] = 0;
                Serial.print("UDP received: ");
                Serial.println(len);
            }

            if (packetSize == 20)
            {
                Send((uint8_t *)"Hello to you", 13);
            }
        }
    }

    void Send(const uint8_t *pBuffer, size_t bufferSize)
    {
        _pWifiUdp->beginPacket("255.255.255.255", _port);
        _pWifiUdp->write(pBuffer, bufferSize);
        _pWifiUdp->endPacket();
    }
};

//MyWebServer *MyWebServer::pMyWebServerInstance;