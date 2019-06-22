/**
 *
 * Driver to bit-bang pulse trains for ws2812s-style RGB LEDs
 *
 * Sourced from data from
 * https://wp.josh.com/2014/05/13/ws2812-neopixels-are-not-so-finicky-once-you-get-to-know-them/
 * and
 * https://github.com/lavallc/nrf51-neopixel
 *
 * @date 6/22/2019
 * @author hamster
 *
 */

#ifndef MODULE_WS2812B_H
#define MODULE_WS2812B_H

#include "common.h"
#include <vector>

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} PIXEL;

class WS2812S {

    public:
        explicit WS2812S(uint16_t numPixels);

        void setColor(uint16_t pixel, uint8_t red, uint8_t green, uint8_t blue);
        void setColor(uint16_t pixel, PIXEL color);

        void show();

    private:

        std::vector<PIXEL> pixels;
        uint16_t numPixels;

        unsigned char reverseByte(unsigned char byte);
        void sendByte(unsigned char byte);
        void sendBit(bool bit);

        NRF_GPIO_Type *outreg;
        uint8_t pin;

};


#endif //MODULE_WS2812B_H
