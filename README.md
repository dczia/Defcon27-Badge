# Defcon27-Badge
``` 
 ____   ____ ______             ____   ____ ____ _____
|  _ \ / ___|__  (_) __ _      |  _ \ / ___|___ \___  |
| | | | |     / /| |/ _` |_____| | | | |     __) | / /
| |_| | |___ / /_| | (_| |_____| |_| | |___ / __/ / /
|____/ \____/____|_|\__,_|     |____/ \____|_____/_/

```

#Idea
##Lazer Theramin Synth Badge
This year we wanted to do something different. Our idea is that we use two laser time of flight sensors that can measure the distance of your hands moving around infront of our badge to control a synthesizer to create something like a laser theremin. 

#Hardware
*nRF52 Microconroller - Bluetooth, Lots of IO's, Fast, Small, Low Power (ish), and cheap ~ $12
*Dev Board - Rigaldo BM-340 Eval Board - https://www.digikey.com/products/en/rf-if-and-rfid/rf-evaluation-and-development-kits-boards/859?k=BMD-340&pkeyword=&sv=0&sf=0&FV=ffe0035b&quantity=&ColumnSort=0&page=1&pageSize=25
*Time of Flight Sensors - To Be Determined for final board, dev modules are from Adafruit - https://www.adafruit.com/product/3317
*DAC / Amplifier - Not yet chosen. Perhaps we can just bit bang pwm off the NRF52 and we dont need a dac. May need an amp still.
*Speaker - Not yet chosen. The one mediumrehr used for his addon to our DC26 badge is a contender though.
*Buttons - Low Profile Mechanical Keyswitches (Kalih Low Profile Whites). Oh yeah. This is the shit right here. - https://novelkeys.xyz/collections/switches/products/kailh-low-profile-switches
*Keycaps - We can 3D print our own, or go with these. Note, the low profile caps are for the Kalih low profile switches only. - https://novelkeys.xyz/collections/keycaps/products/kailh-low-profile-keycaps-blank
*Screen - SSD1306 128x64? -- Bigger version of what we used last year.
*Microphone - TBD


#Documentation
[Documentation library - nordicsemi.com](https://www.nordicsemi.com/DocLib)    

#Program Structure

#
