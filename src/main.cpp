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
#elif defined(PLATFORM_PROMICRO)
#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above
const int buzzer = 7;
const int led = 5;
const bool is_promicro = 1;
#endif

#include <SPI.h>
#include <MFRC522.h>

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

#include <cli.h>
#include <read.h>
#include <write.h>

void setup() {

    if (is_promicro) {
        pinMode(buzzer, OUTPUT);
        pinMode(led, INPUT_PULLUP);
    }

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