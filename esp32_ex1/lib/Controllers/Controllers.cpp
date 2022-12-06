#include "Controllers.h"

Controllers::Controllers(){
    airTemperature = 0;
    airHumidity = 0;
    lightIntensity = 0;
    moisture = 0;
    voltageValue = 0;
    currentValue = 0;
    waterValue = 0;
    fertilizerValue = 0;
    IO1Status = 0;
    IO2Status = 0;
    IO3Status = 0;
    IO4Status = 0;
    dimLevel = 0;
    closeTime = 0;
    openTime = 0;

    seq = 0;
    sendReady = 0;
}

Controllers::~Controllers()
{


}

void Controllers::onSubMessage(char *payload)
{
    // Serial.println("Controllers:: data arrive");
    // String payload_str = payload;
    // Serial.println(payload_str);
}

void Controllers::packData()
{
    String payload;
    uint16_t inx = 3;
    const size_t capacity = 1024;
    DynamicJsonDocument doc(capacity);
    doc["id"] = DEVICE_ID;
    doc["type"] = DEVICE_TYPE_NUMBER;
    JsonObject dataObj = doc.createNestedObject("data");
        dataObj["dts"] = currentTime();
        dataObj["seq"] = seq;
        dataObj["fwVersion"] = "1.0.1";

    serializeJson(doc, payload);
    doc.clear();
}

void Controllers::parseData()
{

}

void Controllers::init() 
{
    mqtt.setMQTTConnectionListener(CONTROLLERS_TYPE, *this);
    sensors.init();

    lastTimeReadDHT11 = millis();
    lastTimeReadSoil  = millis();
    lastTimeReadLight = millis();
    lastTimeReadCurrent = millis();
    lastTimeSendPacket = millis();
}


void Controllers::run()
{

    if (millis() - lastTimeReadDHT11 > INTERVAL_READ_DHT11)
    {
        sensors.readDHT11(&airTemperature, &airHumidity);
        lastTimeReadDHT11 = millis();
    }

    if (millis() - lastTimeReadLight > INTERVAL_READ_LIGHT)
    {
        lightIntensity = sensors.readLight();
        lastTimeReadLight = millis();
    }


    if (millis() - lastTimeReadSoil > INTERVAL_READ_SOIL)
    {
        moisture = sensors.readSoilMoisture();
        if (moisture == 31975)
        {
            moisture = 0;
        }
        lastTimeReadSoil = millis();
    }

    if( millis() - lastTimeReadCurrent > INTERVAL_READ_CURRENT)
    {
        float current = sensors.readCurrent();
        if(current > 0){
            currentValue = current;
        } 

        lastTimeReadCurrent = millis();
    }


    if (millis() - lastTimeSendPacket > INTERVAL_SEND_PACKET)
    {

        seq++;
        if (seq >= 256)
        {
            seq = 0;
        }

        struct tm timeinfo;
        Serial.println(F("\n\r###############################################"));
        if (!getLocalTime(&timeinfo))
        {
            //return;
            Serial.println(F("Get LocalTime error"));
        }
        Serial.print("[Seq:: ");
        Serial.print(seq);
        Serial.print("] ");
        Serial.print(&timeinfo, "%A, %B %d %Y %H:%M:%S");
        Serial.print(F("\r\nTemperature: "));
        Serial.print(airTemperature);
        Serial.println(F(" °C"));
        Serial.print(F("Humidity: "));
        Serial.print(airHumidity);
        Serial.println(F(" %"));

        Serial.print(F("Light: "));
        Serial.print(lightIntensity);
        Serial.println(F(" lx"));

        voltageValue = VOLTAGE_VALUE;
        Serial.print(F("V: "));
        Serial.print(voltageValue);
        Serial.println(F(" V"));

        Serial.print(F("I: "));
        Serial.print(currentValue);
        Serial.println(F(" A"));

        Serial.print(F("Soil Moisture: "));
        Serial.print(moisture);
        Serial.println(F(" %"));
        Serial.println(F("Send Data "));
        Serial.println(F("###############################################"));
        lastTimeSendPacket = millis();
    }

}

