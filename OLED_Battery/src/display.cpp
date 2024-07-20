#include <display.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

    #define OLED_RESET     0 // Reset pin # (or -1 if sharing Arduino reset pin)
    Adafruit_SSD1306* display; // Globale Variable für das Display

    Oled_Display_128_32::Oled_Display_128_32(){
      Serial.println("ctr...");
      display = setupDisplay(128,32,OLED_RESET);
    }

    void Oled_Display_128_32::Clear(){
      if(!display) return;
      display->clearDisplay();
    }

    void Oled_Display_128_32::Flush(){
      
      Serial.println("flushing...");
      if(!display) return;
      display->display();
      Serial.println("flushed");
    }

    void Oled_Display_128_32::PrintTextOnDisplay(const char* text, int lineNumber) {

      int textSize = 2;
      if (!display) return;

      display->setTextSize(textSize);
      
      display->setCursor(0, lineNumber); // Position für den Text festlegen
      display->println(text); // Text auf dem Display ausgeben

    }


    Adafruit_SSD1306* Oled_Display_128_32::setupDisplay(int screenWidth, int screenHeight, int oledReset) {
      // Hier die Display-Einstellungen entsprechend anpassen (je nach Verbindung)
      Adafruit_SSD1306* display = new Adafruit_SSD1306(screenWidth, screenHeight, &Wire, oledReset);

      // Display initialisieren
      if (!display->begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("Display not initialized. Err.");
        delete display; // Speicher freigeben, falls die Initialisierung fehlschlägt
        return nullptr; // Rückgabe von nullptr, um auf fehlgeschlagene Initialisierung hinzuweisen
      }

      Serial.println("setup Text");
      // Einstellungen für das Display (optional)
      display->setTextSize(1);
      display->setTextColor(SSD1306_WHITE);

      return display; // Rückgabe des Zeigers auf das initialisierte Display-Objekt
    }
