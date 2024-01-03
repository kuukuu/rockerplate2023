#include <Arduino.h>
// put function declarations here:
int myFunction(int, int);
  int result = 0;

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(result) ;
  result = myFunction(result, 3);
  delay(200);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}