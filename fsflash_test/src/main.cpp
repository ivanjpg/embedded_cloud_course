#include <Arduino.h>

#include <SPIFFS.h>
#include <WiFi.h>

char* readFile(const char* path) {
  char *s = NULL;

  File fd = SPIFFS.open(path);
  if(!fd) return NULL;

  size_t fsize = fd.available();
  s = (char *) malloc(fsize + 1);
  
  if(s == NULL) {
    Serial.println("\n\nDynamic memory cannot be allocated!");

    return NULL;
  }

  fd.readBytes(s, fsize);
  fd.close();

  s[fsize] = '\0';

  return s;
}

void connect2wifi() {
  Serial.println("Connecting to WiFi...");

  char *ssid = readFile("/ssid.txt");
  char *pass = readFile("/passwd.txt");

  Serial.printf("\tConnecting to SSID *%s*...\n", ssid);
  // Serial.printf("\t\twith password *%s*...\n", pass);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while( WiFi.status() != WL_CONNECTED ) {
    Serial.print(".");
    //Serial.println(WiFi.status());
    delay(300);
  }

  Serial.println("\nConnected to WiFi!");
  Serial.printf("IP: %s\n", WiFi.localIP().toString());

}

void setup() {
  Serial.begin(115200);

  if( !SPIFFS.begin(true) ) {
    Serial.println("SPIFFS could not be initialized!");
    return;
  }

  File fd = SPIFFS.open("/Ivan.txt");
  if(!fd) {
    Serial.println("The file cannot be opened!");
    return;
  }

  Serial.println("Reading /Ivan.txt file...");
  Serial.println("=============================");
  while( fd.available() ) {
    Serial.write( fd.read() );
  }
  Serial.println("\n=============================\nFile reading ends!");

  fd.close();

  fd = SPIFFS.open("/creado.txt", "w+");
  fd.print("Archivo creado desde el código\n");
  fd.close();

  Serial.println("Listing files in the flash memory...");
  File root = SPIFFS.open("/");

  while( fd = root.openNextFile() ) {
    Serial.printf("\tFILE: %s\tSIZE: %d\n", fd.name(), fd.size());
  }

  root.close();

  connect2wifi();
}

void loop() {
  // put your main code here, to run repeatedly:
}