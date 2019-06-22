/*********************************************************************
This is a port of the Adafruit SSD1306 driver to Nordic nRF52832.

Currently only I2C supported.

Original:

https://github.com/adafruit/Adafruit_SSD1306

electronut.in
*********************************************************************/


#ifndef _SSD1306_H_
#define _SSD1306_H_

#include <stdint.h>
#include <stdbool.h>
#include "nordic_common.h"
#include "custom_board.h"

typedef enum {
    COLOR_BLACK,
    COLOR_WHITE,
    COLOR_INVERSE
} SSD1306_COLOR;

typedef enum {
    ROTATION_0,
    ROTATION_90,
    ROTATION_180,
    ROTATION_270
} SSD1306_ROTATION;

#define SSD1306_I2C_ADDRESS   0x3C  // 011110+SA0+RW - 0x3C or 0x3D

#if defined SSD1306_128_64
    #define SSD1306_LCDWIDTH                  128
    #define SSD1306_LCDHEIGHT                 64
#endif
#if defined SSD1306_128_32
    #define SSD1306_LCDWIDTH                  128
    #define SSD1306_LCDHEIGHT                 32
#endif
#if defined SSD1306_96_16
    #define SSD1306_LCDWIDTH                  96
    #define SSD1306_LCDHEIGHT                 16
#endif


#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9

#define SSD1306_SETMULTIPLEX 0xA8

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10

#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22

#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8

#define SSD1306_SEGREMAP 0xA0

#define SSD1306_CHARGEPUMP 0x8D

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

void SSD1306_begin(uint8_t switchvcc, uint8_t i2caddr, bool reset);
void SSD1306_command(uint8_t c);

void SSD1306_clearDisplay(void);
void SSD1306_invertDisplay(uint8_t i);
void SSD1306_setRotation(SSD1306_ROTATION rotation);
void SSD1306_display();

void SSD1306_startscrollright(uint8_t start, uint8_t stop);
void SSD1306_startscrollleft(uint8_t start, uint8_t stop);

void SSD1306_startscrolldiagright(uint8_t start, uint8_t stop);
void SSD1306_startscrolldiagleft(uint8_t start, uint8_t stop);
void SSD1306_stopscroll(void);

void SSD1306_dim(bool dim);

void SSD1306_drawPixel(int16_t x, int16_t y, SSD1306_COLOR color);

void SSD1306_drawFastVLine(int16_t x, int16_t y, int16_t h, SSD1306_COLOR color);
void SSD1306_drawFastHLine(int16_t x, int16_t y, int16_t w, SSD1306_COLOR color);

void SSD1306_drawFastVLineInternal(int16_t x, int16_t y, int16_t h, SSD1306_COLOR color); //__attribute__((always_inline));
void SSD1306_drawFastHLineInternal(int16_t x, int16_t y, int16_t w, SSD1306_COLOR color); //__attribute__((always_inline));


#endif /* _SSD1306_H_ */
