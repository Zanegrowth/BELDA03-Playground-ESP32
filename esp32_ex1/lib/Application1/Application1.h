#ifndef Application1_H
#define Application1_H
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include "Network.h"
#include "MQTTConnection.h"
#include "Util.h"
#include "Config.h"
#include "Sensors.h"
#include "Controllers.h"
// #include <ArduinoJson.h>

class Application1 : public NetworkListener
{


private:
  Controllers controllers;

private:
    // Network Listener
    void onNetworkEvent(NetworkEvent event);

public:
    Application1();
    ~Application1();
    void init(void);
    void run();


};

extern Application1 app1;

#endif
