class ILedDeviceCreator
{
public:
    virtual ILedDevice* Create(const char* pLedType, int ledCount, int pin1, int pin2, int pin3, int pin4) = 0;
};