#include "Sensors.h"

DHT_Unified dht(DHT_PIN, DHTTYPE);
ACS712 current(ACS712_05B, CURRENT_PIN);

Sensors::Sensors(/* args */)
{
}

Sensors::~Sensors()
{
}

void Sensors::onSubMessage(char *payload)
{
    // Serial.println("Sensors:: data arrive");
}

void Sensors::writeI2CRegister8bit(int addr, int value)
{
    Wire.beginTransmission(addr);
    Wire.write(value);
    Wire.endTransmission();
}

unsigned int Sensors::readI2CRegister16bit(int addr, int reg)
{
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission();
    delay(20);
    Wire.requestFrom(addr, 2);
    unsigned int t = Wire.read() << 8;
    t = t | Wire.read();
    return t;
}

void Sensors::init()
{
    // mqtt.setMQTTConnectionListener(SENSORS_TYPE, *this);

    Serial.println(F("Sensors setup..."));
    Wire.begin(SDA_PIN, SCL_PIN);

#if USE_AIR_SENSOR == 1
    Serial.println(F("DHT11 Air Temp&Humd sensor begin"));
    dht.begin();
#endif

#if USE_SOIL_MOIS_SENSOR == 1
    Serial.println(F("Soil Moisture begin"));
    pinMode(SOIL_RESET_PIN, OUTPUT);
    digitalWrite(SOIL_RESET_PIN, LOW);
    delay(1000);
    digitalWrite(SOIL_RESET_PIN, HIGH);
    delay(500);
    pinMode(SOIL_RESET_PIN, INPUT);
    writeI2CRegister8bit(0x20, 6); // reset

#endif

#if USE_LIGHT_SENSOR == 1
    // begin returns a boolean that can be used to detect setup problems.
    if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE))
    {
        Serial.println(F("BH1750 Light sensor begin"));
        SENSOR_NORMAL();
    }
    else
    {
        Serial.println(F("Error initialising BH1750"));
        SENSOR_ABNORMAL();
    }
#endif

#if USE_CURRENT_SENSOR == 1
    Serial.println(F("ACS712 Current sensor begin"));
    current.calibrate();
#endif

}

void Sensors::readDHT11(float *airTemperature, float *airHumidity)
{
#if USE_AIR_SENSOR == 1
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature))
    {
        Serial.println(F("Error reading temperature!"));
        SENSOR_ABNORMAL();
    }
    else
    {
        *airTemperature = event.temperature;
        SENSOR_NORMAL();
    }
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity))
    {
        Serial.println(F("Error reading humidity!"));
        SENSOR_ABNORMAL();
    }
    else
    {
        *airHumidity = event.relative_humidity;
        SENSOR_NORMAL();
    }
#else
    *airTemperature = 0.0;
    *airHumidity = 0.0;
#endif
}

unsigned int Sensors::readSoilMoisture(void)
{
    uint16_t percentSoilMoisture = 0;
#if USE_SOIL_MOIS_SENSOR == 1
    uint16_t soilMoisture = readI2CRegister16bit(0x20, 0);
    percentSoilMoisture = map(soilMoisture, 304, 508, 0, 100);
    return percentSoilMoisture;
    // delay(9000);                                 // this can take a while
#endif
    return percentSoilMoisture;
}

float Sensors::readLight(void)
{
    float lux = 0.0;
#if USE_LIGHT_SENSOR == 1
    if (lightMeter.measurementReady())
    {
        lux = lightMeter.readLightLevel();
    }
#endif
    return lux;
}

float Sensors::readCurrent(void)
{
    float currentValue = 0.0;
#if USE_CURRENT_SENSOR == 1
    currentValue = current.getCurrentDC();
#endif
    return currentValue;
}

void Sensors::run()
{

}

// https://wemakethings.net/chirp/