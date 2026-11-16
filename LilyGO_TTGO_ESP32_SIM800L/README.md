# LilyGO TTGO T-Call ESP32 + SIM800L — Arduino IDE Test

Test the ESP32-to-modem connection, SIM detection, signal strength, and GSM network registration. No separate Arduino Uno or Nano is required: upload directly to the T-Call using Arduino IDE.

## Requirements

- LilyGO TTGO T-Call ESP32 + SIM800L (this guide targets the IP5306 power-management version).
- USB-C data cable and a computer with Arduino IDE.
- GSM antenna connected before powering the board.
- Active SIM card and local 2G GSM coverage for network tests.
- Reliable USB power and a good cable. Weak power can cause modem failures or board resets during transmission.

The SIM800L is a 2G GSM/GPRS modem, not a 4G modem. Confirm 2G service with your operator. A SIM is not required for the basic `AT` communication test.

## 1. Prepare the hardware

1. Disconnect power.
2. Attach the GSM antenna and insert the SIM card.
3. Connect the board to the computer using a USB-C data cable.

Use USB-C for this initial test. Do not apply 5 V to GPIO or 3V3 pins. ESP32 GPIO uses 3.3 V logic. The internal modem connections require no jumper wires.

## 2. Configure Arduino IDE

1. Open Boards Manager and install **esp32 by Espressif Systems**.
2. Select **Tools → Board → ESP32 Arduino → ESP32 Dev Module**.
3. Select the board's USB serial port.
4. Set upload speed to **115200**.

If the ESP32 package is not listed, add this URL under Preferences → Additional Boards Manager URLs:

https://espressif.github.io/arduino-esp32/package_esp32_index.json

If no serial port appears, check the cable and install the driver matching the board's USB-to-serial chip (CP210x or CH9102, depending on revision).

## 3. Upload the modem test sketch

This sketch uses only the libraries bundled with the ESP32 Arduino package. TinyGSM is not needed.

### Internal pin mapping

| Function, viewed from the ESP32 | GPIO |
| --- | --- |
| UART RX, receives modem TX | 26 |
| UART TX, drives modem RX | 27 |
| Modem power enable | 23 |
| Modem reset | 5 |
| Modem power key | 4 |
| IP5306 I2C SDA | 21 |
| IP5306 I2C SCL | 22 |

Check your board revision before using this sketch. The IP5306 initialization is specific to that power-management chip; use the manufacturer's matching example for an AXP192 version.

```cpp
#include <Arduino.h>
#include <Wire.h>

HardwareSerial modem(1);

constexpr int MODEM_RX       = 26;
constexpr int MODEM_TX       = 27;
constexpr int MODEM_POWER_ON = 23;
constexpr int MODEM_RST      = 5;
constexpr int MODEM_PWRKEY   = 4;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Keep the IP5306 power supply enabled.
  Wire.begin(21, 22);
  Wire.beginTransmission(0x75);
  Wire.write(0x00);
  Wire.write(0x37);
  if (Wire.endTransmission() != 0) {
    Serial.println("IP5306 not detected: check board revision.");
  }

  pinMode(MODEM_POWER_ON, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_PWRKEY, OUTPUT);

  digitalWrite(MODEM_POWER_ON, HIGH);
  digitalWrite(MODEM_RST, HIGH);

  // Pulse the modem power key.
  digitalWrite(MODEM_PWRKEY, HIGH);
  delay(100);
  digitalWrite(MODEM_PWRKEY, LOW);
  delay(1000);
  digitalWrite(MODEM_PWRKEY, HIGH);

  modem.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(5000);

  Serial.println("ESP32 running.");
  Serial.println("Send AT commands using Both NL & CR.");
  modem.println("AT");
}

void loop() {
  while (modem.available()) {
    Serial.write(modem.read());
  }
  while (Serial.available()) {
    modem.write(Serial.read());
  }
}
```

The sketch forwards commands between Serial Monitor and the modem. It does not send SMS, make calls, or open a mobile data connection automatically.

## 4. Run the tests

Open Serial Monitor, select **115200 baud**, and set line ending to **Both NL & CR**. Send commands one at a time.

| Command | Expected result / meaning |
| --- | --- |
| `AT` | `OK`: ESP32 and modem communicate. |
| `ATI` | Modem identification. |
| `AT+CPIN?` | `+CPIN: READY`: SIM is detected and unlocked. |
| `AT+CSQ` | `+CSQ: <rssi>,<ber>`: signal report. RSSI 0–31 is the reported level; 99 means unknown or undetectable. |
| `AT+CREG?` | `+CREG: <n>,<stat>`: the second value is registration status. |
| `AT+COPS?` | Selected network operator when available. |

### Registration status

| `<stat>` | Meaning |
| --- | --- |
| 0 | Not registered; not currently searching. |
| 1 | Registered on the home network. |
| 2 | Searching for a network. |
| 3 | Registration denied. |
| 4 | Unknown status. |
| 5 | Registered while roaming. |

Allow a minute or two for registration. For example, `+CREG: 0,1` indicates home-network registration, while `+CREG: 0,5` indicates roaming registration.

If `AT+CPIN?` reports `SIM PIN`, unlock it using the correct PIN or disable the SIM PIN using a phone. Do not guess the PIN repeatedly.

## 5. Troubleshooting

| Symptom | What to check |
| --- | --- |
| Upload remains at “Connecting…” | Hold BOOT while starting upload, then release when writing begins. Check the selected port and cable. |
| No USB serial port | Try a known data cable and check the matching USB-to-serial driver. |
| Garbled Serial Monitor text | Set Serial Monitor to 115200 baud. |
| ESP32 message appears, but no modem reply | Check board revision and modem power. Try changing only the modem baud in `modem.begin(...)` to 9600; keep Serial Monitor at 115200. Send `AT` several times. |
| First automatic `AT` has no reply | Wait several seconds and send `AT` manually. |
| IP5306 not detected | Confirm the power-management chip/revision and use the matching manufacturer example. |
| `AT` works but SIM is not ready | Power off, check SIM orientation/contact, and check whether the SIM requires a PIN. |
| SIM is ready but registration fails | Check antenna, local 2G coverage, operator support, SIM activation, and power stability. |
| Repeated resets during network search | Improve the USB cable and power source; test with the manufacturer's recommended power arrangement. |

Pulsing PWRKEY can toggle modem state. If repeated uploads leave the modem unresponsive, disconnect power fully, reconnect, and retry.

## Success criteria

- **ESP32 upload test:** the sketch uploads and prints `ESP32 running.`
- **Modem communication test:** `AT` returns `OK`.
- **SIM test:** `AT+CPIN?` reports `READY`.
- **Network test:** the registration status is 1 or 5.

These tests do not verify SMS delivery, calls, mobile data, Wi-Fi, Bluetooth, or every GPIO. No APN is needed for these basic tests; an APN is needed when configuring a GPRS data connection.

## References

- [Faranux product page](https://www.faranux.com/product/lilygo-ttgo-t-call-esp32-with-sim800l-com44/)
- [LilyGO official examples and board documentation](https://github.com/Xinyuan-LilyGO/LilyGo-T-Call-SIM800)
- [LilyGO IP5306 version documentation](https://github.com/Xinyuan-LilyGO/LilyGo-T-Call-SIM800/blob/master/doc/SIM800L_IP5306.MD)
- [LilyGO Arduino pin definitions](https://github.com/Xinyuan-LilyGO/LilyGo-T-Call-SIM800/blob/master/examples/Arduino_TinyGSM/utilities.h)

This is a starter procedure. The sketch has not been compiled or tested on physical hardware in this session.
