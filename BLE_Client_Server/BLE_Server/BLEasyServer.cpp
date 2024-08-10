#include "BLEasyServer.h"
#include <Wire.h>

bool BLEasyServer::deviceConnected = false;

char* serviceUUID;

BLEasyServer::BLEasyServer(const std::string& serverName, const std::string& serviceUUID): serviceUUID(serviceUUID),lastTime(0), timerDelay(3000) {
    Serial.begin(115200);
    Serial.println("Ctr..BLEasyServer");

    BLEDevice::init(String(serverName.c_str()));
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new ClientServerCallbacks());

    pService = pServer->createService(BLEUUID(serviceUUID.c_str()));
}

void BLEasyServer::start() {
    pService->start();
    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(BLEUUID(serviceUUID.c_str()));
    pServer->getAdvertising()->start();

    
    Serial.println("Waiting for a client connection to notify...");
}

void BLEasyServer::notify() {
    Serial.println("notify...");
    if (deviceConnected) {
        // if ((millis() - lastTime) > timerDelay) {
            for (auto& pair : characteristics) {
                BLECharacteristic* characteristic = pair.second;
                characteristic->notify();
                Serial.print("Characteristic ");
                Serial.print(pair.first.c_str());
                Serial.println(" notified.");
            }
            lastTime = millis();
        // }
    }
}

void BLEasyServer::registerCharacteristic(const std::string& charUUID, const std::string& descriptorValue) {
    Serial.print("registerCharacteristic ");
    Serial.println(charUUID.c_str());
    BLECharacteristic* characteristic = new BLECharacteristic(BLEUUID(charUUID.c_str()), BLECharacteristic::PROPERTY_NOTIFY);
    BLEDescriptor* descriptor = new BLEDescriptor(BLEUUID((uint16_t)0x2902));
    descriptor->setValue(descriptorValue.c_str());

    pService->addCharacteristic(characteristic);
    characteristic->addDescriptor(descriptor);

    characteristics[charUUID] = characteristic;
}

void BLEasyServer::updateCharacteristic(const std::string& charUUID, const std::string& value) {
  Serial.print("updateCharacteristic ");
  Serial.println(charUUID.c_str());
    if (characteristics.find(charUUID) != characteristics.end()) {
        characteristics[charUUID]->setValue(value.c_str());
    }
}

void BLEasyServer::ClientServerCallbacks::onConnect(BLEServer* pServer) {
    Serial.println("Connection established.");
    BLEasyServer::deviceConnected = true;
}

void BLEasyServer::ClientServerCallbacks::onDisconnect(BLEServer* pServer) {
    Serial.println("Connection closed.");
    BLEasyServer::deviceConnected = false;
}
