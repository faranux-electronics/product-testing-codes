# SIM800L GSM Module Test/Utility Sketch

A comprehensive Arduino sketch for testing and utilizing the SIM800L EVB V2.2 GSM module. This sketch provides SMS functionality, AT command pass-through, and diagnostic capabilities for the SIM800L module.

## Hardware Requirements

- Arduino UNO (or compatible board)
- SIM800L EVB V2.2 GSM Module
- External 5V, 2A-capable power supply
- 1000uF 16V low-ESR electrolytic capacitor
- IPEX/U.FL pigtail cable with SMA stub antenna
- SIM card with active service
- Jumper wires for connections

## Wiring Diagram

### Power Connections (CRITICAL)
- **5V IN (VBAT)** → External 5V, 2A power supply
  - **IMPORTANT**: Do NOT use Arduino 5V pin - the module needs up to ~2A peak current during transmit bursts
  - Add 1000uF 16V low-ESR electrolytic capacitor across VBAT/GND, close to the module
- **GND** → Common ground: connect external supply GND AND Arduino GND together
- **VDD** → Arduino 5V (logic-level reference pin, not power input)

### Serial Communication
- **TXD (module transmit)** → Arduino pin 2 (RX, SoftwareSerial)
- **RXD (module receive)** → Arduino pin 3 (TX, SoftwareSerial)
  - Note: Crossed connection - module TXD to Arduino RX, module RXD to Arduino TX

### Antenna
- **Antenna connector** → IPEX/U.FL pigtail cable to SMA stub antenna
  - **ALWAYS attach antenna before powering on**
  - Transmitting without antenna can damage the RF power amplifier
  - Module will never register on network without antenna (LED will fast-blink indefinitely)

## Setup Instructions

1. **Hardware Setup**
   - Connect all components according to the wiring diagram
   - Ensure external power supply is connected before powering Arduino
   - Verify SIM card is properly inserted in the SIM800L module
   - Attach antenna before applying power

2. **Software Configuration**
   - Open `faranux_sim800_combined.ino` in Arduino IDE
   - Modify the default phone number in the code:
     ```cpp
     const char* DEFAULT_NUMBER = "+ZZxxxxxxxxxx"; // Replace with your number
     ```
   - Optionally customize the default message:
     ```cpp
     const char* DEFAULT_MSG = "Faranux Electronics | faranux.com";
     ```

3. **Upload**
   - Select your Arduino board and port in Arduino IDE
   - Upload the sketch to your Arduino

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

### AT Command Pass-Through
- Type any AT command directly to send it to the module
- Examples:
  - `AT+CSQ` - Check signal quality
  - `AT+CMGL="ALL"` - List all SMS messages
  - `AT+CMGF=1` - Set text mode
  - `ATD+1234567890;` - Make a call

### Incoming SMS
- Incoming SMS messages are automatically forwarded to the Serial Monitor
- The sketch is configured to forward SMS immediately using `AT+CNMI=1,2,0,0,0`

## LED Behavior Indicators

- **LED OFF**: Module not powered
- **LED Fast Blink (~1/sec)**: Powered but not registered on network
  - Check antenna connection
  - Verify SIM card is present and active
  - Check signal strength
- **LED Slow Blink (~1 per 3 sec)**: Registered on network and idle

## Troubleshooting

### No Response to AT Commands
1. **Check Power Supply**
   - Ensure external 5V, 2A power supply is connected
   - Verify common ground between external supply and Arduino
   - Check capacitor is properly installed across VBAT/GND

2. **Check Antenna**
   - Ensure antenna is connected before power-on
   - Verify antenna has good signal reception
   - Try different location for better signal

3. **Check SIM Card**
   - Verify SIM card is properly inserted
   - Ensure SIM card has active service
   - Try SIM card in a phone to verify it works

4. **Check Connections**
   - Verify TX/RX connections are crossed correctly
   - Check all ground connections are common
   - Ensure VDD is connected to Arduino 5V

### Module Won't Register on Network
- Check antenna connection and signal strength
- Verify SIM card is active and has service
- Try `AT+CREG?` to check registration status
- Check signal quality with `AT+CSQ` (should be >10 for reliable operation)

### SMS Send Fails
- Verify phone number format includes country code
- Check network registration status
- Ensure sufficient balance/service on SIM card
- Try sending to a different number

## Features

- **SMS Sending**: Send text messages to any phone number
- **AT Command Pass-Through**: Direct access to all SIM800L AT commands
- **Automatic Diagnostics**: Built-in checks for signal, SIM, and network status
- **Incoming SMS Forwarding**: Real-time display of received messages
- **LED Status Monitoring**: Visual indication of module status
- **Configurable Parameters**: Easy customization of phone number and message

## Technical Specifications

- **Module**: SIM800L EVB V2.2
- **Baud Rate**: 9600 (default)
- **Communication**: SoftwareSerial on pins 2 (RX) and 3 (TX)
- **Power Requirements**: 5V, 2A peak current
- **Operating Voltage**: 3.4V - 4.4V (module internal)
- **Frequency Bands**: Quad-band (850/900/1800/1900 MHz)

## Safety Notes

- **ALWAYS** connect antenna before powering on the module
- **NEVER** power the module from Arduino 5V pin - use external supply
- Ensure proper grounding to prevent communication issues
- The module can draw up to 2A during transmission bursts
- Use appropriate wire gauge for power connections

## License

This sketch is provided by Faranux Electronics for testing and utility purposes.

## Support

For more information, visit: [faranux.com](https://faranux.com)