# 433MHz FS1000A - XY-MK-5V RF Kit

Arduino project for wireless communication using 433MHz ASK/OOK RF modules. This kit includes transmitter and receiver code for testing and prototyping wireless data communication between Arduino boards.

## Hardware Components

- **FS1000A Transmitter Module**: 433MHz ASK/OOK transmitter (4-pin: ANT, GND, DATA, VCC)
- **XY-MK-5V Receiver Module**: 433MHz ASK/OOK receiver (3-pin: VCC, DATA, GND)
- **Arduino Board(s)**: Arduino UNO or compatible (one for transmitter, one for receiver)
- **Antenna**: ~17.3cm wire for improved range and reliability

## Library Dependencies

This project uses the **RadioHead RH_ASK library** for RF communication.

### Installation

1. Open Arduino IDE
2. Go to Sketch → Include Library → Manage Libraries
3. Search for "RadioHead"
4. Install the RadioHead library by jecrespo
5. Alternatively, download from: https://github.com/jecrespo/RadioHead

## Project Structure

```
433MHz FS1000A - XY-MK-5V Kit/
├── faranux_433mhz_transmitter/
│   └── faranux_433mhz_transmitter.ino    # Transmitter sketch
├── faranux_433mhz_receiver/
│   └── faranux_433mhz_receiver.ino       # Receiver sketch
├── schematics/
│   ├── 433mhz-rf-wireless-transmitter-module.jpg
│   └── 433mhz-rf-wireless-receiver-module.jpg
└── ReadMe.md
```

## Wiring Instructions

### Transmitter Setup (FS1000A Module)

Connect the FS1000A transmitter to Arduino UNO as follows:

| Arduino Pin | Module Pin | Wire Color |
|-------------|------------|------------|
| 5V          | VCC        | Red        |
| GND         | GND        | Black      |
| Pin 12      | DATA       | Green      |
| ANT pad     | Antenna    | ~17.3cm wire |

**Note**: The RH_ASK library uses pin 12 as the default TX data pin. If you need to use a different pin, modify the constructor call in the code.

### Receiver Setup (XY-MK-5V Module)

Connect the XY-MK-5V receiver to Arduino UNO as follows:

| Arduino Pin | Module Pin | Wire Color |
|-------------|------------|------------|
| 5V          | VCC        | Red        |
| GND         | GND        | Black      |
| Pin 11      | DATA       | Green      |
| ANT pad     | Antenna    | ~17.3cm wire |

**Note**: The RH_ASK library uses pin 11 as the default RX data pin. If you need to use a different pin, modify the constructor call in the code.

## Usage

### 1. Upload Transmitter Code

1. Open `faranux_433mhz_transmitter/faranux_433mhz_transmitter.ino`
2. Connect the transmitter Arduino to your computer
3. Select the correct board and port in Arduino IDE
4. Upload the sketch

The transmitter will continuously send "hello" messages every 200ms.

### 2. Upload Receiver Code

1. Open `faranux_433mhz_receiver/faranux_433mhz_receiver.ino`
2. Connect the receiver Arduino to your computer
3. Select the correct board and port in Arduino IDE
4. Upload the sketch

### 3. Monitor Communication

1. Open the Serial Monitor on the receiver Arduino (Tools → Serial Monitor)
2. Set baud rate to 9600
3. You should see "Received Message: hello" appearing repeatedly

**Important**: The transmitter and receiver communicate wirelessly via RF. They do not need to be connected to each other by wires. Each Arduino should be powered independently (via USB or external power).

## Code Customization

### Changing the Message

In the transmitter sketch (`faranux_433mhz_transmitter.ino`), modify line 39:

```cpp
const char *msg = "hello";  // Change this to your desired message
```

### Changing Transmission Speed

Modify the RH_ASK constructor to change the data rate. Default is 2000 bps:

```cpp
RH_ASK driver(2000);  // Speed in bits per second
```

### Using Different Pins

For ESP8266/ESP32 or custom pin configurations, use the constructor with pin parameters:

```cpp
RH_ASK driver(2000, 2, 4, 5);  // (speed, RX pin, TX pin, PTT pin)
```

## Troubleshooting

### No messages received

- **Check power**: Ensure both modules are receiving 5V power
- **Check antenna**: Make sure antennas are properly connected (~17.3cm wire)
- **Check range**: Start with modules within 1-2 meters of each other
- **Check wiring**: Verify all connections match the wiring diagram
- **Check Serial Monitor**: Ensure baud rate is set to 9600
- **Check library**: Verify RadioHead library is properly installed

### Intermittent reception

- **Add antenna**: A proper antenna significantly improves reliability
- **Reduce interference**: Keep away from WiFi routers, microwaves, and other 433MHz devices
- **Check power supply**: Use stable power supply, avoid sharing power with motors/servos
- **Increase distance**: Sometimes being too close can cause overload

### "init failed" message

- **Check library installation**: Ensure RadioHead library is correctly installed
- **Check SPI library**: The SPI library is required (included in Arduino IDE)
- **Check pin conflicts**: Ensure no other libraries are using the same pins

## Technical Specifications

### Transmitter (FS1000A)
- **Frequency**: 433.92 MHz
- **Modulation**: ASK/OOK (Amplitude Shift Keying/On-Off Keying)
- **Voltage**: 3.5V - 12V DC
- **Current**: ~10mA (transmitting)
- **Data Rate**: Up to 10 kbps (typically 2-4 kbps for reliability)

### Receiver (XY-MK-5V)
- **Frequency**: 433.92 MHz
- **Modulation**: ASK/OOK
- **Voltage**: 5V DC
- **Current**: ~5mA
- **Sensitivity**: -110 dBm
- **Data Rate**: Up to 10 kbps

## License

This project is provided as-is for educational and testing purposes.

## Credits

- **Library**: RadioHead RH_ASK by jecrespo (https://github.com/jecrespo/RadioHead)
- **Hardware**: FS1000A Transmitter and XY-MK-5V Receiver modules

## Version History

- **v1.0** - Initial release with basic transmitter/receiver functionality