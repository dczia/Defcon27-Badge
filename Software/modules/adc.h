/**
 *
 * Driver to handle the ADC to get battery level
 *
 * @date 6/21/2019
 * @author hamster
 *
 */

#ifndef MODULE_ADC_H
#define MODULE_ADC_H

#include "common.h"

class ADC {

    public:
        ADC();

        uint8_t getBatteryPercent();
        uint16_t getBatteryVoltage();

    private:

        static void battery_level_meas_timeout_handler(void *p_context);
        uint8_t calcBatteryPercent(uint16_t millivolts);
        nrf_saadc_value_t adc_buf[2];

};

#endif //MODULE_ADC_H
