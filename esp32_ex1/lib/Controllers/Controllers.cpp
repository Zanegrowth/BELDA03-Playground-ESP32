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
    errorCode = 0;

    onlineStatus = 0;
    status = 1;
    seq = 0;
    sendReady = 0;
    controlReday = 0;
}

Controllers::~Controllers()
{


}

void Controllers::onSubMessage(char *payload)
{
    Serial.println("<-- Dta arrive");
    String payload_str = payload;
    Serial.println(payload_str);

    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
    if (doc.containsKey("onlineStatus"))
    {
        onlineStatus = doc["onlineStatus"];
    }
    if (doc.containsKey("IO1Status"))
    {
        IO1Status = doc["IO1Status"];
    }
    if (doc.containsKey("IO2Status"))
    {
        IO2Status = doc["IO2Status"];
    }
    if (doc.containsKey("IO3Status"))
    {
        IO3Status = doc["IO3Status"];
    }
    if (doc.containsKey("IO4Status"))
    {
        IO4Status = doc["IO4Status"];
    }
    if (doc.containsKey("dimLevel"))
    {
        dimLevel = doc["dimLevel"];
        // setDimLevel(dimLevel);
    }
    if (doc.containsKey("closeTime"))
    {
        closeTime = doc["closeTime"];
    }
    if (doc.containsKey("openTime"))
    {
        openTime = doc["openTime"];
    }

    Serial.print(F("onlineStatus: "));
    Serial.println(onlineStatus);
    Serial.print(F("IO1Status: "));
    Serial.println(IO1Status);
    Serial.print(F("IO2Status: "));
    Serial.println(IO2Status);
    Serial.print(F("IO3Status: "));
    Serial.println(IO3Status);
    Serial.print(F("IO4Status: "));
    Serial.println(IO4Status);
    Serial.print(F("dimLevel: "));
    Serial.println(dimLevel);
    Serial.print(F("closeTime: "));
    Serial.println(closeTime);
    Serial.print(F("openTime: "));
    Serial.println(openTime);

    sendReady = 1;
    controlReday = 1;
    doc.clear();
}

String Controllers::packData()
{
    String payload;
    uint16_t inx = 3;
    const size_t capacity = 1024;
    DynamicJsonDocument doc(capacity);
    // doc["id"] = DEVICE_ID;
    // doc["type"] = DEVICE_TYPE_NUMBER;
    // JsonObject dataObj = doc.createNestedObject("data");
    doc["dts"] = currentTime();
    doc["seq"] = seq;
    doc["status"] = status;
    doc["airtemp"] = serialized(String(airTemperature, 2)); // roundf(val * 100) / 100
    doc["airhumd"] = serialized(String(airHumidity, 2));
    doc["lightIntensity"] = serialized(String(lightIntensity, 2));
    doc["moisture"] = moisture;
    doc["voltageValue"] = serialized(String(voltageValue, 2));
    doc["currentValue"] = serialized(String(currentValue, 2));
    doc["waterValue"] = waterValue;
    doc["fertilizerValue"] = fertilizerValue;
    doc["IO1Status"] = IO1Status;
    doc["IO2Status"] = IO2Status;
    doc["IO3Status"] = IO3Status;
    doc["IO4Status"] = IO4Status;
    doc["dimLevel"] = dimLevel;
    doc["closeTime"] = closeTime;
    doc["openTime"] = openTime;
    doc["errorCode"] = errorCode;

    serializeJson(doc, payload);
    // Serial.println(payload);
    doc.clear();
    return payload;
}

void Controllers::parseData()
{

}

void Controllers::init() 
{

    pinMode(IO1, OUTPUT);
    pinMode(IO2, OUTPUT);
    pinMode(IO3, OUTPUT);
    pinMode(IO4, OUTPUT);

    digitalWrite(IO1, IO1Status);
    digitalWrite(IO2, IO2Status);
    digitalWrite(IO3, IO3Status);
    digitalWrite(IO4, IO4Status);

    dimInit();
    mqtt.setMQTTConnectionListener(CONTROLLERS_TYPE, *this);
    sensors.init();

    lastTimeReadDHT11 = millis();
    lastTimeReadSoil  = millis();
    lastTimeReadLight = millis();
    lastTimeReadCurrent = millis();
    lastTimeSendPacket = millis();
}

void Controllers::taskLEDToggle(void *param)
{
    digitalWrite(LED_YELLOW_PIN, LOW);
    vTaskDelay(ms(500));
    digitalWrite(LED_YELLOW_PIN, HIGH);
    vTaskDelete(NULL);
}

void Controllers::dimInit()
{
    // setting PWM properties
    const int freq = 10000;
    // const int channel = 0;
    const int resolution = 8;

    // configure LED PWM functionalitites
    ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESULOTION);

    // attach the channel to the GPIO to be controlled
    ledcAttachPin(DIM_PIN, PWM_CHANNEL);
    setDimLevel(dimLevel);
}

void Controllers::setDimLevel(int level)
{
    int dutyCycle = map(level, 0, 100, 0, 255);
    Serial.print(F("dutyCycle :: "));
    Serial.print(dutyCycle);
    ledcWrite(PWM_CHANNEL, dutyCycle);
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
        // Serial.println(F("Send Data "));
        Serial.println(F("###############################################"));
        sendReady = 1;
        // lastTimeSendPacket = millis();
    }

    if(sendReady){
        Serial.println(F("--> Send Data "));
        String packMsg = packData();
        // Serial.println(packMsg);
        mqtt.publishToCloud(MQTT_DEVICE_TOPIC, packMsg.c_str());
        xTaskCreate(taskLEDToggle, "taskLEDToggle", 1024, this, 11, NULL);



        lastTimeSendPacket = millis();
        sendReady = 0;
    }
    if(controlReday){
#if USE_DIM == 1
        digitalWrite(LED_GREEN_PIN, onlineStatus);
#endif
#if USE_IO1 == 1
        digitalWrite(IO1, IO1Status);
#endif
#if USE_IO2 == 1
        digitalWrite(IO2, IO2Status);
#endif
#if USE_IO3 == 1
        digitalWrite(IO3, IO3Status);
#endif
#if USE_IO4 == 1
        digitalWrite(IO4, IO4Status);
#endif
        /*Todo...
            Setting ON or OFF with openTime and closeTime
        */
        setDimLevel(dimLevel);
        controlReday = 0;
    }


}

