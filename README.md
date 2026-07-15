# Faranux Electronics — Product Testing Codes

This repository is a collection of hardware test sketches, example projects, and reference schematics for a variety of sensors and modules used by Faranux Electronics. The projects are primarily Arduino sketches (.ino) intended for quick testing and demonstration.

Repository layout (top-level folders):

- `A7670E/`: A7670E GSM/GNSS module test sketches and baud rate changer.
- `Air780E/`: Air780E LTE module full commander and reference materials.
- `CameraWebServer/`: ESP32 camera web server with streaming capabilities.
- `DFMiniMp3/`: Example sketches for the DFMiniMp3 audio module (multiple libraries/examples).
- `dht11/`: DHT11 temperature/humidity sensor example and schematics.
- `433MHz FS1000A - XY-MK-5V Kit/`: 433MHz RF wireless communication kit with transmitter and receiver sketches, wiring diagrams, and troubleshooting guide.
- `laser-sensor/`: Laser receiver/detection module example.
- `ldr/`: LDR (light-dependent resistor) test sketch.
- `max9814/`: Microphone preamp (MAX9814) example and schematics.
- `oled/`: OLED display examples for different sizes and boards (SSD1306, SH110X, U8g2).
- `read-switch/`: Switch reading example and schematic.
- `SIM800C/`: SIM800C GSM module test sketches and documentation.
- `SIM800L/`: SIM800L GSM module test sketch with comprehensive wiring diagrams and troubleshooting guide.
- `SIM900/`: SIM900 GSM/GPRS shield test sketch with power-up sequence and configuration options.
- `Soil-Moisture-Temperature-EC-Sensor/`: Soil sensor test sketch.
- `tft-touch-screen/`: TFT touchscreen examples, touch calibration, and media resources.

Usage
- Open the specific folder for the module you want to test.
- Load the `.ino` sketch in the Arduino IDE (or import into PlatformIO), select the appropriate board and port, then compile and upload.
- Some folders include a `schematics/` or `media/` subfolder with wiring diagrams and reference images.

Contributing
- Contributions and improvements are welcome via pull requests. Keep examples focused and hardware-specific.

Notes
- These are demonstration/test sketches; review code and wiring carefully before powering hardware.
- No global license is specified in this repository; consult the owner for reuse permissions.

Contact
- Repository owner: faranux-electronics (GitHub)

---
Generated: July 15, 2026
