#define TINY_GSM_MODEM_SIM800

#include <Arduino.h>
#include <Wire.h>
#include <TinyGsmClient.h>

constexpr int MODEM_RX       = 26;
constexpr int MODEM_TX       = 27;
constexpr int MODEM_POWER_ON = 23;
constexpr int MODEM_RST      = 5;
constexpr int MODEM_PWRKEY   = 4;

const char RECIPIENT[] = "+250785313949";
const char MESSAGE[] =
  "Hello! This is a faranux.com test SMS from my LilyGO T-Call ESP32.";

HardwareSerial SerialAT(1);
TinyGsm modem(SerialAT);

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Prevent accidentally sending to the placeholder.
  if (String(RECIPIENT).indexOf('X') >= 0) {
    Serial.println("Replace RECIPIENT with your phone number.");
    return;
  }

  // IP5306 power management: keep power enabled.
  Wire.begin(21, 22);
  Wire.beginTransmission(0x75);
  Wire.write(0x00);
  Wire.write(0x37);

  if (Wire.endTransmission() != 0) {
    Serial.println("IP5306 not detected. Check board revision.");
    return;
  }

  pinMode(MODEM_POWER_ON, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_PWRKEY, OUTPUT);

  digitalWrite(MODEM_POWER_ON, HIGH);
  digitalWrite(MODEM_RST, HIGH);

  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(1000);

  // Start the modem only if it is not already responding.
  if (!modem.testAT(2000)) {
    digitalWrite(MODEM_PWRKEY, HIGH);
    delay(100);
    digitalWrite(MODEM_PWRKEY, LOW);
    delay(1000);
    digitalWrite(MODEM_PWRKEY, HIGH);
    delay(5000);
  }

  Serial.println("Initializing modem...");

  if (!modem.init()) {
    Serial.println("Modem initialization failed.");
    Serial.println("Check power, board revision and modem baud.");
    return;
  }

  Serial.print("Modem: ");
  Serial.println(modem.getModemInfo());

  // SIM must be present and unlocked.
  if (modem.getSimStatus() != SIM_READY) {
    Serial.println("SIM is not ready. Check SIM and disable its PIN.");
    return;
  }

  Serial.println("Waiting for GSM network (up to 120 seconds)...");

  if (!modem.waitForNetwork(120000L)) {
    Serial.println("Network registration failed.");
    Serial.println("Check antenna, SIM, 2G coverage and power.");
    return;
  }

  Serial.print("Operator: ");
  Serial.println(modem.getOperator());

  Serial.print("Signal quality: ");
  Serial.println(modem.getSignalQuality());

  Serial.print("Sending SMS to ");
  Serial.println(RECIPIENT);

  bool sent = modem.sendSMS(RECIPIENT, MESSAGE);

  if (sent) {
    Serial.println("SMS submission successful. Check recipient phone.");
  } else {
    Serial.println("SMS submission failed. Check credit and network.");
  }
}

void loop() {
  // No automatic retries: avoids repeatedly sending SMS.
}