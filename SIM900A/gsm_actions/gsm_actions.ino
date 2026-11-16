#include <SoftwareSerial.h>

// --- Configuration ---
const int RX_PIN = 10;
const int TX_PIN = 11;
const long BAUD_RATE = 9600;

String targetPhone = "+250785313949";
String smsText     = "Faranux SIM900A SMS";

// --- Timeouts ---
const int DELAY_SHORT  = 100;
const int DELAY_NORMAL = 1000;
const int DELAY_SMS    = 3000;
const int RESPONSE_TIMEOUT = 5000;

SoftwareSerial mySerial(RX_PIN, TX_PIN);

// ─────────────────────────────────────────
// CORE HELPER: Send AT command, wait for expected response
// ─────────────────────────────────────────
bool sendAT(String command, String expected, int timeout = RESPONSE_TIMEOUT) {
  // Flush stale data before sending
  while (mySerial.available()) mySerial.read();
  delay(50);

  if (command.length() > 0) {
    mySerial.print(command + "\r\n");  // Manual CRLF, not println
    Serial.println(">> " + command);
  }

  String response = "";
  unsigned long start = millis();

  while (millis() - start < (unsigned long)timeout) {
    while (mySerial.available()) {
      response += (char)mySerial.read();
    }
    if (response.indexOf(expected) >= 0) {
      Serial.println("<< " + response);
      return true;
    }
  }

  Serial.println("<< TIMEOUT / NO MATCH: " + response);
  return false;
}

// ─────────────────────────────────────────
// Read all pending GSM output to Serial
// ─────────────────────────────────────────
void drainGSM() {
  delay(DELAY_SHORT);
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }
}

// ─────────────────────────────────────────
// SETUP
// ─────────────────────────────────────────
void setup() {
  Serial.begin(BAUD_RATE);
  mySerial.begin(BAUD_RATE);
  delay(DELAY_NORMAL);

  Serial.println(F("=== SIM900A Controller ==="));

  // Verify module is alive
  if (sendAT("AT", "OK", 2000)) {
    Serial.println(F("[OK] Module responding"));
  } else {
    Serial.println(F("[ERR] Module not responding — check wiring/power"));
  }

  // Check SIM
  if (sendAT("AT+CPIN?", "READY", 2000)) {
    Serial.println(F("[OK] SIM ready"));
  } else {
    Serial.println(F("[WARN] SIM not ready or missing"));
  }

  // Signal quality
  mySerial.print("AT+CSQ\r\n");
  delay(500);
  drainGSM();

  printMenu();
}

void printMenu() {
  Serial.println();
  Serial.println(F("Commands:"));
  Serial.println(F("  s - Send SMS"));
  Serial.println(F("  c - Make call"));
  Serial.println(F("  h - Hang up"));
  Serial.println(F("  e - Redial"));
  Serial.println(F("  i - Answer call"));
  Serial.println(F("  r - Read SMS inbox"));
  Serial.println(F("  ? - Show this menu"));
  Serial.println();
}

// ─────────────────────────────────────────
// LOOP
// ─────────────────────────────────────────
void loop() {
  // Forward GSM responses to Serial Monitor
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }

  // Handle user commands
  if (Serial.available()) {
    char cmd = Serial.read();
    Serial.println(cmd);  // Echo the command

    switch (cmd) {
      case 's': sendSMS();    break;
      case 'c': makeCall();   break;
      case 'h': hangUp();     break;
      case 'e': redial();     break;
      case 'i': answerCall(); break;
      case 'r': readSMS();    break;
      case '?': printMenu();  break;
    }
  }
}

// ─────────────────────────────────────────
// SMS
// ─────────────────────────────────────────
void sendSMS() {
  Serial.println(F("[SMS] Sending..."));

  // Set text mode
  if (!sendAT("AT+CMGF=1", "OK")) {
    Serial.println(F("[ERR] Could not set text mode"));
    return;
  }

  // Send recipient — wait for '>' prompt
  if (!sendAT("AT+CMGS=\"" + targetPhone + "\"", ">")) {
    Serial.println(F("[ERR] No prompt from module"));
    return;
  }

  // Send message body then Ctrl+Z
  mySerial.print(smsText);
  mySerial.write(26);  // Ctrl+Z
  Serial.println(F("[SMS] Message body sent, waiting for confirmation..."));

  // Wait for +CMGS acknowledgement
  if (sendAT("", "+CMGS", DELAY_SMS)) {
    Serial.println(F("[OK] SMS sent successfully"));
  } else {
    Serial.println(F("[ERR] SMS send failed or timed out"));
  }
}

void readSMS() {
  Serial.println(F("[SMS] Reading inbox (unread)..."));
  sendAT("AT+CMGF=1", "OK");
  mySerial.print("AT+CMGL=\"REC UNREAD\"\r\n");
  delay(DELAY_NORMAL);
  drainGSM();
}

// ─────────────────────────────────────────
// CALLS
// ─────────────────────────────────────────
void makeCall() {
  Serial.println("[CALL] Dialling " + targetPhone);

  // Flush any pending data first
  while (mySerial.available()) mySerial.read();
  delay(100);

  // Use print with \r — ATD uses ; as terminator, not \r\n
  mySerial.print("ATD" + targetPhone + ";\r");

  // ATD returns OK after connecting — wait up to 8 seconds
  unsigned long start = millis();
  String response = "";

  while (millis() - start < 8000) {
    while (mySerial.available()) {
      char c = mySerial.read();
      response += c;
      Serial.write(c);
    }
    if (response.indexOf("OK")         >= 0 ||
        response.indexOf("CONNECT")    >= 0 ||
        response.indexOf("ERROR")      >= 0 ||
        response.indexOf("NO CARRIER") >= 0 ||
        response.indexOf("BUSY")       >= 0) {
      break;
    }
  }

  if (response.indexOf("OK") >= 0 || response.indexOf("CONNECT") >= 0) {
    Serial.println(F("[OK] Call connected"));
  } else if (response.indexOf("BUSY") >= 0) {
    Serial.println(F("[CALL] Line busy"));
  } else if (response.indexOf("NO CARRIER") >= 0) {
    Serial.println(F("[CALL] No carrier / not answered"));
  } else if (response.indexOf("ERROR") >= 0) {
    Serial.println(F("[ERR] Call failed"));
  } else {
    Serial.println(F("[CALL] Ringing... (no response yet)"));
  }
}

void hangUp() {
  Serial.println(F("[CALL] Hanging up..."));
  if (sendAT("ATH", "OK")) {
    Serial.println(F("[OK] Call ended"));
  } else {
    Serial.println(F("[ERR] Hang up failed"));
  }
}

void answerCall() {
  Serial.println(F("[CALL] Answering..."));
  if (sendAT("ATA", "OK")) {
    Serial.println(F("[OK] Call connected"));
  } else {
    Serial.println(F("[ERR] Could not answer call"));
  }
}

void redial() {
  Serial.println(F("[CALL] Redialling last number..."));

  // Flush before send
  while (mySerial.available()) mySerial.read();
  delay(100);

  mySerial.print("ATDL\r");

  unsigned long start = millis();
  String response = "";

  while (millis() - start < 8000) {
    while (mySerial.available()) {
      char c = mySerial.read();
      response += c;
      Serial.write(c);
    }
    if (response.indexOf("OK")         >= 0 ||
        response.indexOf("CONNECT")    >= 0 ||
        response.indexOf("ERROR")      >= 0 ||
        response.indexOf("NO CARRIER") >= 0 ||
        response.indexOf("BUSY")       >= 0) {
      break;
    }
  }

  if (response.indexOf("OK") >= 0 || response.indexOf("CONNECT") >= 0) {
    Serial.println(F("[OK] Redialling connected"));
  } else if (response.indexOf("BUSY") >= 0) {
    Serial.println(F("[CALL] Line busy"));
  } else {
    Serial.println(F("[CALL] Redialling... (no response yet)"));
  }
}