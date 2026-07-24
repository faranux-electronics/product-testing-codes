# A7670E Product Testing Codes

**Made by Faranux Electronics**  
**Visit us at: [faranux.com](https://faranux.com)**

This repository contains Arduino sketches for testing and configuring the A7670E LTE Cat-1 module with an Arduino Uno.

## Sketches

### A7670E_BaudChanger

**Purpose:** One-time baud rate changer for the A7670E module.

**Description:** This sketch changes the module's baud rate from the factory default of 115200 to 9600. SoftwareSerial on Arduino Uno is more stable at 9600 baud, making it suitable for reliable communication.

**Usage:**
1. Upload this sketch to your Arduino Uno.
2. Open the Serial Monitor at 115200 baud.
3. The sketch will send `AT+IPR=9600` to the module.
4. After seeing "Done!" in the Serial Monitor, power-cycle the module.
5. Now you can upload the main testing sketch.

**Note:** Run this sketch only once per module. The new baud rate is saved to flash memory.

### A7670E_MasterTool_9600

**Purpose:** Comprehensive testing tool for A7670E module functionality.

**Description:** This is a master testing tool that provides a menu-driven interface to test various features of the A7670E LTE module, including network diagnostics, SMS sending, voice calls, and HTTP requests.

**Features:**
- Network and signal strength checking
- Send test SMS messages
- Make and receive voice calls
- HTTP GET and POST requests
- SIM status verification

**How the Sketch Works:**
The sketch initializes communication with the A7670E module at 9600 baud using SoftwareSerial. It provides a text-based menu in the Arduino Serial Monitor where users can select options by typing letters. Each option sends specific AT commands to the module to perform network checks, send SMS, make calls, or perform HTTP requests. Responses from the module are displayed in the Serial Monitor for debugging and verification.

**Prerequisites:**
- Run `A7670E_BaudChanger.ino` first to set the module to 9600 baud.
- Ensure the module is powered and connected correctly.

**Hardware Wiring (7-Pin Breakout Header):**
- `5-12V`: 6V external power supply positive
- `GND`: shared ground for Arduino and the power supply
- `TX`: module TX → Arduino Pin 10 (SoftwareSerial RX)
- `RX`: module RX → Arduino Pin 11 (SoftwareSerial TX)
- `3.8EN`: leave unconnected if the board auto-boots
- `NET`: not used for this sketch
- `DTR`: not used for this sketch

**Connector order on the board header (top to bottom):** `DTR`, `NET`, `3.8EN`, `RX`, `TX`, `GND`, `5-12V`.

**Power Requirements:** The board requires at least 6V with 2A current for proper operation.

**LED Status Indicators:**
- Slow Blinking (~5 sec): Low power / Not registered
- Fast Blinking (~100ms): Successfully registered
- Off: Module is powered down or stuck in a boot-loop.

**Configuration:**
- Edit `TARGET_PHONE` with your test phone number
- Edit `APN` with your carrier's APN (default: "internet" for MTN/Airtel Rwanda)

**Usage:**
1. Upload the sketch to Arduino Uno.
2. Open Serial Monitor at 9600 baud.
3. Follow the menu prompts to test different features.

**Menu Options:**
- [N] - Check Network & Signal
- [S] - Send Test SMS
- [C] - Call Phone Number
- [H] - Hang Up Call
- [A] - Answer Incoming Call
- [G] - HTTP GET Request
- [P] - HTTP POST Request
- [M] - Reprint Menu

## Requirements

- Arduino Uno
- A7670E LTE module with 7-pin breakout board
- External 6V power supply with 2A current
- SIM card with data plan
- Arduino IDE

## Datasheet and References

- [A7670E Datasheet](https://docs.ai-thinker.com/en/a7670e)
- [A7670E AT Commands Manual](https://docs.ai-thinker.com/en/a7670e/specification/at)

## Notes

- SoftwareSerial is used for communication between Arduino and the module.
- The module operates at 9600 baud for stability.
- Ensure proper power supply; the module requires more current than Arduino can provide.</content>
<parameter name="filePath">d:\dev\arduino\product-testing-codes\A7680E\README.MD