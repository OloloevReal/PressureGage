#ifndef _S_EMUL_h
#define _S_EMUL_h

class Sensor_EMUL: public Sensor_P
{
public:
    Sensor_EMUL(gpio_num_t gpio_num):Sensor_P(gpio_num){}
    ~Sensor_EMUL(){}
        virtual esp_err_t begin(){
        return ESP_OK;
    };
    virtual const sensor_value_t* getValue(){
        Sensor_P::_sensor_value->clear();
        Sensor_P::_sensor_value->volt = random(1000, 1400)/1000.0;
        Sensor_P::_sensor_value->pa = (Sensor_P::_sensor_value->volt*1000-0.2)/SENS;
        Sensor_P::_sensor_value->bar = (Sensor_P::_sensor_value->pa - Sensor_P::_sensor_params->_zero_level)/100;
        return Sensor_P::_sensor_value;
    }
};

#endif