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

    Serial.println(F("DHT11 Air Temp&Humd sensor begin"));
    dht.begin();

    // Wire.begin();

    Serial.println(F("Soil Moisture begin"));
    pinMode(SOIL_RESET_PIN, OUTPUT);
    digitalWrite(SOIL_RESET_PIN, LOW);
    delay(1000);
    digitalWrite(SOIL_RESET_PIN, HIGH);
    delay(500);
    pinMode(SOIL_RESET_PIN, INPUT);
    writeI2CRegister8bit(0x20, 6); // reset

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
    // lastTimeGetVol = millis();
    // Serial.println("Calibrating... Ensure that no current flows through the sensor at this moment");
    // int zero =
    Serial.println(F("ACS712 Current sensor begin"));
    current.calibrate();
    // Serial.println("Done!");
    // Serial.println("Zero point for this sensor = " + zero);
}

void Sensors::readDHT11(float *airTemperature, float *airHumidity)
{
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
}

unsigned int Sensors::readSoilMoisture(void)
{
   uint16_t soilMoisture = readI2CRegister16bit(0x20, 0);
   uint16_t percentSoilMoisture = map(soilMoisture, 304, 508, 0, 100);
   return percentSoilMoisture;
   // delay(9000);                                 // this can take a while
}

float Sensors::readLight(void)
{
    float lux;
    if (lightMeter.measurementReady())
    {
        lux = lightMeter.readLightLevel();
    }
    return lux;
}

float Sensors::readCurrent(void)
{
    return current.getCurrentDC();
}

void Sensors::run()
{


}

// https://wemakethings.net/chirp/