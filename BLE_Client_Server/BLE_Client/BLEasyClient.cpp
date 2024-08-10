#include "BLEDevice.h"
#include <Wire.h>

class BLEasyClient {
public:
    BLEasyClient(const char* serverName, BLEUUID serviceUUID, BLEUUID characteristicUUID)
        : bleServerName(serverName), SERVICE_UUID(serviceUUID), CHARACTERISTIC_UUID(characteristicUUID) {
        doConnect = false;
        connected = false;
        waterLevel = nullptr;
        newWaterLevel = false;
    }

    void begin() {
        Serial.begin(115200);
        Serial.println("Starting Arduino BLE Client application...");
        BLEDevice::init("");
        BLEScan* pBLEScan = BLEDevice::getScan();
        pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(this));
        pBLEScan->setActiveScan(true);
        pBLEScan->start(30);
    }

    void update() {
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

    bool isConnected() const {
        return connected;
    }

    char* getWaterLevel() const {
        return waterLevel;
    }

private:
    const char* bleServerName;
    BLEUUID SERVICE_UUID;
    BLEUUID CHARACTERISTIC_UUID;

    boolean doConnect;
    boolean connected;
    BLEAddress* pServerAddress = nullptr;
    BLERemoteCharacteristic* waterLevelCharacteristics = nullptr;

    char* waterLevel;
    boolean newWaterLevel;

    const uint8_t notificationOn[2] = {0x1, 0x0};
    const uint8_t notificationOff[2] = {0x0, 0x0};

    bool connectToServer(BLEAddress pAddress) {
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
        waterLevelCharacteristics->registerForNotify(waterLevelNotifyCallback, true);
        return true;
    }

    static void waterLevelNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic,
                                         uint8_t* pData, size_t length, bool isNotify) {
        // char* tmpValue = (char*)pData;
        // tmpValue[length] = 0;
        // BLEasyClient* client = (BLEasyClient*)pBLERemoteCharacteristic->getClient();
        // client->waterLevel = tmpValue;
        // client->newWaterLevel = true;
    }

    void printReadings() {
        Serial.print("WaterLevel: ");
        Serial.print(waterLevel);
        Serial.print(" %");
    }

    class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    public:
        MyAdvertisedDeviceCallbacks(BLEasyClient* client) : client(client) {}

        void onResult(BLEAdvertisedDevice advertisedDevice) override {
            if (advertisedDevice.getName() == client->bleServerName) {
                advertisedDevice.getScan()->stop();
                client->pServerAddress = new BLEAddress(advertisedDevice.getAddress());
                client->doConnect = true;
                Serial.println("Device found. Connecting!");
            }
        }

    private:
        BLEasyClient* client;
    };
};
