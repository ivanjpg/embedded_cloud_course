#include "helpers.hpp"

const char *cstr(const char *s) {
  int len = strlen(s);
  char *sstr = (char *) malloc( 1 + sizeof(char) * len );

  sprintf(sstr, "%s\0", s);

  return sstr;
}

char *readFile(const char* path) {
  char *s = NULL;

  SPIFFS.begin();
  File fd = SPIFFS.open(path);
  if(!fd) {
    SPIFFS.end();

    return NULL;
  }

  size_t fsize = fd.available();
  s = (char *) malloc(fsize + 1);
  
  if(s == NULL) {
    Serial.println("\n\nDynamic memory cannot be allocated!");
    SPIFFS.end();

    return NULL;
  }

  fd.readBytes(s, fsize);
  fd.close();
  SPIFFS.end();

  s[fsize] = '\0';

  return s;
}

int connect2WiFi(const char *ssid, const char *password) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  fprint("Connecting to ╣%s╠\n", ssid);
  fprint("with password ╣%s╠\n", password);

  long int t1 = millis();
  while( WiFi.status() != WL_CONNECTED ) {
    long int t2 = millis();
    if(t2-t1 >= WIFI_MAX_CONNECTING_TIME) break;

    sprint(".");
    delay(500);

    if( WiFi.status() == WL_CONNECT_FAILED ) break;
  }

  if( WiFi.status() != WL_CONNECTED ) {
    sprintln("\nThe WiFi connection attemp failed!");

    return 1;
  }

  fprint("\nConnected to ╣%s╠ with IP ╣%s╠\n", ssid, WiFi.localIP().toString());

  return 0;
}

byte scanForI2C() {
  Wire.begin();

  byte error, addr;
  int ndev = 0;

  for(addr = 0x01; addr < 0x7f; addr++) {
    Wire.beginTransmission(addr);
    error = Wire.endTransmission();

    // Should check for more error codes!
    if(!error) {
      Serial.printf("I2C: 0x%02X\n", addr);
      ndev++;
    }
  }

  if(!ndev) {
    Serial.printf("There is no devices!\n");

    return 0x0;
  }

  return addr;
}

double dformat(double n, int digits) {
  int nsize = snprintf(NULL, 0, "%.*f", digits, n) + 1;
  char *s = (char *) malloc( sizeof(char) * nsize + 1 );

  snprintf(s, nsize, "%.*f", digits, n);
  double d = atof(s);
  free(s);

  return d;
}