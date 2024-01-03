#include <Arduino.h>
#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include <BLEDevice.h>


#define INNER_LED_PIN 2
#define INNER_LED_COUNT 1

Adafruit_NeoPixel inner_pixels(INNER_LED_COUNT, INNER_LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  Serial.println("ledTest start");
  inner_pixels.begin();
  inner_pixels.setBrightness(50);
  inner_pixels.show(); // Send the updated pixel colors to the hardware.
}

void loop() {
  Serial.print("show");
  Serial.println();
  for (int i = 0; i < inner_pixels.numPixels(); i++) {
    inner_pixels.setPixelColor(i, inner_pixels.Color(255, 255, 0)); // Set all LEDs to White
  }
  inner_pixels.show(); // Send the updated pixel colors to the hardware.
  delay(1000); // Wait for 1 second
}
