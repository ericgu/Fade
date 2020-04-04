#include <Arduino.h>
#include <arduinonvs.h>
#include <stdio.h>
#include <stdlib.h>

#include <Preferences.h>
#include <WiFi.h>

#define strcpy_s strcpy
#define strncpy_s strncpy
#define strncmp_s strncmp

#include "StackWatcher.h"
#include "SystemCallback.h"
#include "MyRandom.h"
#include "TickSource.h"
#include "Delayer.h"
#include "Command.h"
#include "ListParser.h"
#include "CommandSource.h"
#include "CommandFormatter.h"
#include "ParseErrors.h"
#include "Variable.h"
#include "LedState.h"
#include "CommandResult.h"
#include "IExecutionFlow.h"
#include "LedCommand.h"
#include "ILedPwm.h"
#include "LedPwmEsp32.h"
#include "LedRGB.h"
#include "LedManager.h"
#include "ExpressionTokenizer.h"
#include "FunctionStore.h"
#include "Stack.h"
#include "ExpressionBuiltInFunctions.h"
#include "ExpressionOperators.h"
#include "ExpressionFunctionCall.h"
#include "Expression.h"
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

ILedPwm* _pLedPwm;
LedManager* _pLedManager;

Supervisor* _pSupervisor;
Settings* _pSettings;

MyWebServer* pMyWebServer;

void Callback()
{
  //Serial.println("Callback");
  pMyWebServer->HandleClient();

  //TrackMemory();
    
  delay(10);
}

void taskOne( void * parameter )
{
  StackWatcher::Init();

  StackWatcher::Log("Task Start");
  _pSupervisor->ExecuteLoop();

}

void setup() {

  Serial.begin(115200);
  StackWatcher::Log("setup");
  _pSupervisor = new Supervisor();
  _pSettings = new Settings();
  WiFi.setHostname(_pSupervisor->GetNodeName());

  //_pLedPwm = new LedPwmEsp32();
  //_pLedManager = new LedManager(_pLedPwm, 16);

  _pLedPwm = new LedRGB(16, 13);
  _pLedManager = new LedManager(_pLedPwm, 16);

  WiFiManager wifiManager;
  
  wifiManager.autoConnect("SequenceController", "12345678");
  //wifiManager.startConfigPortal("SequenceController", "12345678");
  Serial.print("after autoconnect: ");
  Serial.println(WiFi.localIP());

  pMyWebServer = new MyWebServer(_pSupervisor);

  _pSettings->Init();
  _pSupervisor->Init(_pLedManager, _pSettings, Callback);

  Serial.println("Setup completed");

    xTaskCreate(
                    taskOne,          /* Task function. */
                    "TaskOne",        /* String with name of task. */
                    30000,            /* Stack size in bytes. */
                    NULL,             /* Parameter passed as input of the task */
                    1,                /* Priority of the task. */
                    NULL);            /* Task handle. */

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
  delay(1000000);
  //StackWatcher::Log("loop");
  //_pSupervisor->ExecuteLoop();

  //pMyWebServer->HandleClient();

  //supervisor.Execute();

  //TrackMemory();
    
  //delay(10);
}
