
class UdpLoggerClass
{
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
        Serial.print("UDPLog: ");
        Serial.println(pString);
    }
};

UdpLoggerClass UdpLogger;