
#include <FastLED.h>

#define LED_PIN 5
#define LED_COUNT 15
#define LED_TYPE WS2812B
// #define COLOR_ORDER BRG
#define COLOR_ORDER GRB
#define INNER_LED_PIN 2
#define INNER_LED_COUNT 1

CRGB leds[LED_COUNT];
CRGB inner_leds[INNER_LED_COUNT];


void setup() {
  Serial.begin(115200);
  Serial.println("LED Test Start");
  //FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, LED_COUNT).setCorrection(TypicalLEDStrip);
  //FastLED.setBrightness(5);



  
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, LED_COUNT);
  FastLED.setBrightness(5);
  FastLED.clear();

  //FastLED.addLeds<NEOPIXEL, INNER_LED_PIN>(inner_leds, INNER_LED_COUNT);

  //inner_leds[0] = CRGB::Blue;
  //FastLED.show();
}

void loop() {

    
  Serial.print("RED");
  Serial.println();
  //inner_leds[0] = CRGB::Red;
  //FastLED.show();
  for (int i = 0; i < LED_COUNT; i++) {
    leds[i] = CRGB::Red; // 全てのLEDを青に設定
  }
    FastLED.show();
  delay(1000);

  Serial.print("Blue");
  Serial.println();
  //inner_leds[0] = CRGB::Blue;
  //FastLED.show();
  for (int i = 0; i < LED_COUNT; i++) {
    leds[i] = CRGB::Blue; // 全てのLEDを青に設定
  }
    FastLED.show();
  
  delay(1000);

  Serial.print("Green");
  Serial.println();
  //inner_leds[0] = CRGB::Green;
  //FastLED.show();
  for (int i = 0; i < LED_COUNT; i++) {
    leds[i] = CRGB::Green; // 全てのLEDを青に設定
  }
    FastLED.show();
  
  delay(1000);
}
