//TODO
//autoparing?? https://randomnerdtutorials.com/esp-now-auto-pairing-esp32-esp8266/


/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/
#include <esp_now.h>
#include <WiFi.h>
#include "DistanceSensor.h"

//ESP8266
// #include <espnow.h>
// #include <ESP8266WiFi.h>
// #define ESP_NOW_SEND_SUCCESS 0
// #define ESP_OK 0


// typedef struct {
//     uint8_t peer_addr[6];
//     uint8_t channel;
//     bool encrypt;
// } message_struct;

// esp_now_peer_info_t  esp_now_send_status_t;

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[6] = {0xEC, 0xDA, 0x3B, 0xBE, 0xF2, 0x84};
//ec:da:3b:be:f2:84
// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  int waterLevelInPercentage;
} struct_message;

// Create a struct_message called myData
struct_message _notificationData;

esp_now_peer_info_t peerInfo;
DistanceSensor distanceSensor;


// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// void OnDataSent(uint8_t *mac_addr, uint8_t status) {
//   Serial.print("\r\nLast Packet Send Status:\t");
//   Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
// }
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  // if (esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0) != 0) {
  //   Serial.println("Failed to add peer");
  //   return;
  // }
 
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  // Set values to send
 
  distanceSensor.Setup(150,30);     
  int percentage = distanceSensor.CalculateDistanceInPercentage();

  _notificationData.waterLevelInPercentage = percentage;
  
  // Send message via ESP-NOW
  int result = esp_now_send(broadcastAddress, (uint8_t *) &_notificationData, sizeof(_notificationData));
  
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    // ESP_OK: 0 (Successful execution)
    // ESP_FAIL: -1 (General failure)
    // ESP_ERR_NO_MEM: 0x101 (Not enough memory)
    // ESP_ERR_INVALID_ARG: 0x102 (Invalid argument)
    // ESP_ERR_INVALID_STATE: 0x103 (Invalid state)
    // ESP_ERR_INVALID_SIZE: 0x104 (Invalid size) .

    Serial.println("Error sending the data. Error-Code="+String(result));
  }
  delay(2000);
}