/*
  Faranux Electronics - SIM800L GSM Module Test/Utility Sketch
  ---------------------------------------------------------------
  Module: SIM800L EVB V2.2

  Wiring (SIM800L EVB <-> Arduino UNO + external supply):
    5V IN (VBAT) -> External 5V, 2A-capable power supply
                    (NOT the Arduino 5V pin - module needs up to
                    ~2A peak current during transmit bursts, more
                    than the Arduino's onboard regulator can supply)
                    A 1000uF 16V low-ESR electrolytic capacitor is
                    added across VBAT/GND, close to the module, to
                    absorb these current spikes and prevent brownouts
                    during transmit.
    GND          -> Common ground: tie external supply GND AND
                    Arduino GND together here (shared reference is
                    required for TX/RX signaling to work correctly)
    VDD          -> Arduino 5V
                    (logic-level reference pin on this EVB - NOT a
                    power input for the module itself; lets the
                    board's onboard level shifting match the
                    Arduino's 5V logic on TX/RX)
    TXD (module transmit) -> Arduino pin 2 (RX, SoftwareSerial)
    RXD (module receive)  -> Arduino pin 3 (TX, SoftwareSerial)
                    (crossed: module TXD -> Arduino RX,
                    module RXD -> Arduino TX)
    Antenna      -> IPEX/U.FL pigtail cable from the board's antenna
                    connector to the SMA stub antenna. Always attach
                    before powering on - transmitting with no antenna
                    connected can damage the RF power amp, and the
                    module will never register on the network (LED
                    will fast-blink indefinitely, searching).

  Notes:
    - If AT commands get no response at all, first suspect power
      (unstable/insufficient supply, missing common ground, or
      missing antenna) before suspecting code.
    - LED behavior: off = not powered; fast blink (~1/sec) =
      powered but not registered (check antenna/SIM/signal); slow
      blink (~1 per 3 sec) = registered and idle.
*/

#include <SoftwareSerial.h>

// SIM800L EVB Tx & Rx connected to Arduino #3 & #2
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
