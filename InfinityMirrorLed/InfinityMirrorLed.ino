// Neopixel
// https://github.com/adafruit/Adafruit_NeoPixel
#include <Adafruit_NeoPixel.h>

// VISUAL OUTPUT CONFIG
#define LED_SERIAL_PIN 6 // D2
#define MAX_BRIGHTNESS 80 // 0-255
#define LED_STRIP_SIZE 150 // number of LEDs on the strip
#define LED_STRIP_OFFSET 50

#define SIGNAL_PIN 11
#define SIGNAL_FEEDBACK_PIN 13

#define DIV 6.5

//mito purple: #2f1f31 (r47 g31 b49)
//mito red: #b72025 (r183 g32 b37)

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_STRIP_SIZE, LED_SERIAL_PIN, NEO_GRB + NEO_KHZ800);


// GLOBAL PARAMETERS FOR COMPUTING
bool status = false;
bool currentStatus = false;

void setup() {
  pinMode(SIGNAL_PIN, INPUT_PULLUP);
  pinMode(SIGNAL_FEEDBACK_PIN, OUTPUT);

  // initialize led strip
  strip.begin();
  clearStrip();
}

void loop() {
  currentStatus = digitalRead(SIGNAL_PIN);
  digitalWrite(SIGNAL_FEEDBACK_PIN, currentStatus);
  // trigger controller
  if (currentStatus) {
    fillStrip();
    status = currentStatus;
  } 
  else if (!currentStatus) {
    if (status != currentStatus) {
      clearStrip();
    }
    rainbowCycle(10);
    status = currentStatus;
  }

  //delay(10); // increase this to increse latency
}

int ledIndexOffset (int currentIndex, int offset = 0) {
  if (currentIndex + offset >= LED_STRIP_SIZE) {
    return currentIndex + offset - LED_STRIP_SIZE;
  } else if (currentIndex + offset < 0) {
    return currentIndex + offset + LED_STRIP_SIZE;
  }
  return currentIndex + offset;
}

void clearStrip() {
  for(int i=0; i< strip.numPixels()/2; i++) {
      strip.setPixelColor(ledIndexOffset(i, LED_STRIP_OFFSET), Wheel(((i * MAX_BRIGHTNESS / strip.numPixels())) & 255));
      strip.setPixelColor(ledIndexOffset(strip.numPixels() - i - 1, LED_STRIP_OFFSET), Wheel(((i * MAX_BRIGHTNESS / strip.numPixels())) & 255));
      strip.show();
      delay(5);
    }
}

void fillStrip() {
  //uint32_t color = strip.Color(22, 6, 24); // red
  //uint32_t color = strip.Color(132, 0, 0); // purple
  uint32_t color = strip.Color(MAX_BRIGHTNESS, MAX_BRIGHTNESS, MAX_BRIGHTNESS);
  for(uint16_t i=strip.numPixels()/2; i>0; i--) {
    strip.setPixelColor(ledIndexOffset(i-1, LED_STRIP_OFFSET), color);
    strip.setPixelColor(ledIndexOffset(strip.numPixels() - i, LED_STRIP_OFFSET), color);
    strip.show();
    delay(5);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(ledIndexOffset(i, LED_STRIP_OFFSET), Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    if (digitalRead(SIGNAL_PIN) != status) {
      return;
    }
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  float blue =  WheelPos / DIV;

  if(WheelPos < 128) {
    return strip.Color(MAX_BRIGHTNESS, 0, blue);
  }
  return strip.Color(MAX_BRIGHTNESS, 0, 255 / DIV - blue);
}

