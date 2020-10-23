#ifndef _SENSOR_h
#define _SENSOR_h

#include "Arduino.h"
#include <esp32/ulp.h>
#include <esp_adc_cal.h>
#include "setup.h"
#include "utils.h"

#define MPXH6400A

#if defined(MPXH6400A)
#else
  #error "Please define MAP model"
#endif

typedef struct {
    float volt;
    float pa;
    float bar;
    void clear();
} sensor_value_t;

typedef struct{
    float _calibrated_pressure;
    uint32_t volt_offset;
} sensor_params_t;

class Sensor_P
{
    public:
        Sensor_P(gpio_num_t gpio_num);
        ~Sensor_P();
        float getZeroLevel();
        void setZeroLevel(float value);
        void setZeroLevel(String value);
        void setZeroLevel(const char* value);
        void setVoltOffset(String value);
        void setVoltOffset(const char* value);
        void setVoltOffset(uint32_t value);
        const sensor_value_t* getValue();
        sensor_params_t getParams();
        
        void restore();

    private:
        portMUX_TYPE mutex = portMUX_INITIALIZER_UNLOCKED;
        adc_unit_t _unit;
        adc_channel_t _channel;
        adc_atten_t _atten;
        adc_bits_width_t _width;
        int _no_of_samples;
        esp_adc_cal_characteristics_t *_adc_chars;
        sensor_params_t *_sensor_params;
        sensor_value_t *_sensor_value;
        void initADC(gpio_num_t gpio_num);
        uint32_t getADCValue();
        void calculateValue();
};

#endif