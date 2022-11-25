#ifndef PACKET_H
#define PACKET_H

#include <Arduino.h>
#include <ArduinoJson.h>

#define AIR_QUALITY_8CLICK_ID 0x01
#define ENVIRONMENT_3CLICK_ID 0x02
#define HEART_RATE_10CLICK_ID 0x03
#define OXIMETER_2CLICK_ID 0x04
#define PROXIMITY_15CLICK_ID 0x05
#define IRHTERMO_CLICK_ID 0x06
#define MICROWAVE_2CLICK_ID 0x07
#define HALL_CURRENT_4CLICK_ID 0x08
#define LOAD_CELL_5CLICK_ID 0x09
#define VIBRA_SENSE_CLICK_ID 0x0A
#define IR_GESTURE_CLICK_ID 0x0B
#define EARTHQUAKE_CLICK 0x0C
#define DOF_13CLICK_ID 0x0D

#define MESSAGE_MODE 0x01
#define SENSOR_MODE 0x02

#define SSID_TYPE 0x01
#define DEVICE_ID_TYPE 0x02
#define MAX_SENSORS 3

#define MAX_SENSORS 3

typedef struct
{
    /* data */
    uint8_t id;
    uint16_t temp; // temp * 100
    uint16_t humd; // humd * 100
    uint16_t pressure;
    uint16_t gasRes;
    uint16_t accel_x;
    uint16_t accel_y;
    uint16_t accel_z;
    uint16_t gyro_x;
    uint16_t gyro_y;
    uint16_t gyro_z;
    uint16_t mag_x;
    uint16_t mag_y;
    uint16_t mag_z;
} __attribute__((packed)) dof_13click;

typedef struct
{
    /* data */
    uint8_t id;
    uint8_t aqi;
    uint16_t oZone;
    uint16_t humd;
} __attribute__((packed)) air_quality_8click_t;

typedef struct
{
    /* data */
    uint8_t id;
    uint16_t temp; // temp * 100
    uint16_t humd; // humd * 100
    uint16_t pressure;
    uint16_t gasRes;
} __attribute__((packed)) environment_3click_t;

typedef struct
{
    /* data */
    uint8_t id;
    uint16_t ir;
    uint16_t red;
    uint16_t green;
    uint16_t blue;

} __attribute__((packed)) heart_rate_10click_t;

typedef struct
{
    /* data */
    uint8_t id;
    uint8_t pulse;
} __attribute__((packed)) oximeter_2click_t;

typedef struct
{
    /* data */
    uint8_t id;
    uint16_t distance;
} __attribute__((packed)) proximity_15click_t;

typedef struct
{
    /* data */
    uint8_t id;
    uint16_t ambient_temp; // temp * 100
    uint16_t object_temp;  // temp * 100
} __attribute__((packed)) irthermo_click_t;

typedef struct
{
    /* data */
    uint8_t id;
    uint8_t motion_detection;

} __attribute__((packed)) microwave_2click_t;

typedef struct
{
    /* data */
    uint8_t id;
    uint16_t current;
} __attribute__((packed)) hall_carrent_4click_t;

typedef struct
{
    /* data */
    uint8_t id;
    uint16_t weight;
} __attribute__((packed)) load_cell_5click_t;

typedef struct
{
    /* data */
    uint8_t id;
    uint8_t status;
} vibra_sense_click_t;

typedef struct
{
    /* data */
    uint8_t id;
    uint8_t gesture;
} ir_gesture_click_t;

typedef struct
{
    /* data */
    uint8_t id;
    uint16_t max_SI;
    uint16_t max_PGA;
} earthquake_click_t;

struct SensorPacket
{
    String device_id;
    String payload;
};

struct SSIDPacket
{
    String ssid;
    String pass;
};

class Packet
{

private:
    Packet();

public:
    ~Packet();
    // void parse(const char *data);
    static void parse(SSIDPacket &msg, const char *data);
    static void parse(String &msg, const char *data);
    static void parse(SensorPacket &msg, const char *data);

};

#endif