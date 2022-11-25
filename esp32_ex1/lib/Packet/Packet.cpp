#include "Packet.h"
#include "Util.h"

Packet::Packet(/* args */)
{

}

Packet::~Packet()
{

}

// void Packet::parse(const char *data)
// {

void Packet::parse(SSIDPacket &msg, const char *data)
{
    uint8_t index = 3;
    uint8_t ssid_len = data[index++];
    char ssid_buf[ssid_len + 1];
    memcpy(ssid_buf, &data[index], ssid_len);
    ssid_buf[ssid_len] = '\0';
    msg.ssid = String(ssid_buf);
    Serial.println(msg.ssid);

    index += ssid_len;
    uint8_t pass_len = data[index++];
    char pass_buf[pass_len + 1];
    memcpy(pass_buf, &data[index], pass_len);
    pass_buf[pass_len] = '\0';
    msg.pass = String(pass_buf);
    Serial.println(msg.pass);
}


void Packet::parse(String &msg, const char *data)
{
    uint8_t index = 3;
    uint8_t len = data[index++];
    char buf[len + 1];
    memcpy(buf, &data[index], len);
    buf[len] = '\0';
    msg = String(buf);
    Serial.println(msg);
}

void Packet::parse(SensorPacket &msg, const char *data)
{
    static uint8_t seq = 1;
    uint16_t inx = 3;
    uint8_t number = data[2];
    const size_t capacity = 1024;
    DynamicJsonDocument doc(capacity);
    doc["id"] = msg.device_id;
    doc["type"] = "380001";
    JsonObject dataObj = doc.createNestedObject("data");
    dataObj["dts"] = currentTime();
    dataObj["seq"] = seq;
    dataObj["fwVersion"] = "1.0.1";
    JsonObject fusionBoard = dataObj.createNestedObject("fusionBoard");

    if (seq++ > 255)
        seq = 1;

    for (uint8_t i = 0; i < number; i++)
    {
        switch (data[inx])
        {
        case AIR_QUALITY_8CLICK_ID:
        {
            uint8_t len_sensor = sizeof(air_quality_8click_t);
            air_quality_8click_t air_data;
            memcpy(&air_data, &data[inx], len_sensor);

            JsonObject airq = fusionBoard.createNestedObject("airQuality8Click");
            airq["aqi"] = air_data.aqi;
            airq["oZone"] = air_data.oZone;
            airq["nitrogenDioxide"] = 0;

            inx += len_sensor;
        }
        break;
        case ENVIRONMENT_3CLICK_ID:
        {
            uint8_t len_sensor = sizeof(environment_3click_t);
            environment_3click_t env_data;
            memcpy(&env_data, &data[inx], len_sensor);

            JsonObject env = fusionBoard.createNestedObject("environment3Click");
            env["temp"] = (float)env_data.temp / 100.0;
            env["humd"] = (float)env_data.humd / 100.0;
            env["pressure"] = env_data.pressure;
            env["gasRes"] = env_data.gasRes;

            inx += len_sensor;
        }
        break;
        case HEART_RATE_10CLICK_ID:
        {
            uint8_t len_sensor = sizeof(heart_rate_10click_t);
            heart_rate_10click_t hr_data;
            memcpy(&hr_data, &data[inx], len_sensor);

            JsonObject hr = fusionBoard.createNestedObject("heartRate10Click");
            hr["ir"] = hr_data.ir;
            hr["red"] = hr_data.red;
            hr["green"] = hr_data.green;
            hr["blue"] = hr_data.blue;

            inx += len_sensor;
        }
        break;
        case OXIMETER_2CLICK_ID:
        {
            uint8_t len_sensor = sizeof(oximeter_2click_t);
            oximeter_2click_t oxi_data;
            memcpy(&oxi_data, &data[inx], len_sensor);

            JsonObject oxi = fusionBoard.createNestedObject("oximeter2Click");
            oxi["pulse"] = oxi_data.pulse;

            inx += len_sensor;
        }
        break;
        case PROXIMITY_15CLICK_ID:
        {
            uint8_t len_sensor = sizeof(proximity_15click_t);
            proximity_15click_t proximity_data;
            memcpy(&proximity_data, &data[inx], len_sensor);

            JsonObject prox = fusionBoard.createNestedObject("proximity15Click");
            prox["distance"] = proximity_data.distance;
            
            inx += len_sensor;
        }
        break;
        case IRHTERMO_CLICK_ID:
        {
            uint8_t len_sensor = sizeof(irthermo_click_t);
            irthermo_click_t irthermo_data;
            memcpy(&irthermo_data, &data[inx], len_sensor);

            JsonObject irt = fusionBoard.createNestedObject("irThermoClick");
            irt["ambientTemp"] = (float)irthermo_data.ambient_temp / 100.0;
            irt["objectTemp"] = (float)irthermo_data.object_temp / 100.0;

            inx += len_sensor;
        }
        break;
        case MICROWAVE_2CLICK_ID:
        {
            uint8_t len_sensor = sizeof(microwave_2click_t);
            microwave_2click_t microwave_data;
            memcpy(&microwave_data, &data[inx], len_sensor);

            JsonObject mic = fusionBoard.createNestedObject("microwave2Click");
            mic["motion_detection"] = microwave_data.motion_detection;

            inx += len_sensor;
        }
        break;
        case HALL_CURRENT_4CLICK_ID:
        {
            uint8_t len_sensor = sizeof(hall_carrent_4click_t);
            hall_carrent_4click_t hall_data;
            memcpy(&hall_data, &data[inx], len_sensor);

            JsonObject hall = fusionBoard.createNestedObject("hallCurrent4Click");
            hall["current"] = (float)hall_data.current / 100.0;

            inx += len_sensor;
        }
        break;
        case LOAD_CELL_5CLICK_ID:
        {
            uint8_t len_sensor = sizeof(load_cell_5click_t);
            load_cell_5click_t load_cell_data;
            memcpy(&load_cell_data, &data[inx], len_sensor);

            JsonObject load = fusionBoard.createNestedObject("loadCell5Click");
            load["weight"] = (float)load_cell_data.weight / 100.0;

            inx += len_sensor;
        }
        break;
        case VIBRA_SENSE_CLICK_ID:
        {
            uint8_t len_sensor = sizeof(vibra_sense_click_t);
            vibra_sense_click_t vibra_data;
            memcpy(&vibra_data, &data[inx], len_sensor);

            JsonObject vibra = doc.createNestedObject("vibraSenseClick");
            vibra["status"] = vibra_data.status;

            inx += len_sensor;
        }
        break;
        case IR_GESTURE_CLICK_ID:
        {
            uint8_t len_sensor = sizeof(ir_gesture_click_t);
            ir_gesture_click_t ir_data;
            memcpy(&ir_data, &data[inx], len_sensor);

            JsonObject irg = fusionBoard.createNestedObject("irGestureClick");
            irg["gesture"] = ir_data.gesture;

            inx += len_sensor;
        }
        break;

        case EARTHQUAKE_CLICK:
        {
            uint8_t len_sensor = sizeof(earthquake_click_t);
            earthquake_click_t eartquake_data;
            memcpy(&eartquake_data, &data[inx], len_sensor);

            JsonObject earth = fusionBoard.createNestedObject("earthquakeClick");
            earth["maxSI"] = eartquake_data.max_SI;
            earth["maxPGA"] = eartquake_data.max_PGA;

            inx += len_sensor;
        }
        break;
        case DOF_13CLICK_ID:
        {
            uint8_t len_sensor = sizeof(dof_13click);

            dof_13click dof_data;
            memcpy(&dof_data, &data[inx], len_sensor);

            JsonObject dof = fusionBoard.createNestedObject("13DofClick");
            dof["temp"] = (float)dof_data.temp / 100.0;
            dof["humd"] = (float)dof_data.humd / 100.0;
            dof["pressure"] = dof_data.pressure;
            dof["gasRes"] = dof_data.gasRes;
            dof["accel_x"] = (int16_t)dof_data.accel_x;
            dof["accel_y"] = (int16_t)dof_data.accel_y;
            dof["accel_z"] = (int16_t)dof_data.accel_z;
            dof["gyro_x"] = (int16_t)dof_data.gyro_x;
            dof["gyro_y"] = (int16_t)dof_data.gyro_y;
            dof["gyro_z"] = (int16_t)dof_data.gyro_z;
            dof["mag_x"] = (int16_t)dof_data.mag_x;
            dof["mag_y"] = (int16_t)dof_data.mag_y;
            dof["mag_z"] = (int16_t)dof_data.mag_z;

            inx += len_sensor;
        }
        break;

        default:
            break;
        }
    }

    serializeJson(doc, msg.payload);
    doc.clear();

    // Serial.print(msg.payload);
    // Serial.println();
}