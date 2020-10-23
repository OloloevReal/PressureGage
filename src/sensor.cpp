#include "sensor.h"

Sensor_P::Sensor_P(gpio_num_t gpio_num){
    _sensor_value = new sensor_value_t;
    _sensor_value->clear();

    _sensor_params = new sensor_params_t;
    _sensor_params->volt_offset = SHIFT_VOLT;
    _sensor_params->_calibrated_pressure = PRESSURE_N;

    initADC(gpio_num);
}

Sensor_P::~Sensor_P(){
    delete _adc_chars;
    delete _sensor_value;
    delete _sensor_params;
}

float Sensor_P::getZeroLevel(){
    return _sensor_params->_calibrated_pressure;
}

void Sensor_P::setZeroLevel(String value){
    setZeroLevel(value.c_str());
}
void Sensor_P::setZeroLevel(const char* value){
    setZeroLevel(atof(value));
}
void Sensor_P::setZeroLevel(float value){
    portENTER_CRITICAL(&mutex);
    _sensor_params->_calibrated_pressure = value;
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

uint32_t Sensor_P::getADCValue(){
    return (RTC_SLOW_MEM[RTC_MEM_ADC_CH0] & 0xFFFF);
}

const sensor_value_t* Sensor_P::getValue(){
    calculateValue();
    return _sensor_value;
}

sensor_params_t Sensor_P::getParams(){
    return *_sensor_params;
}

void Sensor_P::restore(){
    portENTER_CRITICAL(&mutex);
    _sensor_params->volt_offset = SHIFT_VOLT;
    _sensor_params->_calibrated_pressure = PRESSURE_N;
    portEXIT_CRITICAL(&mutex);
}

void Sensor_P::initADC(gpio_num_t gpio_num){
    io_num_get_adc_unit(PIN_ADC, &_unit);
    io_num_get_adc_channel(PIN_ADC, &_channel);
     _atten = ADC_ATTEN_DB_11;
    //_atten = ADC_ATTEN_DB_6;
    //_atten = ADC_ATTEN_DB_2_5; // max 1.5v
    _width = ADC_WIDTH_BIT_12;
    _adc_chars = (esp_adc_cal_characteristics_t *)calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(_unit, _atten, _width, DEFAULT_VREF, _adc_chars);

    gpio_set_direction(PIN_ADC, GPIO_MODE_INPUT);
    if (_unit == ADC_UNIT_1) {
      adc1_config_channel_atten((adc1_channel_t)_channel, _atten);
      adc1_config_width(_width);
    }else{
      adc2_config_channel_atten((adc2_channel_t)_channel, _atten);
    }
    gpio_pullup_dis(PIN_ADC);
    //gpio_pulldown_dis(PIN_ADC);
    gpio_pulldown_en(PIN_ADC);

    adc1_ulp_enable();
}

void sensor_value_t::clear(){
    bar = pa = volt = 0.0;
}

void Sensor_P::calculateValue(){
    _sensor_value->clear();
    uint32_t voltage = esp_adc_cal_raw_to_voltage(getADCValue() - _sensor_params->volt_offset, _adc_chars);
    _sensor_value->volt = voltage/1000.0;
    _sensor_value->volt *= SCALE;
#ifdef ALG_V1
    _sensor_value->pa = (_sensor_value->volt + (VS*K2))/(VS*K1);
#elif ALG_V2
    _sensor_value->pa = (_sensor_value->volt*1000-0.2)/SENS;
#endif
    _sensor_value->bar = (_sensor_value->pa - _sensor_params->_calibrated_pressure)/100;
}

