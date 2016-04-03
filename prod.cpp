#include "main.h"

// Networking
extern uint8_t atemip[4];

// GPIO
extern mcp23s17 gpio;

// ATEM
#include <ATEMmin.h>
ATEMmin AtemSwitcher;
uint16_t tally[2] = { 0, 0 };
bool atmconnected;

void prodSetup()
{
  // set led to red
  LED(0, 1)

  // init atem
  AtemSwitcher.begin(IPAddress(atemip[0], atemip[1], atemip[2], atemip[3]), 56417);
#ifdef DEBUG
  DBGOUT("atemip from eeprom: ");
  Serial.print(atemip[0]);Serial.print(".");Serial.print(atemip[1]);Serial.print(".");Serial.print(atemip[2]);Serial.print(".");Serial.println(atemip[3]);
  AtemSwitcher.serialOutput(0x80);
#endif

  // initialize a connection to le switcher
  atmconnected = false;
  AtemSwitcher.connect();

#ifdef DEBUG
  // shows free memory:  
  DBGOUT("freeMemory()=");
  Serial.println(freeMemory());
#endif
}

void prodLoop()
{
  // user feedback
  if (atmconnected != AtemSwitcher.isConnected())
  {
    atmconnected = AtemSwitcher.isConnected();

    if (atmconnected)
      LED(1, 0)
    else
      LED(0, 1)
  }

  // Check for packets, respond to them etc. Keeping the connection alive!
  AtemSwitcher.runLoop();

  // If disconnected, try to reconnect
  if (AtemSwitcher.isConnectionTimedOut())
  {
    tally[0] = 0;
    tally[1] = 0;
    gpio.gpioDigitalWrite((tally[0] - 1)*2 + 1, LOW);
    gpio.gpioDigitalWrite((tally[1] - 1)*2, LOW);
    
    DBGOUT("Connection to ATEM Switcher has timed out - reconnecting!");
    AtemSwitcher.connect();
  }

  if (tally[0] != AtemSwitcher.getProgramInputVideoSource(0))
  {
    // reset previous tally led
    if (tally[0] > 0 && tally[0] < 9)
      gpio.gpioDigitalWrite((tally[0] - 1)*2 + 1, LOW);
    // store & set new led
    tally[0] = AtemSwitcher.getProgramInputVideoSource(0);
    if (tally[0] > 0 && tally[0] < 9)
      gpio.gpioDigitalWrite((tally[0] - 1)*2 + 1, HIGH);

#ifdef DEBUG
    DBGOUT("tally pgm in");
    Serial.print(AtemSwitcher.getProgramInputVideoSource(0));
    DBGOUT(" pin ");
    Serial.println((tally[0] - 1)*2 + 1);
#endif
  }
  if (tally[1] != AtemSwitcher.getPreviewInputVideoSource(0))
  {
    // reset previous tally led
    if (tally[1] > 0 && tally[1] < 9)
      gpio.gpioDigitalWrite((tally[1] - 1)*2, LOW);
    // store & set new led
    tally[1] = AtemSwitcher.getPreviewInputVideoSource(0);
    if (tally[1] > 0 && tally[1] < 9)
      gpio.gpioDigitalWrite((tally[1] - 1)*2, HIGH);

#ifdef DEBUG
    DBGOUT("tally pvw in");
    Serial.print(AtemSwitcher.getPreviewInputVideoSource(0));
    DBGOUT(" pin ");
    Serial.println((tally[1] - 1)*2);
#endif
  }
}
