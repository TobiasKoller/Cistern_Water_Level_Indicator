#include "BLEasyServer.h"
#include <Wire.h>

bool BLEasyServer::deviceConnected = false;

BLEasyServer::BLEasyServer(const std::string& serverName, const std::string& serviceUUID) 
    : lastTime(0), timerDelay(3000) {
    BLEDevice::init(String(serverName.c_str()));
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    pService = pServer->createService(BLEUUID(serviceUUID.c_str()));
    pService->start();

    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(BLEUUID(serviceUUID.c_str()));
    pServer->getAdvertising()->start();
}

void BLEasyServer::start() {
    Serial.begin(115200);
    Serial.println("Waiting for a client connection to notify...");
}

void BLEasyServer::loop() {
    if (deviceConnected) {
        if ((millis() - lastTime) > timerDelay) {
            for (auto& pair : characteristics) {
                BLECharacteristic* characteristic = pair.second;
                characteristic->notify();
                Serial.print("Characteristic ");
                Serial.print(pair.first.c_str());
                Serial.println(" notified.");
            }
            lastTime = millis();
        }
    }
}

void BLEasyServer::registerCharacteristic(const std::string& charUUID, const std::string& descriptorValue) {
    BLECharacteristic* characteristic = new BLECharacteristic(BLEUUID(charUUID.c_str()), BLECharacteristic::PROPERTY_NOTIFY);
    BLEDescriptor* descriptor = new BLEDescriptor(BLEUUID((uint16_t)0x2902));
    descriptor->setValue(descriptorValue.c_str());

    pService->addCharacteristic(characteristic);
    characteristic->addDescriptor(descriptor);

    characteristics[charUUID] = characteristic;
}

void BLEasyServer::updateCharacteristic(const std::string& charUUID, const std::string& value) {
    if (characteristics.find(charUUID) != characteristics.end()) {
        characteristics[charUUID]->setValue(value.c_str());
    }
}

void BLEasyServer::MyServerCallbacks::onConnect(BLEServer* pServer) {
    Serial.println("Connection established.");
    BLEasyServer::deviceConnected = true;
}

void BLEasyServer::MyServerCallbacks::onDisconnect(BLEServer* pServer) {
    Serial.println("Connection closed.");
    BLEasyServer::deviceConnected = false;
}
