#include <Arduino.h>

#define PIN_BTN 26
#define PIN_LED 32

#define PWM_CHANNEL     0
#define PWM_FREQ        10000
#define PWM_RESOLUTION  8

#define DEBOUNCE_TIME   250
volatile unsigned long ldebounce = 0;

volatile int wave_form = 0;

int t = 0;
float duty_cycle = 0.0;

void IRAM_ATTR cb_button() {
  if( (millis() - ldebounce) < DEBOUNCE_TIME ) return;

  wave_form++;

  if(wave_form > 7) wave_form = 0;
  ldebounce = millis();
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_BTN, INPUT_PULLDOWN);
  attachInterrupt(PIN_BTN, cb_button, RISING);

  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(PIN_LED, PWM_CHANNEL);
}

void loop() {
  t++;
  if(t > 255) t = 0;

  switch(wave_form) {
    case 0:
      duty_cycle = 0.0;
      break;

    case 1:
      duty_cycle = 0.3;
      break;

    case 2:
      duty_cycle = 0.5;
      break;

    case 3:
      duty_cycle = 0.8;
      break;

    case 4:
      duty_cycle = 1.0;
      break;

    case 5:
      duty_cycle = (t + 0.1) / 256.0;
      delay(2.5);
      break;

    case 6:
      duty_cycle = 0.5 * sin(2 * PI * t / 256.0) + 0.5;
      delay(0.5);
      break;

    case 7:
      duty_cycle = pow(2.71828, ( (t / 255) - 1) * 2 );
      delay(4);
      break;
  }

  ledcWrite(PWM_CHANNEL, duty_cycle * 255);

  Serial.println(duty_cycle * 100);
}
