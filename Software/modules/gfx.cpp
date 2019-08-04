/*****************************************************************************
 * (C) Copyright 2017 AND!XOR LLC (http://andnxor.com/).
 *
 * PROPRIETARY AND CONFIDENTIAL UNTIL AUGUST 1ST, 2017 then,
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Contributors:
 * 	@andnxor
 * 	@zappbrandnxor
 * 	@hyr0n1
 * 	@andrewnriley
 * 	@lacosteaef
 * 	@bitstr3m
 *
 * 	Adapted for the dc801 dc26 badge and SDK15 by @hamster
 *
 *****************************************************************************/

// System headers
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <common.h>

#include "nordic_common.h"
#include "nrf52840_peripherals.h"
#include "nrf_spim.h"
#include "nrf_drv_spi.h"
#include "ff.h"
#include "nrf_block_dev_sdc.h"
#include "nrf_block_dev.h"
#include "diskio.h"
#include "diskio_blkdev.h"
#include "nrf_delay.h"
#include "nrf_soc.h"

#include "gfx.h"
#include "drv_ssd1306.h"
#include "../adafruit/gfxfont.h"

#include "../fonts/computerfont12pt7b.h"
#include "../fonts/monof55.h"
#include "../fonts/gameplay.h"
#include "../fonts/veramono5pt7b.h"

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

//Cursor coordinates
static int16_t m_cursor_x = 0;
static int16_t m_cursor_y = 0;
static area_t m_cursor_area = { 0, 0, 0, 0 };
static uint8_t m_font = FONT_MONO55_8PT;
static bool m_wrap = true;

//Track validity of screen state
static volatile bool m_valid_state = false;


#define __FONT_MONO55_8PT       monof558pt7b
#define __FONT_COMPUTER_12PT    Computerfont12pt7b
#define __FONT_GAMEPLAY_5PT     gameplay5pt7b
#define __FONT_VERAMONO_5PT     VeraMono5pt7b

static inline GFXfont __get_font() {
	GFXfont font = __FONT_MONO55_8PT;

	switch(m_font){
	    case FONT_COMPUTER_12PT:
            font = __FONT_COMPUTER_12PT;
	        break;
	    case FONT_MONO55_8PT:
            font = __FONT_MONO55_8PT;
	        break;
        case FONT_GAMEPLAY_5PT:
            font = __FONT_GAMEPLAY_5PT;
            break;
		case FONT_VERAMONO_5PT:
			font = __FONT_VERAMONO_5PT;
			break;
	    default:
	        break;
	}

	return font;
}

/**
 * Draw single character anywhere on the screen. Adapted from Adafruit GFX library.
 * Only supports custom fonts.
 */
static void __draw_char(int16_t x, int16_t y, unsigned char c, SSD1306_COLOR color, GFXfont font) {

	// Character is assumed previously filtered by write() to eliminate
	// newlines, returns, non-printable characters, etc.  Calling drawChar()
	// directly with 'bad' characters of font may cause mayhem!

	c -= font.first;
	GFXglyph *glyph = &(font.glyph[c]);
	uint8_t *bitmap = font.bitmap;

	uint16_t bo = glyph->bitmapOffset;
	uint8_t w = glyph->width;
	uint8_t h = glyph->height;

	int8_t xo = glyph->xOffset;
	int8_t yo = glyph->yOffset;
	uint8_t xx, yy, bits = 0, bit = 0;

	int16_t yyy;
	for (yy = 0; yy < h; yy++) {
		for (xx = 0; xx < w; xx++) {
			if (!(bit++ & 7)) {
				bits = bitmap[bo++];
			}
			if (bits & 0x80) {
				yyy = y + yo + yy;
				if (yyy >= m_cursor_area.ys && yyy <= m_cursor_area.ye) {
					util_gfx_set_pixel(x + xo + xx, y + yo + yy, color);
				}
			}
			bits <<= 1;
		}
	}
}

/**
 * Adapted from Adafruit GFX library. Draws custom font to the screen
 * at the current cursor position.
 *
 *
 */
static void __write_char(uint8_t c, GFXfont font, SSD1306_COLOR color) {
	//If newline, move down a row
	if (c == '\n') {
		m_cursor_x = m_cursor_area.xs;
		m_cursor_y += font.yAdvance;
	}
	//Otherwise, print the character (ignoring carriage return)
	else if (c != '\r') {
		uint8_t first = font.first;

		//Valid char?
		if ((c >= first) && (c <= font.last)) {
			uint8_t c2 = c - first;
			GFXglyph *glyph = &(font.glyph[c2]);

			uint8_t w = glyph->width;
			uint8_t h = glyph->height;

			if ((w > 0) && (h > 0)) { // Is there an associated bitmap?
				int16_t xo = glyph->xOffset;

				if ((m_cursor_x + (xo + w)) >= m_cursor_area.xe && m_wrap) {
					// Drawing character would go off right edge; wrap to new line
					m_cursor_x = m_cursor_area.xs;
					m_cursor_y += font.yAdvance;
				}

				__draw_char(m_cursor_x, m_cursor_y, c, color, font);
			}
			m_cursor_x += glyph->xAdvance;
		}
	}
}

void util_gfx_cursor_area_reset() {
	m_cursor_area.xs = 0;
	m_cursor_area.xe = SSD1306_LCDWIDTH;
	m_cursor_area.ys = 0;
	m_cursor_area.ye = SSD1306_LCDHEIGHT;
}


/**
 * Draw the DCZia symbol at x,y with color
 * NOTE: Color is 16-bit (565)
 */
extern void util_gfx_draw_dczia(int16_t x0, int16_t y0, SSD1306_COLOR color) {
	/* left lines */
	SSD1306_drawFastHLine(x0 + 2, y0 + 10, 10, color);
	SSD1306_drawFastHLine(x0, y0 + 12, 10, color);
	SSD1306_drawFastHLine(x0, y0 + 14, 10, color);
	SSD1306_drawFastHLine(x0 + 2, y0 + 16, 10, color);
	/* right lines */
	SSD1306_drawFastHLine(x0 + 15, y0 + 10, 10, color);
	SSD1306_drawFastHLine(x0 + 17, y0 + 12, 10, color);
	SSD1306_drawFastHLine(x0 + 17, y0 + 14, 10, color);
	SSD1306_drawFastHLine(x0 + 15, y0 + 16, 10, color);
	/* top lines */
	SSD1306_drawFastVLine(x0 + 10, y0 + 2, 10, color);
	SSD1306_drawFastVLine(x0 + 12, y0, 10, color);
	SSD1306_drawFastVLine(x0 + 14, y0, 10, color);
	SSD1306_drawFastVLine(x0 + 16, y0 + 2, 10, color);
	/* bottom lines */
	SSD1306_drawFastVLine(x0 + 10, y0 + 15, 10, color);
	SSD1306_drawFastVLine(x0 + 12, y0 + 17, 10, color);
	SSD1306_drawFastVLine(x0 + 14, y0 + 17, 10, color);
	SSD1306_drawFastVLine(x0 + 16, y0 + 15, 10, color);
	/* circle sides */
	SSD1306_drawFastHLine(x0 + 11, y0 + 9, 5, color);
	SSD1306_drawFastHLine(x0 + 11, y0 + 17, 5, color);
	SSD1306_drawFastVLine(x0 + 9, y0 + 11, 5, color);
	SSD1306_drawFastVLine(x0 + 17, y0 + 11, 5, color);
	/* center sphere reflection */
	SSD1306_drawPixel(x0 + 11, y0 + 12, color);
	SSD1306_drawPixel(x0 + 12, y0 + 11, color);
	SSD1306_drawPixel(x0 + 13, y0 + 11, color);
}


/**
 * Draw a circle at x, y with radius r and color.
 * NOTE: Color is 16-bit (565)
 */
void util_gfx_draw_circle(int16_t x0, int16_t y0, int16_t r, SSD1306_COLOR color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    SSD1306_drawPixel(x0, y0 + r, color);
    SSD1306_drawPixel(x0, y0 - r, color);
    SSD1306_drawPixel(x0 + r, y0, color);
    SSD1306_drawPixel(x0 - r, y0, color);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

        SSD1306_drawPixel(x0 + x, y0 + y, color);
        SSD1306_drawPixel(x0 - x, y0 + y, color);
        SSD1306_drawPixel(x0 + x, y0 - y, color);
        SSD1306_drawPixel(x0 - x, y0 - y, color);
        SSD1306_drawPixel(x0 + y, y0 + x, color);
        SSD1306_drawPixel(x0 - y, y0 + x, color);
        SSD1306_drawPixel(x0 + y, y0 - x, color);
        SSD1306_drawPixel(x0 - y, y0 - x, color);
	}
}

void util_gfx_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, SSD1306_COLOR color) {

	//Draw lines fast if they are horizontal or vertical
	if (y0 == y1) {
        SSD1306_drawFastHLine(x0, y0, 1, color);
		return;
	} else if (x0 == x1) {
        SSD1306_drawFastVLine(x0, y0, 1, color);
		return;
	}

	int16_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		_swap_int16_t(x0, y0);
		_swap_int16_t(x1, y1);
	}

	//Simplify direction we're drawing the line in
	if (x0 > x1) {
		_swap_int16_t(x0, x1);
		_swap_int16_t(y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}

	for (; x0 <= x1; x0++) {
		if (steep) {
            SSD1306_drawPixel(y0, x0, color);
		} else {
            SSD1306_drawPixel(x0, y0, color);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}


void util_gfx_draw_rect(int16_t x, int16_t y, int16_t w, int16_t h, SSD1306_COLOR color) {
	util_gfx_draw_line(x, y, x + w - 1, y, color);
	util_gfx_draw_line(x, y, x, y + h - 1, color);
	util_gfx_draw_line(x + w - 1, y, x + w - 1, y + h, color);
	util_gfx_draw_line(x, y + h - 1, x + w - 1, y + h - 1, color);
}

void util_gfx_draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
		int16_t x2, int16_t y2, SSD1306_COLOR color) {
	util_gfx_draw_line(x0, y0, x1, y1, color);
	util_gfx_draw_line(x1, y1, x2, y2, color);
	util_gfx_draw_line(x2, y2, x0, y0, color);
}

void util_gfx_fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, SSD1306_COLOR color) {
    util_gfx_draw_rect(x, y, w, h, color);
}

void util_gfx_fill_screen(SSD1306_COLOR color) {
    util_gfx_fill_rect(0, 0, SSD1306_LCDWIDTH, SSD1306_LCDHEIGHT, color);
}

uint8_t util_gfx_font_height() {
	return __get_font().yAdvance;
}

/**
 * Returns the width of a character for the current font, careful,  this is unreliable with non-fixed fonts
 */
uint8_t util_gfx_font_width() {
	GFXfont font = __get_font();
	return font.glyph[0].xAdvance;
}

int16_t util_gfx_cursor_x_get() {
	return m_cursor_x;
}

int16_t util_gfx_cursor_y_get() {
	return m_cursor_y;
}

inline void util_gfx_invalidate() {
	m_valid_state = false;
}

inline void util_gfx_validate() {
	m_valid_state = true;
}

inline bool util_gfx_is_valid_state() {
	return m_valid_state;
}


void util_gfx_get_text_bounds(char *str, int16_t x, int16_t y, uint16_t *w, uint16_t *h) {
	uint8_t c; // Current character

	GFXfont font = __get_font();

	*w = *h = 0;

	GFXglyph glyph;

	uint8_t first = font.first;
	uint8_t last = font.last;
	uint8_t gw, gh, xa;
	int8_t xo, yo;
	int16_t minx = SSD1306_LCDWIDTH;
	int16_t miny = SSD1306_LCDHEIGHT;
	int16_t maxx = -1, maxy = -1, gx1, gy1, gx2, gy2;
	int16_t ya = font.yAdvance;

	//Walk through each character
	while ((c = *str++)) {
		if (c != '\n') { // Not a newline
			if (c != '\r') { // Not a carriage return, is normal char
				if ((c >= first) && (c <= last)) { // Char present in current font
					c -= first;
					glyph = font.glyph[c];
					gw = glyph.width;
					gh = glyph.height;
					xa = glyph.xAdvance;
					xo = glyph.xOffset;
					yo = glyph.yOffset;
					if (m_wrap && ((x + ((int16_t) xo + gw)) >= SSD1306_LCDWIDTH)) {
						// Line wrap
						x = 0;  // Reset x to 0
						y += ya; // Advance y by 1 line
					}
					gx1 = x + xo;
					gy1 = y + yo;
					gx2 = gx1 + gw - 1;
					gy2 = gy1 + gh - 1;
					if (gx1 < minx)
						minx = gx1;
					if (gy1 < miny)
						miny = gy1;
					if (gx2 > maxx)
						maxx = gx2;
					if (gy2 > maxy)
						maxy = gy2;
					x += xa;
				}
			} // Carriage return = do nothing
		} else { // Newline
			x = 0;  // Reset x
			y += ya; // Advance y by 1 line
		}
	}
	// End of string

	if (maxx >= minx)
		*w = maxx - minx + 1;
	if (maxy >= miny)
		*h = maxy - miny + 1;
}

void util_gfx_init() {
	util_gfx_cursor_area_reset();
    SSD1306_clearDisplay();
    SSD1306_display();
}

void util_gfx_print_char(char letter, SSD1306_COLOR color){
    GFXfont font = __get_font();

    __write_char(letter, font, color);
}

void util_gfx_print(const char *text, SSD1306_COLOR color) {
	GFXfont font = __get_font();

	for (uint16_t i = 0; i < strlen(text); i++) {
		__write_char(text[i], font, color);
	}

}


void util_gfx_set_cursor(int16_t x, int16_t y) {
	m_cursor_x = x;
	m_cursor_y = y + (util_gfx_font_height() / 2);
}

void util_gfx_cursor_area_set(area_t area) {
	m_cursor_area = area;
}

void util_gfx_set_font(uint8_t font) {
	m_font = font;	//May need to do some range checking
}

void util_gfx_set_pixel(int16_t x, int16_t y, SSD1306_COLOR color) {
    SSD1306_drawPixel(x, y, color);
}

void util_gfx_set_wrap(bool wrap) {
	m_wrap = wrap;
}

