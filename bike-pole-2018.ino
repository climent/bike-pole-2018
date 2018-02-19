#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 210
#define LINE 70

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 7
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

// Button pins for up and down
#define BRI_UP 9
#define BRI_DW 11

// Global Brightness
const uint8_t brightnessCount = 5;
uint8_t brightnessMap[brightnessCount] = { 16, 32, 64, 128, 255 };
uint8_t briLevel = 2;


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
  EVERY_N_MILLISECONDS(10) {
    for (int i = 0; i < LINE; i++) {
      // Set the i'th led, and show it
      SetPixel(i, CHSV(hue++, 255, 255));
      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      // leds[i] = CRGB::Black;
      fadeall();
      // Wait a little bit before we loop around and do it again
      CheckButtons();
    }
    // Now go in the other direction.
    for (int i = (LINE) - 1; i >= 0; i--) {
      // Set the i'th led, and show it
      SetPixel(i, CHSV(hue++, 255, 255));
      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      // leds[i] = CRGB::Black;
      fadeall();
      CheckButtons();
    }
  }
}

int millisBtwnPushes = 100;
int lastPush = 0;
int lastUpState = HIGH;
int lastDwState = HIGH;
int now = millis();
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
        Serial.println("blink!");
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
        Serial.println("blink!");
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

void Blink()
{
  for (int j = 0; j < 2; j++)
  {
    for (int i = 0; i < LINE; i++)
    {
      SetPixel(i, CHSV(96, 255, 255));
      delay(0);
    }
    FastLED.show();
    delay(10);
    for (int i = 0; i < LINE; i++)
    {
      SetPixel(i, CHSV(0, 0, 0));
      delay(0);
    }
    delay(10);
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
