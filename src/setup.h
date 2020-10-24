#ifndef _SETUP_h
#define _SETUP_h

#include <Arduino.h>

#define FIRMWARE_VERSION "0.0.2"

//#define MPXH6400A
#define MPXH6400A_ULP
//#define S_EMUL

#define LED GPIO_NUM_2

#define AP_NAME         "ESP_PRESSURE"
#define DEFAULT_VREF    1114
#define SHIFT_VOLT      16   // mV
#define PRESSURE_N      100.0 // 750 mm
//#define PRESSURE_N 101.325 // 760 mm

#define DIVIDER

#define ALG_V1
//#define ALG_V2
#define SENS 12.1
#define VS 5.04
#define K1 0.002421
#define K2 0.00842


#ifdef DIVIDER
    #define SCALE 1
    #define PIN_ADC GPIO_NUM_33
#else
    #define SCALE 2
    #define PIN_ADC GPIO_NUM_32
#endif




#endif