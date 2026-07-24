/*
  ============================================================
  GM805 Barcode/QR Scanner Module - Arduino UART Reader
  ============================================================

  HARDWARE:
    GM805 Series Barcode Reader Module
    Communicates over TTL-232 serial (default: 9600 baud, 8N1)

  WIRING (GM805 6-pin UART/USB header -> Arduino Uno):
    GM805 GND [labeled -] (black) -----> Arduino GND
    GM805 VCC [labeled +] (green) -----> Arduino 5V
    GM805 TXD (red)   -----> Arduino Pin 10 (SoftwareSerial RX)
    GM805 RXD (white) -----> Arduino Pin 11 (SoftwareSerial TX)
    GM805 D+  (yellow) ----> NOT CONNECTED (USB data line, unused in UART mode)
    GM805 D-  (orange) ----> NOT CONNECTED (USB data line, unused in UART mode)

  NOTE ON PIN NAMING:
    Wire colors are NOT standardized between manufacturers/cables.
    Always confirm with a multimeter continuity test against the
    board's silkscreen labels (TXD/RXD/VCC/GND/D+/D-) before wiring
    a new/different cable - do not assume colors match this sketch.

  IMPORTANT - MODULE OUTPUT MODE:
    The GM805 can operate in different output modes:
      - Serial / UART output   (what this sketch expects)
      - USB HID keyboard mode  (module types like a keyboard into
                                 whatever text field is focused on a PC)
      - USB Virtual Serial     (shows up as a COM port over USB)
    If you get zero serial data despite the scanner beeping/lighting
    on a successful read, the module is very likely in USB HID mode.
    Scan the "Serial Output" setup barcode from the datasheet
    (or use generate_setup_barcodes.py included in this project) to
    force it back into UART mode. Then use the "Save Current Settings
    as User Default Settings" barcode so it survives a power cycle.

  BEHAVIOR:
    The GM805 defaults to Continuous Mode: it will keep re-reading
    the same code repeatedly while it's in view. This sketch includes
    simple duplicate-suppression so the same code isn't printed twice
    in a row within a short time window.

  ============================================================
*/

#include <SoftwareSerial.h>

// ---- Pin configuration ----
// GM805 TXD -> Arduino RX pin (module transmits, Arduino receives)
// GM805 RXD -> Arduino TX pin (Arduino transmits, module receives - optional)
const uint8_t SCANNER_RX_PIN = 10;
const uint8_t SCANNER_TX_PIN = 11;

SoftwareSerial scanner(SCANNER_RX_PIN, SCANNER_TX_PIN);

// ---- Duplicate-read suppression ----
String lastCode = "";
unsigned long lastReadTime = 0;
const unsigned long DUPLICATE_SUPPRESS_MS = 1500; // ignore identical repeats within this window

void setup() {
  // USB serial for debugging / viewing output on your PC
  Serial.begin(9600);

  // GM805 default UART settings: 9600 baud, 8 data bits, no parity, 1 stop bit
  scanner.begin(9600);

  Serial.println(F("GM805 ready. Scan a barcode..."));
}

void loop() {
  if (scanner.available()) {
    // Read one line of decoded barcode data.
    // Default module "tail" character can be CR, LF, CRLF, or none -
    // adjust the terminator below if your module's tail setting differs.
    String code = scanner.readStringUntil('\n');
    code.trim(); // strip any leftover \r or whitespace

    if (code.length() > 0) {
      unsigned long now = millis();
      bool isDuplicate = (code == lastCode) &&
                          (now - lastReadTime < DUPLICATE_SUPPRESS_MS);

      if (!isDuplicate) {
        Serial.print(F("Scanned: "));
        Serial.println(code);

        // ---- Hook your own logic here ----
        // e.g. forward to a POS lookup, add to a queue, trigger an action, etc.

        lastCode = code;
        lastReadTime = now;
      }
    }
  }
}
