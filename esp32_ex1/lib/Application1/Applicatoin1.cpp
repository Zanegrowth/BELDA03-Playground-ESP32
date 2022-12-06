#include "Application1.h"

Application1::Application1()
{

}

Application1::~Application1()
{
}

void Application1::init(void)
{
    //IO pins setup
    pinMode(LED_RED_PIN, OUTPUT);
    pinMode(LED_YELLOW_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_YELLOW_PIN, LOW);


    //Serial setup
    Serial.begin(9600);
    delay(1000);
    Serial.println("\n\rESP Start");

    /*Network setup*/
    net.setNetworkListener(*this); // Set callback function for Network events.
    net.setWifiConfig(WIFI_SSID, WIFI_PASS);
    net.init();
   
    /*MQTT setup*/
    // mqtt.init();


    delay(10000);
    /*Controllers setup*/
    controllers.init();

    //Senssors setup
    // sensors.init();

    //Stanby...
   

}


void Application1::run()
{
    // mqtt.run();
    controllers.run();
    // sensors.run();
    // uc.run();
    // Serial.println("Hello ESP32");
    // digitalWrite(LED_IO1, HIGH);
    // delay(1000);
    // digitalWrite(LED_IO1, LOW);
    // delay(2000);
    // vTaskDelay(ms(2000));
}

/* Network Listener */
void Application1::onNetworkEvent(NetworkEvent event)
{
    switch (event)
    {
    case WIFI_CONNECTED:
        {
            // board->ledAllOff();
            // board->ledPinkOn();
            Serial.println("Wifi Connected");
            digitalWrite(LED_YELLOW_PIN, HIGH);
        }
        break;
    case WIFI_DISCONNECTED:
        {
            // board->ledAllOff();
            // board->startLedPinkBlink();
            Serial.println("Wifi Disconnected");
            digitalWrite(LED_YELLOW_PIN, LOW);
        }
        break;
    case INTERNET_CONNECTED:
        {
            // if (!net->isMqttConnected())
            // {
            //     board->offLedAll();
            //     board->onLedGreen();
            //     Serial.println("Internet");
            // }
        }
        break;
    case MQTT_HUB_CONNECTED:
        {
            Serial.println("MQTT Connected");
        }
        break;
    case MQTT_HUB_DISCONNECTED:
       {
           Serial.println("MQTT Disconnected");
       }
       break;
    case TIME_SYNCED:
        {
            Serial.println("Time synchronize already");
        }
        break;
    }
}
/* End Network Listener */

Application1 app1;
