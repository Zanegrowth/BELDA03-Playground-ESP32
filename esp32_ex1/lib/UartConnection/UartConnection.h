#ifndef UartConnection_H
#define UartConnection_H
#include <Arduino.h>
#include <FastCRC.h>
#include "Util.h"
#include "Packet.h"
#include "UartConnectionListener.h"

class UartConnection
{
private:
    static const int MAX_BUFFER_SIZE = 128;
    // char buffer[MAX_BUFFER_SIZE];
    // size_t read_inx = 0;
    HardwareSerial *sp;
    FastCRC16 crc16;
    String device_id;
    UartConnectionListener *listener;

private:
    void parsePacket(const char *data);
    bool validCRC(const char *data, const int len);
    // size_t readBytesUntil(const char *terminator, char *buffer);

public:
    UartConnection();
    ~UartConnection();
   
    void setConnection(HardwareSerial &serial);
    void setListener(UartConnectionListener &l);
    void run(void);
};

extern UartConnection uc;

#endif