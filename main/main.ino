/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/

#include <CapacitiveSensor.h>

CapacitiveSensor cs_4_8 = CapacitiveSensor(4,8); // 1M resistor

int SLEEP_TIME = 100;
int LED_9 = 9;
int LED_10 = 10;
int sensor_is_touched_this_round; 
int sensor_is_touched_last_round = 0; 
int number_of_states = 3; //Includes 0 state
int state = 0;

// the setup function runs once when you press reset or power the board
void setup() {  
  Serial.begin(9600);

  cs_4_8.set_CS_AutocaL_Millis(0xFFFFFFFF); // Turn off autocalibration
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_9, OUTPUT);
  pinMode(LED_10, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  
  long sensor1 =  cs_4_8.capacitiveSensor(50);
  
  
  

  // Check if sensor is considered touched
  sensor_is_touched_this_round = sensor1 > 1000;
  Serial.println(sensor1);  // print sensor output 

  // Change state only when sensors goes from low to high
  if(sensor_is_touched_this_round && !sensor_is_touched_last_round)
  {
    state = (state + 1) % number_of_states;
  }
  sensor_is_touched_last_round = sensor_is_touched_this_round;

  switch (state) {
    case 0:
      digitalWrite(LED_9, LOW);
      digitalWrite(LED_10, LOW);
      break;
    case 1:
      digitalWrite(LED_9, HIGH);
      digitalWrite(LED_10, LOW);
      break;
    case 2:
      digitalWrite(LED_9, LOW);
      digitalWrite(LED_10, HIGH);
      break;
  }
  delay(10);
  
}
