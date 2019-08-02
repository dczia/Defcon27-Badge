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

#define VERSION "1.00"
#endif /* MAIN_H_ */

/* badge modes */
const uint8_t DEFAULT_BADGE_MODE   = 0;
const uint8_t THEREMIN_BADGE_MODE  = 1;
const uint8_t FIXED_VOL_BADGE_MODE = 2;
const uint8_t HOLDING_BADGE_MODE   = 3;
const uint8_t DOOM_BADGE_MODE      = 4;
const uint8_t TOTAL_BADGE_MODES    = 5;

void audio_off();
void incrementBadgeMode();
void button_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);
void startup_sequence();

void led_theramin();
void led_animation(uint16_t led_speed);
void led_handler_blink();
void led_handler_cylon();
void led_flash_yellow();

void e1m1();
void noteDoomBase(int octave, int speed,int volume);
uint8_t tof_pitch(uint8_t prevRange);
uint8_t tof_volume(uint8_t prevRange);
void oled_init();

APP_TIMER_DEF(m_led_timer_id);
