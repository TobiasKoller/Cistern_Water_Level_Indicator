

#include <Adafruit_SSD1306.h>

// void initDisplay(int width, int height);
// void printTextOnDisplay(const char* text, int x, int y, int textSize, bool clearDisplay);

// void Clear();
// void Flush();

#define LINE_1 0
#define LINE_2 16

class Oled_Display_128_32{
  public:
    Oled_Display_128_32(int sdaPin, int sclPin);
    void Clear();
    void Flush();
    void PrintTextOnDisplay(const char* text, int lineNumber, bool clear, bool flush);

  private:
    void setupDisplay(int screenWidth, int screenHeight, int oledReset,int sdaPin, int sclPin);
};