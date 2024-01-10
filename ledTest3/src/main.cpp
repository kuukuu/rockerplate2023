#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN 5
#define LED_COUNT 20
#define LED_TYPE WS2811
#define COLOR_ORDER BRG
// #define COLOR_ORDER GRB
#define INNER_LED_PIN 2
#define INNER_LED_COUNT 1

CRGB leds[LED_COUNT];
CRGB inner_leds[INNER_LED_COUNT];


void setup() {
  Serial.begin(115200);
  Serial.println("LED Test Start");
  //FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, LED_COUNT).setCorrection(TypicalLEDStrip);
  //FastLED.setBrightness(5);



  FastLED.clear();
  
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, LED_COUNT).setCorrection(TypicalLEDStrip);;

  //FastLED.addLeds<NEOPIXEL, INNER_LED_PIN>(inner_leds, INNER_LED_COUNT);
  FastLED.setBrightness(100);

}

void loop() {

    
  Serial.print("RED");
  Serial.println();
  FastLED.clear();
  inner_leds[0] = CRGB( 255,0,0);
  for (int i = 0; i < LED_COUNT; i++) {
    leds[i] = CRGB( 255,0,0);
  }
    FastLED.show();
  delay(1000);

  Serial.print("Blue");
  Serial.println();
  FastLED.clear();
  inner_leds[0] = CRGB( 0,0,255);
  for (int i = 0; i < LED_COUNT; i++) {
    leds[i] = CRGB( 0,0,255);
  }
    FastLED.show();
  
  delay(1000);

  Serial.print("Green");
  Serial.println();
  FastLED.clear();
  inner_leds[0] = CRGB( 0,255,0);
  for (int i = 0; i < LED_COUNT; i++) {
    leds[i] = CRGB( 0,255,0);
  }
    FastLED.show();
  
  delay(1000);
}
