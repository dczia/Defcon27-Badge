/**
 *
 * Audio driver
 *
 * @date 6/21/2019
 * @author hamster, rehr
 *
 */

#include "audio.h"
#include "common.h"

/**
 * Callback/handler for timer1 compare event
 * 
 * @param event_type Timer event
 * @param p_context User data, used as a pointer to the Audio instance
 */
void Audio::timer_event_handler(nrf_timer_event_t event_type, void* p_context) {
    static uint32_t i = 0;

    switch (event_type) {
        case NRF_TIMER_EVENT_COMPARE0:
            ((Audio *)p_context)->setPWMCh0Value(sineTable[i]);
            i++;
            if (i >= 32) {
                i = 0;
            }
            break;

        default:
            //Do nothing.
            break;
    }
}


/**
 * Create the object and initialize the port
 */
Audio::Audio(){
    enabled = false;
    headphones = false;

    // initPWM();

    initTimer();

    nrf_gpio_cfg_input(AUDIO_HP_DETECT_PIN, NRF_GPIO_PIN_NOPULL);

    nrf_gpio_cfg_output(AUDIO_SHUTDOWN_PIN);
    nrf_gpio_cfg_output(AUDIO_PWM_PIN);

    nrf_gpio_pin_write(AUDIO_SHUTDOWN_PIN, 1);
    nrf_gpio_pin_write(AUDIO_PWM_PIN, 0);
}

/**
 * Enable or disable the audio
 * @parameter enable Boolean, should audio be enabled?
 */
void Audio::enable(bool enable){
    if(enable){
        nrf_gpio_pin_write(AUDIO_SHUTDOWN_PIN, 0);
        enabled = true;
    }
    else{
        nrf_gpio_pin_write(AUDIO_SHUTDOWN_PIN, 1);
        enabled = false;
    }
}

/**
 * Is audio enabled?
 * @return true is enabled
 */
bool Audio::isEnabled(){
    return this->enabled;
}

/**
 * Are headphones plugged in?
 * @return true if inserted
 */
bool Audio::headphonesInserted(){

    // Low level when inserted
    headphones = !nrf_gpio_pin_read(AUDIO_HP_DETECT_PIN);
    return headphones;
}

/**
 * @brief Beeps the speaker for a duration at a certain frequency
 * @param duration How long to beep
 * @param frequency Tone freq in hz
 *
 * @note Busy waits, frequency might not be exact, might sound uneven if the softdevice needs to do BLE things
 */
void Audio::beep(int duration, int frequency){

    // Figure out how many beeps
    float period = 1000 / (float)frequency;
    long counter = period * duration;
    float delay = period / 2;

    for(long i = 0; i < counter; i++){
        nrf_gpio_pin_write(AUDIO_PWM_PIN, 1);
        nrf_delay_us(delay * 1000);
        nrf_gpio_pin_write(AUDIO_PWM_PIN, 0);
        nrf_delay_us(delay * 1000);
    }

}

/**
 * initialize the PWM0 module for audio generation
 * 
 * @return error code value from nrf_drv_pwm_init
 */
uint32_t Audio::initPWM() {
    pwm0 = NRF_DRV_PWM_INSTANCE(0); // PWM0
    pwm0_seq_values.channel_0 = 0; // start with off/0V
    pwm0_seq_values.channel_1 = 0; // start with off/0V
    pwm0_seq_values.channel_2 = 0; // start with off/0V
    pwm0_seq_values.channel_3 = 0; // start with off/0V
    pwm0_seq =
    {
        (nrf_pwm_values_t){ .p_individual = &pwm0_seq_values }, // values.p_individual
        NRF_PWM_VALUES_LENGTH(pwm0_seq_values), // length
        0, // repeats
        0 // end_delay
    };

    uint16_t pwm_top = 10000; // might make this a member var...
    
    nrf_drv_pwm_config_t const pwm0_config = {
        { // .output_pins
            LED_D_PIN | NRF_DRV_PWM_PIN_INVERTED, // channel 0
            LED_C_PIN | NRF_DRV_PWM_PIN_INVERTED, // channel 1
            NRF_DRV_PWM_PIN_NOT_USED,             // channel 2
            NRF_DRV_PWM_PIN_NOT_USED,             // channel 3
        },
        APP_IRQ_PRIORITY_LOW, // .irq_priority
        NRF_PWM_CLK_1MHz, // .base_clock
        NRF_PWM_MODE_UP, // .count_mode
        pwm_top, // .top_value
        NRF_PWM_LOAD_INDIVIDUAL, // .load_mode
        NRF_PWM_STEP_AUTO // .step_mode
    };

    return nrf_drv_pwm_init(&pwm0, &pwm0_config, NULL);
}

/**
 * set PWM0 channel 0 value
 * 
 * @param value New PWM0 channel 0 value between 0 and 1023
 */
void Audio::setPWMCh0Value(uint16_t value) {
    pwm0_seq_values.channel_0 = value;
    (void)nrf_drv_pwm_simple_playback(&pwm0, &pwm0_seq, 1, NRF_DRV_PWM_FLAG_LOOP);
}

/**
 * set PWM0 channel 1 value
 * 
 * @param value New PWM0 channel 1 value between 0 and 1023
 */
void Audio::setPWMCh1Value(uint16_t value) {
    pwm0_seq_values.channel_1 = value;
    (void)nrf_drv_pwm_simple_playback(&pwm0, &pwm0_seq, 1, NRF_DRV_PWM_FLAG_LOOP);
}

/**
 * initialize the Timer1 module for audio generation
 * 
 * @return error code value from nrf_drv_timer_init
 * 
 * @note Timer1 is not enabled yet. Call one of the setTimer functions to enable.
 */
uint32_t Audio::initTimer() {
    timer1 = NRF_DRV_TIMER_INSTANCE(1);

    nrf_drv_timer_config_t timer1_config = NRF_DRV_TIMER_DEFAULT_CONFIG;
    timer1_config.p_context = this;
    return nrf_drv_timer_init(&timer1, &timer1_config, timer_event_handler);
}

/**
 * set Timer1 period in milliseconds
 * 
 * @param period Timer1 period in milliseconds
 */
void Audio::setTimerWithPeriod_ms(uint32_t period) {
    if (nrf_drv_timer_is_enabled(&timer1)) {
        // disable timer while changing (not necessary, but doing it just in case)
        nrf_drv_timer_disable(&timer1);
    }

    uint32_t time_ticks = nrf_drv_timer_ms_to_ticks(&timer1, period);

    nrf_drv_timer_extended_compare(&timer1, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);

    nrf_drv_timer_enable(&timer1);
}

/**
 * set Timer1 period in microseconds
 * 
 * @param period Timer1 period in microseconds
 */
void Audio::setTimerWithPeriod_us(uint32_t period) {
    if (nrf_drv_timer_is_enabled(&timer1)) {
        // disable timer while changing (not necessary, but doing it just in case)
        nrf_drv_timer_disable(&timer1);
    }

    uint32_t time_ticks = nrf_drv_timer_us_to_ticks(&timer1, period);

    nrf_drv_timer_extended_compare(&timer1, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);

    nrf_drv_timer_enable(&timer1);
}

/**
 * disable Timer1
 */
void Audio::disableTimer() {
    if (nrf_drv_timer_is_enabled(&timer1)) {
        nrf_drv_timer_disable(&timer1);
    }
}
