/**
 * Copyright (c) 2014 - 2019, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/** @file
 *
 * @defgroup bsp_example_main main.c
 * @{
 * @ingroup bsp_example
 * @brief BSP Example Application main file.
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include "boards.h"
#include "bsp.h"
#include "app_timer.h"
#include "nordic_common.h"
#include "nrf_error.h"

#include "nrf_drv_pwm.h"
#include "nrf_drv_twi.h"

#include "nrf.h"
#include "nrf_delay.h"
#include "app_pwm.h"


#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#define SPEAKER 3

APP_PWM_INSTANCE(PWM1,1);                   // Create the instance "PWM1" using TIMER1.


static volatile bool ready_flag;            // A flag indicating PWM status.

void pwm_ready_callback(uint32_t pwm_id)    // PWM callback function
{
    ready_flag = true;
}

/**
 * @brief Beeps the speaker for a duration at a certain frequency
 * @param duration How long to beep
 * @param frequency Tone freq in hz
 *
 * @note Busy waits, frequency might not be exact, might sound uneven if the softdevice needs to do BLE things
 */
void beep(int duration, int frequency){

	// Figure out how many beeps
	float period = 1000 / (float)frequency;
	long counter = period * duration;
	float delay = period / 2;

	for(long i = 0; i < counter; i++){
		nrf_gpio_pin_write(SPEAKER, 1);
		nrf_delay_us(delay * 1000);
		nrf_gpio_pin_write(SPEAKER, 0);
		nrf_delay_us(delay * 1000);
	}

}

/**@brief Function for handling bsp events.
 */
static void bsp_evt_handler(bsp_event_t evt) {
    switch (evt) {
        // Button 1 - play note 1
        case BSP_EVENT_KEY_0:
            if (bsp_button_is_pressed(0)) {
                bsp_board_led_invert(0);
                beep(75, 600);
            } else {
                
            }
            break;
                
        // Button 2 - play note 2
        case BSP_EVENT_KEY_1:
            if (bsp_button_is_pressed(1)) {
                bsp_board_led_invert(1);
                beep(75, 800);
            } else {
                
            }
            break;
                        
        // Button 3 - play note 3
        case BSP_EVENT_KEY_2:
            if (bsp_button_is_pressed(2)) {
                bsp_board_led_invert(2);
                beep(75, 1000);
            } else {
                
            }
            break;
                
        // Button 4 - play note 4
        case BSP_EVENT_KEY_3:
            if (bsp_button_is_pressed(3)) {
               bsp_board_led_invert(3);
               beep(75, 1200);
            } else {
               
            }
            break;
            

        default:
            
            break;
    }
}


/**@brief Function for initializing low frequency clock.
 */
void clock_initialization()
{
    NRF_CLOCK->LFCLKSRC            = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);
    NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_LFCLKSTART    = 1;

    while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)
    {
        // Do nothing.
    }
}



/**@brief Function for initializing bsp module.
 */
void bsp_configuration()
{
    uint32_t err_code;

    err_code = bsp_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS, bsp_evt_handler);
    APP_ERROR_CHECK(err_code);

    nrf_gpio_cfg_output(SPEAKER);
}


/**
 * @brief Function for application main entry.
 */
int main(void)
{
    clock_initialization();

    uint32_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    NRF_LOG_INFO("BSP example started.");
    bsp_configuration();

    // Pop pop!
    beep(75, 600);
    nrf_delay_ms(50);
    beep(150, 800);
    nrf_delay_ms(50);

    while (true)
    {
        NRF_LOG_FLUSH();
        __SEV();
        __WFE();
        __WFE();
        // no implementation needed
    }

     //ret_code_t err_code;

    /* 2-channel PWM, 200Hz, output on DK LED pins. */
   app_pwm_config_t pwm1_cfg = APP_PWM_DEFAULT_CONFIG_2CH(5000L, BSP_BOARD_LED_0, BSP_BOARD_LED_1);

    /* Switch the polarity of the second channel. */
    pwm1_cfg.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;

    /* Initialize and enable PWM. */
    app_pwm_init(&PWM1,&pwm1_cfg,pwm_ready_callback);
   // APP_ERROR_CHECK(err_code);
    app_pwm_enable(&PWM1);

    uint32_t value;
    while (true)
    {
        for (uint8_t i = 0; i < 40; ++i)
        {
            value = (i < 20) ? (i * 5) : (100 - (i - 20) * 5);

            ready_flag = false;
            /* Set the duty cycle - keep trying until PWM is ready... */
            while (app_pwm_channel_duty_set(&PWM1, 0, value) == NRF_ERROR_BUSY);

            /* ... or wait for callback. */
            while (!ready_flag);
            app_pwm_channel_duty_set(&PWM1, 1, value);
            nrf_delay_ms(25);
        }
    }

}


/** @} */
