#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN A0

#define NUM_LEDS 8

#define BRIGHTNESS 50

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_RGBW + NEO_KHZ800);

byte neopix_gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };


void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  dawnSimulate(50);

}

void dawnSimulate(uint8_t wait){
  for(uint16_t i=0; i<255; i++) {
    for(uint16_t ledPosition=0; ledPosition < NUM_LEDS; ledPosition++){
      strip.setPixelColor(ledPosition, strip.Color(0, i, 0));
    }
    strip.show();
    delay(50);
  }
  for(uint16_t i=0; i<255; i++) {
    for(uint16_t ledPosition=0; ledPosition < NUM_LEDS; ledPosition++){
      strip.setPixelColor(ledPosition, strip.Color(i, 255, 0));
    }
    strip.show();
    delay(50);
  }
  for(uint16_t i=0; i<255; i++) {
    for(uint16_t ledPosition=0; ledPosition < NUM_LEDS; ledPosition++){
      strip.setPixelColor(ledPosition, strip.Color(255, 255, i));
    }
    strip.show();
    delay(50);
  }
  for(uint16_t i=0; i<255; i++) {
    for(uint16_t ledPosition=0; ledPosition < NUM_LEDS; ledPosition++){
      strip.setPixelColor(ledPosition, strip.Color(255, 255, 255, i));
    }
    strip.show();
    delay(50);
  }
}

uint8_t red(uint32_t c) {
  return (c >> 16);
}
uint8_t green(uint32_t c) {
  return (c >> 8);
}
uint8_t blue(uint32_t c) {
  return (c);
}

