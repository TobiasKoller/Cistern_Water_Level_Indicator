#include <Arduino.h>
#include <display.h>
#include <chrono>
#include <ctime>
#include <distanceSensor.h>
#include <string>

// put function declarations here:
int myFunction(int, int);

//https://znil.net/index.php/ESP8266_Wemos_D1_Mini_mit_0,96_Zoll_OLED_128x64_I2C_volle_Aufl%C3%B6sung_nutzen_Beispiel#

Oled_Display_128_32 oled;

void setup() {
  Serial.begin(9600);
  Serial.println("Start");

  oled.Clear();
  oled.PrintTextOnDisplay("Initializing..",LINE_1);
  oled.Flush();
  
}

void loop() {

  int liters = calculate_liters();
  std::string output = "Liter: "+liters;

  oled.Clear();
  oled.PrintTextOnDisplay(output.c_str(),LINE_1);
  oled.Flush();
  // setLcdText(text,"Cosi");
  delay(5000);

  // auto now = std::chrono::system_clock::now();
  // auto nowTime = std::chrono::system_clock::to_time_t(now);

  // oled.Clear();
  // oled.PrintTextOnDisplay(std::ctime(&nowTime),LINE_1);
  // oled.Flush();

  // delay(1000);
  // Serial.println("time displayed");
}
