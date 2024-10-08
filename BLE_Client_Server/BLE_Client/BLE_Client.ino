#include <BLEasyClient.h>

// Deine BLE UUIDs hier definieren
BLEUUID serviceUUID("91bad492-b950-4226-aa2b-4ede9fa42f59");
BLEUUID characteristicUUID("cba1d466-344c-4be3-ab3f-189f80dd7518");
#define SERVER_NAME "WaterLevelIndicator_ESP32"


// Erstelle ein BLEasyClient-Objekt
BLEasyClient bleClient(SERVER_NAME, serviceUUID, characteristicUUID);

void setup() {
    Serial.begin(115200);

    // Initialisiere den BLEasyClient
    bleClient.begin();

    // Setze die Callback-Funktion
    bleClient.setNotifyCallback(myNotifyCallback);
}

void loop() {
    bleClient.update();
}

// Definiere die Callback-Funktion
void myNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
    Serial.print("Received data: ");
    for (size_t i = 0; i < length; ++i) {
        Serial.print((char)pData[i]);
    }
    Serial.println();
}





// // /*********
// //   Rui Santos
// //   Complete instructions at https://RandomNerdTutorials.com/esp32-ble-server-client/
// //   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
// //   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
// // *********/

// #include "BLEDevice.h"
// #include <Wire.h>

// //BLE Server name (the other ESP32 name running the server sketch)
// #define bleServerName "WaterLevelIndicator_ESP32"

// /* UUID's of the service, characteristic that we want to read*/
// // BLE Service
// static BLEUUID SERVICE_UUID_WATERLEVEL("91bad492-b950-4226-aa2b-4ede9fa42f59");

// // BLE Characteristics
// //WaterLevel Characteristic
// static BLEUUID waterLevelCharacteristicsUUID("cba1d466-344c-4be3-ab3f-189f80dd7518");

// //Flags stating if should begin connecting and if the connection is up
// static boolean doConnect = false;
// static boolean connected = false;

// //Address of the peripheral device. Address will be found during scanning...
// static BLEAddress *pServerAddress;

// //Characteristicd that we want to read
// static BLERemoteCharacteristic* waterLevelCharacteristics;

// //Activate notify
// const uint8_t notificationOn[] = {0x1, 0x0};
// const uint8_t notificationOff[] = {0x0, 0x0};


// //Variables to store waterlevel
// char* waterLevel;

// //Flags to check whether new waterlevel readings are available
// boolean newWaterLevel = false;

// //Connect to the BLE Server that has the name, Service, and Characteristics
// bool connectToServer(BLEAddress pAddress) {
//   Serial.println("connectToServer");
//   BLEClient* pClient = BLEDevice::createClient();

//   // Connect to the remove BLE Server.
//   pClient->connect(pAddress);
//   Serial.println(" - Connected to server");

//   // Obtain a reference to the service we are after in the remote BLE server.
//   BLERemoteService* remoteServiceWaterLevel = pClient->getService(SERVICE_UUID_WATERLEVEL);
//   if (remoteServiceWaterLevel == nullptr) {
//     Serial.print("Failed to find our service UUID: ");
//     Serial.println(SERVICE_UUID_WATERLEVEL.toString().c_str());
//     return (false);
//   }

//   // Obtain a reference to the characteristics in the service of the remote BLE server.
//   waterLevelCharacteristics = remoteServiceWaterLevel->getCharacteristic(waterLevelCharacteristicsUUID);

//   if (waterLevelCharacteristics == nullptr) {
//     Serial.print("Failed to find our characteristic UUID");
//     return false;
//   }
//   Serial.println(" - Found our characteristics");

//   //Assign callback functions for the Characteristics
//   waterLevelCharacteristics->registerForNotify(waterLevelNotifyCallback);
//   return true;
// }

// //Callback function that gets called, when another device's advertisement has been received
// class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
//     void onResult(BLEAdvertisedDevice advertisedDevice) {
//       if (advertisedDevice.getName() == bleServerName) { //Check if the name of the advertiser matches
//         advertisedDevice.getScan()->stop(); //Scan can be stopped, we found what we are looking for
//         pServerAddress = new BLEAddress(advertisedDevice.getAddress()); //Address of advertiser is the one we need
//         doConnect = true; //Set indicator, stating that we are ready to connect
//         Serial.println("Device found. Connecting!");
//       }
//     }
// };

// //When the BLE Server sends a new waterLevel reading with the notify property
// static void waterLevelNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic,
//                                       uint8_t* pData, size_t length, bool isNotify) {
//   //store waterLevel value
//   Serial.println("len="+String(length));
//   char* tmpValue = (char*)pData;
//   tmpValue[length] = 0;
//   waterLevel = tmpValue;
//   newWaterLevel = true;
// }

// //function that prints the latest sensor readings in the OLED display
// void printReadings() {
//   Serial.print("WaterLevel:");
//   Serial.print(waterLevel);
//   Serial.print(" %");
// }

// void setup() {
  
//   //Start serial communication
//   Serial.begin(115200);
//   Serial.println("Starting Arduino BLE Client application...");

//   //Init BLE device
//     //Init BLE device
//   BLEDevice::init("");
 
//   // Retrieve a Scanner and set the callback we want to use to be informed when we
//   // have detected a new device.  Specify that we want active scanning and start the
//   // scan to run for 30 seconds.
//   BLEScan* pBLEScan = BLEDevice::getScan();
//   pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
//   pBLEScan->setActiveScan(true);
//   pBLEScan->start(30);
// }

// void loop() {
//   // If the flag "doConnect" is true then we have scanned for and found the desired
//   // BLE Server with which we wish to connect.  Now we connect to it.  Once we are
//   // connected we set the connected flag to be true.
//   if (doConnect == true) {
//     if (connectToServer(*pServerAddress)) {
//       Serial.println("We are now connected to the BLE Server.");
//       //Activate the Notify property of each Characteristic
//       waterLevelCharacteristics->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOn, 2, true);
//       connected = true;
//     } else {
//       Serial.println("We have failed to connect to the server; Restart your device to scan for nearby BLE server again.");
//     }
//     doConnect = false;
//   }
//   //if new waterLevel readings are available, print in the OLED
//   if (newWaterLevel) {
//     newWaterLevel = false;
//     printReadings();
//   }
//   delay(1000); // Delay a second between loops.
// }