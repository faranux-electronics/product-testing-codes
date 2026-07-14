# SIM900 GSM/GPRS Shield Test/Utility Sketch

A comprehensive Arduino sketch for testing and utilizing the SIM900 GSM/GPRS Shield. This sketch provides SMS functionality, AT command pass-through, and diagnostic capabilities for the SIM900 module in a convenient Arduino shield form factor.

## Hardware Requirements

- Arduino UNO (or compatible board)
- SIM900 GSM/GPRS Shield (stackable Arduino shield)
- 5V-12V DC, 2A capable power supply with barrel jack connector
- SIM card with active service (unlocked)
- Antenna (SMA connector - usually included with shield)
- USB cable for Arduino programming

## Shield Features

- **Stackable Form Factor**: Directly mounts on Arduino UNO
- **Onboard Power Input**: Barrel jack for 5V-12V DC power
- **SIM Card Slot**: Located on the back of the shield
- **Power Control**: Master power select switch and power key button
- **Serial Selection**: Hardware/Software Serial selector switch
- **Status LEDs**: Power LED, STATUS LED, NETLIGHT LED
- **UART Pins**: Dedicated TX/RX pins for module communication

## Power-Up Sequence

1. **Insert SIM Card**: Ensure a valid, unlocked SIM card is inserted in the slot on the back of the shield
2. **Connect Power**: Connect 5V-12V DC, 2A power supply to the barrel jack
3. **Check Power LED**: Confirm the onboard Power LED lights up (indicates sufficient power)
4. **Power On Module**: Press and hold the Power Key for ~2 seconds to turn the module on
5. **Wait for Registration**: STATUS and NETLIGHT LEDs should start blinking
6. **Network Registration**: Wait up to ~30 seconds - once NETLIGHT blinks once every 3 seconds, the SIM has registered on the network and it's ready for AT commands

## Wiring Configuration

### Software Serial Setup (Recommended)
- **Serial selector switch**: Set to "Software Serial" position
- **Arduino D9** → Shield UART TX pin (Arduino TX out to module RX)
- **Arduino D10** → Shield UART RX pin (Arduino RX in from module TX)
- **Shield UART GND** → Arduino GND (common ground, required)

### Hardware Serial Setup (Alternative)
- **Serial selector switch**: Set to "Hardware Serial" position
- **Arduino D0 (RX)** → Shield UART TX
- **Arduino D1 (TX)** → Shield UART RX
- **Note**: This ties up the same pins used for USB programming - you must disconnect the module every time you upload new code

**Software Serial is recommended** to avoid the hassle of disconnection during code uploads.

## Setup Instructions

1. **Hardware Setup**
   - Insert valid, unlocked SIM card into the shield's SIM slot (back of shield)
   - Stack the SIM900 shield onto your Arduino UNO
   - Set the serial selector switch to "Software Serial"
   - Connect antenna to the SMA connector
   - Connect 5V-12V DC, 2A power supply to barrel jack
   - Press and hold Power Key for 2 seconds to power on the module
   - Wait for NETLIGHT to blink once every 3 seconds (network registered)

2. **Software Configuration**
   - Open `faranux_sim900_combined.ino` in Arduino IDE
   - Modify the default phone number in the code:
     ```cpp
     const char* DEFAULT_NUMBER = "+ZZxxxxxxxxx"; // Replace with your number
     ```
   - Optionally customize the default message:
     ```cpp
     const char* DEFAULT_MSG = "Faranux Electronics | faranux.com";
     ```

3. **Upload**
   - Select your Arduino board and port in Arduino IDE
   - Upload the sketch to your Arduino
   - Open Serial Monitor at 9600 baud

## Usage

### Serial Monitor Interface
1. Open Serial Monitor at 9600 baud
2. The sketch will initialize and run diagnostic checks:
   - Signal quality (CSQ)
   - SIM card presence (CCID)
   - Network registration status (CREG)
3. Once "Ready" appears, you can:

### Send Test SMS
- Type `SEND` and press Enter to send the default test SMS to the configured number

### Check Status
- Type `STATUS` and press Enter to re-check:
  - Signal quality
  - SIM card presence
  - Network registration status

### AT Command Pass-Through
- Type any AT command directly to send it to the module
- Examples:
  - `AT+CSQ` - Check signal quality
  - `AT+CMGL="ALL"` - List all SMS messages
  - `AT+CMGF=1` - Set text mode
  - `ATD+1234567890;` - Make a call
  - `AT+CMGR=1` - Read SMS from memory location 1

### Incoming SMS
- Incoming SMS messages are automatically forwarded to the Serial Monitor
- The sketch is configured to forward SMS immediately using `AT+CNMI=1,2,0,0,0`

## LED Behavior Indicators

### Power LED
- **ON**: Shield is receiving sufficient power
- **OFF**: No power or insufficient power supply

### STATUS LED
- **Blinking**: Module is powered on
- **OFF**: Module is powered off

### NETLIGHT LED
- **Fast/Erratic Blinking**: Powered but not yet registered on network
  - Module is searching for network
  - Check SIM card (missing, locked, or invalid)
  - Check antenna connection
  - Check signal strength in your area
- **Steady Blink (once per ~3 seconds)**: Registered on network and idle
  - Ready for AT commands and SMS operations
- **OFF**: Module is powered off

## Troubleshooting

### No Response to AT Commands
1. **Check Power Supply**
   - Ensure 5V-12V DC, 2A power supply is connected to barrel jack
   - Verify Power LED is lit
   - Try a different power supply if available

2. **Check Module Power-On**
   - Confirm Power Key was pressed and held for ~2 seconds
   - Verify STATUS LED is blinking
   - Try pressing Power Key again if module didn't power on

3. **Check Serial Selector Switch**
   - Ensure switch is set to "Software Serial" (not Hardware Serial)
   - If using Hardware Serial, ensure module is disconnected during upload

4. **Check Connections**
   - Verify shield is properly seated on Arduino
   - Check that Arduino and shield share common ground
   - Ensure TX/RX connections are correct

### Module Won't Register on Network
- **Check SIM Card**: Ensure SIM is inserted, unlocked, and has active service
- **Check Antenna**: Verify antenna is connected to SMA connector
- **Check Signal**: Try location with better cellular reception
- **Check Registration**: Type `STATUS` to check registration status
- **Wait Longer**: Network registration can take up to 30 seconds

### SMS Send Fails
- Verify phone number format includes country code
- Check network registration status with `STATUS`
- Ensure sufficient balance/service on SIM card
- Try sending to a different number
- Check signal strength (should be >10 for reliable operation)

### Power LED Not Lit
- Check power supply is connected to barrel jack
- Verify power supply is 5V-12V DC, 2A capable
- Try a different power supply
- Check barrel jack connector is properly seated

### STATUS LED Not Blinking
- Press and hold Power Key for 2 seconds to power on module
- Check if Power LED is lit first
- Try pressing Power Key again
- Verify SIM card is properly inserted

## Features

- **SMS Sending**: Send text messages to any phone number
- **AT Command Pass-Through**: Direct access to all SIM900 AT commands
- **Automatic Diagnostics**: Built-in checks for signal, SIM, and network status
- **Status Check Command**: Re-check diagnostics with `STATUS` command
- **Incoming SMS Forwarding**: Real-time display of received messages
- **LED Status Monitoring**: Visual indication of module and network status
- **Configurable Parameters**: Easy customization of phone number and message
- **Shield Form Factor**: Convenient stackable design for Arduino UNO

## Technical Specifications

- **Module**: SIM900 GSM/GPRS Module
- **Shield Form**: Stackable Arduino UNO shield
- **Baud Rate**: 9600 (default)
- **Communication**: SoftwareSerial on pins 9 (TX) and 10 (RX)
- **Power Input**: 5V-12V DC via barrel jack
- **Current Requirements**: Up to 2A during transmission
- **Operating Voltage**: 3.2V - 4.8V (module internal)
- **Frequency Bands**: Quad-band (850/900/1800/1900 MHz)
- **AT Commands**: Compatible with SIM800 command set for basic operations

## AT Command Compatibility

The SIM900's AT command set overlaps almost entirely with SIM800 for the commands used in this sketch:
- `AT` - Handshake
- `AT+CMGF=1` - Set text mode
- `AT+CNMI=1,2,0,0,0` - Configure SMS notification
- `AT+CSQ` - Signal quality
- `AT+CCID` - SIM card information
- `AT+CREG?` - Network registration
- `AT+CMGS` - Send SMS

## Safety Notes

- **ALWAYS** use external power supply via barrel jack - do not rely on Arduino power
- The module can draw up to 2A during transmission bursts
- Ensure antenna is connected before powering on for best performance
- Use appropriate power supply rated for 2A current
- Handle SIM card carefully to avoid damage
- Ensure SIM card is unlocked and compatible with your network

## Documentation

Additional AT command reference is available in the included PDF:
- `SIM800-Series_AT-Command-Manual_V1.09.pdf` (SIM900 compatible)

## License

This sketch is provided by Faranux Electronics for testing and utility purposes.

## Support

For more information, visit: [faranux.com](https://faranux.com)