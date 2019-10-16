#include <Arduino.h>
#include <arduinonvs.h>
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
#include "ParseErrors.h"
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

#include <ArduinoNvs.h>
#include <Settings.h>
#include <Supervisor.h>

#include "MyWebServer.h"

LedPwmEsp32 ledPwm;
LedManager ledManager(&ledPwm, 16);

Supervisor supervisor;
Settings settings;

MyWebServer* pMyWebServer;

void setup() {
  Serial.begin(115200);
  WiFiManager wifiManager;

  wifiManager.autoConnect("SequenceController", "12345678");
  Serial.print("after autoconnect: ");
  Serial.println(WiFi.localIP());

  pMyWebServer = new MyWebServer(&supervisor);

  settings.Init();
  supervisor.Init(&ledManager, &settings);

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

void loop() 
{
  pMyWebServer->HandleClient();

  supervisor.Execute();

  //TrackMemory();
    
  delay(10);
}
