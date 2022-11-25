#ifndef NetworkListener_H
#define NetworkListener_H

enum NetworkEvent
{
    WIFI_CONNECTED,
    WIFI_DISCONNECTED,
    INTERNET_CONNECTED,
    INTERNET_DISCONNECTED,
    MQTT_HUB_CONNECTED,
    MQTT_HUB_DISCONNECTED,
    TIME_SYNCED
};

enum DownloadEvent
{
    DOWNLOAD_FAIL,
    DOWNLOAD_RESPONSE_ERROR
};

class NetworkListener
{
public:
    virtual void onNetworkEvent(NetworkEvent event) = 0;
    virtual void onHttpResponse(const String &url, int statusCode, const String &responseText) = 0;
};

#endif