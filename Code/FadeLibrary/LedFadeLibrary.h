#pragma once

class LedFadeLibrary : public ILedDevice
{
    int _ledGroupNumber;
  char _ledType[32];
  int _ledCount;
  int _pin1;
  int _pin2;
  int _pin3;
  int _pin4;


  int _pixelCount;
  int _pixelPin;

public:
  LedFadeLibrary(const char *pLedType, int ledCount, int pin1, int pin2, int pin3, int pin4)
  {
    strcpy(_ledType, pLedType);
    _ledCount = ledCount;
    _pin1 = pin1;
    _pin2 = pin2;
    _pin3 = pin3;
    _pin4 = pin4;
    _ledGroupNumber = -1;
  }

  void SetLedGroupNumber(int ledGroupNumber)
  {
      _ledGroupNumber = ledGroupNumber;
  }

  void UpdateLed(int channel, Variable* pBrightness)
  {
      if (_pLedUpdatedCallback)
      {
          _pLedUpdatedCallback(
              _ledGroupNumber,
              channel,
              pBrightness->GetValueCount(),
              pBrightness->GetValueFloat(0),
              pBrightness->GetValueFloat(1),
              pBrightness->GetValueFloat(2),
              pBrightness->GetValueFloat(3));
      }
  }

  void Show()
  {
  }

  virtual void FreeDevice()
  {
  }
};