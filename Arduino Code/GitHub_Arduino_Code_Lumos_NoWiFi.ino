#include <ArduinoJson.h>
#include <Adafruit_AS7341.h>

//citations: https://github.com/adafruit/Adafruit_AS7341

Adafruit_AS7341 as7341;
#define FREQUENCY_HZ 104
#define INTERVAL_MS (50000 / (FREQUENCY_HZ + 1))//control the sampling rate, 20000 ~= 200 ms
static char payload[256];
StaticJsonDocument<256> doc;
#define TOKEN "" //Spectrometer Token
#define DEVICEID "" //Spectrometer Device ID

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  while (!Serial);
  if (!as7341.begin()){
    Serial.println("Could not find AS7341");
    while (1) { delay(10); }
  }
  as7341.setATIME(100);
  as7341.setASTEP(999);
  as7341.setGain(AS7341_GAIN_256X);
}

void loop() {
  uint16_t readings[12];
  uint16_t counts[12];
  static unsigned long last_interval_ms = 0;
  if (millis() > last_interval_ms + INTERVAL_MS)
  {
    last_interval_ms = millis();
    for(uint8_t i = 0; i < 12; i++) {
      if(i == 4 || i == 5) continue;
      // we skip the first set of duplicate clear/NIR readings
      // (indices 4 and 5)
      counts[i] = readings[i];
    }
    doc["F1_415nm"] = counts[0];
    doc["F2_445nm"] = counts[1];
    doc["F3_480nm"] = counts[2];
    doc["F4_515nm"] = counts[3];
    doc["F5_555nm"] = counts[6];
    doc["F6_590nm"] = counts[7];
    doc["F7_630nm"] = counts[8];
    doc["F8_680nm"] = counts[9];
    doc["Clear"]    = counts[10];
    doc["NIR"]      = counts[11];
    Serial.println(payload);
  }
}
