# DCZia 2019 Badge Build Guide

Rough draft of build guide

## Parts List

The following parts should be included with your badge:

- Partially asasembled badge PCB
- 5x Mechanical "low profile" Keyswitches
- 5x keycaps
- 1 OLED Screen
- 1 Rotary Encoder
- 1 Speaker
- 1 Battery Box
- 3x AAA Batteries
- 2x SAO Headers
- Please note, no SD card is included, this is optional
- Stickers
- Lanyard

## Build Instructions

- Step 1. Solder on keyswitches
- Step 2. Solder on rotaty encoder
- Step 3. Solder on battery box
  - **Note** If you did solder on the battery box first, genly bend it upwards to access the components you need to solder underneath it.
- Step 4. Attach screen
  - **Note** Mount the header to the board putting the long legs through the four holes from the front of the pcb. The black plastic part of the header should lay flat ontop of the front of the pcb. Solder the display to the short legs, this way the screen will lay flat on the font of the pcb. Then solder the back side of the headers to the board.
- Step 5. Attach SAO Headers
  - If you wish to use SAO's solder the two headers to the front of the pcb with the notch pointing up
- Step 5. Attach Speaker
  - The positive (RED) and negative (BLACK) wires of the speaker get soldered to the two square pads on the back of the badge on the left side of the pcb looking at it from the rear. Then take the sticky tape backing off the speaker and attach it centered on the prefroated holes on the back of the board.
- Step 6. Install keycaps
- Step 7. Place DCZIA stickers on the keycaps
- Step 6. Install Batteries

--- Initial Badge Flash Instructions ---

- Step 1. Attach board to segger J-Link with cable
  - **Note** Production boards will come pre flashed.
- Step 2. make flash-sd
- Step 3. make flash
