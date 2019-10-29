#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         D3          // Configurable, see typical pin layout above
#define SS_PIN          D8          // Configurable, see typical pin layout above

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