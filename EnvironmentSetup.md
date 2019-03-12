# Setting up the DCZia DC27 Badge Environment

## Mac / Linux

- Download the Command Line Tools - [nRF5 Command Line Tools - nordicsemi.com](https://www.nordicsemi.com/Software-and-Tools/Development-Tools/nRF5-Command-Line-Tools)

- Download the J-Link Driver - [SEGGER - The Embedded Experts - Downloads](https://www.segger.com/downloads/jlink/JLink_MacOSX.pkg)

- Download the SDK - [nRF5 SDK - nordicsemi.com](https://www.nordicsemi.com/Software-and-Tools/Software/nRF5-SDK)

- Download the Arm GCC Compiler - [GNU Toolchain | GNU-RM Downloads – Arm Developer](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads)

- Download your IDE of chizzoice - VS Code, Sublime Text, VIM, Emacs (eww) etc…

## Windows
- Use Segger Embedded studio, or figure out how to get GCC Arm setup on there.

## Installation

### Setup Directory Structure
You can create your own directory structure, but the current makefile is setup to look for something similar to:

```
$ mkdir -p ~/dev/bin
$ mkdir -p ~/dev/nordic-sdk15
```

Put the Nordic Command Link Programing tools (mergehex and nrfjprog) into the bin folder.

Extract the SDK Version 15 into the /dev/nordic-sdk15 folder

Clone our repo into the dev folder.

The config folder from our repo contains pin to variable name mappings in the custom_board.h file.

The devboard folder contains the make file and linker script for the devboard. The final board will get its own folder and config setup.

The code for the project is in the Software folder.

### Setup Makefile

If you installed stuff into other areas you may need to edit the makefile.

### Build

CD into the Defcon27-Badge/Software/devboard folder. This contains the makefile for building software for the development board.

Run 'make' to build the firmware. Then run 'make flash' to flash the device. Boom, done!

## Misc Links
[The complete cross-platform nRF development tutorial - Novel Bits](https://www.novelbits.io/cross-platform-nrf-development-tutorial/) - SES

Oldskool GCC Way - [Programming an nRF52 on a Mac | Aaron Eiche](https://aaroneiche.com/2016/06/01/programming-an-nrf52-on-a-mac/)
