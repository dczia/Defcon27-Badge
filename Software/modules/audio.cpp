/**
 *
 * Audio driver
 *
 * @date 6/21/2019
 * @author hamster, rehr
 *
 */

#include "audio.h"

/**
 * Create the object and initialize the port
 */
Audio::Audio(){
    enabled = false;
    headphones = false;

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
