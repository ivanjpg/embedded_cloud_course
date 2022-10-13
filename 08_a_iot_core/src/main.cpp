#include <Arduino.h>

#include <ArduinoJson.h>
#include <Adafruit_BMP085.h>

#include "secrets.hpp"
#include "helpers.hpp"

#define AWS_MQTT_SERVER           "a9zwczf1oqpq2-ats.iot.us-east-1.amazonaws.com"
#define AWS_MQTT_PORT             8883
#define AWS_MQTT_SUSCRIBE_TOPIC   "embebed_cloud/Axolote/test_topic_pub"
#define AWS_MQTT_PUBLISH_TOPIC    AWS_MQTT_SUSCRIBE_TOPIC
#define AWS_MQTT_THING            "Axolote_Ivan_ESP32"

#define AWS_CA_CERT_FILE          "/aws_certs/root-CA.pem"
#define AWS_PEM_CERT_FILE         "/aws_certs/ESP32.cert.pem"
#define AWS_PRIVATE_KEY_FILE      "/aws_certs/ESP32.private.key"

#define AWS_RECONNECT_DELAY_TIME  5000

WiFiClientSecure sclient;
PubSubClient awsClient = PubSubClient(sclient);

Adafruit_BMP085 bmp;

char payload[PAYLOAD_MAX_BUFFER_LENGTH];

const int jcap = JSON_OBJECT_SIZE(8);
DynamicJsonDocument jdoc(jcap);

byte bmp_addr;
byte mac[6];
char mac_id[18];

long lastMsg = 0;

void getAWSClient(AWS_MQTT_SETTINGS s, void (*callback)(char *, uint8_t *, unsigned int)) {
  sprint("Setting up AWS client... ");

  const char *caCert = readFile(s.ca_cert_path);
  const char *cert = readFile(s.pem_cert_path);
  const char *key = readFile(s.private_key_path);

  sclient.setCACert(caCert);
  sclient.setCertificate(cert);
  sclient.setPrivateKey(key);

  awsClient.setServer(s.server, s.port);
  awsClient.setCallback(callback);

  sprintln("done!");
}

void awsCallback(char *topic, uint8_t *payload_in, unsigned int length) {
  deserializeJson(jdoc, payload_in);
  const char* msg = jdoc["message"];

  // JsonObject documentRoot = jdoc.as<JsonObject>();
  // for(JsonPair e : documentRoot) {
  //   sprintln(e.key().c_str());
  // }

  serializeJson(jdoc, payload);
  fprint("MSG Received!\n\t[%s]: ", topic);
  sprintln(payload);

  jdoc.clear();
}

int connect2AWS(const char *thing, const char *topic) {
  if( awsClient.connected() ) return 1;

  fprint("Connecting thing ╣%s╠ to AWS IoT...\n", thing);
    
  if( awsClient.connect(thing) ) {
    fprint("\tConnected ╣%s╠!\n", thing);

    awsClient.subscribe(topic);

    return 1;
  }

  fprint("\n\tCannot connect ╣%s╠, failed with: ", thing);
  sprintln( awsClient.state() );

  return 0;
}

void setup() {
  Serial.begin(115200);

  bmp_addr = scanForI2C();

  if( !bmp.begin(bmp_addr) ) {
    Serial.println("Cannot find the BMP sensor!");

    // If the sensor cannot be initialized we never leave setup()
    while(1);
  }

  connect2WiFi(WIFI_SSID, WIFI_PASSWORD);
  
  WiFi.macAddress(mac);
  snprintf(
    mac_id,
    sizeof(mac_id),
    "%02x:%02x:%02x:%02x:%02x:%02x",
    mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]
  );
  
  AWS_MQTT_SETTINGS s;
  s.ca_cert_path = AWS_CA_CERT_FILE;
  s.pem_cert_path = AWS_PEM_CERT_FILE;
  s.private_key_path = AWS_PRIVATE_KEY_FILE;
  s.server = AWS_MQTT_SERVER;
  s.port = AWS_MQTT_PORT;

  getAWSClient(s, awsCallback);
}

void publish2AWS() {
  long now = millis();

  if(now - lastMsg > 5000) {
    lastMsg = now;

    jdoc["mac_Id"] = mac_id;
    jdoc["device_Id"] = AWS_MQTT_THING;
    jdoc["temp_C"] = String( bmp.readTemperature(), 2 );
    jdoc["press_hPa"] = bmp.readPressure()/100;

    serializeJson(jdoc, payload);

    fprint("Publishing MSG: %s\n", payload);

    awsClient.publish(AWS_MQTT_PUBLISH_TOPIC, payload);
    jdoc.clear();
  }
}

void loop() {
  //Serial.println("Y en un loop, nos miramos...");
  if( !awsClient.connected() ) {
    if( !connect2AWS(AWS_MQTT_THING, AWS_MQTT_SUSCRIBE_TOPIC) ) {
      fprint("Waiting %d to try reconnection...\n", AWS_RECONNECT_DELAY_TIME);
      delay(AWS_RECONNECT_DELAY_TIME);
    }
  } else {
    awsClient.loop();
    publish2AWS();
  }
}