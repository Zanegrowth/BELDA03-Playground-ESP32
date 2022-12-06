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
    /*Todo..*/
}

void UartConnection::run(void){
    int len = 0, last_len = 0, inx = 0;
    char buf_read[MAX_BUFFER_SIZE];
    char buf_all[MAX_BUFFER_SIZE];
    while (sp->available() > 0)
    {
        /*
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
        */
    }
}

UartConnection uc;