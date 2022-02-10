class LedType
{
    int _ledType;

public:
    static const int Undefined = -1;
    //static const int WS2812 = 1;
    static const int PWM = 2;
    static const int Servo = 3;
    static const int Test = 4;
    static const int UdpSender = 5;
    static const int UdpReceiver = 6;
    static const int APA102 = 7;
    static const int WS2812RGB = 8;
    static const int WS2812RBG = 9;
    static const int WS2812GRB = 10;
    static const int WS2812GBR = 11;
    static const int WS2812BRG = 12;
    static const int WS2812BGR = 13;

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
        case LedType::WS2812RGB:
            return "WS2812";
        case LedType::WS2812RBG:
            return "WS2812RBG";
        case LedType::WS2812GRB:
            return "WS2812GRB";
        case LedType::WS2812GBR:
            return "WS2812GBR";
        case LedType::WS2812BRG:
            return "WS2812BRG";
        case LedType::WS2812BGR:
            return "WS2812BGR";
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
        case LedType::APA102:
            return "APA102";
        }

        return "";
    }

    LedType(const char *pLedType)
    {
        if (strcmp(pLedType, "RGB") == 0 || strcmp(pLedType, "WS2812") == 0 || strcmp(pLedType, "WS2812RGB") == 0)
        {
            _ledType = WS2812RGB;
        }
        else if (strcmp(pLedType, "WS2812RBG") == 0)
        {
            _ledType = WS2812RBG;
        }
        else if (strcmp(pLedType, "WS2812GRB") == 0)
        {
            _ledType = WS2812GRB;
        }
        else if (strcmp(pLedType, "WS2812GBR") == 0)
        {
            _ledType = WS2812GBR;
        }
        else if (strcmp(pLedType, "WS2812BRG") == 0)
        {
            _ledType = WS2812BRG;
        }
        else if (strcmp(pLedType, "WS2812BGR") == 0)
        {
            _ledType = WS2812BGR;
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
        else if (strcmp(pLedType, "APA102") == 0)
        {
            _ledType = APA102;
        }
        
    }
};