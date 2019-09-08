#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>

#define strcpy_s strcpy
#define strncpy_s strncpy
#define strncmp_s strncmp

#include "CommandSource.h"
#include "LedState.h"
#include "BrightnessTarget.h"
#include "LedCommand.h"
#include "LedPwm.h"
#include "LedPwmEsp32.h"
#include "LedManager.h"
#include "Variable.h"
#include "ExecutionContext.h"
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

  Serial.println(LED_BUILTIN);

  commandSource.SetCommand("$100$0,1.0,1,0.0$100$0,0.0,1,1.0");
}

void loop() {

  timebase.DoTick();
  delay(10);

  //digitalWrite(LED_BUILTIN, HIGH);
  //delay(1000);
  //digitalWrite(LED_BUILTIN, LOW);
  //delay(1000);
}