/**
 *
 * @file custom_board.h
 *
 * @date June 15 2019
 * @author hamster
 *
 * This is the custom board defines for the LEDs and etc
 *
 */

#ifndef DCZIA_H
#define DCZIA_H

// Setting to zero disables the Nordic initializers
#define LEDS_NUMBER    		0
#define BUTTONS_NUMBER      0


/***
 *      _     _____ ____
 *     | |   | ____|  _ \ ___
 *     | |   |  _| | | | / __|
 *     | |___| |___| |_| \__ \
 *     |_____|_____|____/|___/
 *
 */
#define LED_D_PIN     	23
#define LED_C_PIN       20
#define LED_Z_PIN       21
#define LED_I_PIN       16
#define LED_A_PIN       14

/***
 *      ____        _   _
 *     | __ ) _   _| |_| |_ ___  _ __  ___
 *     |  _ \| | | | __| __/ _ \| '_ \/ __|
 *     | |_) | |_| | |_| || (_) | | | \__ \
 *     |____/ \__,_|\__|\__\___/|_| |_|___/
 *
 */
#define BUTTON_D_PIN    24
#define BUTTON_C_PIN    22
#define BUTTON_Z_PIN    19
#define BUTTON_I_PIN    17
#define BUTTON_A_PIN    15
#define ENC_SW_PIN      5
#define ENC_A_PIN       9
#define ENC_B_PIN       (32 + 15)
#define ENC_C_PIN       7

#define BUTTON_PRESSED 	        0
#define BUTTONS_ACTIVE_STATE    0

/***
 *      _   _   _    ____ _____
 *     | | | | / \  |  _ \_   _|
 *     | | | |/ _ \ | |_) || |
 *     | |_| / ___ \|  _ < | |
 *      \___/_/   \_\_| \_\|_|
 *
 */
#define RX_PIN          6
#define TX_PIN          8
#define CTS_PIN         NRF_UART_PSEL_DISCONNECTED
#define RTS_PIN         NRF_UART_PSEL_DISCONNECTED
#define HWFC           false

/***
 *      _ ____
 *     (_)___ \ ___
 *     | | __) / __|
 *     | |/ __/ (__
 *     |_|_____\___|
 *
 */
#define I2C_SDA_PIN     25
#define I2C_SCL_PIN     26

/***
 *      ____  ____ ___
 *     / ___||  _ \_ _|
 *     \___ \| |_) | |
 *      ___) |  __/| |
 *     |____/|_|  |___|
 *
 */
#define SPI_MISO_PIN    1
#define SPI_SCK_PIN     2
#define SPI_MOSI_PIN    (32 + 12)
#define SPI_SD_CS_PIN   (32 + 13)

/***
 *      ____                          ____
 *     |  _ \ __ _ _ __   __ _  ___  / ___|  ___ _ __  ___  ___  _ __
 *     | |_) / _` | '_ \ / _` |/ _ \ \___ \ / _ \ '_ \/ __|/ _ \| '__|
 *     |  _ < (_| | | | | (_| |  __/  ___) |  __/ | | \__ \ (_) | |
 *     |_| \_\__,_|_| |_|\__, |\___| |____/ \___|_| |_|___/\___/|_|
 *                       |___/
 */
#define TOF1_SHUTDOWN_PIN   11
#define TOF2_SHUTDOWN_PIN   27

/***
 *         _             _ _
 *        / \  _   _  __| (_) ___
 *       / _ \| | | |/ _` | |/ _ \
 *      / ___ \ |_| | (_| | | (_) |
 *     /_/   \_\__,_|\__,_|_|\___/
 *
 */
#define AUDIO_SHUTDOWN_PIN  (32 + 14)
#define AUDIO_HP_DETECT_PIN 13
#define AUDIO_PWM_PIN       3

/***
 *      ____  _          _
 *     |  _ \(_)_  _____| |___
 *     | |_) | \ \/ / _ \ / __|
 *     |  __/| |>  <  __/ \__ \
 *     |_|   |_/_/\_\___|_|___/
 *
 */
#define PIXELS_DOUT_PIN   (32 + 9)

/***
 *      ____  _           _
 *     |  _ \(_)___ _ __ | | __ _ _   _
 *     | | | | / __| '_ \| |/ _` | | | |
 *     | |_| | \__ \ |_) | | (_| | |_| |
 *     |____/|_|___/ .__/|_|\__,_|\__, |
 *                 |_|            |___/
 */
// It's a 128x32 OLED display with a SSD1306 driver
#define SSD1306_128_32

/***
 *     __     __
 *     \ \   / /__  ___ _ __  ___  ___
 *      \ \ / / __|/ _ \ '_ \/ __|/ _ \
 *       \ V /\__ \  __/ | | \__ \  __/
 *        \_/ |___/\___|_| |_|___/\___|
 *
 */
#define VOLTAGE_SENSE_PIN		NRF_SAADC_INPUT_AIN2    // P0.04

//// Clock
//// Low frequency clock source to be used by the SoftDevice
//// We're using the internal RC osc
/*#define NRF_CLOCK_LFCLKSRC      {.source        = NRF_CLOCK_LF_SRC_RC, \
                                 .rc_ctiv       = 16, \
                                 .rc_temp_ctiv  = 2, \
                                 .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM} */



#endif // DCZIA_H
