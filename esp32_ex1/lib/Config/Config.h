#ifndef Config_H
#define Config_H

/*
V2.2
nipon@zanegrowth.com
*/

/*IO pin config*/
#define LED_GREEN_PIN        32
#define LED_YELLOW_PIN       33
#define LED_RED_PIN          25
#define SDA_PIN              21
#define SCL_PIN              22
#define DHT_PIN              4 // Digital pin connected to the DHT sensor
#define SOIL_RESET_PIN       23
#define CURRENT_PIN          34

#define VOLTAGE_VALUE        3.3

/*WiFi config*/
#define WIFI_SSID           "Zanegrowth1"
#define WIFI_PASS           "Zanegrowth15133"

/*RESTFul API config*/
#define TOKEN               "YOUR-TOKEN"
#define API_URL             "https://api.aidery.io/data-log"

#define DEVICE_ID           "SMFBELDA03"
#define DEVICE_TYPE_NUMBER  "390001"

/*MQTT config*/
#define MQTT_HOST           "mqtt.thailand-smartliving.com"
#define MQTT_USER           "belda"
#define MQTT_PASS           "belda03"
#define MQTT_PORT           8883
#define MQTT_CLIENTID       "belda3"

#define MQTT_DEVICE_TOPIC   "smartfarm/belda/device/" DEVICE_ID 
#define MQTT_SERVER_TOPIC   "smartfarm/belda/server/" DEVICE_ID
#define MQTT_SUB_TOPIC      "smartfarm/#"



#endif