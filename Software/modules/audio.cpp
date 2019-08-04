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
    static uint8_t signalVal = 0;
    static uint8_t count = 0;
    uint8_t waveform = ((Audio *)p_context)->getWaveform();

    switch (event_type) {
        case NRF_TIMER_EVENT_COMPARE0:
            if (waveform == WAVE_SINE) {
                signalVal = sineTable[count];
            } else if (waveform == WAVE_TRI) {
                if (count <= 32) {
                    signalVal = count << 1;
                } else {
                    signalVal = (64 - count) << 1;
                }
            } else if (waveform == WAVE_RAMP) {
                signalVal = count;
            } else if (waveform == WAVE_SQR) {
                if (count < 32) {
                    signalVal = 0;
                } else {
                    signalVal = 64;
                }
            } else if (waveform == WAVE_NOIZ) {
                signalVal = noizTable[count];
            }

            count++;
            if (count > 63) {
                count = 0;
            }

            ((Audio *)p_context)->setPWM0Ch0Value(signalVal*((Audio *)p_context)->getVolume());
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
    waveform = WAVE_SINE;
    volume = 0;
    songStepPosition = 0;
    songPlaying = false;

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
uint32_t Audio::initPWM0() {
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
    
    nrf_drv_pwm_config_t const pwm0_config = {
        { // .output_pins
            AUDIO_PWM_PIN,              // channel 0
            NRF_DRV_PWM_PIN_NOT_USED,   // channel 1
            NRF_DRV_PWM_PIN_NOT_USED,   // channel 2
            NRF_DRV_PWM_PIN_NOT_USED,   // channel 3
        },
        APP_IRQ_PRIORITY_LOW, // .irq_priority
        NRF_PWM_CLK_16MHz, // .base_clock
        NRF_PWM_MODE_UP, // .count_mode
        1024, // .top_value
        NRF_PWM_LOAD_INDIVIDUAL, // .load_mode
        NRF_PWM_STEP_AUTO // .step_mode
    };

    uint32_t err_code = nrf_drv_pwm_init(&pwm0, &pwm0_config, NULL);
    if (err_code != NRF_SUCCESS) {
        return err_code;
    } else {
        nrf_drv_pwm_simple_playback(&pwm0, &pwm0_seq, 1, NRF_DRV_PWM_FLAG_LOOP);
    }

    return NRF_SUCCESS;
}

/**
 * set PWM0 channel 0 value
 * 
 * @param value New PWM0 channel 0 value between 0 and 1023
 */
void Audio::setPWM0Ch0Value(uint16_t value) {
    pwm0_seq_values.channel_0 = value;
}

/**
 * set PWM0 channel 1 value
 * 
 * @param value New PWM0 channel 1 value between 0 and 1023
 */
void Audio::setPWM0Ch1Value(uint16_t value) {
    pwm0_seq_values.channel_1 = value;
}

/**
 * set audio waveform for output audio signal
 * 
 * @param value New waveform for output audio signal
 *      available waveforms are defined in header
 *      sine, triangle, ramp, square, and noise
 */
void Audio::setWaveform(uint8_t newWaveform) {
    waveform = newWaveform;
}

/**
 * get synthesized waveform
 * 
 * @return current waveform
 */
uint8_t Audio::getWaveform() {
    return waveform;
}

/**
 * set audio volume
 * 
 * @param newVolume New volume between 0 and 16
 */
void Audio::setVolume(uint8_t newVolume) {
    volume = newVolume;
}

/**
 * get audio volume
 * 
 * @return current volume value between 0 and 16
 */
uint8_t Audio::getVolume() {
    return volume;
}

/**
 * initialize the Timer1 module for audio generation
 * 
 * @return error code value from nrf_drv_timer_init
 * 
 * @note Timer1 is not enabled yet. Call one of the setTimer functions to enable.
 */
uint32_t Audio::initTimer1() {
    timer1 = NRF_DRV_TIMER_INSTANCE(1);

    nrf_drv_timer_config_t timer1_config = NRF_DRV_TIMER_DEFAULT_CONFIG;
    timer1_config.p_context = this;
    return nrf_drv_timer_init(&timer1, &timer1_config, timer_event_handler);
}

/**
 * set Timer1 frequency in hertz
 * 
 * @param frequency Frequency in hertz to set Timer1 interrupt
 */
void Audio::setTimerWithFreq(uint16_t frequency) {
    if (nrf_drv_timer_is_enabled(&timer1)) {
        // disable timer while changing (not necessary, but doing it just in case)
        nrf_drv_timer_disable(&timer1);
    }
    
    double period = 1/frequency;
    uint32_t time_ticks = nrf_drv_timer_ms_to_ticks(&timer1, period);

    nrf_drv_timer_extended_compare(&timer1, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);

    nrf_drv_timer_enable(&timer1);
}

/**
 * set Timer1 period in milliseconds
 * 
 * @param period Period in milliseconds to set Timer1 interrupt
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
 * @param period Period in microseconds to set Timer1 interrupt
 * 
 * @note this assumes Timer1 is running at 16MHz
 */
void Audio::setTimerWithPeriod_us(uint32_t period) {
    if (nrf_drv_timer_is_enabled(&timer1)) {
        // disable timer while changing (not necessary, but doing it just in case)
        nrf_drv_timer_disable(&timer1);
    }

    // this loses resolution when we have to down convert then up convert in this function so we do it ourselves below
    // period = period >> 6;
    // uint32_t time_ticks = nrf_drv_timer_us_to_ticks(&timer1, period);

    // period needs be divided by 64 for sampling ( >> 6 )
    // then multiplied by 16 for clock speed ( << 4 )
    // those can be combined to result in only shifting period right by 2 bits and saving some resolution :D
    uint32_t time_ticks = period  >> 2;
    NRFX_ASSERT(time_ticks <= UINT32_MAX);

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

void Audio::startSongPlayback() {
    songPlaying = true;
    resetStepPosition();

    enable(true);    
    setVolume(16);

    uint8_t note = doomSong[0];
    setTimerWithPeriod_us(notes[5][note]);
}

void Audio::stopSongPlayback() {
    enable(false);
    disableTimer();
    setPWM0Ch0Value(0);
    songPlaying = false;
}

bool Audio::songIsPlaying() {
    return songPlaying;
}

void Audio::resetStepPosition() {
    songStepPosition = 0;
}

uint8_t Audio::incStepPosition() {
    songStepPosition++;
    if (songStepPosition > doomSongLength) {
        /* relay to main that song is over */
        return 0;
    } else {
        /* play next note in song */
        uint8_t note = doomSong[songStepPosition];
        if (note != REST) {
            setVolume(16);
            setTimerWithPeriod_us(notes[5][note]);
        } else {
            setVolume(0);
        }
    }

    return 1;
}

