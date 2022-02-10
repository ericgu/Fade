class LedDeviceCreator : public ILedDeviceCreator
{
    LedType _ledType;
    int _ledCount;
    int _pin1;
    int _pin2;
    int _pin3;
    int _pin4;

public:
    LedDeviceCreator()
    {
    }

    ILedDevice *Create(LedType ledType, int ledCount, int pinCount, int *pins, ILedManager *pLedManager)
    {
        Serial.print("Creating: ");
        Serial.flush();
        Serial.print(ledType.GetName());
        Serial.flush();
        Serial.print(" ");
        Serial.flush();
        Serial.print(ledCount);
        Serial.flush();
        Serial.print(" ");
        Serial.flush();
        Serial.println(pins[0]);
        Serial.flush();

        _ledType = ledType;
        _ledCount = ledCount;
        _pin1 = pins[0];
        _pin2 = pins[1];
        _pin3 = pins[2];
        _pin4 = pins[3];

        Serial.print("Creating ");
        Serial.println(ledType.GetName());

        switch (ledType.Get())
        {
        case LedType::WS2812RGB:
        case LedType::WS2812RBG:
        case LedType::WS2812GRB:
        case LedType::WS2812GBR:
        case LedType::WS2812BRG:
        case LedType::WS2812BGR:
            return new LedRGB(ledType, ledCount, pins[0]);

        case LedType::PWM:
            return new LedPwmEsp32(ledCount, pins);

        case LedType::Servo:
            return new LedServoEsp32(pinCount, pins);

        case LedType::Test:
            return 0;

        case LedType::UdpSender:
            return new LedUdpSender(ledCount, pins);

        case LedType::UdpReceiver:
            return new LedUdpReceiver(ledCount, pins, pLedManager);

        case LedType::APA102:
            return new LedApa102(ledCount, pinCount, pins);
        }

        return 0;
    }
};