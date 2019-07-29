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

void audio_off();
uint8_t button_handler(uint8_t mode);
void startup_sequence();
void led_theramin();
void led_walk();
void led_handler_blink();
void led_handler_cylon();

uint8_t tof_pitch(uint8_t prevRange);
uint8_t tof_volume(uint8_t prevRange);

APP_TIMER_DEF(m_led_timer_id);
