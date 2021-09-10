// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FADELIBRARY_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FADELIBRARY_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef FADELIBRARY_EXPORTS
#define FADELIBRARY_API __declspec(dllexport)
#else
#define FADELIBRARY_API __declspec(dllimport)
#endif
#define _CRT_SECURE_NO_WARNINGS

extern "C"
{
  typedef void(*NewSerialTextAvailableCallback)(const char* pString);

  FADELIBRARY_API void NewSerialTextAvailable(const char* pText);

  typedef void(*CreateNewLedDeviceCallback)(int groupNumber, const char *pLedType, int ledCount, int pin1, int pin2, int pin3, int pin4);
  CreateNewLedDeviceCallback _pCreateNewLedDeviceCallback;

  typedef void(*LedUpdatedCallback)(int ledGroup, int channel, int brightnessCount, float brightness1, float brightness2, float brightness3, float brightness4);
  LedUpdatedCallback _pLedUpdatedCallback;

  typedef void(*LedRenderCycleDoneCallback)();
  LedRenderCycleDoneCallback _pLedRenderCycleDoneCallback;

  typedef void(*ParseErrorCallback)(const char *pMessage, int lineNumber);
  ParseErrorCallback _pParseErrorCallback;

  void NewSerialTextAvailable(const char* pText);
  void NewLedDeviceCreated(int groupNumber, const char *pLedType, int ledCount, int pin1, int pin2, int pin3, int pin4);
  void LedRenderCycleDone();
  void ParseErrorFound(const char *pMessage, int lineNumber);

}


#include "stdio.h"

#include "Environment.h"
#include "SafeString.h"
#include "Serial.h"
#include "TimeServices.h"
#include "EspFunctions.h"
#include "MyRandom.h"
#include "StackWatcher.h"
#include "Instrumentation.h"
#include "Profiler.h"
#include "Vector.h"
#include "Delayer.h"
#include "SystemCallback.h"
#include "InternedStrings.h"
#include "Command.h"
#include "ParseErrors.h"
#include "CommandSource.h"
#include "CommandFormatter.h"
#include "VariableData.h"
#include "VariableStoreChunk.h"
#include "VariableStore.h"
#include "Variable.h"
#include "VariableCollection.h"
#include "ExpressionResult.h"
#include "LedState.h"

#include "CommandResult.h"
#include "ILedManager.h"
#include "LedType.h"
#include "ILedDevice.h"
#include "LedFadeLibrary.h"
#include "ILedDeviceCreator.h"
#include "FunctionStore.h"
#include "Stack.h"
#include "IExecutionFlow.h"
#include "IExecutionContext.h"
#include "ExpressionNode.h"
#include "CharacterClassifier.h"
#include "ExpressionTokenSource.h"
#include "BuiltInFunctions.h"
#include "RDEvaluater.h"
#include "ExecutionContext.h"
#include "LedGroups.h"
#include "LedManager.h"
#include "ILedMessageHandler.h"
#include "IButton.h"
#include "IButtonCreator.h"
#include "ExecutionFlow.h"
#include "Timebase.h"
#include "Settings.h"
#include "Supervisor.h"


#include "ButtonCreator.h"
#include "LedDeviceCreator.h"





LedDeviceCreator* _pLedDeviceCreator = nullptr;
ILedDevice* _pLedDevice = nullptr;
LedManager* _pLedManager = nullptr;

ButtonCreator *_pButtonCreator = nullptr;

Supervisor *_pSupervisor = nullptr;
Settings *_pSettings = nullptr;

extern "C"
{
  NewSerialTextAvailableCallback _pNewSerialTextAvailableCallback;



  FADELIBRARY_API int MyCFunc(); 

  FADELIBRARY_API int Run(
    const char* pProgram, 
    NewSerialTextAvailableCallback pNewSerialTextAvailableCallback,
    CreateNewLedDeviceCallback pCreateNewLedDeviceCallback,
    LedUpdatedCallback pLedUpdatedCallback,
    LedRenderCycleDoneCallback pLedRenderCycleDoneCallback,
    ParseErrorCallback pParseErrorCallback);

  FADELIBRARY_API int PressButton(int buttonNumber);

  FADELIBRARY_API void PrettyFormat(const char* pCommand, char* pOutput, int bufferSize);


}



#if fred
// This class is exported from the FadeLibrary.dll
class FADELIBRARY_API CFadeLibrary {
public:
	CFadeLibrary(void);
	// TODO: add your methods here.

  FADELIBRARY_API int StaticFunction(void);
};

extern FADELIBRARY_API int nFadeLibrary;

FADELIBRARY_API int fnFadeLibrary(void);

FADELIBRARY_API int StaticFunction(void);
#endif