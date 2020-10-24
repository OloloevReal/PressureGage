#ifndef _SENSOR_h
#define _SENSOR_h

#include "Arduino.h"
#include <esp32/ulp.h>
#include <esp_adc_cal.h>
#include "setup.h"
#include "utils.h"

//#define MPXH6400A
//#define MPXH6400A_ULP
//#define S_EMUL


typedef struct {
    float volt;
    float pa;
    float bar;
    void clear(){
        bar = pa = volt = 0.0;
    }
} sensor_value_t;

typedef struct{
    float _zero_level;
    uint32_t volt_offset;
} sensor_params_t;

class Sensor_P
{
    public:
        Sensor_P(gpio_num_t gpio_num);
        ~Sensor_P();
        virtual esp_err_t begin() = 0;
        float getZeroLevel();
        void setZeroLevel(float value);
        void setZeroLevel(String value);
        void setZeroLevel(const char* value);
        void setVoltOffset(String value);
        void setVoltOffset(const char* value);
        void setVoltOffset(uint32_t value);
        virtual const sensor_value_t* getValue() = 0;
        sensor_params_t getParams();
        void restore();

    protected:
        portMUX_TYPE mutex = portMUX_INITIALIZER_UNLOCKED;
        sensor_params_t *_sensor_params;
        sensor_value_t *_sensor_value;
};


#if defined(MPXH6400A)
    #include "sensors\s_MPXH6400A.h"
    typedef Sensor_MPXH6400A Sensor;
#elif defined(MPXH6400A_ULP)
    #include "ulp.h"
    #include "sensors\s_MPXH6400A_ulp.h"
    typedef Sensor_MPXH6400A_ulp Sensor;
#elif defined(S_EMUL)
    #include "sensors\s_EMUL.h"
    typedef Sensor_EMUL Sensor;
#else
  #error "Please define MAP model"
#endif

#endif