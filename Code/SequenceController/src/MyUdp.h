
class MyUdp
{
    //static MyWebServer *pMyWebServerInstance;

    WiFiUDP *_pWifiUdp;

    volatile int _port;
    char _buffer[1024];

public:
    void Begin(int port)
    {
        _port = port;

        Serial.print("UDP begin on port: ");
        Serial.println(_port);
        Serial.flush();
        _pWifiUdp = new WiFiUDP();
        _pWifiUdp->begin(_port);
    }

    char *ReadData()
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
                //Serial.print("UDP received: ");
                //Serial.println(len);
                return _buffer;
            }
        }

        return 0;
    }

    void Send(int port, const uint8_t *pBuffer, size_t bufferSize)
    {
        //Serial.print("UDP Send: ");
        //Serial.println(bufferSize);
        //Serial.print(" port: ");
        //Serial.println(port);
        //Serial.println((char *)_pWifiUdp);
        //Serial.flush();
        _pWifiUdp->beginPacket("255.255.255.255", port);
        //Serial.print("UDP Send 2: ");
        _pWifiUdp->write(pBuffer, bufferSize);
        //Serial.print("UDP Send 3: ");
        _pWifiUdp->endPacket();
        //Serial.print("UDP Send 4: ");
    }
};

//MyWebServer *MyWebServer::pMyWebServerInstance;