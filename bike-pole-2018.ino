#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 210
#define LINE 70

// Use CLOCK_PIN for apa102 type of LED strips
#define DATA_PIN 7
//#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

// Button pins for up and down
#define BRI_UP 9
#define BRI_DW 11

// Global Brightness
const uint8_t brightnessCount = 5;
uint8_t brightnessMap[brightnessCount] = { 16, 32, 64, 128, 255 };
uint8_t briLevel = 2;
uint8_t position = 0;

struct Leds {
  int o;
  int p;
  int q;
};
typedef struct Leds Leds;

void setup() {
  Serial.begin(57600);
  Serial.println("resetting");
  LEDS.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  LEDS.setBrightness(brightnessMap[briLevel]);

  pinMode(BRI_UP, INPUT_PULLUP);
  digitalWrite(BRI_UP, HIGH);
  pinMode(BRI_DW, INPUT_PULLUP);
  digitalWrite(BRI_DW, HIGH);
}

void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(200);
  }
}
static uint8_t hue = 0;

void loop() {
  // First slide the led in one direction
  CheckButtons();
  // Bounce();
  Flash();
  FastLED.show();
}
bool direction = true;

int timeToFlash = 2000;
int lastFlash;
int now = millis();

void Flash()
{
  now = millis();
  if (now - timeToFlash > lastFlash)
  {
    Blink();
    lastFlash = millis();
  }
}

void Bounce()
{
  fadeall();
  SetPixel(position, CHSV(hue++, 255, 255));
  // if direction == true, we are going up
  if (direction == true)
  {
    if (position == LINE - 1)
    {
      direction = false;
      position--;
    } else {
      position++;
    }
  } else {
    if (position == 0)
    {
      direction = true;
      position++;
    } else {
      position--;
    }
  }
}

int millisBtwnPushes = 100;
int lastPush = 0;
int lastUpState = HIGH;
int lastDwState = HIGH;
// int now = millis();
int upState;
int dwState;

void CheckButtons()
{
  now = millis();
  upState = digitalRead(BRI_UP);
  dwState = digitalRead(BRI_DW);
  if (upState == lastUpState && dwState == lastDwState) return;
  if (upState != lastUpState && upState == LOW)
  {
    if (now - millisBtwnPushes > lastPush)
    {
      if (briLevel < 4)
      {
        briLevel += 1;
      }
      else
      {
        Blink();
      }
    }
    lastPush = millis();
    Serial.println("up");
    lastUpState = upState;
    Serial.println(briLevel);
  } else {
    lastUpState = upState;
  }
  if (dwState != lastDwState && dwState == LOW)
  {
    if (now - millisBtwnPushes > lastPush)
    {
      if (briLevel > 0)
      {
        briLevel -= 1;
      }
      else
      {
        Blink();
      }
    }
    lastPush = millis();
    Serial.println("down");
    lastDwState = dwState;
    Serial.println(briLevel);
  } else {
    lastDwState = dwState;
  }
  LEDS.setBrightness(brightnessMap[briLevel]);
}

int lastBlink = 0;
int timeToBlink = 50;

void Blink()
{
  for (int j = 0; j < 2; j++)
  {
    for (int i = 0; i < LINE; i++)
    {
      SetPixel(i, CHSV(96, 255, 255));
    }
    FastLED.show();
    delay(40);
    for (int i = 0; i < LINE; i++)
    {
      SetPixel(i, CHSV(0, 0, 0));
    }
    delay(40);
    FastLED.show();
  }
}

Leds X(int pos)
{
  Leds leds;
  if (pos < 30)
  {
    leds.o = pos;
    leds.p = 60 - pos - 1;
    leds.q = pos + 60;
  }
  if (pos >= 30)
  {
    leds.o = pos + 60;
    leds.p = 200 - pos - 1;
    leds.q = pos + 140;
  }
  return leds;
}

void SetPixel(int i, CHSV chsv)
{
  Leds l;
  l = X(i);
  leds[l.o] = leds[l.p] = leds[l.q] = chsv;
}
