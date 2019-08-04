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
 *****************************************************************************/
#ifndef UTIL_GFX_H_
#define UTIL_GFX_H_

#include "drv_ssd1306.h"

#define FONT_COMPUTER_12PT  0
#define FONT_MONO55_8PT     1
#define FONT_GAMEPLAY_5PT	2
#define FONT_VERAMONO_5PT  3

typedef struct {
	uint8_t width;
	uint8_t height;
	uint8_t *data;
} bitmap_t;

typedef struct {
	int16_t xs, ys, xe, ye;
} area_t;

/**
 * Set the visible area for text cursor for printing text. Any printing outside of it will not be visible
 *
 * @param 	area		Bounds of text area to clip
 */
extern void util_gfx_cursor_area_set(area_t area);

/**
 * Reset visible text area to defaults (full screen)
 */
extern void util_gfx_cursor_area_reset();

/**
 * Draw DCZia logo
 * @param 	x0			X coordinate for the left of the logo
 * @param	y0			Y coordinate for the top of the logo
 * @param	color		RGB565 color of the logo
 */
extern void util_gfx_draw_dczia(int16_t x0, int16_t y0, SSD1306_COLOR color);

/**
 * Draw audio waveform
 * @param 	x0			X coordinate for the left of the waveform
 * @param	y0			Y coordinate for the top of the waveform
 * @param	color		RGB565 color of the waveform
 * @param   waveform 	Waveform to be drawn (0: sine, 1: triangle, 2: ramp, 3: square, 4: noise)
 */
extern void util_gfx_draw_waveform(int16_t x0, int16_t y0, SSD1306_COLOR color, uint8_t waveform);

/**
 * Draw a circle to the screen
 *
 * @param 	x0			X coordinate for the center of the circle
 * @param	y0			Y coordinate for the center of the circle
 * @param	r			Radius of the circle
 * @param	color		RGB565 color of the circle
 */
extern void util_gfx_draw_circle(int16_t x0, int16_t y0, int16_t r, SSD1306_COLOR color);

/**
 * Draw a line to the screen
 *
 * @param	x0			Starting X coordinate for the line
 * @param 	y0			Starting Y coordinate for the line
 * @param	x1			Ending Y coordinate for the line
 * @param	y1			Ending Y coordinate for the line
 * @param	color		RGB565 color of the circle
 */
extern void util_gfx_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, SSD1306_COLOR color);


/**
 * Draw a rectangle to the screen.
 *
 * @param	x			X coordinate of the upper left corner of the rectangle
 * @param	y			Y coordinate of the upper left corner of the rectangle
 * @param	w			Width of the rectangle
 * @param	h			Height of the rectangle
 * @param	color		RGB565 color for the rectangle
 */
extern void util_gfx_draw_rect(int16_t x, int16_t y, int16_t w, int16_t h, SSD1306_COLOR color);

/**
 * Draw a triangle to the screen
 *
 * @param	x0			X coordinate of the first corner of the triangle
 * @param	y0			Y coordinate of the first corner of the triangle
 * @param	x1			X coordinate of the second corner of the triangle
 * @param	y1			Y coordinate of the second corner of the triangle
 * @param	x2			X coordinate of the third corner of the triangle
 * @param	y3			Y coordinate of the third corner of the triangle
 * @param	color		RGB565 color for the triangle
 */
extern void util_gfx_draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, SSD1306_COLOR color);

/**
 * Draw a filled rectangle to the screen
 *
 * @param	x			X coordinate of the upper left corner of the rectangle
 * @param	y			Y coordinate of the upper left corner of the rectangle
 * @param	w			Width of the rectangle
 * @param	h			Height of the rectangle
 * @param	color		RGB565 color for the rectangle
 */
extern void util_gfx_fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, SSD1306_COLOR color);

/**
 * Fill the screen with a color
 *
 * @param	color		RGB565 color to fill the screen with
 */
extern void util_gfx_fill_screen(SSD1306_COLOR color);

/**
 * Get the height of the current font
 *
 * @return	The height of the current font
 */
extern uint8_t util_gfx_font_height();

/**
 * Returns the width of the current font. Careful! Unreliable results for non-fixed width fonts
 *
 * @return	The width of the current font
 */
extern uint8_t util_gfx_font_width();

/**
 * Gets the current RGB565 color of the font
 */
extern uint16_t util_gfx_color_get();

/**
 * Gets the cursor's X coordinate
 */
extern int16_t util_gfx_cursor_x_get();

/**
 * Gets the cursor's Y coordinate
 */
extern int16_t util_gfx_cursor_y_get();

/**
 * Helper function that gets the text bounds of a string
 *
 * @param 	str			Pointer to string to check
 * @param 	x			Starting x point
 * @param 	y			Starting y point
 * @param 	w [out]		Pointer to memory to return height of text
 * @param 	h [out]		Pointer to memory to return width of text
 */
extern void util_gfx_get_text_bounds(char *str, int16_t x, int16_t y, uint16_t *w, uint16_t *h);

/**
 * Initialize the graphics library. This only needs to be called once and immediately after
 * graphics driver is initaialized.
 */
extern void util_gfx_init();

/**
 * Invalidate the current graphics state. Call this when something is changed on the display
 * that will need a full redraw. This should be used after interrupting the user to show
 * something graphically on the screen.
 */
extern void util_gfx_invalidate();

/**
 * Validate the current graphics state. Call this when a full render of the display is complete.
 */
extern void util_gfx_validate();

/**
 * Returns if the graphics state is still considered valid.
 *
 * @return 	true if state is valid
 */
extern bool util_gfx_is_valid_state();

/**
 * Handler for scheduler event that occurs when the display is inverted.
 */
extern void util_gfx_inverted_schedule_handler(void *p_data, uint16_t length);


/**
 * Print a single char at the current location, in the current color
 *
 * @param letter
 */
void util_gfx_print_char(char letter, SSD1306_COLOR color);

/**
 * Prints a string of text to the display at the current cursor position. Text will
 * wrap if it reaches the end of the current cursor area. Text will not be printed
 * outside of the current cursor area. The current color will be used to render the
 * text as well.
 *
 * @param	text			Pointer to character array of text to print.
 */
extern void util_gfx_print(const char *text, SSD1306_COLOR color);

/**
 * Sets the current cursor position for the text
 *
 * @param	x				X coordinate to print at
 * @param	y				Y coordinate to print at
 */
extern void util_gfx_set_cursor(int16_t x, int16_t y);

/**
 * Sets the font to use for printing text
 *
 * @param	font			Font to use. Use FONT_SMALL, FONT_MEDIUM, or FONT_LARGE
 */
extern void util_gfx_set_font(uint8_t font);

/**
 *	Sets a single pixel to a given color
 *
 *	@param	x				X coordinate to set
 *	@param	y				Y coordinate to set
 *	@param	color			RGB565BE color to set the pixel to
 */
extern void util_gfx_set_pixel(int16_t x, int16_t y, SSD1306_COLOR color);

/**
 * Enable or disable automatic word wrapping when outside the viewport
 *
 * @param wrap True to enable word wrapping
 */
extern void util_gfx_set_wrap(bool wrap);

#endif /* UTIL_GFX_H_ */
