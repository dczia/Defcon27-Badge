/**
 * Driver for handling LEDs
 */

#include "common.h"
#include "led.h"

LED::LED(){

    // Setup and turn off the LEDs
    nrf_gpio_cfg_output(LED_D_PIN);
    nrf_gpio_cfg_output(LED_C_PIN);
    nrf_gpio_cfg_output(LED_Z_PIN);
    nrf_gpio_cfg_output(LED_I_PIN);
    nrf_gpio_cfg_output(LED_A_PIN);

    nrf_gpio_pin_set(LED_D_PIN);
    nrf_gpio_pin_set(LED_C_PIN);
    nrf_gpio_pin_set(LED_Z_PIN);
    nrf_gpio_pin_set(LED_I_PIN);
    nrf_gpio_pin_set(LED_A_PIN);

}


/**
 * Set the state of an individual LED
 * @param led
 * @param state
 */
void LED::set(LEDS led, LED_STATE state){

    void (*led_func)(uint32_t);

    switch(state){
        case ON:
            led_func = &nrf_gpio_pin_clear;
            break;
        case OFF:
            led_func = &nrf_gpio_pin_set;
            break;
        case TOGGLE:
            led_func = &nrf_gpio_pin_toggle;
            break;
        default:
            // Nothing to do
            return;
    }

    switch(led){
        case LED_D:
            led_func(LED_D_PIN);
            break;
        case LED_C:
            led_func(LED_C_PIN);
            break;
        case LED_Z:
            led_func(LED_Z_PIN);
            break;
        case LED_I:
            led_func(LED_I_PIN);
            break;
        case LED_A:
            led_func(LED_A_PIN);
            break;
        default:
            break;
    }

}


void LED::bitmap(uint8_t state) {
    if (state & 1) {
        nrf_gpio_pin_write(LED_D_PIN, 0);
    } else {
        nrf_gpio_pin_write(LED_D_PIN, 1);
    }
    if (state & 2) {
        nrf_gpio_pin_write(LED_C_PIN, 0);
    } else {
        nrf_gpio_pin_write(LED_C_PIN, 1);
    }
    if (state & 4) {
        nrf_gpio_pin_write(LED_Z_PIN, 0);
    } else {
        nrf_gpio_pin_write(LED_Z_PIN, 1);
    }
    if (state & 8) {
        nrf_gpio_pin_write(LED_I_PIN, 0);
    } else {
        nrf_gpio_pin_write(LED_I_PIN, 1);
    }
    if (state & 16) {
        nrf_gpio_pin_write(LED_A_PIN, 0);
    } else {
        nrf_gpio_pin_write(LED_A_PIN, 1);
    }
}
/**
 * Turn all the LEDs on
 */
void LED::allOn(){
    nrf_gpio_pin_write(LED_D_PIN, 0);
    nrf_gpio_pin_write(LED_C_PIN, 0);
    nrf_gpio_pin_write(LED_Z_PIN, 0);
    nrf_gpio_pin_write(LED_I_PIN, 0);
    nrf_gpio_pin_write(LED_A_PIN, 0);
}

/**
 * Turn all the LEDs off
 */
void LED::allOff(){
    nrf_gpio_pin_write(LED_D_PIN, 1);
    nrf_gpio_pin_write(LED_C_PIN, 1);
    nrf_gpio_pin_write(LED_Z_PIN, 1);
    nrf_gpio_pin_write(LED_I_PIN, 1);
    nrf_gpio_pin_write(LED_A_PIN, 1);
}