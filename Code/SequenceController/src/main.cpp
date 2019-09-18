#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>

#include <Preferences.h>
#include <WiFi.h>

#define strcpy_s strcpy
#define strncpy_s strncpy
#define strncmp_s strncmp

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

//int LED_BUILTIN = 2;

const int ledPins[16] = { 2,  4,  5, 18, 19, 21, 22, 23,   // main row
                          14, 27, 26, 25, 33, 32, 12, 13};  // extension row

const int PwmFrequency = 500;

CommandSource commandSource;
LedPwmEsp32 ledPwm;
LedManager ledManager(&ledPwm, 16);

Timebase timebase(&commandSource, &ledManager);

MyWebServer* pMyWebServer;

char* pCurrentCommand;

Settings *pSettings;

void ProgramUpdatedImplementation(const char* pProgram)
{
  strcpy(pCurrentCommand, pProgram);
  commandSource.SetCommand(pCurrentCommand);
  pMyWebServer->SetCurrentProgram(pCurrentCommand);
  pSettings->SetString("Program", pProgram);

  Serial.print("Program: ");
  Serial.println(pCurrentCommand);
}

void setup() {
  Serial.println("Setup"); Serial.flush();

  pCurrentCommand = new char[16636];

  //pinMode (LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("Setup"); Serial.flush();

  WiFiManager wifiManager;

  wifiManager.autoConnect("SequenceController", "12345678");

  Serial.println("after autoconnect");
  Serial.println(WiFi.localIP());

  pMyWebServer = new MyWebServer(ProgramUpdatedImplementation);
  Serial.println("Setup"); Serial.flush();

  strcpy(pCurrentCommand, "LOOP %B 100:10:-10\nLOOP %A 0:7\nD %B %A,1.0\nD %B %A,0.0\nENDLOOP\nENDLOOP");
  strcpy(pCurrentCommand, "LOOP %B 20:10:-10\nLOOP %A 0:7\nD %B %A,1.0\nD %B %A,0.0\nENDLOOP\nENDLOOP");

  pSettings = new Settings();
  String savedProgram = pSettings->GetString("Program");
  if (savedProgram.length() != 0)
  {
    strcpy(pCurrentCommand, savedProgram.c_str());
    Serial.println("Loaded program: ");
    Serial.println(pCurrentCommand);
  }

  commandSource.SetCommand(pCurrentCommand);
  pMyWebServer->SetCurrentProgram(pCurrentCommand);

  Serial.println("Setup"); Serial.flush();

  Serial.println("Setup 2"); Serial.flush();
}

int iterations = 0;

void loop() {
  pMyWebServer->HandleClient();
  timebase.DoTick();
  //UdpLogger.print("Heartbeat");
  if (iterations % 100 == 0)
  {
    Serial.print(iterations);
    Serial.print(" = ");
    Serial.println(ESP.getFreeHeap());
  }
  iterations++;
  delay(10);
}
