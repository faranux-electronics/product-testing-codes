/*
  Faranux Electronics - SIM900 GSM/GPRS Shield Test/Utility Sketch
  ---------------------------------------------------------------
  Module: SIM900 GSM/GPRS Shield (stackable Arduino shield form,
          onboard barrel-jack power input, SIM slot on back,
          master power select + power key, UART serial selector
          switch for Hardware vs Software Serial)

  AT command compatibility note:
    SIM900's AT command set overlaps almost entirely with SIM800's
    for the commands used in this sketch (AT, AT+CMGF, AT+CNMI,
    AT+CSQ, AT+CCID, AT+CREG?, AT+CMGS).

  Power-up sequence:
    1. Power the shield via its barrel jack: 5V-12V DC, 2A capable.
    2. Confirm the onboard Power LED lights up (means it's getting
       enough power).
    3. Press and hold the Power Key for ~2 seconds to turn the
       module on.
    4. STATUS and NETLIGHT LEDs should start blinking.
    5. Wait up to ~30 seconds - once NETLIGHT blinks once every
       3 seconds, the SIM has registered on the network and it's
       ready for AT commands.
    (A valid, unlocked SIM card must be inserted in the slot on
    the back of the shield before power-up.)

  Wiring (this build - SoftwareSerial on D9/D10):
    Serial selector switch on shield -> set to Software Serial
    Arduino D9  -> shield UART TX pin  (Arduino TX out to module RX)
    Arduino D10 -> shield UART RX pin  (Arduino RX in from module TX)
    Shield UART GND -> Arduino GND (common ground, required)

    Note: the shield also has a Hardware Serial option (D0/D1),
    but that ties up the same pins used for USB programming - you'd
    have to disconnect the module every time you upload new code.
    Software Serial (this build) avoids that hassle.

  Notes:
    - If AT commands get no response at all, first check: power LED
      lit, power key held long enough to boot, SIM inserted, and
      the serial selector switch actually set to Software Serial
      mode (not left on Hardware Serial).
    - LED behavior: off = not powered; STATUS/NETLIGHT blinking
      fast/erratically = powered but not yet registered (searching
      for network, or missing/locked SIM, or no antenna); NETLIGHT
      blinking once every ~3 seconds = registered and idle.
*/

#include <SoftwareSerial.h>

// SIM900 shield: Arduino D9 = TX (to module RX), D10 = RX (from module TX)
SoftwareSerial mySerial(10, 9);

// ---- CONFIG ----
const char* DEFAULT_NUMBER = "+ZZxxxxxxxxx"; // change ZZ (country code) + number
const char* DEFAULT_MSG    = "Faranux Electronics | faranux.com";

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  Serial.println(F("Initializing SIM900..."));
  delay(1000);

  sendAT("AT");             // handshake
  sendAT("AT+CMGF=1");      // text mode
  sendAT("AT+CNMI=1,2,0,0,0"); // forward incoming SMS to serial immediately

  // Status/diagnostic checks
  checkStatus();

  Serial.println(F("Ready."));
  Serial.println(F("Type: SEND to send the default test SMS."));
  Serial.println(F("Type: STATUS to re-check signal/SIM/network."));
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
    } else if (input.equalsIgnoreCase("STATUS")) {
      checkStatus();
    } else if (input.length() > 0) {
      // Pass through any raw AT command typed by the user
      sendAT(input);
    }
  }
}

// Runs the signal quality / SIM presence / network registration checks
void checkStatus() {
  sendAT("AT+CSQ");   // signal quality (0-31, higher = better)
  sendAT("AT+CCID");  // confirm SIM is present
  sendAT("AT+CREG?"); // network registration status
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
