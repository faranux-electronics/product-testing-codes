# ESP32 Camera Web Server

A feature-rich web server implementation for ESP32 camera modules with support for streaming, face detection, and face recognition.

## Hardware Requirements

### Supported Camera Modules

- **AI Thinker (Recommended)** - Has PSRAM ✓
- WROVER KIT - Has PSRAM
- ESP-EYE - Has PSRAM
- ESP32S3 EYE - Has PSRAM
- M5Stack PSRAM variants - Have PSRAM
- M5Stack ESP32CAM - No PSRAM
- XIAO ESP32S3 - Has PSRAM
- And more...

## Configuration

### Step 1: Select Camera Model

In `CameraWebServer.ino`, uncomment the appropriate camera model:

```cpp
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
```
```
### Step 2: Configure Pixel Format
Choose between streaming and face detection:
``` cpp
// For streaming (JPEG)
// config.pixel_format = PIXFORMAT_JPEG;

// For face detection/recognition (RGB565) - Recommended
config.pixel_format = PIXFORMAT_RGB565;
```
### Step 3: WiFi Credentials
Update the WiFi credentials in : `CameraWebServer.ino`
``` cpp
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
```
## Upload Instructions
### Important: GPIO 0 Connection Required
1. **Before uploading:**
    - Connect **GPIO 0 to GND** using a jumper wire
    - This puts the device into bootloader mode

2. **Upload the code**
    - Upload via Arduino IDE or your preferred method

3. **After upload:**
    - Remove the jumper wire between GPIO 0 and GND
    - Press the **Reset** button to run your code

## Features
### Core Functionality
- 📹 **Live Video Streaming** - Real-time camera feed via web interface
- 🎥 **Image Capture** - Single frame capture as JPEG or BMP
- 📊 **Multiple Resolution Support** - From QVGA to UXGA

### Advanced Features (with PSRAM)
- 🔍 **Face Detection** - Automatic face detection in frames
- 👤 **Face Recognition** - Identify enrolled faces (ESP32-S3 only)
- 💡 **LED Flash Control** - Built-in LED flash support

## Performance Notes
⚠️ **PSRAM Requirement:**
- **PSRAM is essential** for UXGA resolution and high JPEG quality
- Select a partition scheme with **at least 3MB APP space**

⚠️ **Face Recognition:**
- **Disabled on ESP32 and ESP32-S2** (takes 15+ seconds per frame)
- **Enabled on ESP32-S3** only
- Face detection available on all boards with PSRAM enabled

## Accessing the Camera
Once uploaded and running:
1. Open the Serial Monitor (115200 baud) to find the IP address
2. Open your browser and navigate to: `http://<YOUR_ESP32_IP>`
3. Use the web interface to:
    - View live stream
    - Capture images
    - Configure camera settings
    - Toggle face detection/recognition

## Project Files
- - Main sketch `CameraWebServer.ino`
- - Pin definitions for different camera models `camera_pins.h`
- - HTTP server implementation with face detection `app_httpd.cpp`
- - Embedded web interface `camera_index.h`

## Troubleshooting
### Camera Init Failed
- Verify camera model selection is correct
- Check GPIO connections
- Ensure proper power supply

### Face Detection Not Working
- Confirm PSRAM is available and enabled
- Check pixel format is set to `PIXFORMAT_RGB565`
- Verify board supports PSRAM

### No Web Interface
- Check Serial Monitor for IP address
- Ensure WiFi credentials are correct
- Verify network connectivity

## License
This project uses code from Espressif Systems. See individual files for license information.
