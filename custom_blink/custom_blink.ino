#include "FastLED.h"
#include <CapacitiveSensor.h>

// FastLED params
#define NUM_LEDS 7
#define BRIGHTNESS  255
#define DATA_PIN 5
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

// Setup Sensors
CapacitiveSensor cs_4_8 = CapacitiveSensor(4,8); // 1M resistor

// Colors
#define COLOR_RED     0
#define COLOR_GREEN   103
#define COLOR_BLUE    160 

// Scenarios
#define OFF           0
#define HEART_ONLY    1
#define PARTY_MODE    2

// LED NAMES
#define LEFT_EYE_LED  0
#define RIGHT_EYE_LED 1
#define HEART_LED     2
#define GROUND_LED    3

void setup() {
  Serial.begin(9600);
  cs_4_8.set_CS_AutocaL_Millis(0xFFFFFFFF); // Turn off autocalibration
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}


CHSV scn_square_wave(int i, int hue, int saturation, bool is_on) {

  CHSV temp = CHSV( 0, 255, 0);
  if(is_on) {
    temp = CHSV( hue, (255*saturation)/100, 255);
  }
  return temp;
}

CHSV scn_heartbeat(int i, int hue, int saturation, int frequency, bool is_on) {

  CHSV temp = CHSV( 0, 255, 0);
  if(is_on) {
    temp = CHSV( hue, (255*saturation)/100, cubicwave8(frequency*i));
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

// Application variables
int i = 0;
int offset_i;               // Used to offset LEDs
int scenario = PARTY_MODE;  // Initial scenario
int number_of_scenarios = 3; 
bool sensor_is_touched_last_round = false;

void loop() { 
  int HEARTBEAT_FREQUENCY = 2;
  int HEARTBEAT_TOTAL_SLOTS = 3;
  int SLOT_LENGTH = 256; //Number of i's per timeslot
  int GROUND_FREQUENCY = 1;
  int GROUND_SILENT_PERIOD = 6;
  int GROUND_RANDOM_OFFSET = 652; //Chosen by dice roll

  long current_sensor_value =  cs_4_8.capacitiveSensor(50);
  
  // Check if sensor is considered touched
  bool sensor_is_touched_this_round = current_sensor_value > 1000;
    // print sensor output
   
  // Change state only when sensors goes from low to high
  if(sensor_is_touched_this_round && !sensor_is_touched_last_round)
  {
    scenario = (scenario + 1) % number_of_scenarios;
  }
  sensor_is_touched_last_round = sensor_is_touched_this_round;
  Serial.println(scenario);

  EVERY_N_MILLISECONDS(2) {
    i = (i + 6) % (256*6);
    //Serial.println(i);
    
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
