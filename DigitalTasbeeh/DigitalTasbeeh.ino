/* 
PIN DEFINITIONS
LED_1_O -> PB2 = 7
LED_1_W -> PB3 = 6
LED_2_W -> PB1 = 8
LED_2_O -> PB5 = 4
LED_3_O -> PA4 = 0
LED_3_W -> PA5 = 1
LED_4_O -> PA6 = 2
LED_4_W -> PA7 = 3

VIB_1    -> PA3 = 16

SW_1    -> PB4 = 5
*/

#define SW_1    5 // PB4

#define VIB_1   16 // PA3

#define LED_1_O 7 // PB2
#define LED_1_W 6 // PB3

#define LED_2_O 4 // PB5
#define LED_2_W 8 // PB1

#define LED_3_O 0 // PA4
#define LED_3_W 1 // PA5

#define LED_4_O 2 // PA6
#define LED_4_W 3 // PA7

void setup() {
  // Initialize Blue(White) Leds
  pinMode(LED_1_W, OUTPUT);
  pinMode(LED_2_W, OUTPUT);
  pinMode(LED_3_W, OUTPUT);
  pinMode(LED_4_W, OUTPUT);

  // Initialize Orange Leds
  pinMode(LED_1_O, OUTPUT);
  pinMode(LED_2_O, OUTPUT);
  pinMode(LED_3_O, OUTPUT);
  pinMode(LED_4_O, OUTPUT);

  // Turn OFF all LEDs
  digitalWrite(LED_1_W, LOW);
  digitalWrite(LED_2_W, LOW);
  digitalWrite(LED_3_W, LOW);
  digitalWrite(LED_4_W, LOW);
  digitalWrite(LED_1_O, LOW);
  digitalWrite(LED_2_O, LOW);
  digitalWrite(LED_3_O, LOW);
  digitalWrite(LED_4_O, LOW);

  // Initialize Vibration Motor
  pinMode(VIB_1, OUTPUT);

  // Initialize Switch Pin
  pinMode(SW_1, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

}
