#include <Arduino.h>

#define PIN_LM 34

// Must be the SYSTEM's reference voltaje
// This particular case, the LM35 is connected
// to V5 pin of the ESP32.
#define ADC_SYS_VOL 5000.0

// ADC has a 12-bit maximum resolution.
#define ADC_RES 4096.0

void setup() {
  Serial.begin(115200);
}

void loop() {
  int adc = analogRead(PIN_LM);
  float mV = adc * (ADC_SYS_VOL / ADC_RES);
  float temp = mV / 10.0;

  Serial.print("T=");
  Serial.print(temp);
  Serial.println("°C");

  delay(2000);
}