/*
 * A7670E ONE-TIME BAUD RATE CHANGER
 * Upload this sketch ONCE to change the module from 115200 to 9600.
 * After the Serial Monitor shows "Done!", upload the main sketch.
 */
#include <SoftwareSerial.h>

SoftwareSerial gsmSerial(10, 11); // RX=10, TX=11

void setup() {
  Serial.begin(115200);
  gsmSerial.begin(115200); // Talk to module at its current (factory) speed

  delay(3000);
  Serial.println("Changing A7670E baud rate from 115200 to 9600...");

  // AT+IPR=9600 saves the new baud rate to flash (survives power cycles)
  gsmSerial.println("AT+IPR=9600");
  delay(2000);

  // Print whatever the module replied (may still be garbled — that is OK)
  while (gsmSerial.available()) {
    Serial.write(gsmSerial.read());
  }

  Serial.println("\nDone! Now power-cycle the module, then upload the main sketch.");
}

void loop() {}
