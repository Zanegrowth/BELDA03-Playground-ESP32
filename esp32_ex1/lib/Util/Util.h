#include <Arduino.h>

int64_t currentTime();
int16_t readInt16LE(const char *data, int offset);
int16_t readInt16BE(const char *data, int offset);
uint16_t readUInt16LE(const char *data, int offset);
uint16_t readUInt16BE(const char *data, int offset);
TickType_t ms(uint32_t millis);
void printTaskSize();