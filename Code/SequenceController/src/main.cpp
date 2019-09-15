#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>

#include <Preferences.h>
#include <WiFi.h>

#define strcpy_s strcpy
#define strncpy_s strncpy
#define strncmp_s strncmp

#include "Command.h"
#include "CommandSource.h"
#include "LedState.h"
#include "CommandResult.h"
#include "LedCommand.h"
#include "LedPwm.h"
#include "LedPwmEsp32.h"
#include "LedManager.h"
#include "Variable.h"
#include "Stack.h"
#include "ExecutionContext.h"
#include "ListParser.h"
#include "CommandDecoder.h"
#include "ExecutionFlow.h"
#include "Timebase.h"

//int LED_BUILTIN = 2;

const int ledPins[16] = { 2,  4,  5, 18, 19, 21, 22, 23,   // main row
                          14, 27, 26, 25, 33, 32, 12, 13};  // extension row

const int PwmFrequency = 500;

CommandSource commandSource;
LedPwmEsp32 ledPwm;
LedManager ledManager(&ledPwm, 16);

Timebase timebase(&commandSource, &ledManager);

void setup() {
  //pinMode (LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  commandSource.SetCommand("$LOOP %B 100:10:-10$LOOP %A 0:7$D %B %A,1.0$D %B %A,0.0$ENDLOOP$ENDLOOP");
}

void loop() {
  timebase.DoTick();
  delay(10);
}