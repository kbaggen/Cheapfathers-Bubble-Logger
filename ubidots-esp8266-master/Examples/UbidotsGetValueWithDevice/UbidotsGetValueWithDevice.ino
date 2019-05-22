#include "UbidotsMicroESP8266.h"

#define DEVICE  "..."  // Put here your Ubidots device label
#define VARIABLE  "..."  // Put here your Ubidots variable label
#define TOKEN  "..."  // Put here your Ubidots TOKEN
#define WIFISSID "..." // Put here your Wi-Fi SSID
#define PASSWORD "..." // Put here your Wi-Fi password

Ubidots client(TOKEN);

void setup() {
    Serial.begin(115200);
    client.wifiConnection(WIFISSID, PASSWORD);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, 1);
    //client.setDebug(true); // Uncomment this line to set DEBUG on
}

void loop() {
    float value = client.getValueWithDevice(DEVICE, VARIABLE);
    if (value != ERROR_VALUE){
      Serial.print(F("value obtained: "));
      Serial.println(value);
    }else{
      Serial.println(F("Error getting value"));
    }
    delay(1000);
}
