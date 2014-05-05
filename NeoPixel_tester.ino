#include <Adafruit_NeoPixel.h>

// Modified from the strandtest code provided with
// the Adafruit NeoPixel library

#define PIN A0
#define N_PIXELS  5

uint8_t  maxBrightness = 10;  // Maximum brightness of the pixels

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(9600);
}

void loop() {
  // Some example procedures showing how to display to the pixels:
  //RGBTest(500);
  //rainbow(5);
  rainbowCycle(5);
  //levelMeter(30000, 255, 0, 0);
  delay(1000);
  levelSweepRed(5);
}

void RGBTest(uint8_t wait) {
  colorWipe(strip.Color(maxBrightness, 0, 0), wait); // Red
  colorWipe(strip.Color(0, maxBrightness, 0), wait); // Green
  colorWipe(strip.Color(0, 0, maxBrightness), wait); // Blue
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j< 256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(((WheelPos * 3)*maxBrightness)/255, ((255 - WheelPos * 3)*maxBrightness)/255, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(((255 - WheelPos * 3)*maxBrightness)/255, 0, ((WheelPos * 3)*maxBrightness)/255);
  } else {
   WheelPos -= 170;
   return strip.Color(0, ((WheelPos * 3)*maxBrightness)/255, ((255 - WheelPos * 3)*maxBrightness)/255);
  }
}

void levelMeter(uint16_t value, uint8_t red, uint8_t green, uint8_t blue) {
  red = (red * maxBrightness)/255;
  green = (green * maxBrightness)/255;
  blue = (blue * maxBrightness)/255;
  uint16_t i;
  for(i=0; i < (uint16_t)((value/65535.) * strip.numPixels()); i++){
    strip.setPixelColor(i, strip.Color(red, green, blue));
  }
  // Calculate the fraction of the way to filling the next pixel
  uint8_t frac = ((value * strip.numPixels()) % 65535)/255;
  strip.setPixelColor(i, strip.Color((frac*red)/255, (frac*green)/255, (frac*blue)/255));
  for(i++; i< strip.numPixels(); i++){
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
}

void levelSweepRed(uint8_t wait) {
  uint8_t i;
  for(i=0; i < 255; i++){
    levelMeter(65535*(i/255.), 255, 0, 0);
    delay(wait);
  }
  for(i=254; i > 0; i--){
    levelMeter(65535*(i/255.), 255, 0, 0);
    delay(wait);
  }
}

