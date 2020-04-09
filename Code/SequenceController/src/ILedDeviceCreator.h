class ILedDeviceCreator
{
public:
    virtual ILedDevice* Create(const char* pLedType, int ledCount, int pin) = 0;
};