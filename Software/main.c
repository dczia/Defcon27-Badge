/**
* DCZia Shit
* Creative Commons Something License?
* @lithochasm
 */


/** @file
 *
 * @defgroup bsp_example_main main.c
 * @{
 * @ingroup bsp_example
 * @brief BSP Example Application main file.
 *
 */

#include <stdio.h>
#include <string.h>
#include "nrf_drv_pwm.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "boards.h"
#include "bsp.h"
#include "app_timer.h"
#include "nrf_drv_clock.h"

#include "nrf_delay.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#define SPEAKER 3  // Connect a speaker positive to P0.03 and the negative to GND

static nrf_drv_pwm_t m_pwm0 = NRF_DRV_PWM_INSTANCE(0);

static void demo3(void)
{
    NRF_LOG_INFO("Demo 3");

    /*
     * This demo uses only one channel, which is reflected on LED 1.
     * The LED blinks three times (200 ms on, 200 ms off), then it stays off
     * for one second.
     * This scheme is performed three times before the peripheral is stopped.
     */

    nrf_drv_pwm_config_t const config0 =
    {
        .output_pins =
        {
            BSP_LED_0 | NRF_DRV_PWM_PIN_INVERTED, // channel 0
            NRF_DRV_PWM_PIN_NOT_USED,             // channel 1
            NRF_DRV_PWM_PIN_NOT_USED,             // channel 2
            NRF_DRV_PWM_PIN_NOT_USED,             // channel 3
        },
        .irq_priority = APP_IRQ_PRIORITY_LOWEST,
        .base_clock   = NRF_PWM_CLK_125kHz,
        .count_mode   = NRF_PWM_MODE_UP,
        .top_value    = 25000,
        .load_mode    = NRF_PWM_LOAD_COMMON,
        .step_mode    = NRF_PWM_STEP_AUTO
    };
    APP_ERROR_CHECK(nrf_drv_pwm_init(&m_pwm0, &config0, NULL));
    //makem_used |= USED_PWM(0);

    // This array cannot be allocated on stack (hence "static") and it must
    // be in RAM (hence no "const", though its content is not changed).
    static uint16_t /*const*/ seq_values[] =
    {
        0x8000,
             0,
        0x8000,
             0,
        0x8000,
             0
    };
    nrf_pwm_sequence_t const seq =
    {
        .values.p_common = seq_values,
        .length          = NRF_PWM_VALUES_LENGTH(seq_values),
        .repeats         = 0,
        .end_delay       = 4
    };

    (void)nrf_drv_pwm_simple_playback(&m_pwm0, &seq, 3, NRF_DRV_PWM_FLAG_STOP);
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

    APP_ERROR_CHECK(nrf_drv_clock_init());
    nrf_gpio_cfg_output(SPEAKER); // Setup Speaker
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

    demo3();
    while (true)
    {
        NRF_LOG_FLUSH();
        __SEV();
        __WFE();
        __WFE();
        // no implementation needed
    }

    


}


/** @} */
