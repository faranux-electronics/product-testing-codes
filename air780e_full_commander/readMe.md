# Air780E Full Commander

## Overview
This Arduino sketch provides a comprehensive command interface for the Air780E 4G Cat.1 module using an Arduino Uno. It allows users to interact with the module through a serial menu, sending AT commands for various functionalities including basic module control, network operations, SMS handling, internet connectivity, HTTP requests, TCP connections, and power management.

## Features
- Menu-driven interface for easy command selection
- Support for basic AT commands (ping, info, IMEI, firmware)
- Network status checking (registration, signal strength, operator)
- SIM card operations (IMSI, ICCID, PIN status)
- SMS sending, receiving, listing, and deletion
- Internet connectivity (APN setup, data activation)
- HTTP GET and POST requests
- TCP socket connections
- Power management (shutdown, sleep mode, flight mode)
- Full diagnostics mode
- Support for raw AT command input

## Hardware Requirements
- Arduino Uno (or compatible board)
- Air780E Mini 4G Dev Board
- SIM card with data plan
- Power supply (Arduino 5V or external 4.2V 2A for Air780E)
- Jumper wires
- Voltage divider components (1kΩ and 2kΩ resistors)

## Wiring Diagram

### Serial Communication
- Air780E TXD → Arduino Pin 10 (RX)
- Air780E RXD → Arduino Pin 11 (TX) via voltage divider
- Air780E GND → Arduino GND

### Power Management
- Air780E PWK → Arduino Pin 7
- Air780E VIN → Arduino 5V (or Air780E VBAT → External 4.2V 2A)

### Voltage Divider for RXD
```
Arduino Pin 11 → 1kΩ → Junction → Air780E RXD
Junction → 2kΩ → GND
```

## Software Setup
1. Install Arduino IDE
2. Install SoftwareSerial library (usually included with Arduino IDE)
3. Open the `air780e_full_commander.ino` file
4. Configure the following variables in the sketch:
   - `phoneNumber`: Your target phone number for SMS
   - `smsMessage`: SMS message content
   - `apnName`: Your carrier's APN (default: "internet")
   - `httpGetUrl`: URL for HTTP GET test
   - `httpPostUrl`: URL for HTTP POST test
   - `tcpHost`: TCP server hostname
   - `tcpPort`: TCP server port
5. Upload the sketch to Arduino Uno

## Usage
1. Open Arduino Serial Monitor at 115200 baud
2. Set line ending to "Both NL & CR"
3. The module will boot up automatically
4. Type a command number and press Enter, or type "menu" to see available commands
5. You can also send raw AT commands directly

## Command Menu

### Basic Commands
- 1 = AT (ping the module)
- 2 = ATI (module information)
- 3 = AT+CGSN (get IMEI)
- 4 = AT+CGMR (firmware version)
- 5 = ATZ (factory reset)

### Network Commands
- 10 = AT+CREG? (GSM registration status)
- 11 = AT+CSQ (signal strength)
- 12 = AT+COPS? (operator name)
- 13 = AT+CGATT? (GPRS attach status)
- 14 = AT+CEREG? (LTE registration status)
- 15 = AT+NITZ? (network time)

### SIM Commands
- 20 = AT+CIMI (IMSI number)
- 21 = AT+CCID (SIM ICCID)
- 22 = AT+CPIN? (SIM PIN status)

### SMS Commands
- 40 = Set SMS text mode
- 41 = Send SMS to configured phone number
- 42 = List all received SMS
- 43 = Read SMS at index 1
- 44 = Delete SMS at index 1
- 45 = Delete ALL SMS

### Internet Commands
- 50 = Set APN
- 51 = Activate data connection
- 52 = Get assigned IP address
- 53 = Deactivate data connection

### HTTP Commands
- 60 = HTTP GET request to httpbin.org
- 61 = HTTP POST request to httpbin.org

### TCP Commands
- 70 = Open TCP connection to httpbin.org
- 71 = Send data over TCP
- 72 = Close TCP connection

### Power Commands
- 80 = Power off module
- 81 = Enable sleep mode
- 82 = Disable sleep mode
- 83 = Flight mode ON (RF off)
- 84 = Flight mode OFF (RF on)

### Tools
- 99 = Run full diagnostics
- menu = Show this menu

## Important Notes
- Ensure proper voltage levels: Arduino Uno operates at 5V, while Air780E uses 3.3V logic
- Use a voltage divider on the Arduino TX line to prevent damage to the Air780E RX pin
- The module requires a valid SIM card with appropriate data plan for internet functionality
- Power consumption can be high during data transmission; ensure adequate power supply
- Some commands may require network registration or data activation to work properly

## Troubleshooting
- If the module doesn't respond, check wiring and power connections
- Verify SIM card is inserted correctly and has credit/data
- Check signal strength with command 11
- Use diagnostics command 99 for comprehensive status check
- Ensure correct APN settings for your carrier

## Resources
- Manufacturer: OpenLuat (https://www.openluat.com)
- Documentation: https://doc.openluat.com
- LuatOS Wiki: https://wiki.luatos.org
- Air780E Module: https://wiki.luatos.org/chips/air780e/index.html
- Hardware Manual: https://c.vue2.cn/attachment/20240819170438346_Air780E_Hardware_Design_Manual_V1.2.4.pdf
- AT Command Manual: https://2150692.ru/files/air72x_at_command_luat_4g_modules_v3.89.pdf

## License
This project is provided as-is for educational and development purposes.

## Author
Faranux Team - 2026-05-08</content>
