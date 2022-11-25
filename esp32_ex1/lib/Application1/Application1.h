#ifndef Application1_H
#define Application1_H
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include "UartConnection.h"
#include "Network.h"
#include "Util.h"

class Application1 : public UartConnectionListener, public NetworkListener
{

public:
    Application1();
    ~Application1();
    void init(void);
    void run();

    // Uart Listener
    void onData(SensorPacket &msg);
    void onWifiConfig(SSIDPacket &msg);

    // Network Listener
    void onNetworkEvent(NetworkEvent event);
    void onHttpResponse(const String &url, int statusCode, const String &responseText);

};

extern Application1 app;

#endif
