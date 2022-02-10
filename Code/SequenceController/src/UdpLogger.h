#include "WiFiUdp.h"

class UdpLoggerClass
{
    WiFiUDP _udp;
    int _port;

public:
    UdpLoggerClass()
    {
        _port = 12345;
    }

    void init(int port)
    {
        _port = port;
    }

    void print(const char *pString)
    {
        Serial.print("UDP Logging: ");
        Serial.println(pString);
        _udp.beginPacket("255.255.255.255", _port);
        _udp.write((uint8_t *)"FadeUdpLog: ", 12);
        _udp.write((uint8_t *)pString, strlen(pString));
        _udp.endPacket();
    }
};

UdpLoggerClass UdpLogger;