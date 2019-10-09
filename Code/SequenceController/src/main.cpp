#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>

#include <Preferences.h>
#include <WiFi.h>

#define strcpy_s strcpy
#define strncpy_s strncpy
#define strncmp_s strncmp

#include "MyRandom.h"
#include "Command.h"
#include "ListParser.h"
#include "CommandSource.h"
#include "CommandFormatter.h"
#include "LedState.h"
#include "CommandResult.h"
#include "LedCommand.h"
#include "LedPwm.h"
#include "LedPwmEsp32.h"
#include "LedManager.h"
#include "Variable.h"
#include "Stack.h"
#include "ExecutionContext.h"
#include "CommandDecoder.h"
#include "ExecutionFlow.h"
#include "Timebase.h"

#include <WiFi.h>          

#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h>     
#include <UdpLogger.h>

#include "MyWebServer.h"

#include <ArduinoNvs.h>
#include <Settings.h>
#include <Watchdog.h>

CommandSource commandSource;
LedPwmEsp32 ledPwm;
LedManager ledManager(&ledPwm, 16);

Timebase timebase(&commandSource, &ledManager);

MyWebServer* pMyWebServer;

char* pCurrentCommand;

Settings *pSettings;
Watchdog watchdog;

void ProgramUpdatedImplementation(const char* pProgram)
{
  Serial.println("Program Updated");

  strcpy(pCurrentCommand, pProgram);
  commandSource.SetCommand(pCurrentCommand);
  pSettings->SetString("Program", pProgram);
  watchdog.ProgramUpdated();

  Serial.println("New Program: ");
  Serial.println(pCurrentCommand);
}

const char* GetCurrentProgram()
{
  return pCurrentCommand;
}

bool GetExecutingProgramState()
{
  return watchdog.ShouldExecuteCode();
}

void setup() {
  pCurrentCommand = new char[16636];

  //pinMode (LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  WiFiManager wifiManager;

  wifiManager.autoConnect("SequenceController", "12345678");

  Serial.println("after autoconnect");
  Serial.println(WiFi.localIP());

  pMyWebServer = new MyWebServer(ProgramUpdatedImplementation, GetCurrentProgram, GetExecutingProgramState);

  strcpy(pCurrentCommand, "FOR B 100:10:-10\nFOR A 0:7\nDI(B,A,1.0)\nDI(B,A,0.0)\nENDFOR\nENDFOR");
  strcpy(pCurrentCommand, "FOR B 20:10:-10\nFOR A 0:7\nDI(B,A,1.0)\nDI(B,A,0.0)\nENDFOR\nENDFOR");

  pSettings = new Settings();
  watchdog.Initialize(pSettings);

  String savedProgram = pSettings->GetString("Program");
  if (savedProgram.length() != 0)
  {
    strcpy(pCurrentCommand, savedProgram.c_str());
    Serial.println("Loaded program: ");
    Serial.println(pCurrentCommand);
  }

  Serial.print("Start Program: ");
  Serial.println(watchdog.ShouldExecuteCode());

  if (watchdog.ShouldExecuteCode())
  {
    commandSource.SetCommand(pCurrentCommand);
    Serial.println("Program will be run...");
  }
  else
  {
    Serial.println("Program execution delayed...");
  }

  Serial.println("Setup completed");
}

 int iterations = 0;

 void TrackMemory()
 {
    if (iterations % 100 == 0)
    {
      Serial.print(iterations);
      Serial.print(" = ");
      Serial.println(ESP.getFreeHeap());
    }
    iterations++;
 }

void loop() {
  pMyWebServer->HandleClient();

  if (watchdog.ShouldExecuteCode())
  {
    watchdog.ExecutionTick();

    timebase.DoTick();
    //UdpLogger.print("Heartbeat");

    //TrackMemory();
    
    delay(10);
  }
}
