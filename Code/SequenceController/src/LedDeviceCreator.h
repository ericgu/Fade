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

    ILedDevice *Create(LedType ledType, int ledCount, int pin1, int pin2, int pin3, int pin4, ILedManager *pLedManager)
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
        Serial.print(pin1);
        Serial.flush();

        _ledType = ledType;
        _ledCount = ledCount;
        _pin1 = pin1;
        _pin2 = pin2;
        _pin3 = pin3;
        _pin4 = pin4;

        Serial.print("Creating ");
        Serial.println(ledType.GetName());

        switch (ledType.Get())
        {
        case LedType::WS2812:
            return new LedRGB(ledCount, pin1);

        case LedType::PWM:
            return new LedPwmEsp32(ledCount, pin1, pin2, pin3, pin4);

        case LedType::Servo:
            return new LedServoEsp32(ledCount, pin1, pin2, pin3, pin4);

        case LedType::Test:
            return 0;

        case LedType::UdpSender:
            return new LedUdpSender(ledCount, pin1, pin2, pin3, pin4);

        case LedType::UdpReceiver:
            return new LedUdpReceiver(ledCount, pin1, pin2, pin3, pin4, pLedManager);
        }

        return 0;
    }
};