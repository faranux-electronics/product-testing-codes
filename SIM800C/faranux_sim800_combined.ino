/*
  Faranux Electronics - SIM800C GSM Module Test/Utility Sketch
  ---------------------------------------------------------------
  Module: SIM800C

  Wiring (SIM800C <-> Arduino UNO):
    VBAT (module power in) -> 4.2V - 5V stable current source
                               (NOT the Arduino 5V pin - module needs
                               up to ~2A peak current during transmit;
                               use a dedicated supply capable of that)
    PWR  (power key)        -> GND
                               (grounding PWR at boot powers the module
                               on; some boards auto-power-on and don't
                               need this pulled, check your specific board)
    GND                     -> GND (common ground with Arduino)
    TX   (module transmit)  -> Arduino pin 2 (RX, SoftwareSerial)
    RX   (module receive)   -> Arduino pin 3 (TX, SoftwareSerial)
                               (crossed: module TX -> Arduino RX,
                               module RX -> Arduino TX)

  Notes:
    - SIM800C RX is 3.3V logic. If your board's RX is not already
      level-shifted/tolerant, use a voltage divider or level shifter
      between Arduino TX (pin 3) and module RX.
    - If AT commands get no response at all, first suspect power
      (unstable/insufficient VBAT supply) before suspecting code.
*/

#include <SoftwareSerial.h>

// SIM800C Tx & Rx connected to Arduino #3 & #2
SoftwareSerial mySerial(3, 2);

// ---- CONFIG ----
const char* DEFAULT_NUMBER = "+ZZxxxxxxxxxx"; // change ZZ (country code) + number
const char* DEFAULT_MSG    = "Faranux Electronics | faranux.com";

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  Serial.println(F("Initializing SIM800..."));
  delay(1000);

  sendAT("AT");             // handshake
  sendAT("AT+CMGF=1");      // text mode
  sendAT("AT+CNMI=1,2,0,0,0"); // forward incoming SMS to serial immediately

  // Status/diagnostic checks
  sendAT("AT+CSQ");   // signal quality (0-31, higher = better)
  sendAT("AT+CCID");  // confirm SIM is present
  sendAT("AT+CREG?"); // network registration status

  Serial.println(F("Ready."));
  Serial.println(F("Type: SEND to send the default test SMS."));
  Serial.println(F("Or type any AT command directly to send it to the module."));
}

void loop() {
  // Forward any unsolicited data from SIM800 (incoming SMS, replies) to Serial monitor
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }

  // Handle input from Serial monitor
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.equalsIgnoreCase("SEND")) {
      sendSMS(DEFAULT_NUMBER, DEFAULT_MSG);
    } else if (input.length() > 0) {
      // Pass through any raw AT command typed by the user
      sendAT(input);
    }
  }
}

// Sends an AT command and waits briefly for a response, printing it to Serial
void sendAT(const String &command) {
  mySerial.println(command);
  delay(500);
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }
}

// Sends an SMS text message to the given number
void sendSMS(const char* number, const char* message) {
  mySerial.println("AT+CMGF=1"); // ensure text mode
  delay(200);
  while (mySerial.available()) Serial.write(mySerial.read());

  mySerial.print("AT+CMGS=\"");
  mySerial.print(number);
  mySerial.println("\"");
  delay(500);
  while (mySerial.available()) Serial.write(mySerial.read());

  mySerial.print(message);
  delay(200);

  mySerial.write(26); // Ctrl+Z to send
  delay(3000); // give module time to send and reply
  while (mySerial.available()) Serial.write(mySerial.read());

  Serial.println(F("SMS send attempt complete."));
}
