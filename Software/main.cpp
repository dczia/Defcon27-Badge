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
uint8_t mode;
bool DButtonPressed;
bool CButtonPressed;
bool ZButtonPressed;
bool IButtonPressed;
bool AButtonPressed;
bool encSwPressed;

    // Setup for LED modes 
    bool status = false;
    bool cylonGoDown = false;
    int cylonCurLED = LED_D;

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

    // Set up buttons
    button_init();
    DButtonPressed = false;
    CButtonPressed = false;
    ZButtonPressed = false;
    IButtonPressed = false;
    AButtonPressed = false;
    encSwPressed   = false;

    log_init();

    // Timers
    app_timer_init();

    // Setup I2C
    twi_master_init();

    // LEDs
    leds = new LED();

    // Setup the audio
    audio = new Audio();

    // Play a sequence so we kill time while the SSD1306 Boots
    startup_sequence();

    // Setup the OLED display
    SSD1306_display();

    // Setup the TOF
    TOF = new VL6180X();
    TOF->begin();

    // Setup the battery monitor
    adc = new ADC();

    // BLE
    ble_stack_init();
    scan_start();

    // Init the random number generator
    nrf_drv_rng_init(nullptr);

    // Configure the systick
    sysTickStart();
    // Setup the BLE
    advertising_setUser("test");
    ble_adv_start();

    // Setup TOF range variables
    uint8_t range1 = 0, range2 = 0;
    uint8_t status1= 0, status2 = 0;
    printf("Looping...\n");



    char display2[20];
    char display1[20];
    uint8_t counter = 0;
    uint8_t mode = 0;

    while(true) {
        if(mode == 1){
            range1 = tof_pitch(range1);
            range2 = tof_volume(range2);
            snprintf(display2, 20, "%03dmm  %03dmm", range2, range1);
        }else if(mode == 2){
            range1 = tof_pitch(range1);
            audio->setVolume(127);
            snprintf(display2, 20, "Fixed  %03dmm", range1);
        }else if(mode == 3){
            snprintf(display2, 20, "HOLDING", range1);
        }else{   // Default mode the badge boots into
            audio_off();  // STFU
            led_theramin();  // Enables LED Thearamin Mode
            snprintf(display2, 20, "LED Mode");
            //snprintf(display2, 20, "%03dmm  %03dmm", LEDrange2, LEDrange1);

        }
        snprintf(display1, 20, "%d %dv", counter++, adc->getBatteryVoltage());
        SSD1306_clearDisplay();
        util_gfx_set_cursor(10, 1);
        util_gfx_print(display1, COLOR_WHITE);
        util_gfx_set_cursor(10, 12);
        util_gfx_print(display2, COLOR_WHITE);

        SSD1306_display();
        mode = button_handler(mode);
        if(mode > 3){
            mode = 0;
        }
        nrf_delay_ms(10);  // Do we really need this?

    }
}

void audio_off(){
    audio->enable(false); // turn off amp
    audio->disableTimer(); // disable timer1
    audio->setPWM0Ch0Value(0); // turn off PWM0 ch0
}

uint8_t button_handler(uint8_t mode){
    uint8_t button = getButton();

    nrf_gpio_pin_set(LED_D);
    nrf_gpio_pin_set(LED_C);
    nrf_gpio_pin_set(LED_Z);
    nrf_gpio_pin_set(LED_I);
    nrf_gpio_pin_set(LED_A);

    if (button & (1 << BUTTON_D_PRESSED)) {
        if (!DButtonPressed) {
            pixels->setColor(0, { 0, 0, 64 });
            pixels->setColor(1, { 0, 0, 64});
            pixels->show();
            while(getButton() == BUTTON_D_PIN);

            leds->set(LED_D, ON);
            printf("Button D Pressed\n");

            DButtonPressed = true;
        }
    } else {
        if (DButtonPressed) {
            pixels->setColor(0, { 0, 0, 0 });
            pixels->setColor(1, { 0, 0, 0});
            pixels->show();

            leds->set(LED_D, OFF);
            printf("Button D Released\n");

            DButtonPressed = false;
        }
    }

    if (button & (1 << BUTTON_C_PRESSED)) {
        if (!CButtonPressed) {
            leds->set(LED_C, ON);
            printf("Button C Pressed\n");

            CButtonPressed = true;
        }
    } else {
        if (CButtonPressed) {
            leds->set(LED_C, OFF);
            printf("Button C Released\n");

            CButtonPressed = false;
        }
    }

    if (button & (1 << BUTTON_Z_PRESSED)) {
        if (!ZButtonPressed) {
            leds->set(LED_Z, ON);
            printf("Button Z Pressed\n");

            ZButtonPressed = true;
        }
    } else {
        if (ZButtonPressed) {
            leds->set(LED_Z, OFF);
            printf("Button Z Released\n");

            ZButtonPressed = false;
        }
    }

    if (button & (1 << BUTTON_I_PRESSED)) {
        if (!IButtonPressed) {
            leds->set(LED_I, ON);
            printf("Button I Pressed\n");

            IButtonPressed = true;
        };
    } else {
        if (IButtonPressed) {
            leds->set(LED_I, OFF);
            printf("Button I Released\n");

            IButtonPressed = false;
        }
    }

    if (button & (1 << BUTTON_A_PRESSED)) {
        if (!AButtonPressed) {
            leds->set(LED_A, ON);
            printf("Button A Pressed\n");

            AButtonPressed = true;
        }
    } else {
        if (AButtonPressed) {
            leds->set(LED_A, OFF);
            printf("Button A Released\n");

            AButtonPressed = false;
        }
    }

    if (button & (1 << ENC_SW_PRESSED)) {
        if (!encSwPressed) {
            printf("Encoder Switch Pressed\n");
            mode++;
            encSwPressed = true;
        }
    } else {
        if (encSwPressed) {
            printf("Encoder Switch Released\n");

            encSwPressed = false;
        }
    }
    return mode;
}

void startup_sequence(){
    // Pop pop!
    audio->enable(true);
    leds->set(LED_D, ON);
    //audio->beep(10, 200);
    nrf_delay_ms(50);
    leds->set(LED_D, OFF);
    leds->set(LED_C, ON);
    //audio->beep(25, 400);
    nrf_delay_ms(50);
    leds->set(LED_C, OFF);
    leds->set(LED_Z, ON);
    //audio->beep(75, 600);
    nrf_delay_ms(50);
    leds->set(LED_Z, OFF);
    leds->set(LED_I, ON);
    //audio->beep(150, 800);
    nrf_delay_ms(50);
    leds->set(LED_I, OFF);
    leds->set(LED_A, ON);
    //audio->beep(200, 1000);
    //audio->enable(false);
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
    audio->initPWM0();
    audio->setPWM0Ch0Value(0);
    // audio->setPWM0Ch1Value(0);
    audio->setVolume(0);

    // timer init + test
    audio->initTimer1();

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

   
 
}

uint8_t tof_pitch(uint8_t prevRange){
    // read right sensor
    uint8_t range = TOF->readRange(TOF_SENSOR1);

    // Enable this to smooth the sampling a bit
    range = uint8_t(uint16_t(range + prevRange) >> 1);

    // set pitch with right sensor
    // range_2mm splits range into 2mm chunks
    // +1 prevents divide by 0 x_x
    uint8_t range_2mm     = ( range     >> 1 ) + 1;
    uint8_t prevRange_2mm = ( prevRange >> 1 ) + 1;

    // only change timer/pitch if hand position changes
    if (range_2mm == prevRange_2mm) {
        return prevRange;
    }
    // check if range is at max (255mm)
    if (range == 255) {
        audio_off();
        return range;
    }

    // if no, get note that corresponds hand position
    // calculate hand position
    // TOF sensor dies off around 17cm
    uint8_t handPosition = 0;
    if (range_2mm > 57) { // about 11cm...could be higher
        handPosition = 48;
    } else if (range_2mm >= 10) { // 2cm of padding close to the sensor
        handPosition = range_2mm - 10; // start hand position calc at 2cm
    }

    bool halfNote = handPosition & 0x1; // check if this is an inbetween note
    uint8_t noteIndex = handPosition >> 1; // divide handpositin by 2 to get note
    uint8_t octave = 5, octavesUp = 0;

    // if note is beyond current octave, move to next octave
    if (noteIndex > 12) {
        noteIndex -= 12;
        octavesUp++;
    }

    // get period from note lookup
    uint32_t newPeriod = 0;
    if (halfNote) {
        // get note between (note + (next_note))/ 2
        newPeriod = (notes[octave + octavesUp][noteIndex] + notes[octave + octavesUp][noteIndex + 1]) >> 1;
    } else {
        newPeriod = notes[octave + octavesUp][noteIndex];
    }

    // set new timer1 period
    audio->setTimerWithPeriod_us(newPeriod);
    // make sure timer1 is enabled
    if (!audio->isEnabled()) {
        audio->enable(true);
    }
    // store current hand distance
    return range;
}

uint8_t tof_volume(uint8_t prevRange){
    // read left sensor
    uint8_t range = TOF->readRange(TOF_SENSOR2);

    uint8_t range_cm = range / 10 + 1;
    uint8_t prevRange_cm = prevRange / 10 + 1;

    if (range_cm == prevRange_cm){
        return prevRange;
    }

    if (range == 255) {
        audio->setVolume(0);
        return range;
    }

    uint8_t newVolume = range_cm << 1;
    if (newVolume > 63) {
        newVolume = 63;
    }

    audio->setVolume(newVolume);
    return range;
}

void led_theramin() {

            uint8_t LEDrange1 = TOF->readRange(TOF_SENSOR1);
            uint8_t LEDrange2 = TOF->readRange(TOF_SENSOR2);
            if (LEDrange1 > 190) { LEDrange1 = 190; }  // Rounding as TOF sensor will not measure much past here
            if (LEDrange2 > 190) { LEDrange2 = 190; }

            //output = output_start + ((output_end - output_start) / (input_end - input_start)) * (input - input_start)
            uint8_t output1 = 255 + ((0 - 255) / (190 - 8)) * (LEDrange1 - 8) + -72; //ReMap Sensor Values
            uint8_t output2 = 255 + ((0 - 255) / (190 - 8)) * (LEDrange2 - 8) + -72; //ReMap Sensor Values
            uint8_t outhalf = output1 / 4;
            printf("Output Range 1  = %d\n", output2);  //Debug
            printf("Output Range 2  = %d\n", output1);
            pixels->setColor(0, {output2, 0, output2});
            pixels->setColor(1, {0, outhalf, output1});
            pixels->show();
            led_walk();

}


void led_handler_blink(void *p_context) {
    status = !status;

    if (status == true){
    leds->set(LED_D, ON);
    leds->set(LED_C, ON);
    leds->set(LED_Z, ON);
    leds->set(LED_I, ON);
    leds->set(LED_A, ON);
    }
    else {
    leds->set(LED_D, OFF);
    leds->set(LED_C, OFF);
    leds->set(LED_Z, OFF);
    leds->set(LED_I, OFF);
    leds->set(LED_A, OFF);
    }
}
    void led_handler_cylon(void *p_context) {

        
            if(!cylonGoDown && cylonCurLED > 0){
                leds->set((LEDS)(cylonCurLED - 1), OFF);
            }
            if(cylonGoDown && cylonCurLED < 4){
                leds->set((LEDS)(cylonCurLED + 1), OFF);
            }
            leds->set((LEDS)cylonCurLED, ON);


            if(cylonGoDown){
                if(cylonCurLED == LED_D){
                    cylonGoDown = false;
                }
                else{
                    cylonCurLED -= 1;
                }
            }
            else {
                if (cylonCurLED == LED_A) {
                    cylonGoDown = true;
                }
                else{
                    cylonCurLED += 1;
                }
            }
        
    }

void led_walk(){
 
    // Create a timer, in repeated mode, and register the callback
    app_timer_create(&m_led_timer_id, APP_TIMER_MODE_REPEATED, led_handler_cylon);

    // Start the timer, fire every 100ms
    app_timer_start(m_led_timer_id, APP_TIMER_TICKS(100), nullptr);                
            
}



