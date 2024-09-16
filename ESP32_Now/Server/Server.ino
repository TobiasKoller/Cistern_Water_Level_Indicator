#include "display.h"
#include <esp_now.h>
#include <WiFi.h>

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int waterLevelInPercentage;
} struct_message;

// Create a struct_message called myData
struct_message _notificationData;
Oled_Display_128_32* oled = nullptr;

#define SDA_pin 3
#define SCL_pin 2

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&_notificationData, incomingData, sizeof(_notificationData));
  // RemoteXY.waterlevel_control = static_cast<float>(_notificationData.waterLevelInPercentage);
  int level = static_cast<int>(_notificationData.waterLevelInPercentage);

  std::string result = std::to_string(level) + "%";
  const char* message = result.c_str();

  oled->PrintTextOnDisplay("Fuellstand",LINE_1, true, false);
  oled->PrintTextOnDisplay(message, LINE_2, false, true);
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  oled = new Oled_Display_128_32(SDA_pin, SCL_pin);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // oled->Clear();
  oled->PrintTextOnDisplay("Initializing..",LINE_1,true,true);
  // oled->Flush();

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}
 
void loop() {
  
}