#include <Arduino.h>

#ifdef PLATFORM_ESP8266
#include <ESP8266WiFi.h>
#define RST_PIN         D3          // Configurable, see typical pin layout above
#define SS_PIN          D8          // Configurable, see typical pin layout above
String platform = "esp8266";
#elif defined(PLATFORM_ESP32)
#include <WiFi.h>
#include <WiFiClient.h>
#define RST_PIN         D3          // Configurable, see typical pin layout above
#define SS_PIN          D8          // Configurable, see typical pin layout above
String platform = "esp32";
#endif

#include <SPI.h>
#include <MFRC522.h>

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

#include <cli.h>
#include <read.h>
#include <write.h>

void setup() {
    Serial.begin(115200);
    SPI.begin();
    mfrc522.PCD_Init();
    cli_init();
}

void loop() {
    if (readerMode == "read") {
        execReader();
    } else if (readerMode == "write") {
        execWriter();
    } else {
        my_cli();
    }
}