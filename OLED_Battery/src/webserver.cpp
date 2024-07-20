#include <string>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <LiquidCrystal_I2C.h>
#include <distanceSensor.h>
#include <string>


const char* ssid = "CaterpillarNet";
const char* password = "3XJ&a(Qk4_@P_5Wr";

AsyncWebServer server(80);

// void setup() {
//   Serial.begin(115200);

//   init_display();
  
//   // Starte SPIFFS
//   if (!SPIFFS.begin()) {
//     Serial.println("An Error has occurred while mounting SPIFFS");
//     return;
//   }
//   pinMode(trigger, OUTPUT);
//   pinMode(echo, INPUT);
//   // Connect to Wi-Fi
// //  WiFi.begin(ssid, password);
// //  while (WiFi.status() != WL_CONNECTED) {
// //    delay(1000);
// //    Serial.println("Connecting to WiFi..");
// //  }

// }