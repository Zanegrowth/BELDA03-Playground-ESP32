#ifndef UartConnectionListener_H
#define UartConnectionListener_H
#include "Packet.h"

class UartConnectionListener{
public:
    // virtual void onData(BLEPacket &msg) = 0;
    virtual void onData(SensorPacket &msg) = 0;
    virtual void onWifiConfig(SSIDPacket &msg) = 0;
};

#endif