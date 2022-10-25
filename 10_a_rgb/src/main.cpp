#include <Arduino.h>

#define PIN_LED_R   25
#define PIN_LED_G   14
#define PIN_LED_B   12

#define PIN_BTN     26

#define DEBOUNCE_TIME   250
volatile unsigned long ldebounce = 0;

volatile int led_color = 0;

void IRAM_ATTR cb_button() {
  if( (millis() - ldebounce) < DEBOUNCE_TIME ) return;

  led_color++;

  if(led_color > 7) led_color = 0;
  ldebounce = millis();
}

void setup() {
  Serial.begin(115200);

  attachInterrupt(PIN_BTN, cb_button, RISING);

  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
}

void loop() {
  switch(led_color) {
    // Red
    case 0:
      digitalWrite(PIN_LED_R, LOW);
      digitalWrite(PIN_LED_G, HIGH);
      digitalWrite(PIN_LED_B, HIGH);
      break;
    // Green
    case 1:
      digitalWrite(PIN_LED_R, HIGH);
      digitalWrite(PIN_LED_G, LOW);
      digitalWrite(PIN_LED_B, HIGH);
      break;
    case 2:
      // Blue
      digitalWrite(PIN_LED_R, HIGH);
      digitalWrite(PIN_LED_G, HIGH);
      digitalWrite(PIN_LED_B, LOW);
      break;
    case 3:
      // Cyan
      digitalWrite(PIN_LED_R, HIGH);
      digitalWrite(PIN_LED_G, LOW);
      digitalWrite(PIN_LED_B, LOW);
      break;
    case 4:
      // Yellow
      digitalWrite(PIN_LED_R, LOW);
      digitalWrite(PIN_LED_G, LOW);
      digitalWrite(PIN_LED_B, HIGH);
      break;
    case 5:
      // Magenta
      digitalWrite(PIN_LED_R, LOW);
      digitalWrite(PIN_LED_G, HIGH);
      digitalWrite(PIN_LED_B, LOW);
      break;
    case 6:
      // White
      digitalWrite(PIN_LED_R, LOW);
      digitalWrite(PIN_LED_G, LOW);
      digitalWrite(PIN_LED_B, LOW);
      break;
    default:
      // Off
      digitalWrite(PIN_LED_R, HIGH);
      digitalWrite(PIN_LED_G, HIGH);
      digitalWrite(PIN_LED_B, HIGH);
      break;
  }
}