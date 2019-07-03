/**
 *
 * Audio driver
 *
 * @date 6/21/2019
 * @author hamster, rehr
 *
 */

#ifndef MODULE_AUDIO_H
#define MODULE_AUDIO_H

#include "nrf_drv_pwm.h"
#include "nrf_drv_timer.h"

static uint16_t sineTable[32] = { 511, 611, 707, 795, 872, 936, 983, 1012, 1022, 1012, 983, 936, 872, 795, 707, 611, 511, 411, 315, 227, 150, 86, 39, 10, 0, 10, 39, 86, 150, 227, 315, 411 };

class Audio {

    public:
        Audio();

        void enable(bool enable);
        bool isEnabled();

        bool headphonesInserted();

        void beep(int duration, int frequency);

        uint32_t initPWM(void);
        void setPWMCh0Value(uint16_t value);
        void setPWMCh1Value(uint16_t value);

        uint32_t initTimer(void);
        void setTimerWithFreq(uint32_t frequency);
        void setTimerWithPeriod_ms(uint32_t period);
        void setTimerWithPeriod_us(uint32_t period);
        void disableTimer(void);

    private:
        static void timer_event_handler(nrf_timer_event_t event_type, void* p_context);

        bool enabled;
        bool headphones;

        nrf_drv_pwm_t pwm0;
        nrf_pwm_values_individual_t pwm0_seq_values;
        nrf_pwm_sequence_t pwm0_seq;

        nrf_drv_timer_t timer1;
};



#endif //MODULE_AUDIO_H
