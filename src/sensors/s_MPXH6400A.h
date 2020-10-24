#ifndef _S_MPXH6400A_h
#define _S_MPXH6400A_h

class Sensor_MPXH6400A: public Sensor_P
{
public:
    Sensor_MPXH6400A(gpio_num_t gpio_num):Sensor_P(gpio_num){
        _no_of_samples = 32;
        initADC(gpio_num);
    }
    ~Sensor_MPXH6400A(){
        delete _adc_chars;
    }
    virtual esp_err_t begin(){
        return ESP_OK;
    };
    virtual const sensor_value_t* getValue(){
        calculateValue();
        return Sensor_P::_sensor_value;
    };

private:
    adc_unit_t _unit;
    adc_channel_t _channel;
    adc_atten_t _atten;
    adc_bits_width_t _width;
    esp_adc_cal_characteristics_t *_adc_chars;
    int _no_of_samples;

    void initADC(gpio_num_t gpio_num){
        io_num_get_adc_unit(gpio_num, &_unit);
        io_num_get_adc_channel(gpio_num, &_channel);
        _atten = ADC_ATTEN_DB_11;
        //_atten = ADC_ATTEN_DB_6;
        //_atten = ADC_ATTEN_DB_2_5; // max 1.5v
        _width = ADC_WIDTH_BIT_12;
        _adc_chars = (esp_adc_cal_characteristics_t *)calloc(1, sizeof(esp_adc_cal_characteristics_t));
        esp_adc_cal_characterize(_unit, _atten, _width, DEFAULT_VREF, _adc_chars);

        gpio_set_direction(gpio_num, GPIO_MODE_INPUT);
        if (_unit == ADC_UNIT_1) {
            adc1_config_channel_atten((adc1_channel_t)_channel, _atten);
            adc1_config_width(_width);
        }else{
            adc2_config_channel_atten((adc2_channel_t)_channel, _atten);
        }
        gpio_pullup_dis(gpio_num);
        //gpio_pulldown_dis(gpio_num);
        gpio_pulldown_en(gpio_num);
    };

    uint32_t getADCValue(){
        uint32_t adc_reading = 0;
        for (int i = 0; i < _no_of_samples; i++) {
            if (_unit == ADC_UNIT_1) {
                adc_reading += adc1_get_raw((adc1_channel_t)_channel);
            } else {
                int raw;
                adc2_get_raw((adc2_channel_t)_channel, _width, &raw);
                adc_reading += raw;
            }
        }
        adc_reading /= _no_of_samples;
        return adc_reading;
    };

    void calculateValue(){
        Sensor_P::_sensor_value->clear();
        uint32_t voltage = esp_adc_cal_raw_to_voltage(getADCValue() - Sensor_P::_sensor_params->volt_offset, _adc_chars);
        Sensor_P::_sensor_value->volt = voltage/1000.0;
        Sensor_P::_sensor_value->volt *= SCALE;
        #ifdef ALG_V1
            Sensor_P::_sensor_value->pa = (Sensor_P::_sensor_value->volt + (VS*K2))/(VS*K1);
        #elif ALG_V2
            Sensor_P::_sensor_value->pa = (Sensor_P::_sensor_value->volt*1000-0.2)/SENS;
        #endif
            Sensor_P::_sensor_value->bar = (Sensor_P::_sensor_value->pa - Sensor_P::_sensor_params->_zero_level)/100;
    }
};

#endif