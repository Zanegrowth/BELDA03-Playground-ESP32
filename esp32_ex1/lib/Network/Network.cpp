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
    Network *obj = static_cast<Network *>(param);
    while (!obj->isTimeSync)
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
    return WiFi.status() == WL_CONNECTED;
    Serial.print(WiFi.status());
}

// bool Network::isInternetAvailable()
// {
//     return isInternet;
// }

// void Network::checkInternet()
// {
//     if (isNetworkConnected())
//     {
//         Serial.println("Check internet");
//         isInternet = wifiClient.connect("google.com", 443);
//         wifiClient.stop();
//         if (listener != NULL)
//         {
//             listener->onNetworkEvent(isInternet ? INTERNET_CONNECTED : INTERNET_DISCONNECTED);
//         }
//     }
//     else
//     {
//         isInternet = false;
//     }
// }

void Network::init(void)
{
    // initWiFi();
    wifiClientHttp.setInsecure();

    xTaskCreate(taskSyncTime, "taskSyncTime", 1024 * 2 , this, 10 , NULL);
}

void Network::setNetworkListener(NetworkListener &listener)
{
    this->listener = &listener;
}

void Network::setHttpAuthHeader(String name, String value)
{
    httpHeaderAuthName = name;
    httpHeaderAuthValue = value;
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
    isTimeSync = true;
    if (listener != NULL)
    {
        listener->onNetworkEvent(TIME_SYNCED);
    }
}

void Network::httpGet(String &url)
{
    if (http.begin(wifiClientHttp, url))
    {
        // http.addHeader(httpHeaderAuthName, httpHeaderAuthValue, true, true);
        int statusCode = http.GET();
        if (listener != NULL)
        {
            listener->onHttpResponse(url, statusCode, http.getString());
        }
        http.end();
    }
}

void Network::httpPost(String &url, String &payload)
{
    if (http.begin(wifiClientHttp, url))
    {
        http.addHeader(httpHeaderAuthName, httpHeaderAuthValue, true, true);
        http.addHeader("Content-Type", "application/json", false, false);

        int statusCode = http.POST(payload);
        if (listener != NULL)
        {
            listener->onHttpResponse(url, statusCode, http.getString());
        }
        http.end();
    }
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
            if (listener != NULL){
                listener->onNetworkEvent(WIFI_CONNECTED);
            }
            // Serial.print("WIFI_CONNECTED");
        }
    }
    break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
    {
        if (wifiState != SYSTEM_EVENT_STA_DISCONNECTED)
        {
            wifiState = SYSTEM_EVENT_STA_DISCONNECTED;
            if (listener != NULL){
                listener->onNetworkEvent(WIFI_DISCONNECTED);
            }
            // Serial.print("WIFI_DISCONNECTED");
        }
        WiFi.reconnect();
    }
    break;

    default:
        break;
    }
}



Network net;