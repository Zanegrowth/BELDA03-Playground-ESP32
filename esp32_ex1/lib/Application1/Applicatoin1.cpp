#include "Application1.h"

#include <ArduinoJson.h>

// #include <FastCRC.h>

#define LED_IO  2
#define RXD2_IO 23 
#define TXD2_IO 18 

const char *TOKEN = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6IjYyOGNhZWRkMTgxODU5MDAxMjg4MjIzZCIsInJvbGUiOjUwLCJpYXQiOjE2NTMzODY5NzN9.6quw2KM9YWYshU1i5dJa3K-UbUJAkjZj6kIWdUv7uRY";
const char *API_URL = "https://api.aidery.io/data-log";

// const char *WIFI_SSID = "Zanegrowth";
// const char *WIFI_PASS = "12345678";
// const char *PAYLOAD_TEST = "{\"id\":\"TNB000011\",\"type\":380001,\"data\":{\"dts\":1650946232574,\"seq\":1,\"status\":1,\"fwVersion\":\"1.0.1\",\"fusionBoard\":{\"airQuality8Click\":{\"aqi\":0}}}}";


Application1::Application1()
{
    // uc.init();
}

Application1::~Application1()
{
}

void Application1::init(void)
{
    pinMode(LED_IO, OUTPUT);
    digitalWrite(LED_IO, LOW);
    Serial.begin(9600);
    Serial2.begin(9600, SERIAL_8N1, RXD2_IO, TXD2_IO);
    delay(1000);
    Serial.println("\n\rESP Start");
    Serial.println("\n\rHello Nipon2");
    uc.setConnection(Serial2);

    uc.setListener(*this);

    // delay(1000);
    // net.setWifiConfig(WIFI_SSID, WIFI_PASS);
    // net.initWiFi();
    net.init();
    net.setNetworkListener(*this);
    net.setHttpAuthHeader("Authorization", "Bearer " + String(TOKEN));

    delay(10000);

}


void Application1::run()
{
    uc.run();
    // digitalWrite(LED_IO, HIGH);
    // delay(5000);
    // digitalWrite(LED_IO, LOW);
    // delay(1000);
    vTaskDelay(ms(100));
}

/* Network Listener */
void Application1::onNetworkEvent(NetworkEvent event)
{
    switch (event)
    {
    case WIFI_CONNECTED:
        // board->ledAllOff();
        // board->ledPinkOn();
        Serial.println("Wifi Connected");
        digitalWrite(LED_IO, HIGH);
        break;
    case WIFI_DISCONNECTED:
        // board->ledAllOff();
        // board->startLedPinkBlink();
        Serial.println("Wifi Disconnected");
        digitalWrite(LED_IO, LOW);
        break;
    case INTERNET_CONNECTED:
        // if (!net->isMqttConnected())
        // {
        //     board->offLedAll();
        //     board->onLedGreen();
        //     Serial.println("Internet");
        // }
        break;
    }
}

void Application1::onHttpResponse(const String &url, int statusCode, const String &responseText)
{
    Serial.print("Http response ");
    Serial.println(statusCode);
    // Serial.print(responseText);
    if (statusCode == HTTP_CODE_OK || statusCode == HTTP_CODE_CREATED)
    {
        Serial.print(responseText);
        digitalWrite(LED_IO, !digitalRead(LED_IO));
        // handleCheckUpdate(responseText);
    }
}

void Application1::onData(SensorPacket &msg)
{
    Serial.print(msg.payload);
    Serial.println();
    String url = String(API_URL);
    String payload = String(msg.payload);
    net.httpPost(url, payload);
}

void Application1::onWifiConfig(SSIDPacket &msg)
{
    Serial.print("ssid:: ");
    Serial.println(msg.ssid);
    Serial.print("pass:: ");
    Serial.println(msg.pass);

    net.setWifiConfig(msg.ssid, msg.pass);
    net.initWiFi();
}

    /* End Network Listener */

Application1 app;
