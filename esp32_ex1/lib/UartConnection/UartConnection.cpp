#include "UartConnection.h"

UartConnection::UartConnection()
{
    // device_id = "TNB000012";
}

UartConnection::~UartConnection()
{
}

void UartConnection::setConnection(HardwareSerial &serial)
{
    sp = &serial;
}

void UartConnection::setListener(UartConnectionListener &l)
{
    listener = &l;
}

bool UartConnection::validCRC(const char *data, const int len)
{
    uint8_t message[len];
    memcpy(message, data, len);
    uint16_t dataCRC = readUInt16BE(data, len);
    // Serial.print("dataCRC: 0x"); 
    // Serial.println(dataCRC, HEX);
    uint16_t calCRC = crc16.ccitt(message, len);
    // Serial.print("calCRC: 0x");
    // Serial.println(calCRC, HEX);
    return dataCRC == calCRC;
}

void UartConnection::parsePacket(const char *data)
{

    char mode = data[1];
    switch (mode)
    {
    case MESSAGE_MODE:
        {
            char type = data[2];
            switch (type)
            {
            case SSID_TYPE:
            {
                SSIDPacket ssidMsg;
                Packet::parse(ssidMsg, data);
                listener->onWifiConfig(ssidMsg);
            }
            break;
            case DEVICE_ID_TYPE:
            {
                // String deviceIDMsg;
                Packet::parse(device_id, data);
                Serial.print("device id:: ");
                Serial.println(device_id);
   
            }
            break;
            default:
                break;
            }
        }
        break;

    case SENSOR_MODE:
    {

        SensorPacket sensorMsg;
        sensorMsg.device_id = device_id;
        Packet::parse(sensorMsg, data);
        if (device_id.length() != 0)
        {
            listener->onData(sensorMsg);
        }
    }
        break;
    
    default:
        break;
    }


}

void UartConnection::run(void){
    int len = 0, last_len = 0, inx = 0;
    char buf_read[MAX_BUFFER_SIZE];
    char buf_all[MAX_BUFFER_SIZE];
    while (sp->available() > 0)
    {
        len = sp->readBytesUntil('\n', buf_read, MAX_BUFFER_SIZE) + last_len;

        memcpy(&buf_all[inx], buf_read, len);

        // Serial.print("len: ");
        // Serial.println(len, DEC);
        // byte len = sp->available();
        // for (int i = 0; i < len; i++)
        // {
        // Serial.print("0x");
        // Serial.print(buf_all[i], HEX);
        // Serial.print(", ");
        // }
        // Serial.println();

        if (len > 4 && buf_all[0] == '$' && buf_all[len - 1] == '\r')
        // if (len > 4 && buffer[0] == '$')
        {
            last_len = 0;
            inx = 0;

            // len = len - 4;
            len = len - 3;

            if (validCRC(buf_all, len))
            {
                Serial.println("CRC is match");
                parsePacket(buf_all);
            }
        }else {
            last_len = len + 1;
            inx = len;
            buf_all[inx++] = 0x0A;
        }
    }
}

UartConnection uc;