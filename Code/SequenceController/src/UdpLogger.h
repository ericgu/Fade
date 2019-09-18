#include "WiFiUdp.h"

class UdpLoggerClass
{
    WiFiUDP _udp;
    int _port;
    String _current;
    String _prefix; 

    public:
        UdpLoggerClass()
        {
            _port = 12345;
        }

        void init(int port)
        {
            _port = port;
        }

        void init(int port, const char* prefix)
        {
            _port = port;
            _prefix = prefix;
            _current = prefix;
        }

        void WriteStartMessage()
        {
            println("Logging");
        }

        void print(int number)
        {
            char buffer[33];
            itoa(number, buffer, 10);
            print(buffer);
        }

        void print(String message)
        {
            _current += message;
        }

        void println(String message)
        {
            print(message);

            transmit();
        }

        void println(int number)
        {
            print(number);

            transmit();
        }

        void transmit()
        {
            _udp.beginPacket("255.255.255.255", _port);
            _udp.write((uint8_t*) _current.c_str(), _current.length());
            _udp.endPacket();
            _current = _prefix;
        }
};

UdpLoggerClass UdpLogger;