class ILedManager
{
public:
    virtual ~ILedManager() {}

    virtual void SetDelta(CommandResult *commandResult) = 0;

    virtual void Tick() = 0;
    virtual void ResetState() = 0;

    virtual bool Configure(int ledGroupNumber, const char *pLedType, int ledCount, int pinCount, int pins[16]) = 0;

    virtual int GetLedCount() = 0;

    virtual void FreeDevices() = 0;

    virtual void SetLedState(int channel, Variable *pBrightness) = 0;
};