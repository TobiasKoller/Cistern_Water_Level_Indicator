#include <Arduino.h>
#include "BLEasyServer.h"

// Definiere den Servernamen und die Service-UUID
#define SERVER_NAME "WaterLevelIndicator_ESP32"
#define SERVICE_UUID "91bad492-b950-4226-aa2b-4ede9fa42f59"
#define CHARACTERISTIC_UUID "cba1d466-344c-4be3-ab3f-189f80dd7518"


BLEasyServer* bleServer;
unsigned long previousMillis = 0;
const long interval = 3000; // 3 Sekunden

void setup() {
  Serial.begin(115200);
  
  bleServer = new BLEasyServer(SERVER_NAME, SERVICE_UUID);     
  // Registriere die WaterLevel Characteristic
  bleServer->registerCharacteristic(CHARACTERISTIC_UUID, "Water Level in %");

  // Starte den BLE Server
  bleServer->start();
}

void loop() {
    
  // Beispiel: Aktualisiere den Wasserstandswert
  static uint8_t waterLevel = 0;
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    Serial.println("trigger...");
    Serial.println(String(currentMillis - previousMillis));
      

    previousMillis = currentMillis;
    waterLevel++;
    bleServer->updateCharacteristic(CHARACTERISTIC_UUID, std::to_string(waterLevel));
    Serial.print("WaterLevel: ");
    Serial.print(waterLevel);
    Serial.println(" %");

    bleServer->notify();
  }
}






// // ESP32 BLE Server
  
// /*********
//   Rui Santos
//   Complete instructions at https://RandomNerdTutorials.com/esp32-ble-server-client/
//   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
//   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
// *********/

// #include <BLEDevice.h>
// #include <BLEServer.h>
// #include <BLEUtils.h>
// #include <BLE2902.h>
// #include <Wire.h>

// //BLE server name
// #define bleServerName "WaterLevelIndicator_ESP32"

// uint8_t waterLevel = 0;

// // Timer variables
// unsigned long lastTime = 0;
// unsigned long timerDelay = 3000;

// bool deviceConnected;

// // See the following for generating UUIDs:
// // https://www.uuidgenerator.net/
// #define SERVICE_UUID_WATERLEVEL "91bad492-b950-4226-aa2b-4ede9fa42f59"

// // WaterLevel Characteristic and Descriptor
// BLECharacteristic waterLevelCharacteristics("cba1d466-344c-4be3-ab3f-189f80dd7518", BLECharacteristic::PROPERTY_NOTIFY);
// BLEDescriptor waterLevelDescriptor(BLEUUID((uint16_t)0x2902));


// //Setup callbacks onConnect and onDisconnect
// class MyServerCallbacks: public BLEServerCallbacks {
//   void onConnect(BLEServer* pServer) {
//     Serial.println("connection established.");
//     deviceConnected = true;
//   };
//   void onDisconnect(BLEServer* pServer) {
    
//     Serial.println("connection closed.");
//     deviceConnected = false;
//   }
// };

// void setup() {
//   // Start serial communication 
//   Serial.begin(115200);

//   deviceConnected = false;
//   // Create the BLE Device
//   BLEDevice::init(bleServerName);

//   // Create the BLE Server
//   BLEServer *pServer = BLEDevice::createServer();
//   pServer->setCallbacks(new MyServerCallbacks());

//   // Create the BLE Service
//   BLEService *waterLevelService = pServer->createService(SERVICE_UUID_WATERLEVEL);

//   // Create BLE Characteristics and Create a BLE Descriptor
//   // WaterLevel
//   waterLevelService->addCharacteristic(&waterLevelCharacteristics);
//   waterLevelDescriptor.setValue("Water Level in %");
//   waterLevelCharacteristics.addDescriptor(&waterLevelDescriptor);

//   // Start the service
//   waterLevelService->start();

//   // Start advertising
//   BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
//   pAdvertising->addServiceUUID(SERVICE_UUID_WATERLEVEL);
//   pServer->getAdvertising()->start();
//   Serial.println("Waiting a client connection to notify...");
// }

// void loop() {
  
//   if (deviceConnected) {
//     if ((millis() - lastTime) > timerDelay) {
//       //random(150, 300) / 10.0;
//       if(waterLevel > 100)
//         waterLevel = 0;
      
//       // waterLevel++;
//       //Notify waterLevel reading from Sensor
//       // static char waterLevelTemp[6];

//       // dtostrf(waterLevel, 6, 2, waterLevelTemp);
//       //Set waterLevel Characteristic value and notify connected client
//       //dtostrf(temp, 6, 2, temperatureCTemp);
//       waterLevelCharacteristics.setValue(String(waterLevel).c_str());
//       waterLevelCharacteristics.notify();
//       Serial.print("WaterLevel: ");
//       Serial.print(String(waterLevel));
//       Serial.print(" %");
//       lastTime = millis();
      
//     }
//   }
// }