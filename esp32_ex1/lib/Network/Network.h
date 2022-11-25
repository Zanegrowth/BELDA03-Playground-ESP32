#ifndef Network_H
#define Network_H
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "NetworkListener.h"
// #include <PubSubClient.h>

class Network
{

private:
    int wifiState = 0;
    bool isInternet = false;
    bool isTimeSync = false;
    String wifiSSID;
    String wifiPass;
    String httpHeaderAuthName;
    String httpHeaderAuthValue;
    HTTPClient http;
    WiFiClientSecure wifiClient;
    WiFiClientSecure wifiClientHttp;
    NetworkListener *listener;

private:
    static void taskSyncTime(void *param);
    void onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info);
   
    void connectWiFi(void);
    void syncTime();

public:
    Network();
    ~Network();

    void setWifiConfig(String ssid, String pass);
    void initWiFi(void);
    void init(void);
    void setHttpAuthHeader(String name, String value);
    void httpGet(String &url);
    void httpPost(String &url, String &payload);
    void setNetworkListener(NetworkListener &listener);
    bool isNetworkConnected();
    // bool isInternetAvailable();
    // void checkInternet();
};

extern Network net;

#endif