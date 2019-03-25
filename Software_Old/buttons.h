/*
 * @file buttons.h
 *
 * @date March 12, 2019
 * @author lithochasm
 *
 * Utility functions for buttons.
 *
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

#define BUTTON_PRESSED 	0
#define BUTTON_RELEASED 1
#define BUTTON_DEBOUNCE_MS		15
#define BUTTON_LONG_PRESS_MS	200
#define LONG_PRESS_MASK		0x8000

uint8_t getButton(bool waitForLongPress);
bool isButtonDown(int button);

#endif // BUTTONS_H_