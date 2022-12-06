#ifndef Sensors_H
#define Sensors_H
#include "Config.h"
#include "MQTTConnection.h"
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <BH1750.h>
#include "ACS712.h"

// Uncomment the type of sensor in use:
#define DHTTYPE         DHT11     // DHT 11
//#define DHTTYPE        DHT22      // DHT 22 (AM2302)
//#define DHTTYPE        DHT21      // DHT 21 (AM2301)



class Sensors : public MQTTConnectionListenner
{

private:
    BH1750 lightMeter;


private:
    void onSubMessage(char *payload);
    void writeI2CRegister8bit(int addr, int value);
    unsigned int readI2CRegister16bit(int addr, int reg);

public : 
    Sensors();
    ~Sensors();

    void init();
    void run();
    void readDHT11(float *airTemperature, float *airHumidity);
    unsigned int readSoilMoisture(void);
    float readLight(void);
    float readCurrent(void);
};

#endif

