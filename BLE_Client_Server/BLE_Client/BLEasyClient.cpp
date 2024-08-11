#include "BLEasyClient.h"

// Initialisierung des statischen Funktionszeigers
NotifyCallback BLEasyClient::notifyCallback = nullptr;

BLEasyClient::BLEasyClient(const char* serverName, BLEUUID serviceUUID, BLEUUID characteristicUUID)
    : bleServerName(serverName), SERVICE_UUID(serviceUUID), CHARACTERISTIC_UUID(characteristicUUID) {
    doConnect = false;
    connected = false;
    waterLevel = nullptr;
    newWaterLevel = false;
}

void BLEasyClient::begin() {
    Serial.begin(115200);
    Serial.println("Starting Arduino BLE Client application...");
    BLEDevice::init("");
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(this));
    pBLEScan->setActiveScan(true);
    pBLEScan->start(30);
}

void BLEasyClient::update() {
    if (doConnect) {
        if (connectToServer(*pServerAddress)) {
            Serial.println("We are now connected to the BLE Server.");
            waterLevelCharacteristics->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOn, 2, true);
            connected = true;
        } else {
            Serial.println("We have failed to connect to the server; Restart your device to scan for nearby BLE server again.");
        }
        doConnect = false;
    }

    if (newWaterLevel) {
        newWaterLevel = false;
        printReadings();
    }
}

bool BLEasyClient::isConnected() const {
    return connected;
}

char* BLEasyClient::getWaterLevel() const {
    return waterLevel;
}

bool BLEasyClient::connectToServer(BLEAddress pAddress) {
    Serial.println("connectToServer");
    BLEClient* pClient = BLEDevice::createClient();

    if (!pClient->connect(pAddress)) {
        Serial.println(" - Failed to connect to server");
        return false;
    }
    Serial.println(" - Connected to server");

    BLERemoteService* remoteService = pClient->getService(SERVICE_UUID);
    if (remoteService == nullptr) {
        Serial.print("Failed to find our service UUID: ");
        Serial.println(SERVICE_UUID.toString().c_str());
        return false;
    }

    waterLevelCharacteristics = remoteService->getCharacteristic(CHARACTERISTIC_UUID);
    if (waterLevelCharacteristics == nullptr) {
        Serial.print("Failed to find our characteristic UUID");
        return false;
    }
    Serial.println(" - Found our characteristics");

    // Registrierung des Callbacks
    if (notifyCallback != nullptr) {
        waterLevelCharacteristics->registerForNotify(waterLevelNotifyCallback, true);
    }

    return true;
}

void BLEasyClient::waterLevelNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
    // Call the instance method through the static callback
    if (notifyCallback != nullptr) {
        // Um den Funktionszeiger korrekt zu nutzen, wird hier keine Instanz verwendet.
        // Die Instanzmethode wird nicht direkt aufgerufen.
        notifyCallback(pBLERemoteCharacteristic, pData, length, isNotify);
    }
}

void BLEasyClient::handleNotify(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
    // Verarbeitet die Benachrichtigung
}

void BLEasyClient::setNotifyCallback(NotifyCallback callback) {
    notifyCallback = callback;
}

void BLEasyClient::printReadings() {
    Serial.print("WaterLevel: ");
    Serial.print(waterLevel);
    Serial.print(" %");
}

BLEasyClient::MyAdvertisedDeviceCallbacks::MyAdvertisedDeviceCallbacks(BLEasyClient* client) : client(client) {}

void BLEasyClient::MyAdvertisedDeviceCallbacks::onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getName() == client->bleServerName) {
        advertisedDevice.getScan()->stop();
        client->pServerAddress = new BLEAddress(advertisedDevice.getAddress());
        client->doConnect = true;
        Serial.println("Device found. Connecting!");
    }
}
