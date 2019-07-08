/*
 * @file utility.h
 *
 * @date Jul 24, 2017
 * @author hamster
 *
 * Utility functions
 *
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#define BUTTON_PRESSED 	0
#define BUTTON_RELEASED 1
#define BUTTON_DEBOUNCE_MS		15


#define POLYNOM		0x0801

enum {
    BUTTON_D_PRESSED,
    BUTTON_C_PRESSED,
    BUTTON_Z_PRESSED,
    BUTTON_I_PRESSED,
    BUTTON_A_PRESSED,
    ENC_SW_PRESSED
};

uint16_t calcCRC(uint8_t *data, uint8_t len);
uint16_t crc16(uint16_t crcValue, uint8_t newByte);


uint8_t getButton(void);
bool isButtonDown(int button);
void pauseUntilPress(int button);


APP_TIMER_DEF(sysTickID);
void sysTickStart(void);
void sysTickHandler(void * p_context);
uint32_t getSystick(void);

uint32_t millis_elapsed(uint32_t currentMillis, uint32_t previousMillis);
uint32_t millis();

uint8_t getFiles(char files[][9], char *path, uint8_t fileMax);

#endif /* UTILITY_H_ */
