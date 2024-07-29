#include <Arduino.h>
#include "display.h"
#include <chrono>
#include <ctime>
#include "distance_sensor.h"
#include <string>
#include "webserver.h"

// // put function declarations here:
// int myFunction(int, int);

//https://znil.net/index.php/ESP8266_Wemos_D1_Mini_mit_0,96_Zoll_OLED_128x64_I2C_volle_Aufl%C3%B6sung_nutzen_Beispiel#

Oled_Display_128_32 oled;
Webserver webserver;


void setup() {
  Serial.begin(9600);
  delay(1000);

  Serial.println("Start");

  Serial.println("Init Webserver...");
  webserver.Setup();

  oled.Clear();
  oled.PrintTextOnDisplay("Initializing..",LINE_1);
  oled.Flush();
  
}

void loop() {

  webserver.Watch();

  int liters = calculate_liters();
  std::string caption = "Liter: ";

  std::string value = std::to_string(liters);

  oled.Clear();
  oled.PrintTextOnDisplay(caption.c_str(),LINE_1);
  oled.PrintTextOnDisplay(value.c_str(),LINE_2);
  oled.Flush();
  // setLcdText(text,"Cosi");
  // delay(5000);
  delay(1000);
  // auto now = std::chrono::system_clock::now();
  // auto nowTime = std::chrono::system_clock::to_time_t(now);

  // oled.Clear();
  // oled.PrintTextOnDisplay(std::ctime(&nowTime),LINE_1);
  // oled.Flush();

  // delay(1000);
  // Serial.println("time displayed");
}
