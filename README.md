# Defcon27-Badge

```
 ____   ____ ______             ____   ____ ____ _____
|  _ \ / ___|__  (_) __ _      |  _ \ / ___|___ \___  |
| | | | |     / /| |/ _` |_____| | | | |     __) | / /
| |_| | |___ / /_| | (_| |_____| |_| | |___ / __/ / /
|____/ \____/____|_|\__,_|     |____/ \____|_____/_/
```


# Lazer Theramin Synth Badge

This year we wanted to do something different. Our idea is to use two laser time of flight sensors that can measure the distance of your hands moving around infront of the badge to control a synthesizer to create a laser theremin.

- 64Mhz Arm CortexM4F with Bluetooth 5
- 5 Mechanical Clicky Low Profile Key Switches
- Rotary Encoder with Click Button
- OLED Display
- Onboard Speaker and Amplifier!
- 1/8" Headphone Out Audio Jack
- SD Card Storage
- RGB Leds
- 2 SAO / "Shitty Add-On" Connectors


# Hardware Detail

- Rigado BMD-340 aka NRF52 Microconroller
  - Bluetooth, Lots of IO's, Fast, Small, Low Power, and cheap ~ $12
  - https://www.rigado.com/products/modules/bmd-340/

- Laser Time of Flight Sensors - Dev modules are from Adafruit 
  - https://www.adafruit.com/product/3317

- DAC / Amplifier - mediumRehr Audio™

- Speaker - Oval 1watt onboard speaker

- Buttons - Kalih Low Profile White Clicky Keyswitches. Oh yeah. This is the shit right here.
  - https://novelkeys.xyz/collections/switches/products/kailh-low-profile-switches

- Keycaps - Note, the low profile caps are for the Kalih low profile switches only. 
  - https://novelkeys.xyz/collections/keycaps/products/kailh-low-profile-keycaps-blank
  
- LEDs
  - Neopixel RGB

- Screen - SSD1306 OLED

- SD Card


# Software

- Environment is GNU ARM GCC with your choice of IDE

- Programming can be done via a J-Link Segger or by SD Card (coming soon)

- Uses Nordic SoftDevice version XXX

- See [EnvironmentSetup](Software/EnvironmentSetup.md) for more info


# Board

- Designed in KiCad


# Documentation

- [Board Build Guide](BuildGuide.md)
- [EnvironmentSetup](Software/EnvironmentSetup.md)


# Links to relevant information

- [Documentation library - nordicsemi.com](https://www.nordicsemi.com/DocLib)

- DC801 Party Badge - Good source for setting up dev env, and seeing some Rigado code in action - https://github.com/DC801/DC26PartyBadge

- oxvox - Rigado based pwm synth - https://github.com/mediumrehr/oxvox

- Basic audio synthesis with an Arduino - https://makezine.com/projects/make-35/advanced-arduino-sound-synthesis/

- mintysynth - simple arduino synth - https://mintysynth.com/

- Nordic PWM Driver info: https://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.sdk5.v15.3.0%2Fhardware_driver_pwm.html&cp=5_1_2_0_8

- PWM Primer - https://provideyourown.com/2011/analogwrite-convert-pwm-to-voltage/


# Devboard pinout

![BMD-340-DK Pinout](https://i.imgur.com/5dEBRfZ.jpg)
