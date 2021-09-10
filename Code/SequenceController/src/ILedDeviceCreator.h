class ILedDeviceCreator
{
public:
    virtual ILedDevice *Create(LedType ledType, int ledCount, int pinCount, int pins[16], ILedManager *pLedManager) = 0;
};