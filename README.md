# coke-can-arduino
Simple Arduino project using a capacitive sensor that lights up various LEDs and Light bulbs when touched

#### Iterations

| Sprint # |                      Target                      | Time (actual) |
|----------|--------------------------------------------------|---------------|
|       10 |                                                  |               |
|       10 | Add Party Mode scenario                          | 40 ()           |
|        9 | Add 7 basic LED scenarios, name scenarios        | 30 (49)       |
|        8 | Use capacitive sensor with LED + switch scenario | 45 (31)       |
|        7 | Add two LED scenarios, Break out square-wave     | 45 (44)       |
|        6 | Solder LEDs and mount plastic figure             | (2h+)         |
|        5 | Use digispark as Arduino (CapacitySensor)        | 30 (48)       |
|        4 | Use digispark as Arduino (LED)                   | 30 (47)       |
|        3 | Play with programmable LED strip                 | 30 (10)       |
|        2 | Git repo, Statemachine, Multi-step state machie  | 40 (40)       |
|        1 |                                                  |               |



#### Todos
* Add Party Mode scenario
* Glue EYE Leds to body
* Fix sensitivity issues with sensor
* Investigate Electronics Safet y Rules :-)
* Use Digispark as Arduino (CapacitySensor)
* Use 12 V output for light
* Add image of capacity graph
* Add image of state machine
* Add photo of basic setup

#### Done
* ~~Name scenarios~~
* ~~Add 7 LED scenarios~~
* ~~Use capacitive sensor in parallel to LEDs~~
* ~~Switch scenarios using capacitive sensor~~
* ~~Use EVERY_N_MILLISECONDS~~
* ~~Break out Square-wave multiplier function~~
* ~~Add all two LED scenarios~~
* ~~Use Digispark as Arduino (LED)~~
* ~~Use programmable LED strip~~
* ~~Create git repo~~
* ~~Introduce statemachine~~
* ~~Multi-step statemachine 0 -> 1 -> 2 -> 0~~

#### scenarios
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

#### References
[FastLED HSV colors](https://raw.githubusercontent.com/FastLED/FastLED/gh-pages/images/HSV-rainbow-with-desc.jpg)
[Fast LED tutorial](http://jwhendy.blogspot.se/2015/10/fastled-series-getting-started.html)
