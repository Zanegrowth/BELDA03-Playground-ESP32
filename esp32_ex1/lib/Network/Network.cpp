#include "Network.h"
#include "Util.h"
#include <freertos/FreeRTOS.h>
const uint8_t TIMEZONE = 7;

Network::Network(){

}

Network::~Network(){
    delete listener;
}

void Network::setWifiConfig(String ssid, String pass)
{
    wifiSSID = ssid;
    wifiPass = pass;
}

void Network::initWiFi(void){
    WiFi.mode(WIFI_MODE_STA);
    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info){ 
        net.onWiFiEvent(event, info); 
    });
    connectWiFi();
}

void Network::connectWiFi(void)
{
    if (!wifiSSID.isEmpty() && !wifiPass.isEmpty())
    {
        WiFi.begin(wifiSSID.c_str(), wifiPass.c_str());
    }
}

void Network::taskSyncTime(void *param)
{
    Serial.println("Time syncing...");
    Network *obj = static_cast<Network *>(param);
    while (!obj->timeSyncStatus)
    {
        if (obj->isNetworkConnected())
        {
            obj->syncTime();
        }
        vTaskDelay(ms(1000));
    }
    vTaskDelete(NULL);
}

bool Network::isNetworkConnected()
{
    // Serial.print(WiFi.status());
    return WiFi.status() == WL_CONNECTED;
}

bool Network::isInternetAvailable()
{
    return internetStatus;
}

void Network::checkInternet()
{
    if (isNetworkConnected())
    {
        Serial.println("Check internet...");
        internetStatus = wifiClient.connect("google.com", 443);
        wifiClient.stop();
        onNetworkEvent(internetStatus ? INTERNET_CONNECTED : INTERNET_DISCONNECTED);
    }
    else
    {
        internetStatus = false;
    }
}

void Network::init(void)
{
    initWiFi(); 
    // wifiClientHttp.setInsecure();
    wifiClient.setInsecure();
   
}

void Network::setNetworkListener(NetworkListener &listener)
{
    this->listener = &listener;
}

void Network::onNetworkEvent(NetworkEvent event)
{
    if (listener != NULL)
    {
        listener->onNetworkEvent(event);
    }
}

void Network::syncTime()
{
    configTime(TIMEZONE * 3600, 0, "pool.ntp.org", "time.nist.gov", "time.uni.net.th");
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        return;
    }
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    timeSyncStatus = true;
    onNetworkEvent(TIME_SYNCED);
}

bool Network::isTimeSync()
{
    return timeSyncStatus;
}


void Network::onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info)
{

    switch (event)
    {
    case SYSTEM_EVENT_STA_CONNECTED:
    {
    }
    break;
    case SYSTEM_EVENT_STA_GOT_IP:
    {
        if (wifiState != SYSTEM_EVENT_STA_GOT_IP)
        {
            wifiState = SYSTEM_EVENT_STA_GOT_IP;
            onNetworkEvent(WIFI_CONNECTED);
            if (!isTimeSync())
            {
                xTaskCreate(taskSyncTime, "taskSyncTime", 1024 * 2, this, 10, NULL);
            } 
        }
    }
    break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
    {
        if (wifiState != SYSTEM_EVENT_STA_DISCONNECTED)
        {
            wifiState = SYSTEM_EVENT_STA_DISCONNECTED;
            onNetworkEvent(WIFI_DISCONNECTED);
        }
        WiFi.reconnect();
    }
    break;

    default:
        break;
    }
}

Network net;