/***
* Faranux Electronics - 433MHz ASK Receiver
* Library: RadioHead RH_ASK - https://github.com/jecrespo/RadioHead
* ----------------------------------------------------------------
* Module: 3-pin 433MHz ASK/OOK receiver (round can/superregen style
*         with coil antenna on top, labeled VCC, DATA, GND)
*
* Wiring (Arduino UNO <-> receiver module):
*   Arduino 5V          -> Module VCC (breadboard power rail, red wire)
*   Arduino GND          -> Module GND (breadboard ground rail, black wire)
*   Arduino Pin 11       -> Module DATA (green wire)
*                          (RH_ASK default RX data pin is 11 - this
*                          sketch uses the library default, so no
*                          pins are passed to the RH_ASK constructor
*                          below. If you rewire DATA to a different
*                          pin, use the commented-out constructor
*                          line to specify it explicitly.)
*   Antenna: a ~17.3cm wire on the ANT pad improves range and
*            reception reliability.
*
* This board runs the RECEIVER sketch only. It listens for packets
* sent by a separate Arduino running the companion transmitter
* sketch - they communicate over RF, not by direct wiring.
**/
#include <RH_ASK.h>
#include <SPI.h>  // Not actually used but needed to compile

RH_ASK driver;  // Uses library defaults: RX data pin 11, PTT pin 10 (unused for ASK/OOK)
// RH_ASK driver(2000, 2, 4, 5); // ESP8266 or ESP32: do not use pin 11

void setup() {
  Serial.begin(9600);  // Debugging only
  if (!driver.init())
    Serial.println("init failed");
}

void loop() {
  uint8_t buf[5];
  uint8_t buflen = sizeof(buf);

  if (driver.recv(buf, &buflen))  // Non-blocking
  {
    int i;

    // Message with a good checksum received, dump it.
    //driver.printBuffer("Got:", buf, buflen);
    String msg = "Received Message: ";
    Serial.println(msg + (char*)buf);
  }
}