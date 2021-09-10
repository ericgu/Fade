class LedDeviceCreator : public ILedDeviceCreator
{
    char _ledType[64];
    int _ledCount;
    int _pin1;
    int _pin2;
    int _pin3;
    int _pin4;
    int _nextGroupNumber;

public:
    LedDeviceCreator()
    {
        _nextGroupNumber = 0;
    }


    ILedDevice *Create(LedType ledType, int ledCount, int pinCount, int pins[16], ILedManager *pLedManager)
    {

      NewLedDeviceCreated(_nextGroupNumber, ledType.GetName(), ledCount, pinCount, pins);

      LedFadeLibrary* pLedDevice = new LedFadeLibrary(ledType, ledCount, pinCount, pins);
      pLedDevice->SetLedGroupNumber(_nextGroupNumber);
      _nextGroupNumber++;

      return pLedDevice;
    }
};