#ifndef _UTILS_h
#define _UTILS_h
#include <Arduino.h>
#include <driver/rtc_io.h>
#include <driver/adc.h>
#include <esp_adc_cal.h>

esp_err_t io_num_get_adc1(gpio_num_t gpio_num, adc1_channel_t *channel);
esp_err_t io_num_get_adc_channel(gpio_num_t gpio_num, adc_channel_t *channel);
esp_err_t io_num_get_adc_unit(gpio_num_t gpio_num, adc_unit_t *unit);

void check_efuse();
void print_char_val_type(esp_adc_cal_value_t val_type);
esp_sleep_wakeup_cause_t print_wakeup_reason();

#endif