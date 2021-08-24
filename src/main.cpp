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
int ledPin = LED_BUILTIN;
#elif defined(PLATFORM_MICRO)
#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above
const int buzzer = 11;
const int led = 4;
const bool is_promicro = 0;
int ledPin = LED_BUILTIN;
#endif

bool disconnected = 0;

#include <SPI.h>
#include <MFRC522.h>

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

#include <write.h>

void setup() {

    if (is_promicro) {
        pinMode(buzzer, OUTPUT);
        pinMode(led, INPUT_PULLUP);
    }

    Serial.begin(115200);
    SPI.begin();
    mfrc522.PCD_Init();

    // buzzer
    tone(buzzer, 5000); // Send 1KHz sound signal...
    digitalWrite(led, HIGH);
    delay(200);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    digitalWrite(led, LOW);

    delay(100);        // ...for 1 sec

    tone(buzzer, 5000); // Send 1KHz sound signal...
    digitalWrite(led, HIGH);
    delay(200);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    digitalWrite(led, LOW);

    while(!Serial);
    sendWelcomeMessage();
}

void loop() {
    if (Serial) {
        // send welcome message after serial is connected again
        if (disconnected) {
            sendWelcomeMessage();
            disconnected = false;
        }

        execWriter();
    } else {
        disconnected = true;
    }
}
