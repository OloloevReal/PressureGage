#include "sensor.h"

Sensor_P::Sensor_P(gpio_num_t gpio_num){
    _sensor_value = new sensor_value_t;
    _sensor_value->clear();

    _sensor_params = new sensor_params_t;
    _sensor_params->volt_offset = SHIFT_VOLT;
    _sensor_params->_zero_level = PRESSURE_N;
}

Sensor_P::~Sensor_P(){
    delete _sensor_value;
    delete _sensor_params;
}

float Sensor_P::getZeroLevel(){
    return _sensor_params->_zero_level;
}

void Sensor_P::setZeroLevel(String value){
    setZeroLevel(value.c_str());
}

void Sensor_P::setZeroLevel(const char* value){
    setZeroLevel(atof(value));
}

void Sensor_P::setZeroLevel(float value){
    portENTER_CRITICAL(&mutex);
    _sensor_params->_zero_level = value;
    portEXIT_CRITICAL(&mutex);
}

void Sensor_P::setVoltOffset(String value){
    setVoltOffset(value.c_str());
}

void Sensor_P::setVoltOffset(const char* value){
    setVoltOffset(atoi(value));
}

void Sensor_P::setVoltOffset(uint32_t value){
    portENTER_CRITICAL(&mutex);
    _sensor_params->volt_offset = value;
    portEXIT_CRITICAL(&mutex);
}

sensor_params_t Sensor_P::getParams(){
    return *_sensor_params;
}

void Sensor_P::restore(){
    portENTER_CRITICAL(&mutex);
    _sensor_params->volt_offset = SHIFT_VOLT;
    _sensor_params->_zero_level = PRESSURE_N;
    portEXIT_CRITICAL(&mutex);
}

