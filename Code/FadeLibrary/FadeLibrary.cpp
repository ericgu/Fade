// FadeLibrary.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "FadeLibrary.h"




FADELIBRARY_API int MyCFunc()
{
  return 42;
}

void Callback()
{
  
}

void NewSerialTextAvailable(const char* pText)
{
  if (_pNewSerialTextAvailableCallback)
  {
    _pNewSerialTextAvailableCallback(pText);
  }
}

void NewLedDeviceCreated(int groupNumber, const char *pLedType, int ledCount, int pin1, int pin2, int pin3, int pin4)
{
  if (_pCreateNewLedDeviceCallback)
  {
    _pCreateNewLedDeviceCallback(groupNumber, pLedType, ledCount, pin1, pin2, pin3, pin4);
  }
}

void LedRenderCycleDone()
{
  if (_pLedRenderCycleDoneCallback)
  {
    _pLedRenderCycleDoneCallback();
  }
}

void ParseErrorFound(const char *pMessage, int lineNumber)
{
  if (_pParseErrorCallback)
  {
    _pParseErrorCallback(pMessage, lineNumber);
  }
}

FADELIBRARY_API int Run(const char* pProgram, 
                        NewSerialTextAvailableCallback pNewSerialTextAvailableCallback,
                        CreateNewLedDeviceCallback pCreateNewLedDeviceCallback,
                        LedUpdatedCallback pLedUpdatedCallback,
                        LedRenderCycleDoneCallback pLedRenderCycleDoneCallback,
                        ParseErrorCallback pParseErrorCallback)
{
  _pNewSerialTextAvailableCallback = pNewSerialTextAvailableCallback;
  _pCreateNewLedDeviceCallback = pCreateNewLedDeviceCallback;
  _pLedUpdatedCallback = pLedUpdatedCallback;
  _pLedRenderCycleDoneCallback = pLedRenderCycleDoneCallback;
  _pParseErrorCallback = pParseErrorCallback;

  Environment.ResetValues();

  ButtonCreator::ResetButtons();

  _pSupervisor = new Supervisor();
  _pSettings = new Settings();

  _pLedDeviceCreator = new LedDeviceCreator();
  //_pLedDevice = new LedRGB(33, 13);

  _pLedManager = new LedManager(_pLedDeviceCreator);
  _pButtonCreator = new ButtonCreator();
  _pSupervisor->Init(_pLedManager, _pSettings, Callback, _pButtonCreator);

  _pSupervisor->UpdateProgram(pProgram);
  _pSupervisor->ExecuteLoop();

  return 0;
}

FADELIBRARY_API int PressButton(int buttonNumber)
{
    ButtonCreator::PushButton(buttonNumber);

    return 0;
}


FADELIBRARY_API int Abort(NewSerialTextAvailableCallback pCallback)
{

  return 0;

}


