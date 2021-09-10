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


    ILedDevice *Create(LedType ledType, int ledCount, int pin1, int pin2, int pin3, int pin4, ILedManager *pLedManager)
    {

      NewLedDeviceCreated(_nextGroupNumber, ledType.GetName(), ledCount, pin1, pin2, pin3, pin4);

      LedFadeLibrary* pLedDevice = new LedFadeLibrary(ledType, ledCount, pin1, pin2, pin3, pin4);
      pLedDevice->SetLedGroupNumber(_nextGroupNumber);
      _nextGroupNumber++;

      return pLedDevice;
    }
};