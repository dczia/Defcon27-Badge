 /**
 *
 * @file main.c
 *
 * @date August 2, 2019
 * @author hamster, rehr, ancients, lithochasm
 *
 * @brief This is the entry point for the DCZia DC27 Theremin badge
 *
 */

#include "./main.h"
uint8_t badgeMode;

ADC *adc;
VL6180X *TOF;
Audio *audio;
LED *leds;
WS2812S *pixels;

// Variables for LED modes
bool led_status = false;
bool cylonGoDown = false;
int cylonCurLED = LED_D;
int led_mode = 1;
uint16_t led_speed;
int led_pattern_step = 1;

// Audio Variables
uint8_t octave = 5;

/**
 * Initialize the button pins
 */
static void button_init() {
    DButtonPressed = false;
    CButtonPressed = false;
    ZButtonPressed = false;
    IButtonPressed = false;
    AButtonPressed = false;
    encSwPressed = false;
    
    /* set encoder pin directions and states */
    nrf_gpio_cfg_input(BUTTON_D_PIN, NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(BUTTON_C_PIN, NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(BUTTON_Z_PIN, NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(BUTTON_I_PIN, NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(BUTTON_A_PIN, NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(ENC_SW_PIN, NRF_GPIO_PIN_NOPULL);

    /* initialize interrupt driver */
    if (nrfx_gpiote_is_init() == false) {
        if (nrf_drv_gpiote_init() != NRF_SUCCESS) {
            printf("ERROR: Initializing button interrupts!\n");
            return;
        }
    }

    /* set config for button pin interrupt */
    nrf_drv_gpiote_in_config_t pin_config = {
        NRF_GPIOTE_POLARITY_TOGGLE,  // sense
        NRF_GPIO_PIN_NOPULL,  // pull
        true,  // is_watcher
        true  // hi_accuracy
    };

    /* init and enable encoder pin A interrupt */
    nrf_drv_gpiote_in_init(BUTTON_D_PIN, &pin_config, button_handler);
    nrf_drv_gpiote_in_init(BUTTON_C_PIN, &pin_config, button_handler);
    nrf_drv_gpiote_in_init(BUTTON_Z_PIN, &pin_config, button_handler);
    nrf_drv_gpiote_in_init(BUTTON_I_PIN, &pin_config, button_handler);
    nrf_drv_gpiote_in_init(BUTTON_A_PIN, &pin_config, button_handler);
    nrf_drv_gpiote_in_init(ENC_SW_PIN, &pin_config, button_handler);
    nrf_drv_gpiote_in_event_enable(BUTTON_D_PIN, true);
    nrf_drv_gpiote_in_event_enable(BUTTON_C_PIN, true);
    nrf_drv_gpiote_in_event_enable(BUTTON_Z_PIN, true);
    nrf_drv_gpiote_in_event_enable(BUTTON_I_PIN, true);
    nrf_drv_gpiote_in_event_enable(BUTTON_A_PIN, true);
    nrf_drv_gpiote_in_event_enable(ENC_SW_PIN, true);
}


/**
 * Handler for encoder pin A change events
 */
void encoder_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {
    static bool prevPinAState = false;

    if (pin == ENC_A_PIN) {
        // grab states for pins A and B
        bool pinBState = nrfx_gpiote_in_is_set(ENC_B_PIN);
        bool pinAState = nrfx_gpiote_in_is_set(ENC_A_PIN);

        // double check for a change on pin A
        if (prevPinAState != pinAState) {
            if (prevPinAState == false) {
                if (pinBState) {
                    // counter-clockwise turn
                    // octave--;
                    // if (octave < 0) {
                    //     octave = 0;
                    // }
                } else {
                    // clockwise turn
                    // octave++;
                    // if (octave > 9) {
                    //     octave = 9; // have to stop at 9 since theremin audio spans two octaves
                    // }
                }
            }

            prevPinAState = pinAState;
        }
    }
}


/**
 * Initialize the encoder pins
 */
static void encoder_init() {
    /* set encoder pin directions and states */
    nrf_gpio_cfg_output(ENC_C_PIN);
    nrf_gpio_pin_clear(ENC_C_PIN);  // set pin C as 0V
    nrf_gpio_cfg_input(ENC_A_PIN, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(ENC_B_PIN, NRF_GPIO_PIN_PULLUP);

    /* initialize interrupt driver */
    if (nrfx_gpiote_is_init() == false) {
        if (nrf_drv_gpiote_init() != NRF_SUCCESS) {
            printf("ERROR: Initializing encoder interrupts!\n");
            return;
        }
    }

    /* set config for encoder pin A interrupt */
    nrf_drv_gpiote_in_config_t pin_config = {
        NRF_GPIOTE_POLARITY_TOGGLE,  // sense
        NRF_GPIO_PIN_PULLUP,  // pull
        true,  // is_watcher
        true  // hi_accuracy
    };

    /* init and enable encoder pin A interrupt */
    nrf_drv_gpiote_in_init(ENC_A_PIN, &pin_config, encoder_handler);
    nrf_drv_gpiote_in_event_enable(ENC_A_PIN, true);
}


/**
 * Initialize the logging backend for logging over JTAG
 */
static void log_init() {
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

void createTimers() {
    // Create a timer in repeated mode for LEDs
    app_timer_create(&m_led_timer_id, APP_TIMER_MODE_REPEATED, led_handler_cylon);

    // Create a timer for audio node stepping
    app_timer_create(&m_audio_step_timer_id, APP_TIMER_MODE_REPEATED, audio_timer_handler);
}


/**
 * @brief Main app
 * @return Not used
 */
int main() {
    // Setup the system

    // Set up buttons
    button_init();

    // set up encoder
    encoder_init();

    log_init();

    // Timers
    app_timer_init();
    createTimers();

    // Setup I2C
    twi_master_init();

    // LEDs
    leds = new LED();

    // RGB LEDs
    pixels = new WS2812S(2);

    // Setup the audio
    audio = new Audio();

    // Play a sequence so we kill time while the SSD1306 Boots
    startup_sequence();

    // Setup the OLED display
    oled_init();

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
    uint8_t status1 = 0, status2 = 0;
    printf("Looping...\n");

    char display2[20];
    char display1[20];
    uint8_t counter = 0;

    badgeMode = DEFAULT_BADGE_MODE;

    while (true) {
        if (badgeMode == THEREMIN_BADGE_MODE) {
            /* theremin mode */
            // pixels->setColor(0, {8, 0, 8});
            // pixels->setColor(1, {0, 2, 8});
            // pixels->show();
            range1 = tof_pitch(range1);
            range2 = tof_volume(range2);
            snprintf(display2, 20, "%03dmm  %03dmm", range2, range1);
        } else if (badgeMode == FIXED_VOL_BADGE_MODE) {
            /* fixed volume mode */
            range1 = tof_pitch(range1);
            audio->setVolume(127);
            snprintf(display2, 20, "Fixed  %03dmm", range1);
        } else if (badgeMode == HOLDING_BADGE_MODE) {
            /* holding mode */
            snprintf(display2, 20, "HOLDING", range1);
        } else if (badgeMode == DOOM_BADGE_MODE) {
            /* doom mode */
            snprintf(display2, 20, "Year Of Doom", range1);
            // audio_off();
            if (!audio->songIsPlaying()) {
                audio_off();
                audio->startSongPlayback();
                leds->set(LED_D, ON);
                app_timer_start(m_audio_step_timer_id, APP_TIMER_TICKS(50), nullptr);
            }
            // e1m1();
        } else {
            /* default mode */
            audio_off();  // STFU <- "i feel personally attacked" - rehr
                         // No offense intended, just wanted it to boot into a quiet mode :)

            led_theramin();  // Enables LED Thearamin Mode
            snprintf(display2, 20, "LED Mode");
            // snprintf(display2, 20, "%03dmm  %03dmm", LEDrange2, LEDrange1);
        }

        snprintf(display1, 20, "%d %dv", counter++, adc->getBatteryVoltage());
        SSD1306_clearDisplay();
        util_gfx_set_cursor(10, 1);
        util_gfx_print(display1, COLOR_WHITE);
        util_gfx_set_cursor(10, 12);
        util_gfx_print(display2, COLOR_WHITE);

        SSD1306_display();
        checkButtonHolds();

        nrf_delay_ms(10);  // Do we really need this?
    }
}

void incrementBadgeMode() {
    badgeMode++;
    if (badgeMode >= TOTAL_BADGE_MODES) {
        badgeMode = 0;
    }
}

void audio_off() {
    audio->enable(false);  // turn off amp
    audio->disableTimer();  // disable timer1
    audio->setPWM0Ch0Value(0);  // turn off PWM0 ch0
}


/**
 * Handler for encoder pin A change events
 */
void button_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {
    if (pin == BUTTON_D_PIN) {
        if (nrfx_gpiote_in_is_set(BUTTON_D_PIN) == false) {
            if (!DButtonPressed) {
                // add code that runs one time when button is pressed
                // snprintf(display2, 20, "Vapor Mode");
                led_mode = 1;
                leds->set(LED_D, ON);
                printf("Button D Pressed\n");

                DButtonPressed = true;
            }
        } else {
            if (DButtonPressed) {
                // add code that runs one time when button is release

                leds->set(LED_D, OFF);
                printf("Button D Released\n");

                DButtonPressed = false;
            }
        }
    }

    if (pin == BUTTON_C_PIN) {
        if (nrfx_gpiote_in_is_set(BUTTON_C_PIN) == false) {
            if (!CButtonPressed) {
                // add code that runs one time when button is pressed
                led_mode = 2;
                leds->set(LED_C, ON);
                printf("Button C Pressed\n");

                CButtonPressed = true;
            }
        } else {
            if (CButtonPressed) {
                // add code that runs one time when button is release

                leds->set(LED_C, OFF);
                printf("Button C Released\n");

                CButtonPressed = false;
            }
        }
    }

    if (pin == BUTTON_Z_PIN) {
        if (nrfx_gpiote_in_is_set(BUTTON_Z_PIN) == false) {
            if (!ZButtonPressed) {
                // add code that runs one time when button is pressed
                led_mode = 3;
                leds->set(LED_Z, ON);
                printf("Button Z Pressed\n");

                ZButtonPressed = true;
            }
        } else {
            if (ZButtonPressed) {
                // add code that runs one time when button is release

                leds->set(LED_Z, OFF);
                printf("Button Z Released\n");

                ZButtonPressed = false;
            }
        }
    }

    if (pin == BUTTON_I_PIN) {
        if (nrfx_gpiote_in_is_set(BUTTON_I_PIN) == false) {
            if (!IButtonPressed) {
                // add code that runs one time when button is pressed
                led_mode = 4;
                leds->set(LED_I, ON);
                printf("Button I Pressed\n");

                IButtonPressed = true;
            }
        } else {
            if (IButtonPressed) {
                // add code that runs one time when button is release

                leds->set(LED_I, OFF);
                printf("Button I Released\n");

                IButtonPressed = false;
            }
        }
    }

    if (pin == BUTTON_A_PIN) {
        if (nrfx_gpiote_in_is_set(BUTTON_A_PIN) == false) {
            if (!AButtonPressed) {
                // add code that runs one time when button is pressed
                led_mode = 5;
                leds->set(LED_A, ON);
                printf("Button A Pressed\n");

                AButtonPressed = true;
            }
        } else {
            if (AButtonPressed) {
                // add code that runs one time when button is release

                leds->set(LED_A, OFF);
                printf("Button A Released\n");

                AButtonPressed = false;
            }
        }
    }

    if (pin == ENC_SW_PIN) {
        if (nrfx_gpiote_in_is_set(ENC_SW_PIN)) {
            if (!encSwPressed) {
                printf("Encoder Switch Pressed\n");
                incrementBadgeMode();
                encSwPressed = true;
            }
        } else {
            if (encSwPressed) {
                printf("Encoder Switch Released\n");

                encSwPressed = false;
            }
        }
    }
}


void checkButtonHolds() {
    if (DButtonPressed && CButtonPressed) {
        leds->set(LED_A, ON);
    }
}


void startup_sequence() {
    audio->enable(true);
    leds->set(LED_D, ON);
    audio->beep(1, 450);
    nrf_delay_ms(60);
    leds->set(LED_D, OFF);
    leds->set(LED_C, ON);
    audio->beep(1, 450);
    nrf_delay_ms(60);
    leds->set(LED_D, ON);
    audio->beep(1, 450);
    nrf_delay_ms(60);
    leds->set(LED_D, OFF);
    leds->set(LED_C, OFF);
    leds->set(LED_Z, ON);
    audio->beep(1, 450);
    nrf_delay_ms(60);
    leds->set(LED_D, ON);
    audio->beep(1, 450);
    nrf_delay_ms(60);
    leds->set(LED_D, OFF);
    leds->set(LED_C, ON);
    audio->beep(1, 450);
    nrf_delay_ms(60);
    leds->set(LED_D, ON);
    audio->beep(1, 450);
    nrf_delay_ms(60);
    leds->set(LED_D, OFF);
    leds->set(LED_C, OFF);
    leds->set(LED_Z, OFF);
    leds->set(LED_I, ON);
    audio->beep(1, 450);
    nrf_delay_ms(60);
    leds->set(LED_D, ON);
    audio->beep(1, 450);
    nrf_delay_ms(60);
    leds->set(LED_D, OFF);
    leds->set(LED_C, ON);
    audio->beep(1, 450);
    nrf_delay_ms(60);
    leds->set(LED_D, ON);
    audio->beep(1, 450);
    nrf_delay_ms(60);
    leds->set(LED_D, OFF);
    leds->set(LED_C, OFF);
    leds->set(LED_Z, ON);
    audio->beep(1, 450);
    nrf_delay_ms(60);
    leds->set(LED_D, ON);
    audio->beep(1, 450);
    nrf_delay_ms(60);
    leds->set(LED_D, OFF);
    leds->set(LED_C, ON);
    audio->beep(1, 450);
    nrf_delay_ms(60);
    leds->set(LED_D, ON);
    audio->beep(1, 450);
    nrf_delay_ms(60);
    leds->set(LED_D, OFF);
    leds->set(LED_C, OFF);
    leds->set(LED_Z, OFF);
    leds->set(LED_I, OFF);
    leds->set(LED_A, ON);
    audio->beep(1, 450);
    nrf_delay_ms(60);
    leds->set(LED_I, ON);
    audio->beep(1, 450);
    nrf_delay_ms(60);
    leds->set(LED_Z, ON);
    audio->beep(1, 450);
    nrf_delay_ms(60);
    leds->set(LED_C, ON);
    audio->beep(1, 450);
    nrf_delay_ms(60);
    leds->set(LED_D, ON);
    audio->beep(1, 450);
    nrf_delay_ms(60);
    leds->allOff();
    nrf_delay_ms(500);
    leds->set(LED_D, ON);
    leds->set(LED_C, ON);
    leds->set(LED_Z, ON);
    leds->set(LED_I, ON);
    leds->set(LED_A, ON);
    audio->beep(1, 450);
    nrf_delay_ms(500);
    leds->allOff();
    audio->beep(1, 450);



    PIXEL colors[] = { { 0, 64, 96 }, { 128, 160, 192}};
    pixels->setColor(0, colors[0]);
    pixels->setColor(1, colors[1]);
    pixels->show();

    bool goDown = false;
    int curLED = LED_D;
    for (int i = 0; i < 20; i++) {
        if (!goDown && curLED > 0) {
            leds->set((LEDS)(curLED - 1), OFF);
        }
        if (goDown && curLED < 4) {
            leds->set((LEDS)(curLED + 1), OFF);
        }
        leds->set((LEDS)curLED, ON);
        nrf_delay_ms(40);

        if (goDown) {
            if (curLED == LED_D) {
                goDown = false;
            } else {
                curLED -= 1;
            }
        } else {
            if (curLED == LED_A) {
                goDown = true;
            } else {
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
        audio->beep(1, 425);
    }
    leds->allOff();
    pixels->setColor(0, {0, 1, 0});
    pixels->setColor(1, {0, 1, 0});
    pixels->show();
    audio->beep(50, 800);
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


}

void oled_init(){
    // Setup the display
    nrf_delay_ms(1500);
    SSD1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS, false);
    SSD1306_setRotation(ROTATION_180);

    // Default text
    util_gfx_init();
    util_gfx_set_font(FONT_MONO55_8PT);
    util_gfx_set_cursor(10, 1);
    util_gfx_print("DCZia", COLOR_WHITE);
    util_gfx_set_cursor(10, 12);
    util_gfx_print("DEFCON 27", COLOR_WHITE);

    // Display the thing
    SSD1306_display();
    nrf_delay_ms(1500);
}

uint8_t tof_pitch(uint8_t prevRange) {
    // read right sensor
    uint8_t range = TOF->readRange(TOF_SENSOR1);

    // Enable this to smooth the sampling a bit
    // range = uint8_t(uint16_t(range + prevRange) >> 1);

    // set pitch with right sensor
    // range_2mm splits range into 2mm chunks
    // +1 prevents divide by 0 x_x
    uint8_t range_2mm = (range >> 1) + 1;
    uint8_t prevRange_2mm = (prevRange >> 1) + 1;

    // only change timer/pitch if hand position changes
    if (range_2mm == prevRange_2mm) {
        return prevRange;
    }
    // check if range is around max (255mm)
    if (range >= 250) {
        audio_off();
        return range;
    }

    // if no, get note that corresponds hand position
    // calculate hand position
    // TOF sensor dies off around 17cm
    uint8_t handPosition = 0;
    if (range_2mm > 57) {  // about 11cm...could be higher
        handPosition = 48;
    } else if (range_2mm >= 10) {  // 2cm of padding close to the sensor
        handPosition = range_2mm - 10;  // start hand position calc at 2cm
    }

    bool halfNote = handPosition & 0x1;  // check if this is an inbetween note
    uint8_t noteIndex = handPosition >> 1;  // divide hand position by 2 to get note
    uint8_t octavesUp = 0;

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

uint8_t tof_volume(uint8_t prevRange) {
    // read left sensor
    uint8_t range = TOF->readRange(TOF_SENSOR2);

    uint8_t range_cm = range / 10 + 1;
    uint8_t prevRange_cm = prevRange / 10 + 1;

    if (range_cm == prevRange_cm) {
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

//
// Light modes
//

void led_flash_yellow() {
    pixels->setColor(0, { 200, 200, 0});
    pixels->setColor(1, { 200, 200, 0});
    pixels->show();
}

void led_theramin() {
    uint8_t LEDrange1 = TOF->readRange(TOF_SENSOR1);
    uint8_t LEDrange2 = TOF->readRange(TOF_SENSOR2);
    if (LEDrange1 > 190) { LEDrange1 = 190; }  // Rounding as TOF sensor will not measure much past here
    if (LEDrange2 > 190) { LEDrange2 = 190; }

    // output = output_start + ((output_end - output_start) / (input_end - input_start)) * (input - input_start)
    uint8_t output1 = 255 + ((0 - 255) / (190 - 8)) * (LEDrange1 - 8) + -72;  // ReMap Sensor Values
    uint8_t output2 = 255 + ((0 - 255) / (190 - 8)) * (LEDrange2 - 8) + -72;  // ReMap Sensor Values
    uint8_t outhalf = output1 >> 2;  // divide by 4
    uint8_t outhalf2 = output2 >> 2;  // divide by 4
    printf("Output Range 1  = %d\n", output2);  // Debug
    printf("Output Range 2  = %d\n", output1);

    if (led_mode == 1) {  // Vapor Mode
        pixels->setColor(0, {output2, 0, output2});
        pixels->setColor(1, {0, 0, output1});
        pixels->show();
        // app_timer_stop(m_led_timer_id);
        led_animation(100);  // Start LED cylon scroll
    }
    if (led_mode == 2) {  // Chile Mode
        pixels->setColor(0, {output2, 0, 0});
        pixels->setColor(1, {0, output1, 0});
        pixels->show();
    }
    if (led_mode == 3) {  // VaporMode2
        pixels->setColor(0, {outhalf, output2, 0});
        pixels->setColor(1, {output2, outhalf2, output2});
        pixels->show();
        // app_timer_stop(m_led_timer_id);
        led_animation(100);  // Start LED cylon scroll
    }
    if (led_mode == 4) {  // VaporMode2
        pixels->setColor(0, {19, output2, output2});
        pixels->setColor(1, {output2, 5, 39});
        pixels->show();
    }
    if (led_mode == 5) {  // VaporMode2
        pixels->setColor(0, {1, output2, 33});
        pixels->setColor(1, {output2, output2, 9});
        pixels->show();
    }
}


void led_handler_blink(void *p_context) {
    led_status = !led_status;

    if (led_status == true) {
        leds->set(LED_D, ON);
        leds->set(LED_C, ON);
        leds->set(LED_Z, ON);
        leds->set(LED_I, ON);
        leds->set(LED_A, ON);
    } else {
        leds->set(LED_D, OFF);
        leds->set(LED_C, OFF);
        leds->set(LED_Z, OFF);
        leds->set(LED_I, OFF);
        leds->set(LED_A, OFF);
    }
}

void led_handler_cylon(void *p_context) {
    if (led_mode == 1) {  // Cylon Scroll
        if (!cylonGoDown && cylonCurLED > 0) {
            leds->set((LEDS)(cylonCurLED - 1), OFF);
        }
        if (cylonGoDown && cylonCurLED < 4) {
            leds->set((LEDS)(cylonCurLED + 1), OFF);
        }
        leds->set((LEDS)cylonCurLED, ON);

        if (cylonGoDown) {
            if (cylonCurLED == LED_D) {
                cylonGoDown = false;
            } else {
                cylonCurLED -= 1;
            }
        } else {
            if (cylonCurLED == LED_A) {
                cylonGoDown = true;
            } else {
                cylonCurLED += 1;
            }
        }
    }

    if (led_mode == 2) {  // Alternating Blink
    led_status = !led_status;

        if (led_status == true) {
        leds->set(LED_D, ON);
        leds->set(LED_C, OFF);
        leds->set(LED_Z, ON);
        leds->set(LED_I, OFF);
        leds->set(LED_A, ON);
        } else {
        leds->set(LED_D, OFF);
        leds->set(LED_C, ON);
        leds->set(LED_Z, OFF);
        leds->set(LED_I, ON);
        leds->set(LED_A, OFF);
        }
    }

    if (led_mode == 3) {  // Alternating Blink
        switch (led_pattern_step) {
        case 1:
        leds->set(LED_D, ON);
        leds->set(LED_C, OFF);
        leds->set(LED_Z, OFF);
        leds->set(LED_I, OFF);
        leds->set(LED_A, OFF);
        led_pattern_step++;
        break;

        case 2:
        leds->set(LED_D, OFF);
        leds->set(LED_C, ON);
        leds->set(LED_Z, OFF);
        leds->set(LED_I, OFF);
        leds->set(LED_A, OFF);
        led_pattern_step++;
        break;

        case 3:
        leds->set(LED_D, OFF);
        leds->set(LED_C, OFF);
        leds->set(LED_Z, ON);
        leds->set(LED_I, OFF);
        leds->set(LED_A, OFF);
        led_pattern_step++;
        break;

        case 4:
        leds->set(LED_D, OFF);
        leds->set(LED_C, OFF);
        leds->set(LED_Z, OFF);
        leds->set(LED_I, ON);
        leds->set(LED_A, OFF);
        led_pattern_step++;
        break;

        case 5:
        leds->set(LED_D, OFF);
        leds->set(LED_C, OFF);
        leds->set(LED_Z, OFF);
        leds->set(LED_I, OFF);
        leds->set(LED_A, ON);
        led_pattern_step = 1;
        break;
        }
    }
}

void audio_timer_handler(void *p_context) {
    if (audio->incStepPosition() == 0) {
        /* song is over */
        audio->stopSongPlayback();
        app_timer_stop(m_audio_step_timer_id);
    }
}

void led_animation(uint16_t led_speed) {
    // app_timer_stop(m_led_timer_id);

    // Start the timer, fire every 100ms
    app_timer_start(m_led_timer_id, APP_TIMER_TICKS(led_speed), nullptr);
}
