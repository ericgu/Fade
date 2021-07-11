class ILedDevice
{
public:
    virtual void UpdateLed(int channel, Variable *pBrightness) = 0;

    //virtual void UpdateLed(LedState *pLedState) = 0;

    virtual void Show() = 0;

    virtual ~ILedDevice()
    {
    }

    virtual void FreeDevice() = 0;
};