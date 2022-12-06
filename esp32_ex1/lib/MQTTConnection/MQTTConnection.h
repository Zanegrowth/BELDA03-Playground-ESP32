#ifndef MQTTConnection_H
#define MQTTConnection_H
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "Network.h"
#include "Config.h"
#include "MQTTConnectionListenner.h"

enum TypeModule
{
    SENSORS_TYPE,
    CONTROLLERS_TYPE
};

class MQTTConnection 
{
private:
    String mqttClientId;
    WiFiClientSecure wifiClient;
    PubSubClient client;
    MQTTConnectionListenner *slistener;
    MQTTConnectionListenner *clistener;

private:
   
public:
    MQTTConnection();
    ~MQTTConnection();


    void init();
    void onMqttMessage(char *topic, byte *payload, unsigned int length);
    void setMQTTConnectionListener(TypeModule type, MQTTConnectionListenner &listener);
    void setClientId(String clientId);
    void publishToCloud(const char *topic, const char *payload);
    bool isMqttConnected();
    void run();
};

extern MQTTConnection mqtt;

#endif