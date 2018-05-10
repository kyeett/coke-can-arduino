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


CHSV scn_square_wave(int hue, int saturation, bool is_on) {

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

// Scenarios
/*
|         Name        |    Heart    |    Eyes   |  Ground  |
|---------------------|-------------|-----------|----------|
| OFF                 |             |           |          |
| HEART               | Blink       |           |          |
| HEART_EYES          | Blink       | Blink     |          |
| HEART_CONSTANT_EYES | Blink       | Constant  |          |
| ALL                 | Blink       | Constant  | Sweep    |
| CONSTANT_HEART      | Constant    |           |          |
| CONSTANT_HEART_EYES | Constant    | Constant  |          |
| CONSTANT_ALL        | Constant    | Constant  | Constant |
| PARTY_MODE          | Double time | Long fade | Blink    |
*/
#define OFF                 0
#define CONSTANT_HEART      1
#define CONSTANT_HEART_EYES 2
#define CONSTANT_ALL        3

#define OFF_2               4 //Added for symmetry
#define PARTY_MODE          5

#define OFF_3               6 //Added for symmetry
#define HEART               7
#define HEART_EYES          8
#define HEART_CONSTANT_EYES 9
#define ALL                 10

#define NUMBER_OF_SCENARIOS 11

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

  // Party mode
  int PARTY_FREQUENCY = 1;
  
  long current_sensor_value =  cs_4_8.capacitiveSensor(50);
  
  // Check if sensor is considered touched
  bool sensor_is_touched_this_round = current_sensor_value > 500;
    // print sensor output
   
  // Change state only when sensors goes from low to high
  if(sensor_is_touched_this_round && !sensor_is_touched_last_round)
  {
    scenario = (scenario + 1) % NUMBER_OF_SCENARIOS;
  }
  sensor_is_touched_last_round = sensor_is_touched_this_round;
  //Serial.println(scenario);
  Serial.println(current_sensor_value);

  EVERY_N_MILLISECONDS(2) {
    i = (i + 8) % (256*6);
    //Serial.println(i);
    
    // EYE LEDS
    offset_i = i + 66;
    switch (scenario) {

      // Constant
      case HEART_CONSTANT_EYES:
      case ALL:
      case CONSTANT_HEART_EYES:
      case CONSTANT_ALL:
        leds[LEFT_EYE_LED]  = CHSV( COLOR_BLUE, 255, 255);
        leds[RIGHT_EYE_LED]  = CHSV( COLOR_BLUE, 255, 255);
        break;

      // Blink
      case HEART_EYES:
        leds[LEFT_EYE_LED]  = scn_square_wave(COLOR_BLUE, 100, square_filter(SLOT_LENGTH, HEARTBEAT_TOTAL_SLOTS, 1, offset_i));
        leds[RIGHT_EYE_LED] = scn_square_wave(COLOR_BLUE, 100, square_filter(SLOT_LENGTH, HEARTBEAT_TOTAL_SLOTS, 1, offset_i));
        break;

      // Special: Long blink (TODO)
      case PARTY_MODE:
        leds[LEFT_EYE_LED]  = scn_heartbeat(i, COLOR_GREEN + i/4, 100, PARTY_FREQUENCY, square_filter(SLOT_LENGTH, 2, 1, i));
        leds[RIGHT_EYE_LED]  = scn_heartbeat(i+SLOT_LENGTH, COLOR_GREEN + i/4, 100, PARTY_FREQUENCY, square_filter(SLOT_LENGTH, 2, 1, i+SLOT_LENGTH));
        break;
      
      case OFF:
      default:
        leds[LEFT_EYE_LED] = CRGB::Black;
        leds[RIGHT_EYE_LED] = CRGB::Black;
        break;
    }

    // HEART LED
    switch (scenario) {

      // Blinking
      case HEART:
      case HEART_EYES:
      case HEART_CONSTANT_EYES:
      case ALL:
        leds[HEART_LED] = scn_heartbeat(i, COLOR_RED, 80, HEARTBEAT_FREQUENCY, square_filter(SLOT_LENGTH, HEARTBEAT_TOTAL_SLOTS, 1, i));
        break;
 
      // Constant heart
      case CONSTANT_HEART:
      case CONSTANT_HEART_EYES:
      case CONSTANT_ALL:
        leds[HEART_LED]  = CHSV( COLOR_RED, 255*0.8, 255);
        leds[HEART_LED]  = CHSV( COLOR_RED, 255*0.8, 255);
        break;
      
      // Special: Double time
      case PARTY_MODE:
        leds[HEART_LED] = scn_heartbeat(2*i, COLOR_RED, 80, PARTY_FREQUENCY, square_filter(SLOT_LENGTH, 2, 1, i));
        break;
      
      // Off
      case OFF:
      default:
        leds[HEART_LED] = CRGB::Black;
        break;
    }

    // GROUND LEDS
    for(int j = 0; j < 4; j++) {
      offset_i = i - j*64 + GROUND_RANDOM_OFFSET;
      
      switch (scenario) {

        // Constant
        case CONSTANT_ALL:
            leds[GROUND_LED + j] = CHSV( COLOR_GREEN, 255*0.8, 255);
          break;

        // Sweeping
        case ALL:
            leds[GROUND_LED + j] = scn_heartbeat(offset_i, COLOR_GREEN, 80, GROUND_FREQUENCY, square_filter(SLOT_LENGTH, HEARTBEAT_TOTAL_SLOTS, 1, offset_i));
          break;

        // Special: Blink
        case PARTY_MODE:
            leds[GROUND_LED + j] = scn_heartbeat(i, COLOR_GREEN + i/4, 100, PARTY_FREQUENCY, true);
          break;
          
        // Off
        case OFF:
        default:
            leds[GROUND_LED + j] = CRGB::Black;
          break;
      }
    }

    //Update LEDs
    FastLED.show();
  }
  delay(10);
}
