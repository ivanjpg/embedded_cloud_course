#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

#include <SPIFFS.h>
#include <SPI.h>
#include <Wire.h>

#define WIFI_MAX_CONNECTING_TIME        30000

#define PAYLOAD_MAX_BUFFER_LENGTH       256

#define sprint(a)   Serial.availableForWrite() && Serial.print(a)
#define sprintln(a) Serial.availableForWrite() && Serial.println(a)
#define fprint(...) Serial.availableForWrite() && Serial.printf(__VA_ARGS__)

typedef struct SAWS_MQTT_SETTINGS {
  const char* server;
  int port;
  const char* suscribe_topic;
  const char* publish_topic;
  const char* ca_cert_path;
  const char* pem_cert_path;
  const char* private_key_path;
} AWS_MQTT_SETTINGS;

const char *cstr(const char *s);
char *readFile(const char* path);
int connect2WiFi(const char *ssid, const char *password);
byte scanForI2C();
//PubSubClient getAWSClient(AWS_MQTT_SETTINGS s, WiFiClientSecure sclient, void (*callback)(char *, uint8_t *, unsigned int));
//int connect2AWS(PubSubClient client, const char *thing, const char *topic);