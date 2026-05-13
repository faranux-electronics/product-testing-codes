/*
 * ==============================================================================
 * A7670E (LTE Cat-1) MASTER TESTING TOOL FOR ARDUINO UNO
 * Fixed for 9600 baud — SoftwareSerial is stable at this speed.
 * ==============================================================================
 *
 * HARDWARE WIRING (7-Pin Breakout Header):
 * - `5-12V`: 6V external power supply positive
 * - `GND`: shared ground for Arduino and the power supply
 * - `TX`: module TX → Arduino Pin 10 (SoftwareSerial RX)
 * - `RX`: module RX → Arduino Pin 11 (SoftwareSerial TX)
 * - `3.8EN`: leave unconnected if the board auto-boots
 * - `NET`: not used for this sketch
 * - `DTR`: not used for this sketch
 *
 * Connector order on the board header (top to bottom): `DTR`, `NET`, `3.8EN`, `RX`, `TX`, `GND`, `5-12V`.
 *
 * POWER REQUIREMENTS: The board requires at least 6V with 2A current for proper operation.
 *
 * LED STATUS INDICATORS:
 * - Slow Blinking (~5 sec): Low power / Not registered
 * - Fast Blinking (~100ms): Successfully registered
 * - Off: Module is powered down or stuck in a boot-loop.
 *
 * PREREQUISITE: You must have already run A7670E_BaudChanger.ino once to
 * reprogram the module from 115200 to 9600. If you skip that step you will
 * see garbled text again.
 *
 * HOW THE SKETCH WORKS: The sketch initializes communication with the A7670E
 * module at 9600 baud using SoftwareSerial. It provides a text-based menu in
 * the Arduino Serial Monitor where users can select options by typing letters.
 * Each option sends specific AT commands to the module to perform network checks,
 * send SMS, make calls, or perform HTTP requests. Responses from the module are
 * displayed in the Serial Monitor for debugging and verification.
 * ==============================================================================
 */

#include <SoftwareSerial.h>

// RX=10 (connect to module TX/Pin 4), TX=11 (connect to module RX/Pin 5)
SoftwareSerial gsmSerial(10, 11);

// --- CONFIGURATION ---
String TARGET_PHONE = "+2507xxxxxxxx"; // Replace with your phone number (with country code)
String APN = "internet"; // MTN and Airtel Rwanda standard APN

void setup() {
  Serial.begin(9600);   // Match this in Serial Monitor (bottom-right dropdown!)
  gsmSerial.begin(9600); // Module is now at 9600 after running BaudChanger

  delay(3000);
  Serial.println(F("\n================================="));
  Serial.println(F("   A7670E MASTER TESTING TOOL    "));
  Serial.println(F("================================="));

  // Basic health check on boot
  sendATCommand(F("AT"), 1000);
  sendATCommand(F("AT+CMEE=2"), 1000); // Verbose text errors

  displayMenu();
}

void loop() {
  // 1. Listen for user commands from the Serial Monitor
  if (Serial.available() > 0) {
    char choice = Serial.read();
    if (choice != '\n' && choice != '\r') {
      executeCommand(choice);
    }
  }

  // 2. Passthrough: print anything the module says (unsolicited SMS, calls, etc.)
  while (gsmSerial.available()) {
    Serial.write(gsmSerial.read());
  }
}

// ==========================================
// MENU SYSTEM
// ==========================================

void displayMenu() {
  Serial.println(F("\n--- MAIN MENU ---"));
  Serial.println(F("[N] - Check Network & Signal"));
  Serial.println(F("[S] - Send Test SMS"));
  Serial.println(F("[C] - Call Phone Number"));
  Serial.println(F("[H] - Hang Up Call"));
  Serial.println(F("[A] - Answer Incoming Call"));
  Serial.println(F("[G] - HTTP GET Request (Download)"));
  Serial.println(F("[P] - HTTP POST Request (Upload)"));
  Serial.println(F("[M] - Reprint this Menu"));
  Serial.println(F("Type a letter and press Enter:"));
}

void executeCommand(char choice) {
  choice = toupper(choice);
  Serial.println(F("\n========================"));

  switch (choice) {
    case 'N':
      Serial.println(F("Running Network Diagnostics..."));
      sendATCommand(F("AT+CPIN?"), 2000);  // SIM status
      sendATCommand(F("AT+CSQ"), 2000);    // Signal strength
      sendATCommand(F("AT+CEREG?"), 2000); // 4G registration
      sendATCommand(F("AT+COPS?"), 3000);  // Carrier name
      break;

    case 'S':
      Serial.print(F("Sending SMS to "));
      Serial.print(TARGET_PHONE);
      Serial.println(F("..."));
      sendATCommand(F("AT+CMGF=1"), 2000); // Text mode
      gsmSerial.print(F("AT+CMGS=\""));
      gsmSerial.print(TARGET_PHONE);
      gsmSerial.println(F("\""));
      delay(1500); // Wait for '>' prompt
      gsmSerial.print(F("Hello! Automated test from A7670E."));
      delay(500);
      gsmSerial.write(26); // CTRL+Z to send
      Serial.println(F(">> Message sent. Waiting for +CMGS confirmation..."));
      delay(5000); // Network needs time
      while (gsmSerial.available()) Serial.write(gsmSerial.read());
      break;

    case 'C':
      Serial.print(F("Dialing "));
      Serial.print(TARGET_PHONE);
      Serial.println(F("..."));
      gsmSerial.print(F("ATD"));
      gsmSerial.print(TARGET_PHONE);
      gsmSerial.println(F(";")); // Semicolon = voice call
      delay(2000);
      while (gsmSerial.available()) Serial.write(gsmSerial.read());
      break;

    case 'H':
      Serial.println(F("Hanging up..."));
      sendATCommand(F("ATH"), 2000);
      break;

    case 'A':
      Serial.println(F("Answering call..."));
      sendATCommand(F("ATA"), 2000);
      break;

    case 'G':
      Serial.println(F("HTTP GET from httpbin.org..."));
      sendATCommand(F("AT+CGDCONT=1,\"IP\",\"internet\""), 2000);
      sendATCommand(F("AT+HTTPINIT"), 2000);
      sendATCommand(F("AT+HTTPPARA=\"URL\",\"http://httpbin.org/get\""), 2000);
      Serial.println(F(">> Waiting for server response..."));
      sendATCommand(F("AT+HTTPACTION=0"), 8000);
      sendATCommand(F("AT+HTTPREAD=0,500"), 4000);
      sendATCommand(F("AT+HTTPTERM"), 2000);
      break;

    case 'P':
      Serial.println(F("HTTP POST to httpbin.org..."));
      sendATCommand(F("AT+CGDCONT=1,\"IP\",\"internet\""), 2000);
      sendATCommand(F("AT+HTTPINIT"), 2000);
      sendATCommand(F("AT+HTTPPARA=\"URL\",\"http://httpbin.org/post\""), 2000);
      sendATCommand(F("AT+HTTPPARA=\"CONTENT\",\"application/json\""), 2000);
      sendATCommand(F("AT+HTTPDATA=18,10000"), 2000);
      gsmSerial.println(F("{\"sensor_val\": 99}"));
      delay(2000);
      Serial.println(F(">> Uploading payload..."));
      sendATCommand(F("AT+HTTPACTION=1"), 8000);
      sendATCommand(F("AT+HTTPREAD=0,500"), 4000);
      sendATCommand(F("AT+HTTPTERM"), 2000);
      break;

    case 'M':
      displayMenu();
      break;

    default:
      Serial.println(F("Invalid choice. Type M for the menu."));
      break;
  }
  Serial.println(F("\n[ Done. Waiting for next command... ]"));
}

// ==========================================
// HELPER — sends command, prints response
// ==========================================

// Overload for Flash strings (F() macro) to save RAM
void sendATCommand(const __FlashStringHelper* command, int delayMs) {
  Serial.print(F(">> "));
  Serial.println(command);
  gsmSerial.println(command);
  delay(delayMs);
  while (gsmSerial.available()) {
    Serial.write(gsmSerial.read());
  }
}

// Overload for regular String objects
void sendATCommand(String command, int delayMs) {
  Serial.print(F(">> "));
  Serial.println(command);
  gsmSerial.println(command);
  delay(delayMs);
  while (gsmSerial.available()) {
    Serial.write(gsmSerial.read());
  }
}
