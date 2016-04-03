# atemTally

This contains the firmware for the project codename atemTally.

What is atemTally? It's a Tally decoder for a DataVideo ITC-100 to interface with a BlackMagic ATEM video switcher.

It's based on an ATmega328p micro-controller, an MCP23S17 SPI GPIO expander, a W5100 SPI NIC and a bunch of HEF4016BP switches.

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
