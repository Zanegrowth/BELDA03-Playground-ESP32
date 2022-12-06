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

class NetworkListener
{
public:
    virtual void onNetworkEvent(NetworkEvent event) = 0;
};

#endif