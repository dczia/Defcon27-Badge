/**
 *
 * @file main.c
 *
 * @date March 11, 2019
 * @author lithochasm
 *
 * @brief This is the entry point for the DCZia Lazer Theramin Synthesizer Badge
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include "boards.h"
#include <buttons.h>
#include "common.h"



/**
 * @brief Function for application main entry.
 */
int main(void)
{
    /* Configure board. */
    bsp_board_init(BSP_INIT_LEDS);
    bsp_board_init(BSP_INIT_BUTTONS);



while(true){
        if(getButton(false)){
            // User pressed a button
            return 0;
        }
        nrf_delay_ms(25);
        if(getButton(true)){
            bsp_board_led_invert(0);
            bsp_board_led_invert(2);
            
        }
    }


}
/**
 *@}
 **/
