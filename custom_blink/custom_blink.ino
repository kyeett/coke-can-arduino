#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 7
#define BRIGHTNESS  255

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 5
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
      // Uncomment/edit one of the following lines for your leds arrangement.
      // FastLED.addLeds<TM1803, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<TM1804, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<TM1809, DATA_PIN, RGB>(leds, NUM_LEDS);
      //FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  	  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
      // FastLED.addLeds<APA104, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<UCS1903, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<UCS1903B, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<GW6205, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<GW6205_400, DATA_PIN, RGB>(leds, NUM_LEDS);
      
      // FastLED.addLeds<WS2801, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<SM16716, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<LPD8806, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<P9813, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<APA102, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<DOTSTAR, RGB>(leds, NUM_LEDS);

      // FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<SM16716, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<DOTSTAR, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
}

// LED_POSITIONS
#define LEFT_EYE_LED  0
#define RIGHT_EYE_LED 1
#define HEART_LED     2
#define GROUND_LED    3

CHSV scn_square_wave(int i, int hue, int saturation)
{

  CHSV temp;
  if(i/256 % 12 == 0)
  {
    temp = CHSV( hue, (255*saturation)/100, 255);
  }
  else 
  {
    temp = CHSV( 0, 255, 0);
  }
  return temp;
}

CHSV scn_heartbeat(int i, int hue, int saturation, int frequency, int silent_period)
{

  CHSV temp = CHSV( 0, 255, 0);
  if(i/256 % silent_period == 0)
  {
    temp = CHSV( hue, (255*saturation)/100, cubicwave8(frequency*i));
  }
  return temp;
}

void loop() { 
  // Turn the LED on, then pause
  
  //leds[2] = CRGB::Red;

  int HEARTBEAT_FREQUENCY = 2;
  int HEARTBEAT_SILENT_PERIOD = 4;
  int GROUND_FREQUENCY = 1;
  int GROUND_SILENT_PERIOD = 6;
  int GROUND_RANDOM_OFFSET = 652; //Chosen by dice roll
  
  for(int i = 0; i < 256*12; i++) {
    
    //leds[1] = heartbeat_hsv(i, 96, 40);
    leds[LEFT_EYE_LED]  = scn_square_wave(i + 66, 160, 100);
    leds[RIGHT_EYE_LED] = scn_square_wave(i + 66, 160, 100);
    leds[HEART_LED]     = scn_heartbeat(i, 0, 80, HEARTBEAT_FREQUENCY, HEARTBEAT_SILENT_PERIOD);
    
    for(int j = 0; j < 4; j++) {
      leds[GROUND_LED + j] = scn_heartbeat(i - j*64 + GROUND_RANDOM_OFFSET, 103, 80, GROUND_FREQUENCY, HEARTBEAT_SILENT_PERIOD);
    }
    FastLED.show();
  }
  
  /*delay(500);
  // Now turn the LED off, then pause
  leds[2] = CRGB::Black;
  FastLED.show();
  delay(500);*/
}
