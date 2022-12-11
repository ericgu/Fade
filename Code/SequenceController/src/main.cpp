#define configUSE_TRACE_FACILITY 1
#define DEBUG_ESP_HTTP_SERVER 1

#include <Arduino.h>
#include <arduinonvs.h>
#include <stdio.h>
#include <stdlib.h>

#include <Preferences.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h>

#include "MyUdp.h"

#include <UdpLogger.h>

#include "DimEnvironment.h"

#include <ArduinoNvs.h>
#include <Settings.h>
#include <Supervisor.h>

//WiFiUDP wifiudp;

#include "MyWebServer.h"

LedDeviceCreator *_pLedDeviceCreator;
ILedDevice *_pLedDevice;
LedManager *_pLedManager;

ButtonCreator *_pButtonCreator;

Supervisor *_pSupervisor;
Settings *_pSettings;

MyWebServer *pMyWebServer;
//MyUdp *pMyUdp;

void Callback()
{
    //Serial.println("Callback");
    //pMyWebServer->HandleClient();

    //TrackMemory();

    //delay(10);
}

void RunDim(void *parameter)
{
    Serial.println("RunDim Start");
    StackWatcher::Init();
    //StackWatcher::Enable();

    StackWatcher::Log("Task Start");
    _pSupervisor->ExecuteLoop();
}

void HandleWebClient(void *parameter)
{
    Serial.println("HandleWebClient Start");
    while (true)
    {
        pMyWebServer->HandleClient();
        //pMyUdp->HandleClient();
        delay(25);
    }
}

void PrintResetReason(esp_reset_reason_t resetReason)
{
    Serial.print("Reset Reason: ");
    Serial.print(resetReason);
    Serial.print(" ");

    switch (resetReason)
    {
    case ESP_RST_UNKNOWN:
        Serial.println("ESP_RST_UNKNOWN");
        break;
    case ESP_RST_POWERON:
        Serial.println("ESP_RST_POWERON");
        break;
    case ESP_RST_EXT:
        Serial.println("ESP_RST_EXT");
        break;
    case ESP_RST_SW:
        Serial.println("ESP_RST_SW");
        break;
    case ESP_RST_PANIC:
        Serial.println("ESP_RST_PANIC");
        break;
    case ESP_RST_INT_WDT:
        Serial.println("ESP_RST_INT_WDT");
        break;
    case ESP_RST_TASK_WDT:
        Serial.println("ESP_RST_TASK_WDT");
        break;
    case ESP_RST_WDT:
        Serial.println("ESP_RST_WDT");
        break;
    case ESP_RST_DEEPSLEEP:
        Serial.println("ESP_RST_DEEPSLEEP");
        break;
    case ESP_RST_BROWNOUT:
        Serial.println("ESP_RST_BROWNOUT");
        break;
    case ESP_RST_SDIO:
        Serial.println("ESP_RST_SDIO");
        break;
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("setup");
    Serial.flush();
    StackWatcher::Log("setup");
    //StackWatcher::Enable();
    _pSupervisor = new Supervisor();
    _pSettings = new Settings();
    _pSettings->Init();

    esp_reset_reason_t resetReason = esp_reset_reason();

    PrintResetReason(resetReason);

    switch (resetReason)
    {
    case ESP_RST_POWERON:
    case ESP_RST_SW:
    case ESP_RST_DEEPSLEEP:
    case ESP_RST_BROWNOUT:
        Serial.println("Enabling execution");
        _pSettings->SaveShouldExecuteCode(true);
        break;

    default:
        Serial.println("Disabling execution because of reset");
        _pSettings->SaveShouldExecuteCode(false);
    }

    WiFi.setHostname(_pSupervisor->GetNodeName());

    //_pLedPwm = new LedPwmEsp32();
    //_pLedManager = new LedManager(_pLedPwm, 16);

    _pLedDeviceCreator = new LedDeviceCreator();
    //_pLedDevice = new LedRGB(33, 13);

    WiFiManager wifiManager;
    wifiManager.setDebugOutput(true);
    //wifiManager.resetSettings();
    //wifiManager.startConfigPortal();

    char networkName[32];

    _pSettings->LoadNetworkName(networkName, sizeof(networkName));
    if (networkName[0] == 0)
    {
        sprintf(networkName, "FadeController_%x", esp_random());
        _pSettings->SaveNetworkName(networkName);
    }

    //Serial.print("Connecting as: ");
    //Serial.println(networkName);
    //Serial.print("|");
    //Serial.print(WiFi.SSID());
    //Serial.println("|");
    //Serial.print("|");
    //Serial.print(WiFi.psk());
    //Serial.println("|");

    wifiManager.setTimeout(60);
    wifiManager.autoConnect(networkName, "12345678");
    //Serial.print("after autoconnect: ");

#if fred
    //WiFi.begin();

    //WiFi.begin("DozerNet", "Gunnerson");

    Serial.print("Connecting to WiFi..");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(50);
        Serial.print(".");
    }
    Serial.println();

    Serial.println("Connected to the WiFi network");
#endif

    Serial.println(WiFi.localIP());

    pMyWebServer = new MyWebServer(_pSupervisor, WiFi.localIP());

    _pLedManager = new LedManager(_pLedDeviceCreator);
    _pButtonCreator = new ButtonCreator();
    _pSupervisor->Init(_pLedManager, _pSettings, Callback, _pButtonCreator);

    Serial.println("Setup completed");

    Serial.print("Setup: priority = ");
    Serial.println(uxTaskPriorityGet(NULL));

    //RunDim(0);



    BaseType_t taskResult =
        xTaskCreatePinnedToCore(
            RunDim, /* Task function. */
            "Dim",  /* String with name of task. */
            30000,  /* Stack size in bytes. */
            NULL,   /* Parameter passed as input of the task */
            5,      /* Priority of the task. */
            NULL,
            1); /* Task handle. */
    Serial.print("RunDim result: ");
    Serial.println(taskResult);

    taskResult =
        xTaskCreate(
            HandleWebClient,   /* Task function. */
            "HandleWebClient", /* String with name of task. */
            10000,             /* Stack size in bytes. */
            NULL,              /* Parameter passed as input of the task */
            5,                 /* Priority of the task. */
            NULL);             /* Task handle. */

    Serial.print("HandleWedClient result: ");
    Serial.println(taskResult);
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
    //pMyWebServer->HandleClient();
    vTaskDelay(portMAX_DELAY);

    //StackWatcher::Log("loop");
    //_pSupervisor->ExecuteLoop();

    //pMyWebServer->HandleClient();

    //supervisor.Execute();

    //TrackMemory();

    //delay(10);
}
