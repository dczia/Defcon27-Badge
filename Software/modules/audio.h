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

static uint16_t sineTable[64] = { 500, 549, 598, 645, 691, 736, 778, 817, 854, 887, 916, 941, 962, 978, 990, 998, 1000, 998, 990, 978, 962, 941, 916, 887, 854, 817, 778, 736, 691, 645, 598, 549, 500, 451, 402, 355, 309, 264, 222, 183, 146, 113, 84, 59, 38, 22, 10, 2, 0, 2, 10, 22, 38, 59, 84, 113, 146, 183, 222, 264, 309, 355, 402, 451 };
static uint32_t notes[11][13] = {
    { 122312, 115447, 108968, 102852, 97079, 91631, 86488, 81634, 77052, 72727, 68645, 64793, 61156 }, // C1
    { 61156, 57724, 54484, 51426, 48540, 45815, 43244, 40817, 38526, 36364, 34323, 32396, 30578 }, // C2
    { 30578, 28862, 27242, 25713, 24270, 22908, 21622, 20408, 19263, 18182, 17161, 16198, 15289 }, // C3
    { 15289, 14431, 13621, 12856, 12135, 11454, 10811, 10204, 9631, 9091, 8581, 8099, 7645 }, // C4
    { 7645, 7215, 6810, 6428, 6067, 5727, 5405, 5102, 4816, 4545, 4290, 4050, 3822 }, // C5
    { 3822, 3608, 3405, 3214, 3034, 2863, 2703, 2551, 2408, 2273, 2145, 2025, 1911 }, // C6
    { 1911, 1804, 1703, 1607, 1517, 1432, 1351, 1276, 1204, 1136, 1073, 1012, 956 }, // C7
    { 956, 902, 851, 804, 758, 716, 676, 638, 602, 568, 536, 506, 478 }, // C8
    { 478, 451, 426, 402, 379, 358, 338, 319, 301, 284, 268, 253, 239 }, // C9
    { 239, 225, 213, 201, 190, 179, 169, 159, 150, 142, 134, 127, 119 }, // C10
    { 119, 113, 106, 100, 95, 89, 84, 80, 80, 80, 80, 80, 80 } // C11
};

const uint8_t NOTE_C  = 0;
const uint8_t NOTE_CS = 1;
const uint8_t NOTE_D  = 2;
const uint8_t NOTE_DS = 3;
const uint8_t NOTE_E  = 4;
const uint8_t NOTE_F  = 5;
const uint8_t NOTE_FS = 6;
const uint8_t NOTE_G  = 7;
const uint8_t NOTE_GS = 8;
const uint8_t NOTE_A  = 9;
const uint8_t NOTE_AS = 10;
const uint8_t NOTE_B  = 11;
const uint8_t REST    = 255;

const uint8_t doomSong[] = {
    NOTE_E, REST, NOTE_E, NOTE_E, NOTE_E, NOTE_E,
    NOTE_E, REST, NOTE_E, NOTE_E, NOTE_D, NOTE_D,
    NOTE_E, REST, NOTE_E, NOTE_E, NOTE_C, NOTE_C,
    NOTE_E, REST, NOTE_E, NOTE_E, NOTE_AS, NOTE_AS,
    NOTE_E, REST, NOTE_E, NOTE_E, NOTE_B, NOTE_B, NOTE_C, NOTE_C,
    NOTE_E, REST, NOTE_E, NOTE_E, NOTE_E, NOTE_E,
    NOTE_E, REST, NOTE_E, NOTE_E, NOTE_D, NOTE_D,
    NOTE_E, REST, NOTE_E, NOTE_E, NOTE_C, NOTE_C,
    NOTE_E, REST, NOTE_E, NOTE_E, NOTE_AS, NOTE_AS
};
const uint8_t doomSongLength = 56; // number of notes

class Audio {

    public:
        Audio();

        void enable(bool enable);
        bool isEnabled();

        bool headphonesInserted();

        void beep(int duration, int frequency);

        uint32_t initPWM0(void);
        void setPWM0Ch0Value(uint16_t value);
        void setPWM0Ch1Value(uint16_t value);
        void setVolume(uint8_t newVolume);
        uint8_t getVolume(void);

        uint32_t initTimer1(void);
        void setTimerWithFreq(uint16_t frequency);
        void setTimerWithPeriod_ms(uint32_t period);
        void setTimerWithPeriod_us(uint32_t period);
        void disableTimer(void);

        void startSongPlayback(void);
        void stopSongPlayback(void);
        bool songIsPlaying(void);
        void resetStepPosition(void);
        uint8_t incStepPosition(void);

    private:
        static void timer_event_handler(nrf_timer_event_t event_type, void* p_context);

        bool enabled;
        bool headphones;

        uint8_t volume;
        // uint8_t octave;

        uint8_t songStepPosition;
        uint8_t songSteps;
        bool songPlaying;

        nrf_drv_pwm_t pwm0;
        nrf_pwm_values_individual_t pwm0_seq_values;
        nrf_pwm_sequence_t pwm0_seq;

        nrf_drv_timer_t timer1;
};

#endif //MODULE_AUDIO_H
