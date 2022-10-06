#include <Arduino.h>

#include <SPIFFS.h>
#include <ArduinoJson.h>

#define JSON_FP "/json_test.txt"

#define PIN_BTN 26
#define PIN_LED 32

const int jcap = JSON_OBJECT_SIZE(1);
DynamicJsonDocument jdoc(jcap);

int btimes = 0;

// This needs to be volatile because can be changed "outside"
// the code that is being executed (interrupts / loop).
volatile int btnPushed = 0;

void IRAM_ATTR cbBtnPressed() {
  btimes++;
  btnPushed = 1;

  int state = digitalRead(PIN_BTN);
  digitalWrite(PIN_LED, state);
}

const char* str(const char *s) {
  int len = strlen(s);
  char *sstr = (char *) malloc( 1 + sizeof(char) * len );

  sprintf(sstr, "%s\0", s);

  return sstr;
}

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

void setup() {
  Serial.begin(115200);

  pinMode(PIN_BTN, INPUT);
  pinMode(PIN_LED, OUTPUT);

  attachInterrupt(PIN_BTN, cbBtnPressed, CHANGE);

  if( !SPIFFS.begin(true) ) {
    Serial.println("SPIFFS could not be initialized!");
    
    while(1);
  }

  // Load the file with JSON
  Serial.print("Reading JSON file... ");
  char *fjson = readFile(JSON_FP);
  Serial.println("done!");

  Serial.printf("\t\tJSON read:\n\t\t%s\n\n", str(fjson));

  Serial.print("Deserializing JSON... ");
  deserializeJson(jdoc, fjson);
  Serial.println("done!");
}

void loop() {
  // The serial printing uses interrupts and causes issues
  // calling it from an interruption callback, so must be done here!
  if(btnPushed) {
    Serial.printf("The button has been pushed %d times!\n", btimes);
    btnPushed = 0;
  }

  // Print the name in JSON every 5 seconds

  Serial.printf("Name in JSON: %s\n", str(jdoc["nombre_alumno"]) );
  delay(5000);
}