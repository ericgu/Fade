class ILedDeviceCreator
{
public:
    virtual ILedDevice *Create(LedType ledType, int ledCount, int pin1, int pin2, int pin3, int pin4) = 0;
};