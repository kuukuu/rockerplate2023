#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN 5
#define LED_COUNT 10
#define LED_TYPE WS2811
#define COLOR_ORDER BRG
// #define COLOR_ORDER GRB

CRGB leds[LED_COUNT];

void setup() {
  Serial.begin(115200);
  Serial.println("LED Test Start");
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, LED_COUNT); // .setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(5);
}

void loop() {
  Serial.print("RED");
  Serial.println();
  FastLED.clear();
  for (int i = 0; i < LED_COUNT; i++) {
    leds[i] = CRGB::Red; // 全てのLEDを青に設定
    FastLED.show();
  }
    delay(1000);
  Serial.print("Green");
  Serial.println();
  FastLED.clear();
  for (int i = 0; i < LED_COUNT; i++) {
    leds[i] = CRGB::Green; // 全てのLEDを青に設定
    FastLED.show();
  }
    delay(1000);
  Serial.print("Blue");
  Serial.println();
  FastLED.clear();
  for (int i = 0; i < LED_COUNT; i++) {
    leds[i] = CRGB::Blue; // 全てのLEDを青に設定
    FastLED.show();
  }
    delay(1000);
}
