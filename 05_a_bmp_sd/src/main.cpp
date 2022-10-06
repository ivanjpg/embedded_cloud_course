#include <Arduino.h>

#include <SPI.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>

#include <FS.h>
#include <SD.h>

#define PIN_LED 32

Adafruit_BMP085 bmp;

char* i2str(int i) {
  return (char*) ( (i % 10) + '0' );
}

char* f2str(float f) {
  char *s = NULL;

  // The integer part of the float
  int ipart = (int) f;
  // Decimal part of the float
  float dpart = f - ipart;
}

void write2File(const char *path, const char* s, int reverse=0) {
  File fd = SD.open(path, FILE_APPEND);

  if(!fd) {
    Serial.println("The file cannot be opened!");
    return;
  }

  // APPEND always will write at the end
  // WRITE is a combination of several constants from SDfat
  // Must find a way to include SDfat constants and use only O_WRITE
  if(reverse) {
    Serial.printf("SEEK: %d\n", fd.position());
    fd.seek(0);
    Serial.printf("SEEK: %d\n", fd.position());
  }

  if( !fd.print(s) ) {
    Serial.println("The file cannot be written!");
  }

  fd.close();
}

char* genRandomString(int size=1, int addReturn = 0) {
  char *s = NULL;

  s = (char *) malloc((size + addReturn + 1) * sizeof(char));
  for(int i=0; i<size; i++) {
    int r = random(33,127);
    s[i] = (char) r;

    Serial.printf("Generated char (%d): *%c*\n", r, s[i]);
  }

  if(addReturn) s[size] = '\n';

  s[size + addReturn] = '\0';

  Serial.printf("Generated string: %s\n", s);

  return s;
}

void sdTest() {
  if( !SD.begin() ) {
    Serial.println("The SD module cannot be initilized!");

    while(1);
  }

  uint8_t cardType = SD.cardType();

  Serial.print("Card type: ");
  switch(cardType) {
    case CARD_NONE:
      Serial.println("There is no SD card!");
      break;
    case CARD_MMC:
      Serial.println("MMC");
      break;
    case CARD_SD:
      Serial.println("SD");
      break;
    case CARD_SDHC:
      Serial.println("SDHC");
      break;
    case CARD_UNKNOWN:
      Serial.println("Unknown type");
      break;
    default:
      Serial.println("WTF?");
  }

  float cardSize = SD.cardSize() / 1024.0 / 1024.0 / 1024.0;
  Serial.printf("Card size: %fGb\n", cardSize);

  Serial.print("Adding to README.md... ");
  write2File("/README.md", genRandomString(random(1,20), 1), 1);
  Serial.println("done!");
}

void scanForI2C() {
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
  }

  delay(5000);
}

void setup() {
  pinMode(PIN_LED, OUTPUT);

  Serial.begin(115200);

  scanForI2C();
  sdTest();
  
  // If the address of the devices is not 0x77 we need
  // to specify in begin(addr)
  if( !bmp.begin() ) {
    Serial.println("Cannot find the BMP sensor!");

    // If the sensor cannot be initialized we never leave setup()
    while(1);
  }
}

void loop() {
  // Serial.println("===================================\n");
  // Serial.printf("Temperature: %f °C\n", bmp.readTemperature());
  // Serial.printf("Pressure: %d Pa\n", bmp.readPressure());
  // Serial.printf("Altitude: %f m\n", bmp.readAltitude());
  // Serial.println("===================================\n");

  // This should be handled in a more elegant way using
  // dynamic allocation memory.
  // TODO: A function to convert float to char*
  char data[120];
  sprintf(data, "%.2f,%d\n", bmp.readTemperature(), bmp.readPressure());

  // stty 115200 -F /dev/ttyUSB0 raw
  Serial.print(data);

  digitalWrite(PIN_LED, HIGH);
  write2File(
    "/temp_pres.csv",
    data
  );
  digitalWrite(PIN_LED, LOW);

  delay(1000);
}