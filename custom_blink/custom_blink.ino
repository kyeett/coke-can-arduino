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

CHSV scn_square_wave(int i, int hue, int saturation, bool is_on) {

  CHSV temp = CHSV( 0, 255, 0);
  if(is_on) {
    temp = CHSV( hue, (255*saturation)/100, 255);
  }
  return temp;
}

bool square_filter(int slot_length, int total_slots, int num_high_slots, int value) {
  if(value/slot_length % total_slots < num_high_slots) { 
    return 1; 
  }
  else { 
    return 0; 
  }
}

CHSV scn_heartbeat(int i, int hue, int saturation, int frequency, bool is_on) {

  CHSV temp = CHSV( 0, 255, 0);
  if(is_on) {
    temp = CHSV( hue, (255*saturation)/100, cubicwave8(frequency*i));
  }
  return temp;
}

// Colors
#define COLOR_RED     0
#define COLOR_GREEN   103
#define COLOR_BLUE    160 

// Scenaris
#define OFF           0
#define HEART_ONLY    1
#define PARTY_MODE    2

void loop() { 
  // Turn the LED on, then pause
  
  //leds[2] = CRGB::Red;

  int HEARTBEAT_FREQUENCY = 2;
  int HEARTBEAT_TOTAL_SLOTS = 3;
  int SLOT_LENGTH = 256; //Number of i's per timeslot
  int GROUND_FREQUENCY = 1;
  int GROUND_SILENT_PERIOD = 6;
  int GROUND_RANDOM_OFFSET = 652; //Chosen by dice roll

  int offset_i;

  for(int scenario = 0; scenario < 3; scenario++) {
    for(int i = 0; i < 256*6; i++) {

      // EYE LEDS
      offset_i = i + 66;
      switch (scenario) {
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
          leds[LEFT_EYE_LED]  = scn_square_wave(offset_i, COLOR_BLUE, 100, square_filter(SLOT_LENGTH, HEARTBEAT_TOTAL_SLOTS, 1, offset_i));
          leds[RIGHT_EYE_LED] = scn_square_wave(offset_i, COLOR_BLUE, 100, square_filter(SLOT_LENGTH, HEARTBEAT_TOTAL_SLOTS, 1, offset_i));
          break;
        default:
          leds[LEFT_EYE_LED] = CRGB::Black;
          leds[RIGHT_EYE_LED] = CRGB::Black;
          break;
      }
      if(scenario == 1) {

      }

      // HEART LED
      switch (scenario) {
        case HEART_ONLY:
        case PARTY_MODE:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
          leds[HEART_LED] = scn_heartbeat(i, COLOR_RED, 80, HEARTBEAT_FREQUENCY, square_filter(SLOT_LENGTH, HEARTBEAT_TOTAL_SLOTS, 1, i));
          break;
        default:
          leds[HEART_LED] = CRGB::Black;
          break;
      }

      // GROUND LEDS
        switch (scenario) {
        case 3:
        case 4:
        case 5:
        case 6:
        case PARTY_MODE:
          for(int j = 0; j < 4; j++) {
            offset_i = i - j*64 + GROUND_RANDOM_OFFSET;
            leds[GROUND_LED + j] = scn_heartbeat(offset_i, COLOR_GREEN, 80, GROUND_FREQUENCY, square_filter(SLOT_LENGTH, HEARTBEAT_TOTAL_SLOTS, 1, offset_i));
          }
          break;
        default:
          for(int j = 0; j < 4; j++) {
            offset_i = i - j*64 + GROUND_RANDOM_OFFSET;
            leds[GROUND_LED + j] = CRGB::Black;
          }
          break;
      }


      //Update LEDs
      FastLED.show();
    }
  }
  
  /*delay(500);
  // Now turn the LED off, then pause
  leds[2] = CRGB::Black;
  FastLED.show();
  delay(500);*/
}
