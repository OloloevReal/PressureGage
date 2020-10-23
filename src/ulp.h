#ifndef _ULP_h
#define _ULP_h

#include <esp32/ulp.h>
#include <Arduino.h>
#include <driver/rtc_io.h>
#include <driver/gpio.h>
#include <driver/adc.h>
#include "setup.h"
#include "utils.h"

#define I_CLEAR_R() \
  I_MOVI(R0, 0), \
  I_MOVI(R1, 0), \
  I_MOVI(R2, 0), \
  I_MOVI(R3, 0)


void ulp_clear_memory(){
        for (int i = RTC_MEM_PROG_START; i < RTC_MEM_BUFFER_TOP; ++i)
      RTC_SLOW_MEM[i] = 0x0000;
}

esp_err_t ulp_init(gpio_num_t gpio_num){

    adc_unit_t _unit;
    adc_channel_t _channel;

    io_num_get_adc_unit(gpio_num, &_unit);
    io_num_get_adc_channel(gpio_num, &_channel);

    ulp_set_wakeup_period(0, ULP_WAKEUP_PARIOD); //100 msec

    const ulp_insn_t program[] = {
        I_CLEAR_R(),
        M_LABEL(1),
            I_ADC(R1, (uint32_t)_unit-1, (uint32_t)_channel), // R1 <- ADC{uni_unit-1}_CH{_channel}
            I_ADDR(R2, R2, R1),      // R2 += R1                           
            I_ADDI(R0, R0, 1),       // R0 ++
        M_BL(1, 16),                 // if (R0 < 16) goto M_LABEL(1)
        I_RSHI(R1, R2, 4),           // R1 = R2 / 16 (average value)
        I_MOVI(R0, RTC_MEM_ADC_CH0), // R0 <- @RTC_MEM_ADC_CH0
        I_ST(R1, R0, 0),             // R1 -> RTC_SLOW_MEM[RTC_MEM_ADC_CH0]
        
        M_LABEL(9),
            I_HALT()
    };

    size_t size = sizeof(program) / sizeof(ulp_insn_t);
    if (ulp_process_macros_and_load(RTC_MEM_PROG_START, program, &size) != ESP_OK) {
        Serial.println(F("Error loading ULP code!"));
        return ESP_FAIL;
    }

    if (ulp_run(RTC_MEM_PROG_START) != ESP_OK) {
        Serial.println(F("Error running ULP code!"));
        return ESP_FAIL;
    }

    Serial.println(F("ULP run!"));
    return ESP_OK;
}

#endif