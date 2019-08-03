/**
 *
 * @file main.h
 *
 * @date June 20 2019
 * @author hamster
 *
 * Main header file
 *
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stdint.h>
#include "./common.h"

// Includes for our app
#include "./utility.h"
#include "./modules/audio.h"
#include "./modules/ble.h"
#include "./modules/sd.h"
#include "./modules/adc.h"
#include "./modules/uart.h"
#include "./modules/i2c.h"
#include "./modules/led.h"
#include "./modules/gfx.h"
#include "./modules/drv_ssd1306.h"
#include "./modules/VL6180X.h"
#include "./modules/ws2812b.h"

#define VERSION "1.00"
#endif /* MAIN_H_ */



/* badge modes */
const uint8_t BADGE_MODE_DEFAULT   = 0;
const uint8_t BADGE_MODE_THEREMIN  = 1;
const uint8_t BADGE_MODE_FIXED_VOL = 2;
const uint8_t BADGE_MODE_CREDITS   = 3;
const uint8_t BADGE_MODES          = 4;

void audio_off();
void incrementBadgeMode();
void button_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);
void startup_sequence();
void checkButtonHolds();

void led_theramin();
void led_handler_blink(void *p_context);
void led_handler_cylon(void *p_context);
void led_animation(uint16_t led_speed);
void led_flash_yellow();

void e1m1();
void noteDoomBase(int octave, int speed, int volume);
uint8_t tof_pitch(uint8_t prevRange);
uint8_t tof_volume(uint8_t prevRange);
void oled_init();

APP_TIMER_DEF(m_led_timer_id);
APP_TIMER_DEF(m_audio_step_timer_id);
void audio_timer_handler(void *p_context);