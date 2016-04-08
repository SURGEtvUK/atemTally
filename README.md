# atemTally

This contains the firmware for the project codename atemTally.

What is atemTally? It's a Tally decoder for a DataVideo ITC-100 to interface with a BlackMagic ATEM video switcher.

It's based on an ATmega328p micro-controller, an MCP23S17 SPI GPIO expander, a W5100 SPI NIC, a L7805 voltage regulator and a bunch of 2N3904 npn's.

The ATmega328p pins are as follows:
*  1 Reset Switch
*  2 Serial TX
*  3 Serial RX
* 12 Config Switch
* 13 Green 'OK' LED
* 14 Red 'Error' LED
* 15 MCP23S17 Slave Select
* 16 W5100 Slave Select
* 17 MOSI
* 18 MISO
* 19 SCK

The project depends on the gpio_expander library which can be found at https://github.com/sumotoy/gpio_expander . It also depends on the ATEM library which can be found at https://github.com/kasperskaarhoj/SKAARHOJ-Open-Engineering .

# Usage

Just connect power, ethernet and the DB25->2xHD15, and it should work.

To change the IP address of the ATEM the tally box connects to press the config button before adding power, it will boot up into config mode. There will be a webpage available to configure the ip address of the tally box and of the atem it connects to, it's available at the tally box ip address, so http://192.168.10.239/setup or whatever you set the tally box ip to. To get out of the config mode simply press the reset button.

On the side of the box there is a bi-colour led, green means it's connected to the atem, red means not connected, and orange means config mode.

# Reference Info
This is for the poor sod who has to fix it when it breaks.

The DB25 port follows the same pinout as the official BMD ATEM GPI & Tally box. Except that the inputs are on a tail with the wrong gender. For reference: 10->3 is IN 1->8 and 23->16 is OUT 1->8. All other pins are tied to ground.

Another thing you should know, the gpio expander library addresses B0 as 0, ..., B7 as 7, A0 as 8, ..., A7 as 15.

On the ITC-100, GPI (IN and OUT) 1->8 from the box corresponds to 3, 1, 8, 6, 13, 11, 15, 5, pins 4, 9, 14 are ground. Which is PVW 1, PGM 1, PVW 2, PGM 2... Port A is In1->4, Port B is In5->8. Port A corresponds to GPI OUT, and Port B corresponds to GPI IN. In the custom cable, the colour coding is as follows: white orange, orange, white green, green, white blue, blue, white brown, brown and (yellow/red for gnd).

In version 2 the 4* HEF4016BP were replaced by 16* 2N3904. Nothing firmware wise was changed, just the hardware.
