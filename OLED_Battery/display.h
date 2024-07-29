

#include <Adafruit_SSD1306.h>

// void initDisplay(int width, int height);
// void printTextOnDisplay(const char* text, int x, int y, int textSize, bool clearDisplay);

// void Clear();
// void Flush();

#define LINE_1 0
#define LINE_2 16

class Oled_Display_128_32{
  public:
    Oled_Display_128_32();
    void Clear();
    void Flush();
    void PrintTextOnDisplay(const char* text, int lineNumber);

  private:
    Adafruit_SSD1306* setupDisplay(int screenWidth, int screenHeight, int oledReset);
};