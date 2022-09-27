#include <Arduino.h>

#define PIN_BTN 26
#define PIN_LED 32

void IRAM_ATTR toggleLED() {
  int state = digitalRead(PIN_BTN);
  digitalWrite(PIN_LED, state);
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_BTN, INPUT);
  pinMode(PIN_LED, OUTPUT);

  attachInterrupt(PIN_BTN, toggleLED, CHANGE);
}

void loop() {
  // int state = digitalRead(PIN_BTN);
  // digitalWrite(PIN_LED, state);
}