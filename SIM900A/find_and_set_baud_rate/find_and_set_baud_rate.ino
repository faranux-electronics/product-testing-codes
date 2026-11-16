#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11);

// Common SIM900A baud rates to try
long baudRates[] = {9600, 4800, 19200, 38400, 57600, 115200};
int totalRates = 6;

void setup() {
  Serial.begin(9600);
  Serial.println("Scanning for SIM900A baud rate...");

  for (int i = 0; i < totalRates; i++) {
    Serial.print("Trying: ");
    Serial.println(baudRates[i]);

    mySerial.begin(baudRates[i]);
    delay(500);

    // Flush any garbage
    while (mySerial.available()) mySerial.read();

    // Send AT and wait for response
    mySerial.println("AT");
    delay(500);

    String response = "";
    while (mySerial.available()) {
      response += (char)mySerial.read();
    }

    if (response.indexOf("OK") >= 0) {
      Serial.print(">>> FOUND! SIM900A is at: ");
      Serial.println(baudRates[i]);

      // Lock it to 9600 permanently
      mySerial.println("AT+IPR=9600");
      delay(500);
      Serial.println("Locked to 9600. Restart and use 9600 sketch.");
      while (true); // Stop here
    }
  }

  Serial.println("Not found on any baud rate. Check wiring.");
}

void loop() {}