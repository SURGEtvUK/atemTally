#include "main.h"

// Networking
uint8_t talyip[4] = { 192, 168, 10, 239 };
uint8_t atemip[4] = { 192, 168, 10, 240 };

// GPIO
mcp23s17 gpio(9, 0x20); //pin 9, haen address

void setup()
{
  // cfg button
  pinMode(6, INPUT);

  // leds
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  LED(0,0)

  // slave select for mcp23s17
  pinMode(9, OUTPUT);
  digitalWrite(10, HIGH);

  // slave select for w5100
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  // read ips from eeprom
  if (EEPROM.read(0) == MAGIC_NUMBER) // check byte
  {
    // we've written here before! so read the values
    EEPROM_readT(1, talyip);
    EEPROM_readT(5, atemip);
  }

  // init w5100
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  Ethernet.begin(mac, talyip);

#ifdef DEBUG
  // why so Serial?
  Serial.begin(115200);
  DBGOUTLN("Serial started");
#endif

  // init mcp23s17
  gpio.begin();
  gpio.gpioPinMode(OUTPUT); // Set all pins to be outputs (by default they are all inputs)
  gpio.gpioPort(0xFFFF); // Change all pins at once, 16-bit value
  delay(100);
  gpio.gpioPort(0x0000); // Set & clear
  delay(100);

#ifdef DEBUG
  DBGOUT("server is at ");
  Serial.println(Ethernet.localIP());
#endif
}

int main(void)
{
  // Initialise the ATmega328p
  init();

  // Initialise variables and SPI devices
  setup();

  // Enter the appropriate loop depending on what mode we're in
  if (digitalRead(6))
  {
    DBGOUT("prod");
    prodSetup();
    for (;;)
    {
      prodLoop();
      if (serialEventRun)
        serialEventRun();
    }
  }
  else
  {
    DBGOUT("cfg");
    cfgSetup();
    for (;;)
    {
      cfgLoop();
      if (serialEventRun)
        serialEventRun();
    }
  }

  return 0;
}
