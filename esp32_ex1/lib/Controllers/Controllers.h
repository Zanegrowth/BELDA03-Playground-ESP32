#ifndef Controllers_H
#define Controllers_H
#include "Config.h"
#include "MQTTConnection.h"
#include <ArduinoJson.h>
#include "Sensors.h"
#include "Util.h"
// #include <Math.h>

#define INTERVAL_READ_DHT11     5000    // 5 s
#define INTERVAL_READ_SOIL      9000    // 9 s
#define INTERVAL_READ_LIGHT     5500    // 5.5 s
#define INTERVAL_READ_CURRENT   6000    // 6 s
#define INTERVAL_SEND_PACKET    10000   // 10 s

class Controllers : public MQTTConnectionListenner
{

private: 
    Sensors sensors;

    float airTemperature;
    float airHumidity;
    float lightIntensity;
    unsigned int moisture;
    float voltageValue;
    float currentValue;
    unsigned int waterValue;
    unsigned int fertilizerValue;
    byte IO1Status;
    byte IO2Status;
    byte IO3Status;
    byte IO4Status;
    unsigned int dimLevel;
    unsigned long long closeTime;
    unsigned long long openTime;
    unsigned int errorCode;

    byte onlineStatus;
    byte status;
    unsigned short seq;
    bool sendReady;
    bool controlReday;

    unsigned long lastTimeReadDHT11;
    unsigned long lastTimeReadSoil;
    unsigned long lastTimeReadLight;
    unsigned long lastTimeReadCurrent;
    unsigned long lastTimeSendPacket;

private:
    void onSubMessage(char *payload);
    String packData();
    void parseData();
    static void taskLEDToggle(void *param);
    void dimInit();
    void setDimLevel(int level);

public:
    Controllers();
    ~Controllers();
    void init();
    void run();
    // void dimInit();
    // void setDimLevel(int level);
    // void onMqttMessage(char *topic, byte *payload, unsigned int length);
};

// extern Controllers controllers;

#endif