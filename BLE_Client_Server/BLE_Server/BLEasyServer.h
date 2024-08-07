#ifndef BLEASY_SERVER_H
#define BLEASY_SERVER_H

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <map>
#include <string>

class BLEasyServer {
public:
    BLEasyServer(const std::string& serverName, const std::string& serviceUUID);
    void start();
    void loop();
    void registerCharacteristic(const std::string& charUUID, const std::string& descriptorValue);
    void updateCharacteristic(const std::string& charUUID, const std::string& value);

    static bool deviceConnected; // Make it static

private:
    class MyServerCallbacks : public BLEServerCallbacks {
        void onConnect(BLEServer* pServer);
        void onDisconnect(BLEServer* pServer);
    };

    BLEServer* pServer;
    BLEService* pService;
    std::map<std::string, BLECharacteristic*> characteristics;
    unsigned long lastTime;
    unsigned long timerDelay;
};


#endif // BLEASY_SERVER_H
