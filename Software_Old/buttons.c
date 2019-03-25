/**
 * @brief Determine if a button is being pressed.
 * @param waitForLongPress true if we want to wait to see if a button is long pressed
 * @return Button pressed, with long press mask if long pressed
 *
 * Buttons are debounced for a few milliseconds
 * Buttons held down for longer than the defined long press duration are sent
 * as a long press button
 */

#include "common.h"

uint8_t getButton(bool waitForLongPress){

	int longPress = 0;
	int button = 0;

	if(isButtonDown(USER_BUTTON_1)){
		// Debounce
		nrf_delay_ms(BUTTON_DEBOUNCE_MS);
		if(isButtonDown(USER_BUTTON_1)) {
            button = USER_BUTTON_1;
            if (waitForLongPress){
                while (isButtonDown(USER_BUTTON_1)) {
                    nrf_delay_ms(1);
                    longPress++;
                    if (longPress > BUTTON_LONG_PRESS_MS) {
                        break;
                    }
                }
            }
		}
	}

    if(isButtonDown(USER_BUTTON_2)){
		// Debounce
		nrf_delay_ms(BUTTON_DEBOUNCE_MS);
		if(isButtonDown(USER_BUTTON_2)) {
            button = USER_BUTTON_2;
            if (waitForLongPress){
                while (isButtonDown(USER_BUTTON_2)) {
                    nrf_delay_ms(1);
                    longPress++;
                    if (longPress > BUTTON_LONG_PRESS_MS) {
                        break;
                    }
                }
            }
		}
	}
	


	if(longPress > BUTTON_LONG_PRESS_MS){
		button |= LONG_PRESS_MASK;
	}

	return button;

}

/**
 * @brief Detects if a button is being held down
 * @param button Is this button pressed down?
 * @return true if button is pressed down
 */
bool isButtonDown(int button){

	if(nrf_gpio_pin_read(button) == BUTTON_PRESSED){
		return true;
	}

	return false;
}