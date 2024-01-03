#include <Arduino.h>

#define BUTTON 3     //実装スイッチ

void setup() {
  Serial.begin(115200);
  Serial.println("buttonTest start");
    pinMode(BUTTON, INPUT_PULLUP);
}

void loop() {
  
  Serial.print(digitalRead(BUTTON));
  Serial.println();
  delay(1000);
}
