#include "display.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

    #define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
    Adafruit_SSD1306* display; // Globale Variable für das Display

    Oled_Display_128_32::Oled_Display_128_32(int sdaPin, int sclPin){
      Serial.println("ctr...");
      setupDisplay(128,32,OLED_RESET, sdaPin, sclPin);
    }

    void Oled_Display_128_32::Clear(){
      Serial.println("Clearing display...");
      if(!display) return;
      display->clearDisplay();
      Serial.println("cleared");
    }

    void Oled_Display_128_32::Flush(){
      
      Serial.println("flushing...");
      if(!display) return;
      display->display();
      Serial.println("flushed");
    }

    void Oled_Display_128_32::PrintTextOnDisplay(const char* text, int lineNumber, bool clear, bool flush) {

      int textSize = 2;
      if (!display) return;

      if(clear){
        Clear();
      }

      display->setTextSize(textSize);
      
      display->setCursor(0, lineNumber); // Position für den Text festlegen
      display->println(text); // Text auf dem Display ausgeben
      Serial.println("text set: "+ String(text));

      if(flush){
        Flush();
      }
    }


    void Oled_Display_128_32::setupDisplay(int screenWidth, int screenHeight, int oledReset, int sdaPin, int sclPin) {
        if (display != nullptr) return; // Verhindert doppelte Initialisierung

        Wire.setPins(sdaPin, sclPin);
        display = new Adafruit_SSD1306(screenWidth, screenHeight, &Wire, oledReset);

        if (!display->begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
            Serial.println("Display not initialized. Err.");
            delete display;
            display = nullptr;
            return;
        }
       
        Serial.println("setup Text");
        display->setTextSize(1);
        display->setTextColor(WHITE);
    }


