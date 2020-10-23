#include "utils.h"

esp_err_t io_num_get_adc1(gpio_num_t gpio_num, adc1_channel_t *channel)
{
  switch (gpio_num) { 
    case GPIO_NUM_36:
      *channel = ADC1_CHANNEL_0;
      break;
    case GPIO_NUM_37:
      *channel = ADC1_CHANNEL_1;
      break;
    case GPIO_NUM_38:
      *channel = ADC1_CHANNEL_2;
      break;
    case GPIO_NUM_39:
      *channel = ADC1_CHANNEL_3;
      break;
    case GPIO_NUM_32:
      *channel = ADC1_CHANNEL_4;
      break;
    case GPIO_NUM_33:
      *channel = ADC1_CHANNEL_5;
      break;
    case GPIO_NUM_34:
      *channel = ADC1_CHANNEL_6;
      break;
    case GPIO_NUM_35:
      *channel = ADC1_CHANNEL_7;
      break;
    default:
        return ESP_ERR_INVALID_ARG;
  }
  return ESP_OK;
}

esp_err_t io_num_get_adc_channel(gpio_num_t gpio_num, adc_channel_t *channel)
{
  switch (gpio_num) { 
    case GPIO_NUM_36:
      *channel = ADC_CHANNEL_0;
      break;
    case GPIO_NUM_37:
      *channel = ADC_CHANNEL_1;
      break;
    case GPIO_NUM_38:
      *channel = ADC_CHANNEL_2;
      break;
    case GPIO_NUM_39:
      *channel = ADC_CHANNEL_3;
      break;
    case GPIO_NUM_32:
      *channel = ADC_CHANNEL_4;
      break;
    case GPIO_NUM_33:
      *channel = ADC_CHANNEL_5;
      break;
    case GPIO_NUM_34:
      *channel = ADC_CHANNEL_6;
      break;
    case GPIO_NUM_35:
      *channel = ADC_CHANNEL_7;
      break;
    case GPIO_NUM_4:
      *channel = ADC_CHANNEL_0;
      break;
    case GPIO_NUM_0:
      *channel = ADC_CHANNEL_1;
      break;
    case GPIO_NUM_2:
      *channel = ADC_CHANNEL_2;
      break;
    case GPIO_NUM_15:
      *channel = ADC_CHANNEL_3;
      break;
    case GPIO_NUM_13:
      *channel = ADC_CHANNEL_4;
      break;
    case GPIO_NUM_12:
      *channel = ADC_CHANNEL_5;
      break;
    case GPIO_NUM_14:
      *channel = ADC_CHANNEL_6;
      break;
    case GPIO_NUM_27:
      *channel = ADC_CHANNEL_7;
      break;
    case GPIO_NUM_25:
      *channel = ADC_CHANNEL_8;
      break;
    case GPIO_NUM_26:
      *channel = ADC_CHANNEL_9;
      break;
    default:
        return ESP_ERR_INVALID_ARG;
  }
  return ESP_OK;
}

esp_err_t io_num_get_adc_unit(gpio_num_t gpio_num, adc_unit_t *unit)
{
  switch (gpio_num) { 
    case GPIO_NUM_36:
      *unit = ADC_UNIT_1;
      break;
    case GPIO_NUM_37:
      *unit = ADC_UNIT_1;
      break;
    case GPIO_NUM_38:
      *unit = ADC_UNIT_1;
      break;
    case GPIO_NUM_39:
      *unit = ADC_UNIT_1;
      break;
    case GPIO_NUM_32:
      *unit = ADC_UNIT_1;
      break;
    case GPIO_NUM_33:
      *unit = ADC_UNIT_1;
      break;
    case GPIO_NUM_34:
      *unit = ADC_UNIT_1;
      break;
    case GPIO_NUM_35:
      *unit = ADC_UNIT_1;
      break;
    case GPIO_NUM_4:
      *unit = ADC_UNIT_2;
      break;
    case GPIO_NUM_0:
      *unit = ADC_UNIT_2;
      break;
    case GPIO_NUM_2:
      *unit = ADC_UNIT_2;
      break;
    case GPIO_NUM_15:
      *unit = ADC_UNIT_2;
      break;
    case GPIO_NUM_13:
      *unit = ADC_UNIT_2;
      break;
    case GPIO_NUM_12:
      *unit = ADC_UNIT_2;
      break;
    case GPIO_NUM_14:
      *unit = ADC_UNIT_2;
      break;
    case GPIO_NUM_27:
      *unit = ADC_UNIT_2;
      break;
    case GPIO_NUM_25:
      *unit = ADC_UNIT_2;
      break;
    case GPIO_NUM_26:
      *unit = ADC_UNIT_2;
      break;
    default:
        return ESP_ERR_INVALID_ARG;
  }
  return ESP_OK;
}

void check_efuse()
{
    //Check TP is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
        Serial.println("eFuse Two Point: Supported");
    } else {
        Serial.println("eFuse Two Point: NOT supported");
    }
    //Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK) {
        Serial.println("eFuse Vref: Supported");
    } else {
        Serial.println("eFuse Vref: NOT supported");
    }
}
void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        Serial.println("Characterized using Two Point Value\n");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        Serial.println("Characterized using eFuse Vref");
    } else {
        Serial.println("Characterized using Default Vref");
    }
}


esp_sleep_wakeup_cause_t print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case 1 : Serial.println ("POWERON_RESET");break;          /**<1,  Vbat power on reset*/
    case 3 : Serial.println ("SW_RESET");break;               /**<3,  Software reset digital core*/
    case 4 : Serial.println ("OWDT_RESET");break;             /**<4,  Legacy watch dog reset digital core*/
    case 5 : Serial.println ("DEEPSLEEP_RESET");break;        /**<5,  Deep Sleep reset digital core*/
    case 6 : Serial.println ("SDIO_RESET");break;             /**<6,  Reset by SLC module, reset digital core*/
    case 7 : Serial.println ("TG0WDT_SYS_RESET");break;       /**<7,  Timer Group0 Watch dog reset digital core*/
    case 8 : Serial.println ("TG1WDT_SYS_RESET");break;       /**<8,  Timer Group1 Watch dog reset digital core*/
    // case 9 : Serial.println ("RTCWDT_SYS_RESET");break;       /**<9,  RTC Watch dog Reset digital core*/
    // case 10 : Serial.println ("INTRUSION_RESET");break;       /**<10, Instrusion tested to reset CPU*/
    // case 11 : Serial.println ("TGWDT_CPU_RESET");break;       /**<11, Time Group reset CPU*/
    // case 12 : Serial.println ("SW_CPU_RESET");break;          /**<12, Software reset CPU*/
    // case 13 : Serial.println ("RTCWDT_CPU_RESET");break;      /**<13, RTC Watch dog Reset CPU*/
    // case 14 : Serial.println ("EXT_CPU_RESET");break;         /**<14, for APP CPU, reseted by PRO CPU*/
    // case 15 : Serial.println ("RTCWDT_BROWN_OUT_RESET");break;/**<15, Reset when the vdd voltage is not stable*/
    // case 16 : Serial.println ("RTCWDT_RTC_RESET");break;      /**<16, RTC Watch dog reset digital core and rtc module*/
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
  return wakeup_reason;
}