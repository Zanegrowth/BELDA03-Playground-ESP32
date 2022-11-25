#include "Util.h"
#include <sys/time.h>

TickType_t ms(uint32_t millis)
{
    return millis / portTICK_PERIOD_MS;
}

int64_t currentTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000LL + (tv.tv_usec / 1000LL));
}

int16_t readInt16(const char *data, int offset, bool isBigEndian)
{
    char buf[2];
    memcpy(buf, (data + offset), 2);
    if (isBigEndian)
    {
        return ((int16_t)((buf[0] << 8) | buf[1]));
    }
    else
    {
        return ((int16_t)((buf[1] << 8) | buf[0]));
    }
}

int16_t readInt16LE(const char *data, int offset)
{
    return readInt16(data, offset, false);
}

uint16_t readUInt16LE(const char *data, int offset)
{
    return uint16_t(readInt16LE(data, offset));
}

int16_t readInt16BE(const char *data, int offset)
{
    return readInt16(data, offset, true);
}

uint16_t readUInt16BE(const char *data, int offset)
{
    return uint16_t(readInt16BE(data, offset));
}


void printTaskSize()
{
    UBaseType_t t = uxTaskGetStackHighWaterMark(NULL);
    Serial.print("task size : ");
    Serial.println(t);
}