#include <Arduino.h>

#define PIN_LED 32

uint8_t led_state = 0;

void setup() {
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  digitalWrite(PIN_LED, led_state);
  delay(500);
  led_state = !led_state;
}