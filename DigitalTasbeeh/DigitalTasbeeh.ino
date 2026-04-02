// ---------------- PIN DEFINITIONS ----------------
#define SW_1    5   // PB4
#define VIB_1   16  // PA3
#define VBATT   15  // PA2
#define LED_1_O 7   // PB2
#define LED_1_W 6   // PB3
#define LED_2_O 4   // PB5
#define LED_2_W 8   // PB1
#define LED_3_O 0   // PA4
#define LED_3_W 1   // PA5
#define LED_4_O 2   // PA6
#define LED_4_W 3   // PA7

// ---------------- CONSTANTS ----------------
const uint16_t cycles[4] = {33, 99, 100, 1000};

#define MAX_VOLTAGE         3.0 // Coil Cell
#define MIN_VOLTAGE         2.0 // Cutoff Voltage
#define LOW_BATTERY_VOLTAGE (MIN_VOLTAGE + (MAX_VOLTAGE-MIN_VOLTAGE)*0.1) //10% of the battery life

#define DEBOUNCE_TIME      50
#define LONG_PRESS_3S      3000 // selection mode
#define LONG_PRESS_6S      6000 // reset
#define MODE_TIMEOUT       5000 // 5 seconds

#define LED_BLINK_TIME     50   //50ms
#define VIBRATION_40       40   //40ms
#define VIBRATION_80       80   //80ms

#define LOW_BATT_TIMEOUT   10000

// ---------------- VARIABLES ----------------
uint16_t count = 0;
uint8_t currentMode = 0;

// Button handling
bool buttonState = HIGH;
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long pressStartTime = 0;

// Events
bool evShortPress = false;
bool evLongPress3s = false;
bool evLongPress6s = false;

// Timing
unsigned long lastActivityTime = 0;
unsigned long ledTimer = 0;
unsigned long vibEndTime = 0;
unsigned long lowBattTimer = 0;

// States
bool vibrationActive = false;
bool orangeLedActive = false;
bool lowBattery = false;

enum State {
  STATE_COUNTING,
  STATE_MODE_SELECTION,
  STATE_CONFIRMATION,
  STATE_RESET
};

State currentState = STATE_COUNTING;

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

  // Initialize Switch Pin
  pinMode(SW_1, INPUT_PULLUP);

  // Initialize Vibration Motor
  pinMode(VIB_1, OUTPUT);
  
  // Initialize Vibration Motor
  pinMode(VBATT, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  readButton();
  checkBattery();

  switch (currentState) {

    case STATE_COUNTING:
      handleCounting();
      break;

    case STATE_MODE_SELECTION:
      handleModeSelection();
      break;

    case STATE_CONFIRMATION:
      handleConfirmation();
      break;

    case STATE_RESET:
      handleReset();
      break;
  }

  handleOutputs(); // LEDs, vibration (non-blocking)
}

void readButton()
{
  bool reading = digitalRead(SW_1);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_TIME) {

    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) { // RELEASED
        unsigned long pressDuration = millis() - pressStartTime;

        if (pressDuration >= LONG_PRESS_6S) {
          evLongPress6s = true;
        } 
        else if (pressDuration >= LONG_PRESS_3S) {
          evLongPress3s = true;
        } 
        else {
          evShortPress = true;
        }
      } 
      else { // PRESSED
        pressStartTime = millis();
      }
    }
  }

  lastButtonState = reading;
}

void checkBattery()
{
  int adc = analogRead(VBATT);
  float voltage = (adc*3.0)/1024.0;

  if(voltage <= LOW_BATTERY_VOLTAGE)
    lowBattery = true;
  else
    lowBattery = false;
}

void handleCounting()
{
  // RESET (highest priority)
  if (evLongPress6s) {
    currentState = STATE_RESET;
    evLongPress6s = false;
    return;
  }

  // Enter Mode Selection
  if (evLongPress3s) {
    currentState = STATE_MODE_SELECTION;
    lastActivityTime = millis();
    updateModeLEDs();
    evLongPress3s = false;
    return;
  }

  // Short press → count
  if (evShortPress) {
    lastActivityTime = millis();

    // Low battery indication (does NOT block)
    if (lowBattery) {
      turnOnAllOrange();
      lowBattTimer = millis();
    }

    count++;

    // Orange LED blink
    turnOnAllOrange();
    ledTimer = millis();
    orangeLedActive = true;

    // Cycle complete
    if (count >= cycles[currentMode]) {
      count = 0;
      startVibration(VIBRATION_40);
    }

    evShortPress = false;
  }
}

void handleModeSelection()
{
  if (evLongPress6s) {
    currentState = STATE_RESET;
    evLongPress6s = false;
    return;
  }

  // Change mode
  if (evShortPress) {
    currentMode = (currentMode + 1) % 4;
    updateModeLEDs();
    lastActivityTime = millis();
    evShortPress = false;
  }

  // Timeout → confirm
  if (millis() - lastActivityTime > MODE_TIMEOUT) {
    currentState = STATE_CONFIRMATION;
  }
}

void handleConfirmation()
{
  // Vibrate 80ms
  startVibration(VIBRATION_80);

  count = 0;

  // Turn off LEDs
  turnOffAllWhite();

  currentState = STATE_COUNTING;
}

void handleReset()
{
  currentMode = 0;
  count = 0;

  turnOffAllWhite();
  turnOffAllOrange();

  currentState = STATE_COUNTING;
}

void handleOutputs()
{
  // Orange LED OFF after blink
  if (orangeLedActive && (millis() - ledTimer > LED_BLINK_TIME)) {
    turnOffAllOrange();
    orangeLedActive = false;
  }

  // Vibration OFF
  if (vibrationActive && millis() > vibEndTime) {
    digitalWrite(VIB_1, LOW);
    vibrationActive = false;
  }

  // Low battery LED timeout
  if (lowBattery && (millis() - lowBattTimer > LOW_BATT_TIMEOUT)) {
    turnOffAllOrange();
  }
}

/*-----------------HELPERS--------------*/
void startVibration(uint16_t duration)
{
  digitalWrite(VIB_1, HIGH);
  vibrationActive = true;
  vibEndTime = millis() + duration;
}

void updateModeLEDs()
{
  digitalWrite(LED_1_W, currentMode >= 0);
  digitalWrite(LED_2_W, currentMode >= 1);
  digitalWrite(LED_3_W, currentMode >= 2);
  digitalWrite(LED_4_W, currentMode >= 3);
}

void turnOnAllOrange()
{
  digitalWrite(LED_1_O, HIGH);
  digitalWrite(LED_2_O, HIGH);
  digitalWrite(LED_3_O, HIGH);
  digitalWrite(LED_4_O, HIGH);
}

void turnOffAllOrange()
{
  digitalWrite(LED_1_O, LOW);
  digitalWrite(LED_2_O, LOW);
  digitalWrite(LED_3_O, LOW);
  digitalWrite(LED_4_O, LOW);
}

void turnOffAllWhite()
{
  digitalWrite(LED_1_W, LOW);
  digitalWrite(LED_2_W, LOW);
  digitalWrite(LED_3_W, LOW);
  digitalWrite(LED_4_W, LOW);
}