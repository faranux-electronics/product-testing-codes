# SIM800C GSM Module Test/Utility Sketch

**Made by Faranux Electronics**  
**Visit us at: [faranux.com](https://faranux.com)**

## Overview

This sketch provides a comprehensive test and utility interface for the SIM800C GSM module. It allows you to send SMS messages, execute AT commands directly, and monitor module status through the Arduino Serial Monitor.

## Features

- Send SMS text messages to configurable phone numbers
- Direct AT command execution via Serial Monitor
- Real-time status monitoring (signal quality, SIM presence, network registration)
- Incoming SMS forwarding to Serial Monitor
- Interactive command interface

## Hardware Requirements

- Arduino UNO (or compatible board)
- SIM800C GSM module
- Stable 4.2V-5V power supply capable of 2A peak current
- SIM card with active service
- Optional: Logic level shifter (if module RX is not 3.3V tolerant)

## Wiring Diagram

### SIM800C ↔ Arduino UNO Connections

| SIM800C Pin | Arduino Pin | Notes |
|-------------|-------------|-------|
| VBAT | 4.2V-5V supply | Dedicated power supply (NOT Arduino 5V pin) |
| PWR | GND | Ground at boot to power on (some boards auto-power-on) |
| GND | GND | Common ground |
| TX | Pin 2 (RX) | SoftwareSerial receive |
| RX | Pin 3 (TX) | SoftwareSerial transmit (use level shifter if needed) |

### Important Power Notes

- **DO NOT** power the SIM800C from the Arduino's 5V pin
- The module requires up to 2A peak current during transmission
- Use a dedicated stable power supply capable of delivering this current
- Unstable power is the most common cause of AT command failures

### Logic Level Considerations

- SIM800C RX operates at 3.3V logic levels
- If your module's RX pin is not level-shifted, use a voltage divider or level shifter between Arduino TX (pin 3) and module RX
- Most SIM800C breakout boards include level shifting circuitry

## Configuration

Before uploading, modify these constants in the sketch:

```cpp
const char* DEFAULT_NUMBER = "+ZZxxxxxxxxxx"; // Your phone number with country code
const char* DEFAULT_MSG    = "Faranux Electronics | faranux.com"; // Default test message
```

Replace `+ZZxxxxxxxxxx` with your actual phone number including country code (e.g., `+1234567890`).

## Usage

1. **Upload the sketch** to your Arduino UNO
2. **Open Serial Monitor** at 9600 baud
3. **Wait for initialization** - the sketch will run diagnostic commands:
   - `AT` - Handshake
   - `AT+CMGF=1` - Set text mode
   - `AT+CNMI=1,2,0,0,0` - Enable SMS forwarding
   - `AT+CSQ` - Check signal quality
   - `AT+CCID` - Verify SIM card presence
   - `AT+CREG?` - Check network registration

4. **Send a test SMS**:
   - Type `SEND` in the Serial Monitor and press Enter
   - This will send the default message to the configured number

5. **Execute AT commands**:
   - Type any AT command directly in the Serial Monitor
   - Examples: `AT`, `AT+CSQ`, `AT+CMGL="ALL"`
   - Responses will be displayed in the Serial Monitor

6. **Monitor incoming SMS**:
   - Incoming SMS messages will be automatically forwarded to the Serial Monitor

## AT Command Reference

Common useful AT commands to try:

- `AT` - Basic handshake
- `AT+CSQ` - Signal quality (0-31, higher is better)
- `AT+CCID` - SIM card number
- `AT+CREG?` - Network registration status
- `AT+CMGF=1` - Set SMS text mode
- `AT+CMGL="ALL"` - List all stored SMS
- `AT+CMGD=1,4` - Delete all SMS
- `AT+COPS?` - Check current network operator

## Troubleshooting

### No response to AT commands

1. **Check power supply** - Most common issue
   - Ensure VBAT is stable and can deliver 2A peak
   - Try a different power supply if available

2. **Verify wiring connections**
   - TX/RX connections must be crossed (module TX → Arduino RX)
   - Check ground connection is common

3. **Check baud rate**
   - Default is 9600 baud in this sketch
   - Some modules may use different rates (115200, etc.)

4. **Logic level issues**
   - If using a bare module without level shifting, add a voltage divider

### SMS sending fails

1. **Verify network registration** with `AT+CREG?`
2. **Check signal quality** with `AT+CSQ` (should be >10 for reliable operation)
3. **Confirm SIM card** is active and has credit/service
4. **Verify phone number** format includes country code

### Module won't power on

1. **Check PWR pin** - Ground it at boot if your board requires it
2. **Verify VBAT voltage** - Should be 4.2V-5V
3. **Some boards auto-power-on** - Try without grounding PWR

## Code Structure

- `setup()` - Initializes serial interfaces and runs diagnostic commands
- `loop()` - Handles bidirectional communication between Serial Monitor and SIM800C
- `sendAT()` - Sends AT commands and displays responses
- `sendSMS()` - Sends SMS messages using AT+CMGS command

## Safety Notes

- **Always double-check wiring** before applying power
- **Never connect VBAT to Arduino 5V pin** - use external supply
- **Allow adequate current capacity** in your power supply
- **SIM800C can get warm** during normal operation, especially during transmission

## License

This is a demonstration/test sketch. Review code and wiring carefully before powering hardware. Consult the repository owner for reuse permissions.

## Contact

- Repository: faranux-electronics (GitHub)
- Website: [faranux.com](https://faranux.com)
