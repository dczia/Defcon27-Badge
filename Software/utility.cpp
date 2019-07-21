/*
 * @file utility.c
 *
 * @date Jul 24, 2017
 * @author hamster
 *
 *  Utility functions
 *
 */

#include "common.h"
#include "utility.h"

volatile static uint32_t systick = 0;

/**
 * Start the local time reference
 */
void sysTickStart(void){
    systick = 0;
    app_timer_create(&sysTickID, APP_TIMER_MODE_REPEATED, sysTickHandler);
    app_timer_start(sysTickID, APP_TIMER_TICKS(1000), NULL);
}

/**
 * @return number of seconds since we started counting time
 */
uint32_t getSystick(void){
    return systick;
}

/**
 * Every second, update the systick handler
 * @param p_context
 */
void sysTickHandler(void * p_context){
    systick++;
}


/**
 * @brief Determine if a button is being pressed.
 * @param waitForLongPress true if we want to wait to see if a button is long pressed
 * @return Button pressed, with long press mask if long pressed
 *
 * Buttons are debounced for a few milliseconds
 * Buttons held down for longer than the defined long press duration are sent
 * as a long press button
 */
uint8_t getButton(void){

    uint8_t button = 0;

    if(isButtonDown(BUTTON_D_PIN)){
        // Debounce
        nrf_delay_ms(BUTTON_DEBOUNCE_MS);
        if(isButtonDown(BUTTON_D_PIN)) {
            button |= 1 << BUTTON_D_PRESSED;
        }
    }
    if(isButtonDown(BUTTON_C_PIN)){
        // Debounce
        nrf_delay_ms(BUTTON_DEBOUNCE_MS);
        if(isButtonDown(BUTTON_C_PIN)){
            button |= 1 << BUTTON_C_PRESSED;
        }
    }
    if(isButtonDown(BUTTON_Z_PIN)){
        // Debounce
        nrf_delay_ms(BUTTON_DEBOUNCE_MS);
        if(isButtonDown(BUTTON_Z_PIN)){
            button |= 1 << BUTTON_Z_PRESSED;
        }
    }
    if(isButtonDown(BUTTON_I_PIN)){
        // Debounce
        nrf_delay_ms(BUTTON_DEBOUNCE_MS);
        if(isButtonDown(BUTTON_I_PIN)){
            button |= 1 << BUTTON_I_PRESSED;
        }
    }
    if(isButtonDown(BUTTON_A_PIN)){
        // Debounce
        nrf_delay_ms(BUTTON_DEBOUNCE_MS);
        if(isButtonDown(BUTTON_A_PIN)){
            button |= 1 << BUTTON_A_PRESSED;
        }
    }
    if(isButtonDown(ENC_SW_PIN)){
        // Debounce
        nrf_delay_ms(BUTTON_DEBOUNCE_MS);
        if(isButtonDown(ENC_SW_PIN)){
            button |= 1 << ENC_SW_PRESSED;
        }
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


/**
 * @brief Pause the program until a button has been pressed and released
 * @param button Wait on this button
 */
void pauseUntilPress(int button){

    while(true){
        if(nrf_gpio_pin_read(button) == BUTTON_PRESSED){
            // Debounce
            nrf_delay_ms(button);
            if(nrf_gpio_pin_read(button) == BUTTON_PRESSED){
                while(nrf_gpio_pin_read(button) == BUTTON_PRESSED);
                return;
            }
        }
    }
}



/**
 * Get a list of files on a path
 * @param files
 * @param path
 * @param fileMax
 * @return
 */
uint8_t getFiles(char files[][9], char *path, uint8_t fileMax){

    FRESULT ff_result;
    DIR dir;
    FILINFO fno;

    ff_result = f_opendir(&dir, path);
    if (ff_result) {
        printf("Can't open extras\n");
        return 0;
    }

    uint8_t counter = 0;
    for (uint8_t i = 0; i < fileMax; i++) {
        ff_result = f_readdir(&dir, &fno);                   /* Read a directory item */
        if (ff_result != FR_OK || fno.fname[0] == 0) {
            break;  /* Break on error or end of dir */
        }
        if ((fno.fattrib & AM_DIR)) {
            // Ignore subdirs
        }
        else{
            char *ext = strrchr(fno.fname, '.') + 1;
            if (strcmp(ext, "RAW") == 0){
                // Add the file
                memcpy(&files[counter++], fno.fname, ext - fno.fname - 1);
            }
        }
    }
    f_closedir(&dir);

    return counter;
}

/**
 * Calculate the CRC on a chunk of memory
 * @param data
 * @param len
 * @return
 */
uint16_t calcCRC(uint8_t *data, uint8_t len){
    uint16_t crc;
    uint8_t aux = 0;

    crc = 0x0000;

    while (aux < len){
        crc = crc16(crc, data[aux]);
        aux++;
    }

    return (crc);
}

/**
 * Calculate the crc16 of a value
 * @param crcValue
 * @param newByte
 * @return
 */
uint16_t crc16(uint16_t crcValue, uint8_t newByte){
    uint8_t i;

    for (i = 0; i < 8; i++) {

        if (((crcValue & 0x8000) >> 8) ^ (newByte & 0x80)){
            crcValue = (crcValue << 1)  ^ POLYNOM;
        }else{
            crcValue = (crcValue << 1);
        }

        newByte <<= 1;
    }

    return crcValue;
}
