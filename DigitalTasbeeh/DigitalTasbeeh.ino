/* 
PIN DEFINITIONS
LED_1_O -> PB2 = 7
LED_2_B -> PB3 = 6
LED_3_B -> PB1 = 8
LED_4_O -> PB5 = 4
LED_5_O -> PA4 = 0
LED_6_B -> PA5 = 1
LED_7_O -> PA6 = 2
LED_8_B -> PA7 = 3

SW_1    -> PB4 = 5

*/

#define LED_1_O 7 // PB2
#define LED_2_B 6 // PB3
#define LED_3_B 8 // PB1
#define LED_4_O 4 // PB5
#define LED_5_O 0 // PA4
#define LED_6_B 1 // PA5
#define LED_7_O 2 // PA6
#define LED_8_B 3 // PA7
#define SW_1    5 // PB4

void setup() {
  // Initialize Blue(White) Leds
  pinMode(LED_2_B, OUTPUT);
  pinMode(LED_3_B, OUTPUT);
  pinMode(LED_6_B, OUTPUT);
  pinMode(LED_8_B, OUTPUT);

  // Initialize Orange Leds
  pinMode(LED_1_O, OUTPUT);
  pinMode(LED_4_O, OUTPUT);
  pinMode(LED_5_O, OUTPUT);
  pinMode(LED_7_O, OUTPUT);

  // Turn OFF all LEDs
  digitalWrite(LED_2_B, LOW);
  digitalWrite(LED_3_B, LOW);
  digitalWrite(LED_6_B, LOW);
  digitalWrite(LED_8_B, LOW);
  digitalWrite(LED_1_O, LOW);
  digitalWrite(LED_4_O, LOW);
  digitalWrite(LED_5_O, LOW);
  digitalWrite(LED_7_O, LOW);

  // Initialize Switch Pin
  pinMode(SW_1, INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:

}
