class LedType
{
    int _ledType;

public:
    static const int Undefined = -1;
    static const int WS2812 = 1;
    static const int PWM = 2;
    static const int Servo = 3;
    static const int Test = 4;
    static const int UdpSender = 5;
    static const int UdpReceiver = 6;

    int Get()
    {
        return _ledType;
    }

    LedType()
    {
        _ledType = Undefined;
    }

    bool IsValid()
    {
        return _ledType != Undefined;
    }

    const char *GetName()
    {
        switch (_ledType)
        {
        case LedType::WS2812:
            return "WS2812";
        case LedType::PWM:
            return "PWM";
        case LedType::Servo:
            return "Servo";
        case LedType::Test:
            return "Test";
        case LedType::UdpSender:
            return "UdpSender";
        case LedType::UdpReceiver:
            return "UdpReceiver";
        }

        return "";
    }

    LedType(const char *pLedType)
    {
        if (strcmp(pLedType, "RGB") == 0 || strcmp(pLedType, "WS2812") == 0)
        {
            _ledType = WS2812;
        }
        else if (strcmp(pLedType, "PWM") == 0)
        {
            _ledType = PWM;
        }
        else if (strcmp(pLedType, "Servo") == 0)
        {
            _ledType = Servo;
        }
        else if (strcmp(pLedType, "Test") == 0)
        {
            _ledType = Test;
        }
        else if (strcmp(pLedType, "UdpSender") == 0)
        {
            _ledType = UdpSender;
        }
        else if (strcmp(pLedType, "UdpReceiver") == 0)
        {
            _ledType = UdpReceiver;
        }
    }
};