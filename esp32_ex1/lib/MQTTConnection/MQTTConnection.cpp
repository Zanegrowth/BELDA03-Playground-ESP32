#include "MQTTConnection.h"

MQTTConnection::MQTTConnection()
{

}

MQTTConnection::~MQTTConnection()
{
    delete clistener;
}

void MQTTConnection::onMqttMessage(char *topic, byte *payload, unsigned int length)
{
    payload[length] = '\0';

    if (clistener != NULL)
    {
        clistener->onSubMessage((char *)payload);
    }
    if (slistener != NULL)
    {
        slistener->onSubMessage((char *)payload);
    }

    // String topic_str = topic;
    // String payload_str = (char *)payload;
    // Serial.println("[" + topic_str + "]: " + payload_str);
}

void MQTTConnection::init()
{
    wifiClient.setInsecure();
    client.setClient(wifiClient);
    client.setServer(MQTT_HOST, MQTT_PORT);
    client.setBufferSize(1024);
    // client.setCallback(onMqttMessage);
    // https://blog.mbedded.ninja/programming/languages/c-plus-plus/callbacks/ --> Callbacks function of cpp
    client.setCallback([this](char *topic, byte *payload, unsigned int length) -> void
                       { return onMqttMessage(topic, payload, length); });
  
    // ([](char *topic, byte *payload, unsigned int length)
    // { 
    //     Serial.println("message incomming"); 
    // });
}

void MQTTConnection::setMQTTConnectionListener(TypeModule type, MQTTConnectionListenner &listener)
{
    if(type == CONTROLLERS_TYPE)
            this->clistener = &listener;
    else if(type == SENSORS_TYPE)
            this->slistener = &listener;
}

void MQTTConnection::setClientId(String clientId)
{
    mqttClientId = clientId;
}

bool MQTTConnection::isMqttConnected()
{
    return client.connected();
}



void MQTTConnection::publishToCloud(const char *topic, const char *payload)
{
    if (client.connected())
    {
#if USE_PUBISH_MQTT == 1
        client.publish(topic, payload);
        Serial.print("Publish : ");
        Serial.println(topic);
        Serial.println(payload);
#endif
    }
}

// const char *PAYLOAD_TEST = "{\"dts\":1650946232574,\"seq\":1,\"status\":1,\"airtemp\":25,\"airhumd\":100,\"lightIntensity\":1000,\"moisture\":100,\"voltageValue\":4.5,\"currentValue\":1,\"waterValue\":0,\"fertilizerValue\":0,\"IO1Status\":1,\"IO2Status\":1,\"IO3Status\":1,\"IO4Status\":1,\"dimLevel\":100,\"closeTime\":1650946232574,\"openTime\":1650946232574,\"errorCode\":1}";
// const char *PAYLOAD_TEST = "{\"dts\":1650946232574,\"seq\":1,\"status\":1,\"airtemp\":25,\"airhumd\":100,\"lightIntensity\":1000,\"moisture\":100,\"voltageValue\":4.5,\"currentValue\":1,\"waterValue\":0,\"fertilizerValue\":0,\"IO1Status\":1}";


void MQTTConnection::run()
{

    if (client.connected())
    {
        client.loop();
    }else{
        if ( net.isInternetAvailable() && net.isTimeSync())
        {
            Serial.println("MQTT Connecting...");
            client.disconnect();

            if (client.connect(MQTT_CLIENTID, MQTT_USER, MQTT_PASS))
            {
#if USE_SUBSCIBE_MQTT == 1
                client.subscribe(MQTT_SERVER_TOPIC);
#endif
                // client.subscribe("smartfarm/#");
                net.onNetworkEvent(MQTT_HUB_CONNECTED);
            }
            else // Disconnected
            {
                // Serial.println("Disconnected");
                net.checkInternet();
                net.onNetworkEvent(MQTT_HUB_DISCONNECTED);
                
            }
        }else{
            if (!net.isInternetAvailable())
            {
                net.checkInternet();
            }
          
        }
    }
}

MQTTConnection mqtt;

    /* End MQTT section */