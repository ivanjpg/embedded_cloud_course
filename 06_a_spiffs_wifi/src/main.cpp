#include <Arduino.h>

#include <SPIFFS.h>
#include <WiFi.h>
#include <Preferences.h>

#define PREFS_NAMESPACE "mynamespace"

Preferences prefs;

typedef struct credentials {
  char *ssid;
  char *passwd;
} Credentials;

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

Credentials getCredentialsFromPrefs() {
  // Initilizing the preferences
  // (namespace, readonly)
  Serial.print("Reading credentials from prefs... ");
  prefs.begin(PREFS_NAMESPACE, false);

  size_t size = prefs.getBytesLength("ssid");
  char *ssid = (char *) malloc( sizeof(char) * size );
  prefs.getBytes("ssid", ssid, size);

  size = prefs.getBytesLength("passwd");
  char *passwd = (char *) malloc( sizeof(char) * size );
  prefs.getBytes("passwd", passwd, size);

  prefs.end();
  Serial.println("done!");

  Credentials ret = {ssid, passwd};
  Serial.printf(
    "\tCredentials from EEPROM:\n\t\tSSID: *%s*\n\t\tPasswd: *%s*\n\n",
    ssid,
    passwd
  );

  return ret;
}

void saveCredentialsToPrefs(Credentials creds) {
  Serial.print("Saving credentials to Prefs... ");
  prefs.begin(PREFS_NAMESPACE, false);

  size_t size = strlen(creds.ssid) + 1;
  char *tmp = (char *) malloc(size);
  sprintf(tmp, "%s\0", creds.ssid);
  prefs.putBytes("ssid", tmp, size);

  size = strlen(creds.passwd) + 1;
  prefs.putBytes("passwd", creds.passwd, size);

  prefs.end();
  Serial.println("done!");
}

void connect2wifi(int from_flash=0) {
  Serial.println("Connecting to WiFi...");

  // This files need to be created first from PlatformIO
  /*
    - Create a directory called «data»
    - Put the files inside
    - Go to «Project Taks» -> «Platform» -> «Build Filesystem Image»
    - ... -> «Upload Filesystem Image»
          (this could be slow, check the loading spinner...)
  */

  Credentials creds;

  creds.ssid = readFile("/ssid.txt");
  creds.passwd = readFile("/passwd.txt");

  // Another way is to store the credentials in the EEPROM
  // The EEPROM library has been deprecated and is now replaced
  // by Preferences library.
  //creds = getCredentialsFromPrefs();

  Serial.printf("\tConnecting to SSID *%s*...\n", creds.ssid);
  Serial.printf("\t\twith password *%s*...\n", creds.passwd);

  WiFi.mode(WIFI_STA);
  WiFi.begin(creds.ssid, creds.passwd);

  while( WiFi.status() != WL_CONNECTED ) {
    Serial.print(".");
    delay(500);

    if( WiFi.status() == WL_CONNECT_FAILED ) {
      Serial.println("Cannot connect with the data provided!");

      return;
    }
  }

  Serial.println("\nConnected to WiFi!");
  Serial.printf("IP: %s\n", WiFi.localIP().toString());

  // If we can connected, save those in the EEPROM
  saveCredentialsToPrefs(creds);
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

  Serial.println("Reading /Ivan.txt file from SPIFFS...");
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