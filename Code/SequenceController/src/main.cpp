#include <Arduino.h>

//int LED_BUILTIN = 2;

const int ledPins[16] = { 2,  4,  5, 18, 19, 21, 22, 23,   // main row
                          14, 27, 26, 25, 33, 32, 12, 13};  // extension row

const int PwmFrequency = 500;

void setup() {
  //pinMode (LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  Serial.println(LED_BUILTIN);

  for (int i = 0; i < 16; i++)
  {
    ledcSetup(i, PwmFrequency, 8);
    ledcAttachPin(ledPins[i], i);
  }
}
void loop() {

  // increase the LED brightness
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    // changing the LED brightness with PWM
    for (int i = 0; i < 16; i++)
    {
      ledcWrite(i, dutyCycle);
    }
    delay(15);
  }

  // decrease the LED brightness
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    // changing the LED brightness with PWM
    for (int i = 0; i < 16; i++)
    {
      ledcWrite(i, dutyCycle);
    }
    delay(15);
  }


  //digitalWrite(LED_BUILTIN, HIGH);
  //delay(1000);
  //digitalWrite(LED_BUILTIN, LOW);
  //delay(1000);
}