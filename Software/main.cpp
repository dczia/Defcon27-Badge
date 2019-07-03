/**
 *
 * @file main.c
 *
 * @date May 24, 2017
 * @author hamster, rehr
 *
 * @brief This is the entry point for the DC801 DC26 party badge
 *
 */

#include <stdint.h>
#include "common.h"

// Includes for our app
#include "utility.h"
#include "main.h"
#include "modules/audio.h"
#include "modules/ble.h"
#include "modules/sd.h"
#include "modules/adc.h"
#include "modules/uart.h"
#include "modules/i2c.h"
#include "modules/led.h"
#include "modules/gfx.h"
#include "modules/drv_ssd1306.h"
#include "modules/VL6180X.h"
#include "modules/ws2812b.h"

ADC *adc;
VL6180X *TOF;
Audio *audio;
LED *leds;
WS2812S *pixels;



/**
 * Initialize the buttons
 */
static void button_init(){
    // Setup the buttons
    nrf_gpio_cfg_input(BUTTON_D_PIN, NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(BUTTON_C_PIN, NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(BUTTON_Z_PIN, NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(BUTTON_I_PIN, NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(BUTTON_A_PIN, NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(ENC_SW_PIN, NRF_GPIO_PIN_NOPULL);
}


/**
 * Initialize the logging backend for logging over JTAG
 */
static void log_init(){
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}


/**
 * @brief Main app
 * @return Not used
 */
int main(){

    // Setup the system


    button_init();
    log_init();

    // Timers
    app_timer_init();

    // Setup I2C
    twi_master_init();

    // LEDs
    leds = new LED();

    // Setup the audio
    audio = new Audio();

    // Pop pop!
    audio->enable(true);
    leds->set(LED_D, ON);
    audio->beep(10, 200);
    nrf_delay_ms(50);
    leds->set(LED_D, OFF);
    leds->set(LED_C, ON);
    audio->beep(25, 400);
    nrf_delay_ms(50);
    leds->set(LED_C, OFF);
    leds->set(LED_Z, ON);
    audio->beep(75, 600);
    nrf_delay_ms(50);
    leds->set(LED_Z, OFF);
    leds->set(LED_I, ON);
    audio->beep(150, 800);
    nrf_delay_ms(50);
    leds->set(LED_I, OFF);
    leds->set(LED_A, ON);
    audio->beep(200, 1000);
    audio->enable(false);
    nrf_delay_ms(50);
    leds->set(LED_A, OFF);

    // RGB LEDs
    pixels = new WS2812S(2);

    PIXEL colors[] = { { 0, 64, 96 }, { 128, 160, 192}};
    pixels->setColor(0, colors[0]);
    pixels->setColor(1, colors[1]);
    pixels->show();

    bool goDown = false;
    int curLED = LED_D;
    for(int i = 0; i < 20; i++){
        if(!goDown && curLED > 0){
            leds->set((LEDS)(curLED - 1), OFF);
        }
        if(goDown && curLED < 4){
            leds->set((LEDS)(curLED + 1), OFF);
        }
        leds->set((LEDS)curLED, ON);
        nrf_delay_ms(40);

        if(goDown){
            if(curLED == LED_D){
                goDown = false;
            }
            else{
                curLED -= 1;
            }
        }
        else {
            if (curLED == LED_A) {
                goDown = true;
            }
            else{
                curLED += 1;
            }
        }
        colors[0].red += 32;
        colors[0].green += 32;
        colors[0].blue += 32;
        colors[1].red += 32;
        colors[1].green += 32;
        colors[1].blue += 32;
        pixels->setColor(0, colors[0]);
        pixels->setColor(1, colors[1]);
        pixels->show();
    }

    leds->allOff();
    pixels->setColor(0, {0, 1, 0});
    pixels->setColor(1, {0, 1, 0});
    pixels->show();

    // pwm init + test
    audio->initPWM();
    audio->setPWMCh0Value(0);
    audio->setPWMCh1Value(0);

    // timer init + test
    audio->initTimer();
    // audio->setTimerWithPeriod_ms(500/32);

    // Delay for the OLED screen to boot and be ready for commands
    // It needs about 2.5 seconds for reliable operation from a cold power on
    // The above stuff takes up enough time, but make sure you account for this if you change the
    // power on sequence

    // Setup the display
    SSD1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS, false);
    SSD1306_setRotation(ROTATION_180);

    util_gfx_init();

    util_gfx_set_font(FONT_MONO55_8PT);
    util_gfx_set_cursor(10, 1);
    util_gfx_print("DCZia Test", COLOR_WHITE);
    util_gfx_set_cursor(10, 12);
    util_gfx_print("defcon 27", COLOR_WHITE);

    SSD1306_display();

    // Setup the TOF
    TOF = new VL6180X();
    TOF->begin();

    // Setup the battery monitor
    adc = new ADC();

    // Setup the UART

    //util_sd_init();

    // BLE
    //gap_params_init();
    ble_stack_init();
    scan_start();

    // Init the random number generator
    nrf_drv_rng_init(nullptr);

    // Boot! Boot! Boot!

    // printf goes to the RTT_Terminal.log after you've fired up debug.sh
    // This assumes you have a Segger attached
    printf("Booted!\n");

    // Configure the systick
    sysTickStart();

    // Setup the BLE
    advertising_setUser("test");

    ble_adv_start();

    //printf("Lux1: %f  Lux2: %f\n", TOF.readLux(TOF_SENSOR1, VL6180X_ALS_GAIN_5), TOF.readLux(TOF_SENSOR2, VL6180X_ALS_GAIN_5));

    uint8_t range1 = 0, range2 = 0;
    uint8_t status1= 0, status2 = 0;
    uint8_t range1_cm = 0, range2_cm = 0;
    uint8_t prevRange1_cm = 26, prevRange2_cm = 26; // nothing in front defaults to 25.5cm

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

    printf("Looping...\n");

    char display2[20];
    char display1[20];
    uint8_t counter = 0;

    while(true) {

        // set frequency/period with sensor1 (right sensor)
        range1 = TOF->readRange(TOF_SENSOR1);
        //status1 = TOF->readRangeStatus(TOF_SENSOR1);
        range1_cm = range1 / 10 + 1;
        if (range1_cm != prevRange1_cm) {
            if (range1_cm == 26) {
                audio->disableTimer();
                audio->setPWMCh0Value(0);
            } else {
                audio->setTimerWithPeriod_ms(500/32/range1_cm);
            }
            prevRange1_cm = range1_cm;
        }

        // set volume with sensor2 (left sensor)
        range2 = TOF->readRange(TOF_SENSOR2);
        //status2 = TOF->readRangeStatus(TOF_SENSOR2);
        range2_cm = range2 / 10 + 1;
        if (range2_cm != prevRange2_cm) {
            if (range2_cm == 26) {
                audio->setPWMCh1Value(0);
            } else {
                uint16_t LEDValue = range2_cm * 60;
                if (LEDValue > 1023) { LEDValue = 1023; }
                audio->setPWMCh1Value(LEDValue);
            }
            prevRange2_cm = range2_cm;
        }

        snprintf(display2, 20, "%03dmm  %03dmm", range2, range1);
        snprintf(display1, 20, "%d %dv", counter++, adc->getBatteryVoltage());

        SSD1306_clearDisplay();
        util_gfx_set_cursor(10, 1);
        util_gfx_print(display1, COLOR_WHITE);
        util_gfx_set_cursor(10, 12);
        util_gfx_print(display2, COLOR_WHITE);

        SSD1306_display();

        //printf("Range1: %d Status1: %d Range2: %d Status2: %d\n", range1, status1, range2, status2);

        uint8_t button = getButton();

        if(button != 0){
            printf("Button was %d\n", button);
        }

        nrf_gpio_pin_set(LED_D);
        nrf_gpio_pin_set(LED_C);
        nrf_gpio_pin_set(LED_Z);
        nrf_gpio_pin_set(LED_I);
        nrf_gpio_pin_set(LED_A);

        if(button == BUTTON_D_PIN){
            pixels->setColor(0, { 0, 0, 64 });
            pixels->setColor(1, { 0, 0, 64});
            pixels->show();
            while(getButton() == BUTTON_D_PIN);

            pixels->setColor(0, { 0, 0, 0 });
            pixels->setColor(1, { 0, 0, 0});
            pixels->show();

            leds->set(LED_D, TOGGLE);
            printf("Button D\n");
        }
        if(button == BUTTON_C_PIN){
            leds->set(LED_C, TOGGLE);
            printf("Button C\n");
        }
        if(button == BUTTON_Z_PIN){
            leds->set(LED_Z, TOGGLE);
            printf("Button Z\n");
        }
        if(button == BUTTON_I_PIN){
            leds->set(LED_I, TOGGLE);
            printf("Button I\n");
        }
        if(button == BUTTON_A_PIN){
            leds->set(LED_A, TOGGLE);
            printf("Button A\n");
        }

        nrf_delay_ms(10);

    }
#pragma clang diagnostic pop

}

