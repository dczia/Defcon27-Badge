/**
 *
 * Driver for LEDs
 *
 */

#ifndef MODULE_LED_H
#define MODULE_LED_H

#include "common.h"

#define COLOR_RED           {32, 0, 0}
#define COLOR_GREEN         {0, 32, 0}
#define COLOR_BLUE          {0, 0, 32}


typedef enum {
    LED_D,
    LED_C,
    LED_Z,
    LED_I,
    LED_A,
    NUM_LEDS
} LEDS;

typedef enum {
    ON,
    OFF,
    TOGGLE
} LED_STATE;

class LED{
    public:
        LED();

        void set(LEDS led, LED_STATE state);
        void allOn();
        void allOff();

    private:
};


#endif //MODULE_LED_H
