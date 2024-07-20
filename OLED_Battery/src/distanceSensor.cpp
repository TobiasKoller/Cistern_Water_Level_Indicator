#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <LiquidCrystal_I2C.h>
#include <distanceSensor.h>
#include <string>

// LCD-Display Konfiguration
// Standard LCD-Adresse ist 0x27 - 16 zeichen - 2 Zeilen 
LiquidCrystal_I2C lcd(0x27,16,2);


// const char* ssid = "CaterpillarNet";
// const char* password = "3XJ&a(Qk4_@P_5Wr";

// AsyncWebServer server(80);

char* Image = (char*) "";

// int trigger = 12;
// int echo = 13;
int liters = 0;
const int PIN_D0 = 16;
const int PIN_D1 = 5;
const int PIN_D2 = 4;
const int PIN_D3 = 0;
const int PIN_D4 = 2;
const int PIN_D5 = 14;
const int PIN_D6 = 12;
const int PIN_D7 = 13;
const int PIN_D8 = 15;

const int TRIGGER = PIN_D3;
const int ECHO = PIN_D4;

//Berechnung der Liter für die Webseite
int calculate_liters() {

  long distance=get_distance_in_cm();
  if(distance!=0)
  {
    liters = 199 - distance;
    liters = liters * 84.7457;
  }

  //test
//   liters = distance;
//   Serial.println(liters);
  return liters;
}

long get_distance_in_cm()
{
  Serial.println("get_distance_in_cm");
  long distanceInCm = 0;
  long dauer = 0;
  
  digitalWrite(TRIGGER, LOW);
  delay(5);
  digitalWrite(TRIGGER, HIGH);
  delay(10);
  digitalWrite(TRIGGER, LOW);
  dauer = pulseIn(ECHO, HIGH);
  // Prüfen ob Messwert zwischen 200cm und 0cm liegt
  distanceInCm = (dauer / 2) / 29.1;
Serial.println("1 distanceInCm=");
  Serial.println(std::to_string(distanceInCm).c_str());

  if (distanceInCm >= 200 || distanceInCm <= 0)
  {
    distanceInCm = 0;
  }
  
  return distanceInCm;
}

// void init_display()
// {
//     // LCD-Display initialisieren
//   lcd.init();

//   // Schlatet die Hintergrundbeleuchtung des LCD-Displays an
//   lcd.backlight();

//   // würde die Hintergrundbeleuchtung ausschalten
//   // lcd.noBacklight();

//   // Startpunkt der Ausgabe setzen. Starte bei Zeichen 0, Zeile 0
//   lcd.setCursor(0,0);
//   lcd.print("Cosmo");

//   // Startpunkt der Ausgabe setzen. Starte bei Zeichen 0, Zeile 1
//   lcd.setCursor(0,1);
//   lcd.print("Koller");  
// }

// void setLcdText(String firstLine, String secondLine)
// {
//   lcd.clear();
//   lcd.setCursor(0,0);
//   lcd.print(firstLine);

//   lcd.setCursor(0,1);
//   lcd.print(secondLine);
// }

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



// void loop() {

//   String liter = calculate_liters();
//   String text = "Liters: "+liter;

//   setLcdText(text,"Cosi");
//   delay(5000);
  
// }
