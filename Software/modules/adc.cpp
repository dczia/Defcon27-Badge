/**
 *
 * Handler for the ADC
 *
 * @date 6/21/2019
 * @author hamster
 *
 */

#include "common.h"
#include "adc.h"

#define BATTERY_LEVEL_MEAS_INTERVAL     APP_TIMER_TICKS(1500)   /**< Battery level measurement interval (ticks). This value corresponds to 1500ms */
#define ADC_REF_VOLTAGE_IN_MILLIVOLTS   600                     /**< Reference voltage (in milli volts) used by ADC while doing conversion. */
#define ADC_PRE_SCALING_COMPENSATION    6                       /**< The ADC is configured to use VDD with 1/3 prescaling as input. And hence the result of conversion is to be multiplied by 3 to get the actual value of the battery voltage.*/
#define ADC_RES_10BIT                   1024                    /**< Maximum digital value for 10-bit ADC conversion. */
#define MULTIPLIER                      1.403                   /**< Multiplier for this board */

#define ADC_RESULT_IN_MILLI_VOLTS(ADC_VALUE)\
(((((ADC_VALUE) * ADC_REF_VOLTAGE_IN_MILLIVOLTS) / ADC_RES_10BIT) * ADC_PRE_SCALING_COMPENSATION) * MULTIPLIER)

APP_TIMER_DEF(m_battery_timer_id);                      /**< Battery measurement timer. */

nrf_saadc_value_t adc_result = 0;

/**
 * Handler for when the ADC has a result
 * @param p_event
 */
static void saadc_event_handler(nrf_drv_saadc_evt_t const *p_event) {
    if (p_event->type == NRF_DRV_SAADC_EVT_DONE) {

        uint32_t err_code;

        adc_result = p_event->data.done.p_buffer[0];

        err_code = nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, 1);
        APP_ERROR_CHECK(err_code);

    }
}

/**
 * Configure the ADC and start the timers
 */
ADC::ADC(){
    adc_buf[0] = 0;
    adc_buf[1] = 0;

    ret_code_t err_code = nrf_drv_saadc_init(nullptr, saadc_event_handler);
    APP_ERROR_CHECK(err_code);

    nrf_saadc_channel_config_t config =
                    NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(VOLTAGE_SENSE_PIN);
    err_code = nrf_drv_saadc_channel_init(0, &config);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_buffer_convert(&adc_buf[0], 1);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_buffer_convert(&adc_buf[1], 1);
    APP_ERROR_CHECK(err_code);

    app_timer_create(&m_battery_timer_id, APP_TIMER_MODE_REPEATED, battery_level_meas_timeout_handler);

    // Start the timer
    app_timer_start(m_battery_timer_id, BATTERY_LEVEL_MEAS_INTERVAL, nullptr);
}

/**
 * Called automatically when the timer times out for battery measurement
 * @param p_context
 */
void ADC::battery_level_meas_timeout_handler(void *p_context) {
    UNUSED_PARAMETER(p_context);

    ret_code_t err_code;
    err_code = nrf_drv_saadc_sample();
    APP_ERROR_CHECK(err_code);
}

/**
 * @return our percentage calculation
 */
uint8_t ADC::getBatteryPercent(){
    return calcBatteryPercent(ADC_RESULT_IN_MILLI_VOLTS(adc_result));
}

/**
 * @return battery voltage in milliamps
 */
uint16_t ADC::getBatteryVoltage(){
    return ADC_RESULT_IN_MILLI_VOLTS(adc_result);
}


/**
 * Determine battery level based on measured millivolts
 * @param millivolts
 * @return in quarters of battery level
 *
 * Special case is when the system is on USB power, 255 is returned instead
 */
uint8_t ADC::calcBatteryPercent(uint16_t millivolts){

    // New AAAs are about 4550mV
    if(millivolts > 4700){
        // Charging
        return 255;
    }
    if(millivolts > 4100){
        return 100;
    }
    if(millivolts > 3800){
        return 75;
    }
    if(millivolts > 3500){
        return 50;
    }
    if(millivolts > 3300){
        return 25;
    }
    if(millivolts > 3000) {
        return 1;
    }
    return 0;

}

