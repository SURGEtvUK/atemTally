#pragma once

// Uncomment this if you want debug feedback in the serial port
//#define DEBUG

// Arduino stuff
#include <Arduino.h>
#include <SPI.h>

// easier to write, it's for toggling the led
#define LED(x, y) { digitalWrite(7, x); digitalWrite(8, y); }

// debugging stuff
#ifdef DEBUG
# include <MemoryFree.h>
# define DBGOUT(x) Serial.print(F(x))
# define DBGOUTLN(x) Serial.println(F(x))
#else
# define DBGOUT(x)
# define DBGOUTLN(x)
#endif

// Cameron's favourite number.
#define MAGIC_NUMBER 0x69

// templates for reading/writing to eeprom
#include <EEPROM.h>
template <class T> int EEPROM_writeT(int ee, const T& value)
{
	const byte* p = (const byte*)(const void*)&value;
	unsigned int i;
	for (i = 0; i < sizeof(value); i++)
		  EEPROM.write(ee++, *p++);
	return i;
}

template <class T> int EEPROM_readT(int ee, T& value)
{
	byte* p = (byte*)(void*)&value;
	unsigned int i;
	for (i = 0; i < sizeof(value); i++)
		  *p++ = EEPROM.read(ee++);
	return i;
}

// Networking
#include <Ethernet.h>

// GPIO Expander
#include <gpio_expander.h>
#include <mcp23s17.h>

// define functions
void prodSetup();
void prodLoop();
void cfgSetup();
void cfgLoop();
