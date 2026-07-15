/***
* Faranux Electronics - 433MHz ASK Transmitter
* Library: RadioHead RH_ASK - https://github.com/jecrespo/RadioHead
* ----------------------------------------------------------------
* Module: FS1000A-style 433MHz ASK/OOK transmitter (4-pin: ANT,
*         GND, DATA, VCC - small green PCB with coil antenna trace)
*
* Wiring (Arduino UNO <-> transmitter module):
*   Arduino 5V         -> Module VCC (breadboard power rail, red wire)
*   Arduino GND         -> Module GND (breadboard ground rail, black wire)
*   Arduino Pin 12      -> Module DATA (green wire)
*                          (RH_ASK default TX data pin is 12 - this
*                          sketch uses the library default, so no
*                          pins are passed to the RH_ASK constructor
*                          below. If you rewire DATA to a different
*                          pin, use the commented-out constructor
*                          line to specify it explicitly.)
*   Antenna: a ~17.3cm wire soldered/inserted at ANT improves range
*            significantly over no antenna at all.
*
* This board runs the TRANSMITTER sketch only. A second, separate
* Arduino running the companion receiver sketch (with a 433MHz
* receiver module) is needed to actually receive "hello" - they
* are not wired to each other, they communicate over RF.
**/
#include <RH_ASK.h>
#include <SPI.h>  // Not actually used but needed to compile

RH_ASK driver;  // Uses library defaults: TX data pin 12, PTT pin 10 (unused for ASK/OOK)
// RH_ASK driver(2000, 2, 4, 5); // ESP8266 or ESP32: do not use pin 11

void setup() {
  Serial.begin(9600);  // Debugging only
  if (!driver.init())
    Serial.println("init failed");
}

void loop() {
  const char *msg = "hello";

  driver.send((uint8_t *)msg, strlen(msg));
  driver.waitPacketSent();
  delay(200);
}