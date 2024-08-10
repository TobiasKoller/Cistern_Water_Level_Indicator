#ifndef BLEASYCLIENT_H
#define BLEASYCLIENT_H

#include "BLEDevice.h"

class BLEasyClient {
public:
    BLEasyClient(const char* serverName, BLEUUID serviceUUID, BLEUUID characteristicUUID);
    void begin();
    void update();
    bool isConnected() const;
    char* getWaterLevel() const;

private:
    const char* bleServerName;
    BLEUUID SERVICE_UUID;
    BLEUUID CHARACTERISTIC_UUID;

    boolean doConnect;
    boolean connected;
    BLEAddress* pServerAddress;
    BLERemoteCharacteristic* waterLevelCharacteristics;

    char* waterLevel;
    boolean newWaterLevel;

    const uint8_t notificationOn[2] = {0x1, 0x0};
    const uint8_t notificationOff[2] = {0x0, 0x0};

    bool connectToServer(BLEAddress pAddress);
    static void waterLevelNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic,
                                         uint8_t* pData, size_t length, bool isNotify);
    void printReadings();

    class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    public:
        MyAdvertisedDeviceCallbacks(BLEasyClient* client);
        void onResult(BLEAdvertisedDevice advertisedDevice) override;

    private:
        BLEasyClient* client;
    };
};

#endif
