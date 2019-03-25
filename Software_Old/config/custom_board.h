/**
 *
 * @file custom_board.h
 *
 * @date March 17, 2019
 * @author lithochasm
 *
 * This is the custom board defines for the LEDs and stuff on the DC27 badge
 *
 */

//#ifndef DC801_H
//#define DC801_H

#ifdef __cplusplus
extern "C" {
#endif

// LEDs definitions

// Low is active
#define LEDS_ACTIVE_STATE 0

#define VOLTAGE_SENSE		4

// LED Pins
/*
#define LED_LEVEL_UP_0     	30
#define LED_LEVEL_UP_1     	29
#define LED_LEVEL_UP_2     	28
#define LED_LEVEL_UP_3		25

#define LED_POWER_UP_0  	2
#define LED_POWER_UP_1      1
#define LED_POWER_UP_2      0
#define LED_POWER_UP_3		31

// Arrays to make iteration easy
// The Nordic SDK uses LEDS_LIST internally
#define LEDS_LIST { \
		LED_LEVEL_UP_0, 	\
		LED_LEVEL_UP_1, 	\
		LED_LEVEL_UP_2,	\
		LED_LEVEL_UP_3,	\
		LED_POWER_UP_0,	\
		LED_POWER_UP_1,	\
		LED_POWER_UP_2,	\
		LED_POWER_UP_3	}

#define LEDS_NUMBER    		8

*/

// Push buttons

#define BUTTONS_NUMBER 4


#define USER_BUTTON_1		11
#define USER_BUTTON_2      	12
#define USER_BUTTON_3       24
#define USER_BUTTON_4       25


#define LONG_PRESS_MASK		0x8000

/*
#define USER_BUTTON_1    	(11  | LONG_PRESS_MASK)
#define USER_BUTTON_2   (12  | LONG_PRESS_MASK)
#define USER_BUTTON_3   (24 | LONG_PRESS_MASK)
#define USER_BUTTON_4  (25 | LONG_PRESS_MASK)
*/

/*

#define BUTTONS_LIST { \
		USER_BUTTON_UP,	\
		USER_BUTTON_DOWN,	\
		USER_BUTTON_LEFT,	\
		USER_BUTTON_RIGHT,	\
		USER_BUTTON_A,		\
		USER_BUTTON_B	}

// Low is active
#define BUTTONS_ACTIVE_STATE	0
// Don't need a pull up
#define BUTTON_PULL				0

#define BSP_BUTTON_0   USER_BUTTON_UP
#define BSP_BUTTON_1   USER_BUTTON_DOWN
#define BSP_BUTTON_2   USER_BUTTON_LEFT
#define BSP_BUTTON_3   USER_BUTTON_RIGHT


#define RX_PIN_NUMBER  6
#define TX_PIN_NUMBER  8
#define CTS_PIN_NUMBER NRF_UART_PSEL_DISCONNECTED
#define RTS_PIN_NUMBER NRF_UART_PSEL_DISCONNECTED
#define HWFC           false

#define BUTTON_PRESSED 	0

// Just one speaker
#define SPEAKER		24

// LCD
// The SPI ports are defined in the config/sdk_config.h
#define LCD_RESET	9
#define LCD_LIGHT	5

// Clock
// Low frequency clock source to be used by the SoftDevice
// We're using the internal RC osc
#define NRF_CLOCK_LFCLKSRC      {.source        = NRF_CLOCK_LF_SRC_RC, \
                                 .rc_ctiv       = 16, \
                                 .rc_temp_ctiv  = 2, \
                                 .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM}


#ifdef __cplusplus
}
#endif

#endif // DC801_H
*/