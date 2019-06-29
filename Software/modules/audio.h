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

#include "common.h"

static nrf_drv_pwm_t m_pwm0 = NRF_DRV_PWM_INSTANCE(0);

class Audio {

    public:
        Audio();

        void enable(bool enable);
        bool isEnabled();

        bool headphonesInserted();

        void beep(int duration, int frequency);

    private:
        bool enabled;
        bool headphones;

};


#endif //MODULE_AUDIO_H
