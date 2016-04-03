#include "main.h"

// Networking
extern uint8_t talyip[4];
extern uint8_t atemip[4];

// Config
#include <TextFinder.h>
EthernetServer server(80);  // create a server at port 80

void cfgSetup()
{
  // start to listen for clients
  server.begin();

  // set led to orange
  LED(1, 1)
}

void cfgLoop(void)
{
  // try to get client
  EthernetClient client = server.available();

  // we has client
  if (client)
  {
    TextFinder finder(client);
    while (client.connected())
    {
      if (client.available())
      {
        if(finder.findUntil("GET /", NULL))
        {
          // basic "/setup" page
          if (finder.findUntil("setup", "\n\r"))
          {
            // form has been submitted
            if (finder.findUntil("meh", "\n\r"))
            {
              // find all the new uipx values
              while(finder.findUntil("uip", "\n\r"))
              {
                int val = finder.getValue();
                if(val >= 0 && val <= 3)
                  talyip[val] = finder.getValue();
                if(val >= 4 && val <= 7)
                  atemip[val - 4] = finder.getValue();
              }

              // set the magic number
              EEPROM.write(0, MAGIC_NUMBER); // check byte

              // write the new ips to eeprom
              EEPROM_writeT(1, talyip);
              EEPROM_writeT(5, atemip);

              // the user now needs to press the reset button
            }

            // build le page
            client.println(F("HTTP/1.1 200 OK"));
            client.println(F("Content-Type: text/html"));
            client.println();

            client.print(F("<!DOCTYPE html><html><head><title>Super Secret Config Page</title></head><body><form method=\"get\"><input type=\"hidden\" name=\"meh\" value=\"1\" />"));
            client.print(F("<p>Warning: there is no validation performed on the server side, so if you enter something other than a valid ip, you might have to reflash the firmware.</p><p><label>GPI &amp; Tally Box Ip</label>"));
            for (uint8_t i = 0; i < 4; i++)
            {
              client.print(F("<input type=\"number\" min=\"0\" max=\"255\" name=\"uip"));
              client.print(i);
              client.print(F("\" value=\""));
              client.print(talyip[i]);
              client.print(F("\" />"));
            }
            client.print(F("</p><p><label>Atem Switcher Ip</label>"));
            for (uint8_t i = 0; i < 4; i++)
            {
              client.print(F("<input type=\"number\" min=\"0\" max=\"255\" name=\"uip"));
              client.print(i + 4);
              client.print(F("\" value=\""));
              client.print(atemip[i]);
              client.print(F("\" />."));
            }
            client.print(F("</p><p><input type=\"submit\" value=\"Einreichen!\" /></p></form><p>Created by Kajusz Dykiel. Does it not work or you need help? email yesusutv@kajusz.me</p></body></html>"));
            break;
          }
        }
        client.println(F("HTTP/1.1 200 OK"));
        client.println(F("Content-Type: text/html"));
        client.println();

        client.print(F("<!DOCTYPE html><html><head></head><body>"));
        client.print(F("<p>Visit the <a href=\"https://github.com/kajusz/atemTally\">GitHub</a> repo for instructions or go to the <a href=\"http://"));
        client.print(Ethernet.localIP());
        client.print(F("/setup\">setup</a> page.</p></body></html>"));

        break;
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection
    client.stop();
  }
}
