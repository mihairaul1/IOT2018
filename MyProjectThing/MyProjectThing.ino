// MyProjectThing.ino
// COM3505 2018 project template sketch. Do your project code here.
// Out of the box the sketch is configured to kick the tyres on all the
// modules, and allow stepping through tests via the touch screen. Change the 
// TestScreen::activate(true); to false to change this behaviour.

#include "unphone.h"

#define PIN A0
#define NUM_LEDS 8
#define BRIGHTNESS 10
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_RGBW + NEO_KHZ800);

// colour definitions
const int BLACK =     HX8357_BLACK;
const int BLUE =      HX8357_BLUE;
const int RED =       HX8357_RED;
const int GREEN =     HX8357_GREEN;
const int CYAN =      HX8357_CYAN;
const int MAGENTA =   HX8357_MAGENTA;
const int YELLOW =    HX8357_YELLOW;
const int WHITE =     HX8357_WHITE;

TS_Point lastPoint;
bool escape = false;
bool stop = false;

static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}

uint32_t targetTime = 0;
uint8_t hh=conv2d(__TIME__), mm=conv2d(__TIME__+3), ss=conv2d(__TIME__+6);  // Get H, M, S from compile time
uint32_t ah = 0; uint32_t am = 0; // Alarm
uint32_t ch = 0; uint32_t cm = 0; // Alarm
int ax = 115; int ay = 280;


void setup() {
  Serial.begin(115200);
  Wire.setClock(100000);
  Wire.begin();
  IOExpander::begin();
  checkPowerSwitch(); // check if the power switch is now off & if so shutdown
  delay(100);

  if(! ts.begin())
    Serial.println("failed to start touchscreen controller");
  else
    Serial.println("touchscreen started");
  
  tft.begin(HX8357D);
  IOExpander::digitalWrite(IOExpander::BACKLIGHT,HIGH);
  tft.fillScreen(BLACK);
  
  tft.setTextColor(WHITE, BLACK);  // Adding a background colour erases previous text automatically
  tft.setTextSize(3);

  targetTime = millis() + 1000;
  
  strip.setBrightness(BRIGHTNESS);
  strip.begin();

}

void loop() {
  bool usbPowerOn = checkPowerSwitch(); // shutdown if switch off
  mainScreen();

}

void mainScreen(){

  // retrieve a point  
  TS_Point p = ts.getPoint();

  p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
  Serial.println(ts.touched());
  Serial.print("X = "); Serial.print(p.x); Serial.print("\tY = ");
  Serial.print(p.y);  Serial.print("\tPressure = "); Serial.println(p.z);

  tft.setTextWrap(false);
  tft.setCursor(30,30);
  tft.println("Dawn Simulator");
  tft.setCursor(80,80);
  tft.println("Alarm Clock");
  tft.setCursor(85, 120);
  if (targetTime < millis()) {
    targetTime = millis()+1000;
    ss++;              // Advance second
    if (ss==60) {
      ss=0;
      mm++;            // Advance minute
      if(mm>59) {
        mm=0;
        hh++;          // Advance hour
        if (hh>23) hh=0;
      }
    }
  }
  
  if (hh<10){
    tft.print("0");
    tft.print(hh);
  }
  else tft.print(hh);
  tft.print(":");
  if (mm<10){
    tft.print("0");
    tft.print(mm);
  }
  else tft.print(mm);
  tft.print(":");
  if (ss<10){
    tft.print("0");
    tft.print(ss);
  }
  else tft.print(ss);

  // Alarm Hour trangle
  tft.fillTriangle(ax-15, ay-60, ax-30, ay-40, ax, ay-40, WHITE);
  tft.fillTriangle(ax-15, ay+80, ax-30, ay+60, ax, ay+60, WHITE);

  // Alarm Hour trangle
  tft.fillTriangle(ax+100, ay-60, ax+85, ay-40, ax+115, ay-40, WHITE);
  tft.fillTriangle(ax+100, ay+80, ax+85, ay+60, ax+115, ay+60, WHITE);

  // Confirm button
  tft.drawRect(ax-10,ay+120,100,50,WHITE);

  tft.setCursor(ax+15,ay+135);
  tft.print("Set");

  if (p.x >= 180 && p.x <= 270 && ts.touched()){
    if (p.y >= 230 && p.y <= 280) {
        ah++;
      }
    if (p.y >= 110 && p.y <= 160) {
        ah--;
      }
  }

  if (p.x >= 70 && p.x <= 150 && ts.touched()){
    if (p.y >= 230 && p.y <= 280) {
        am++;
      }
    if (p.y >= 110 && p.y <= 160) {
        am--;
      }
  }

    if (p.x >= 120 && p.x <= 230 && p.y >= 20 && p.y <= 70 && ts.touched()){
      cm = am;
      ch = ah;
      stop = false;
    }

  // Print next alarm
  tft.setCursor(ax-60,ay-110);
  tft.print("Next: ");
  if (ch<10){
    tft.print("0");
    tft.print(ch);
  }
  else tft.print(ch);
  tft.print(":");
  if (cm<10){
    tft.print("0");
    tft.print(cm);
  }
  else tft.print(cm);

  if(am>59) am=0;
  if (ah>23) ah=0;
  if (ah<0) ah=0;

  // Display alarm status
  tft.setCursor(ax,ay);
  if (ah<10){
    tft.print("0");
    tft.print(ah);
  }
  else tft.print(ah);
  tft.print(":");
  if (am<10){
    tft.print("0");
    tft.print(am);
  }
  else tft.print(am);

  escape = false; // init the escape bool
  
  // if (ch==hh&&cm==mm&&stop){
  if (ch==hh&&cm==mm&&!stop){
    tft.fillScreen(BLACK);
    Serial.println("Alarm!!!");
    tft.setCursor(20, 30);
    tft.println("Alarming!");

    // Making the Snooze button

    tft.drawRect(105, 400, 120, 50, WHITE);
    tft.setCursor(130, 410);
    tft.print("Stop");

    tft.drawRect(105, 200, 120, 50, WHITE);
    tft.setCursor(112, 210);
    tft.print("Snooze");
    
    dawnSimulate();

  }
}



void dawnSimulate(){

  for(uint16_t i=0; i<255; i++) {
    for(uint16_t ledPosition=0; ledPosition < NUM_LEDS; ledPosition++){
      strip.setPixelColor(ledPosition, strip.Color(0, i, 0));
    }
    strip.show();
    delay(40);
    coordinates();
    if (escape) return;
  }
  for(uint16_t i=0; i<255; i++) {
    for(uint16_t ledPosition=0; ledPosition < NUM_LEDS; ledPosition++){
      strip.setPixelColor(ledPosition, strip.Color(i, 255, 0));
    }  
    strip.show();
    delay(40);
    coordinates();
    if (escape) return;
  }
  for(uint16_t i=0; i<255; i++) {
    for(uint16_t ledPosition=0; ledPosition < NUM_LEDS; ledPosition++){
      strip.setPixelColor(ledPosition, strip.Color(255, 255, i));
    }
    strip.show();
    delay(40);
    coordinates();
    if (escape) return;
  }
  for(uint16_t i=0; i<255; i++) {
    for(uint16_t ledPosition=0; ledPosition < NUM_LEDS; ledPosition++){
      strip.setPixelColor(ledPosition, strip.Color(255, 255, 255, i));
    }
    strip.show();
    delay(40);
    coordinates();
    if (escape) return;
  }

  for(uint16_t i=255; i>0; i--) {
    for(uint16_t ledPosition=0; ledPosition < NUM_LEDS; ledPosition++){
      strip.setPixelColor(ledPosition, strip.Color(0, 0, 0, i));
    }  
    strip.show();
    delay(75.3);
    coordinates();
    if (escape) return;
  }

  mm++;

  tft.fillScreen(BLACK);
}

void coordinates(){
  TS_Point p = ts.getPoint();

  p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
  Serial.println(ts.touched());
  Serial.print("INNNNNN X = "); Serial.print(p.x); Serial.print("\tY = ");
  Serial.print(p.y);  Serial.print("\tPressure = "); Serial.println(p.z);

  if (p.x >= 100 && p.x <= 230 && p.y >= 230 && p.y <= 280 && ts.touched()){
    for(uint16_t ledPosition=0; ledPosition < NUM_LEDS; ledPosition++){
      strip.setPixelColor(ledPosition, strip.Color(0, 0, 0, 0));
    }
    strip.show();
    cm +=5;
    tft.fillScreen(BLACK);
    escape = true;
    return;
  }

  if (p.x >= 100 && p.x <= 230 && p.y >= 10 && p.y <= 90 && ts.touched()){
    for(uint16_t ledPosition=0; ledPosition < NUM_LEDS; ledPosition++){
      strip.setPixelColor(ledPosition, strip.Color(0, 0, 0, 0));
    }
    strip.show();
    stop = true;
    tft.fillScreen(BLACK);
    escape = true;
    return;
  }
}
