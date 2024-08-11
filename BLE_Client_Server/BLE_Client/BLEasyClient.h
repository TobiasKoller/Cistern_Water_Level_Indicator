#ifndef BLEASYCLIENT_H
#define BLEASYCLIENT_H

#include "BLEDevice.h"
#include <Wire.h>

// Typedef für den Callback
typedef void (*NotifyCallback)(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);

class BLEasyClient {
public:
    BLEasyClient(const char* serverName, BLEUUID serviceUUID, BLEUUID characteristicUUID);

    void begin();
    void update();

    bool isConnected() const;
    char* getWaterLevel() const;

    // Methode zum Setzen des Benachrichtigungs-Callbacks
    void setNotifyCallback(NotifyCallback callback);

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

    bool connectToServer(BLEAddress pAddress);

    // Statische Callback-Methode
    static void waterLevelNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);

    // Methode zur Verarbeitung der Benachrichtigung
    void handleNotify(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);

    void printReadings();

    static NotifyCallback notifyCallback; // Statischer Funktionszeiger

    class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    public:
        MyAdvertisedDeviceCallbacks(BLEasyClient* client);

        void onResult(BLEAdvertisedDevice advertisedDevice) override;

    private:
        BLEasyClient* client;
    };
};

#endif
