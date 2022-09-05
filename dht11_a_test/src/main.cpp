#include <Arduino.h>

#include "DHT.h"

#define DHT_PIN 25
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(9600);

  Serial.print("Initializing DHT sensor... ");
  dht.begin();
  Serial.println("done!");
}

void loop() {
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if( isnan(h) || isnan(t) ) {
    Serial.println("Cannot read the sensor data!");

    return;
  }

  Serial.print("T=");
  Serial.print(t);
  Serial.println("°C");

  Serial.println();

  Serial.print("H=");
  Serial.print(h);
  Serial.println("%");

  Serial.println("======================");
}