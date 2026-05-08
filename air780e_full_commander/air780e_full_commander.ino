/*
 * ============================================================
 * Air780E 4G Cat.1 Module — Arduino Uno Full Commander
 * Author  : Faranux Team
 * Date    : 2026-05-08
 * Board   : Arduino Uno + Air780E Mini 4G Dev Board
 * ============================================================
 */

#include <SoftwareSerial.h>

SoftwareSerial air780(10, 11); // RX=pin10, TX=pin11

// ============================================================
//   CHANGE THESE TO YOUR OWN VALUES
// ============================================================
String phoneNumber = "+250786396995";
String smsMessage  = "Hello from Arduino and Faranux Air780E!";
String apnName     = "internet";         // Airtel Rwanda APN
String httpGetUrl  = "http://httpbin.org/get";
String httpPostUrl = "http://httpbin.org/post";
String tcpHost     = "httpbin.org";
String tcpPort     = "80";
// ============================================================

void setup() {
  Serial.begin(115200);
  air780.begin(115200);
  delay(3000);

  // --- MODULE WAKE-UP SEQUENCE ---
  Serial.println(F(">>> Booting up Air780E..."));
  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);  // "Press" the power button
  delay(1500);           // Hold it for 1.5 seconds
  digitalWrite(7, HIGH); // "Release" the power button
  delay(3000); // Wait for the module to finish booting

  Serial.println(F("============================================================"));
  Serial.println(F("   Air780E 4G Cat.1 — Arduino Uno Full Commander"));
  Serial.println(F("============================================================"));
  printMenu();
}

void loop() {
  // Forward module responses to Serial Monitor
  while (air780.available()) {
    Serial.write(air780.read());
    delay(1);
  }

  // Read commands from Serial Monitor
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    // ---- BASIC ----
    if      (cmd == "1")  { sendAT("AT"); }
    else if (cmd == "2")  { sendAT("ATI"); }
    else if (cmd == "3")  { sendAT("AT+CGSN"); }
    else if (cmd == "4")  { sendAT("AT+CGMR"); }
    else if (cmd == "5")  { sendAT("ATZ"); }

    // ---- NETWORK ----
    else if (cmd == "10") { sendAT("AT+CREG?"); }
    else if (cmd == "11") { sendAT("AT+CSQ"); }
    else if (cmd == "12") { sendAT("AT+COPS?"); }
    else if (cmd == "13") { sendAT("AT+CGATT?"); }
    else if (cmd == "14") { sendAT("AT+CEREG?"); }
    else if (cmd == "15") { sendAT("AT+NITZ?"); }

    // ---- SIM ----
    else if (cmd == "20") { sendAT("AT+CIMI"); }
    else if (cmd == "21") { sendAT("AT+CCID"); }
    else if (cmd == "22") { sendAT("AT+CPIN?"); }

    // ---- SMS ----
    else if (cmd == "40") { sendAT("AT+CMGF=1"); }
    else if (cmd == "41") { sendSMS(phoneNumber, smsMessage); }
    else if (cmd == "42") { listSMS(); }
    else if (cmd == "43") { readSMS(1); }
    else if (cmd == "44") { deleteSMS(1); }
    else if (cmd == "45") { deleteAllSMS(); }

    // ---- INTERNET ----
    else if (cmd == "50") { setAPN(apnName); }
    else if (cmd == "51") { activateData(); }
    else if (cmd == "52") { sendAT("AT+CGPADDR=1"); }
    else if (cmd == "53") { sendAT("AT+CGACT=0,1"); }

    // ---- HTTP ----
    else if (cmd == "60") { httpGET(httpGetUrl); }
    else if (cmd == "61") { httpPOST(httpPostUrl, "field1=hello&field2=arduino"); }

    // ---- TCP ----
    else if (cmd == "70") { openTCP(tcpHost, tcpPort); }
    else if (cmd == "71") { sendTCP("GET / HTTP/1.0\r\nHost: httpbin.org\r\n\r\n"); }
    else if (cmd == "72") { closeTCP(); }

    // ---- POWER ----
    else if (cmd == "80") { powerOff(); }
    else if (cmd == "81") { sendAT("AT+CSCLK=1"); }
    else if (cmd == "82") { sendAT("AT+CSCLK=0"); }
    else if (cmd == "83") { sendAT("AT+CFUN=4"); }
    else if (cmd == "84") { sendAT("AT+CFUN=1"); }

    // ---- TOOLS ----
    else if (cmd == "99")                    { runDiagnostics(); }
    else if (cmd == "menu" || cmd == "MENU") { printMenu(); }

    // ---- RAW AT COMMAND ----
    else if (cmd.length() > 0) { sendAT(cmd); }
  }
}

// ============================================================
//   BASIC FUNCTIONS
// ============================================================

void sendAT(String cmd) {
  Serial.print(F(">>> "));
  Serial.println(cmd);
  air780.println(cmd);
  delay(800);
}

String sendATwaitResponse(String cmd, int timeout) {
  Serial.print(F(">>> "));
  Serial.println(cmd);
  air780.println(cmd);
  String response = "";
  unsigned long t = millis();
  while (millis() - t < timeout) {
    while (air780.available()) {
      char c = air780.read();
      response += c;
      Serial.write(c);
      delay(1);
    }
  }
  return response;
}

bool sendATwaitOK(String cmd, int timeout) {
  String r = sendATwaitResponse(cmd, timeout);
  return r.indexOf("OK") >= 0;
}

// ============================================================
//   SMS FUNCTIONS
// ============================================================

void sendSMS(String number, String text) {
  Serial.print(F("\n>>> Sending SMS to "));
  Serial.println(number);
  Serial.print(F(">>> Message: "));
  Serial.println(text);

  // Set text mode
  if (!sendATwaitOK("AT+CMGF=1", 2000)) {
    Serial.println(F("ERROR: Could not set SMS text mode."));
    return;
  }

  // Send CMGS command
  String cmd = "AT+CMGS=\"" + number + "\"";
  Serial.print(F(">>> "));
  Serial.println(cmd);
  air780.println(cmd);

  // Wait for '>' prompt
  unsigned long t = millis();
  bool gotPrompt = false;
  while (millis() - t < 5000) {
    if (air780.available()) {
      char c = air780.read();
      Serial.write(c);
      if (c == '>') { gotPrompt = true; break; }
    }
  }

  if (!gotPrompt) {
    Serial.println(F("\nERROR: No '>' prompt received."));
    return;
  }

  // Send message text
  delay(200);
  Serial.print(F("\n>>> "));
  Serial.println(text);
  air780.print(text);

  // Send Ctrl+Z (ASCII 26) to finish
  delay(200);
  air780.write(26);
  Serial.println(F(">>> [Ctrl+Z sent — waiting for confirmation...]"));

  // Wait for +CMGS confirmation
  unsigned long t2 = millis();
  String response = "";
  while (millis() - t2 < 15000) {
    if (air780.available()) {
      char c = air780.read();
      response += c;
      Serial.write(c);
    }
    if (response.indexOf("+CMGS:") >= 0) {
      Serial.println(F("\n>>> ✓ SMS SENT SUCCESSFULLY!"));
      break;
    }
    if (response.indexOf("ERROR") >= 0) {
      Serial.println(F("\n>>> ✗ SMS FAILED. Check SIM and signal."));
      break;
    }
  }
}

void listSMS() {
  Serial.println(F(">>> Listing all SMS..."));
  sendATwaitOK("AT+CMGF=1", 2000);
  sendAT("AT+CMGL=\"ALL\"");
}

void readSMS(int index) {
  Serial.print(F(">>> Reading SMS at index "));
  Serial.println(index);
  sendATwaitOK("AT+CMGF=1", 2000);
  sendAT("AT+CMGR=" + String(index));
}

void deleteSMS(int index) {
  Serial.print(F(">>> Deleting SMS at index "));
  Serial.println(index);
  sendAT("AT+CMGD=" + String(index));
}

void deleteAllSMS() {
  Serial.println(F(">>> Deleting ALL SMS..."));
  sendAT("AT+CMGD=1,4");
}

// ============================================================
//   INTERNET / DATA FUNCTIONS
// ============================================================

void setAPN(String apn) {
  Serial.print(F(">>> Setting APN to: "));
  Serial.println(apn);
  sendAT("AT+CGDCONT=1,\"IP\",\"" + apn + "\"");
}

void activateData() {
  Serial.println(F(">>> Activating data connection..."));
  if (sendATwaitOK("AT+CGACT=1,1", 10000)) {
    Serial.println(F(">>> ✓ Data activated!"));
    sendAT("AT+CGPADDR=1");
  } else {
    Serial.println(F(">>> ✗ Data activation failed. Check APN."));
  }
}

// ============================================================
//   HTTP FUNCTIONS
// ============================================================

void httpGET(String url) {
  Serial.print(F("\n>>> HTTP GET: "));
  Serial.println(url);

  sendATwaitOK("AT+HTTPTERM", 2000);
  delay(500);

  if (!sendATwaitOK("AT+HTTPINIT", 3000)) {
    Serial.println(F("ERROR: HTTP init failed. Activate data first (cmd 51)."));
    return;
  }

  sendATwaitOK("AT+HTTPPARA=\"CID\",1", 2000);
  sendATwaitOK("AT+HTTPPARA=\"URL\",\"" + url + "\"", 2000);

  Serial.println(F(">>> Sending GET request..."));
  String resp = sendATwaitResponse("AT+HTTPACTION=0", 10000);

  delay(2000);
  Serial.println(F(">>> Reading response..."));
  sendAT("AT+HTTPREAD");

  sendAT("AT+HTTPTERM");
  Serial.println(F(">>> ✓ HTTP GET done."));
}

void httpPOST(String url, String data) {
  Serial.print(F("\n>>> HTTP POST: "));
  Serial.println(url);
  Serial.print(F(">>> Data: "));
  Serial.println(data);

  sendATwaitOK("AT+HTTPTERM", 2000);
  delay(500);

  if (!sendATwaitOK("AT+HTTPINIT", 3000)) {
    Serial.println(F("ERROR: HTTP init failed. Activate data first (cmd 51)."));
    return;
  }

  sendATwaitOK("AT+HTTPPARA=\"CID\",1", 2000);
  sendATwaitOK("AT+HTTPPARA=\"URL\",\"" + url + "\"", 2000);
  sendATwaitOK("AT+HTTPPARA=\"CONTENT\",\"application/x-www-form-urlencoded\"", 2000);

  // Set data length
  String dataCmd = "AT+HTTPDATA=" + String(data.length()) + ",5000";
  Serial.print(F(">>> "));
  Serial.println(dataCmd);
  air780.println(dataCmd);

  // Wait for DOWNLOAD prompt
  unsigned long t = millis();
  while (millis() - t < 5000) {
    if (air780.available()) {
      String line = air780.readString();
      Serial.print(line);
      if (line.indexOf("DOWNLOAD") >= 0) break;
    }
  }

  delay(500);
  air780.print(data);
  delay(2000);

  Serial.println(F(">>> Sending POST request..."));
  sendATwaitResponse("AT+HTTPACTION=1", 10000);
  delay(2000);

  Serial.println(F(">>> Reading response..."));
  sendAT("AT+HTTPREAD");

  sendAT("AT+HTTPTERM");
  Serial.println(F(">>> ✓ HTTP POST done."));
}

// ============================================================
//   TCP FUNCTIONS
// ============================================================

void openTCP(String host, String port) {
  Serial.print(F("\n>>> Opening TCP to "));
  Serial.print(host);
  Serial.print(F(":"));
  Serial.println(port);
  
  sendATwaitOK("AT+CIPMUX=0", 2000);
  sendATwaitOK("AT+CSTT=\"" + apnName + "\",\"\",\"\"", 5000);
  sendATwaitOK("AT+CIICR", 10000);
  sendAT("AT+CIFSR");

  String cmd = "AT+CIPSTART=\"TCP\",\"" + host + "\",\"" + port + "\"";
  String resp = sendATwaitResponse(cmd, 10000);
  if (resp.indexOf("CONNECT OK") >= 0) {
    Serial.println(F(">>> ✓ TCP Connected!"));
  } else {
    Serial.println(F(">>> ✗ TCP connection failed."));
  }
}

void sendTCP(String data) {
  Serial.println(F(">>> Sending TCP data..."));
  air780.println("AT+CIPSEND");
  delay(1000);
  air780.print(data);
  air780.write(26); // Ctrl+Z
  delay(3000);
  Serial.println(F(">>> ✓ TCP data sent."));
}

void closeTCP() {
  Serial.println(F(">>> Closing TCP connection..."));
  sendAT("AT+CIPCLOSE");
}

// ============================================================
//   POWER FUNCTIONS
// ============================================================

void powerOff() {
  Serial.println(F(">>> Powering off module..."));
  Serial.println(F(">>> (Reconnect power to restart)"));
  sendAT("AT+CPOWD=1");
}

// ============================================================
//   DIAGNOSTICS
// ============================================================

void runDiagnostics() {
  Serial.println(F("\n============================================================"));
  Serial.println(F("   Running full diagnostics..."));
  Serial.println(F("============================================================"));
  sendATwaitResponse("AT",        1000);
  sendATwaitResponse("AT+CGSN",   1000);
  sendATwaitResponse("AT+CPIN?",  1000);
  sendATwaitResponse("AT+CSQ",    1000);
  sendATwaitResponse("AT+CREG?",  1000);
  sendATwaitResponse("AT+CEREG?", 1000);
  sendATwaitResponse("AT+COPS?",  2000);
  sendATwaitResponse("AT+CGATT?", 1000);
  Serial.println(F("============================================================"));
  Serial.println(F("   Diagnostics complete."));
  Serial.println(F("============================================================\n"));
}

// ============================================================
//   MENU
// ============================================================

void printMenu() {
  Serial.println();
  Serial.println(F("--- BASIC ---"));
  Serial.println(F("  1  = AT (ping)"));
  Serial.println(F("  2  = ATI (module info)"));
  Serial.println(F("  3  = AT+CGSN (IMEI)"));
  Serial.println(F("  4  = AT+CGMR (firmware)"));
  Serial.println(F("  5  = ATZ (factory reset)"));
  Serial.println(F("--- NETWORK ---"));
  Serial.println(F("  10 = AT+CREG? (GSM registration)"));
  Serial.println(F("  11 = AT+CSQ (signal strength)"));
  Serial.println(F("  12 = AT+COPS? (operator)"));
  Serial.println(F("  13 = AT+CGATT? (GPRS)"));
  Serial.println(F("  14 = AT+CEREG? (LTE)"));
  Serial.println(F("  15 = AT+NITZ? (network time)"));
  Serial.println(F("--- SIM ---"));
  Serial.println(F("  20 = AT+CIMI (IMSI)"));
  Serial.println(F("  21 = AT+CCID (ICCID)"));
  Serial.println(F("  22 = AT+CPIN? (PIN status)"));

  Serial.println(F("--- SMS ---"));
  Serial.println(F("  40 = Set SMS text mode"));
  
  Serial.print(F("  41 = Send SMS -> "));
  Serial.println(phoneNumber);
  
  Serial.println(F("  42 = List all SMS"));
  Serial.println(F("  43 = Read SMS #1"));
  Serial.println(F("  44 = Delete SMS #1"));
  Serial.println(F("  45 = Delete ALL SMS"));
  Serial.println(F("--- INTERNET ---"));
  
  Serial.print(F("  50 = Set APN ("));
  Serial.print(apnName);
  Serial.println(F(")"));
  
  Serial.println(F("  51 = Activate data connection"));
  Serial.println(F("  52 = Get IP address"));
  Serial.println(F("  53 = Deactivate data"));
  Serial.println(F("--- HTTP ---"));
  
  Serial.print(F("  60 = HTTP GET  -> "));
  Serial.println(httpGetUrl);
  Serial.print(F("  61 = HTTP POST -> "));
  Serial.println(httpPostUrl);
  
  Serial.println(F("--- TCP ---"));
  Serial.print(F("  70 = Open TCP -> "));
  Serial.println(tcpHost);
  
  Serial.println(F("  71 = Send TCP data"));
  Serial.println(F("  72 = Close TCP"));
  Serial.println(F("--- POWER ---"));
  Serial.println(F("  80 = Power off module"));
  Serial.println(F("  81 = Enable sleep mode"));
  Serial.println(F("  82 = Disable sleep mode"));
  Serial.println(F("  83 = Flight mode ON"));
  Serial.println(F("  84 = Flight mode OFF"));
  Serial.println(F("--- TOOLS ---"));
  Serial.println(F("  99   = Full diagnostics"));
  Serial.println(F("  menu = Show this menu"));
  Serial.println(F("  (or type any raw AT command)"));
  Serial.println(F("============================================================"));
}