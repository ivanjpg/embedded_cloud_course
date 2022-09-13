#include <Arduino.h>

// PINs
#define PIN_LM35 34
#define PIN_LDR 27
#define PIN_BTN 26
#define PIN_LED 32

// System's voltage reference
// LM35 is connected to 3V3 pin
#define ADC_SYS_VOL 3300.0

// Maximum resolution of ADC, 12-bit
#define ADC_RES 4096.0

// Factor to obtain milivolts from analog readings
#define MV_FACTOR ADC_SYS_VOL / ADC_RES / 10.0

// Light threshold to turn the led on
#define LED_THS 300

void setup() {
  Serial.begin(115200);

  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  if( analogRead(PIN_LDR) > LED_THS) {
    digitalWrite(PIN_LED, LOW);
  } else {
    digitalWrite(PIN_LED, HIGH);
  }
}