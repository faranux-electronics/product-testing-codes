# GM805 Barcode/QR Scanner — Getting Started Guide

**Made by Faranux Electronics**  
**Visit us at: [faranux.com](https://faranux.com)**

This guide covers wiring, configuring, and reading data from a **GM805 series
barcode/QR scanner module**, both over **Arduino UART** and as a **plug-and-play
USB device** on a PC. No prior experience with the module is assumed.

---

## 1. What's on the board

The GM805 has **two separate 6-pin headers** — don't mix them up:

| Header | Pins | Purpose |
|---|---|---|
| **Control header** | TRIG, LED1, LED2, LED3, BEEP, OUT | Simple GPIO signals (trigger scan, drive LEDs/buzzer). **Does not carry decoded barcode data.** |
| **UART/USB header** | VCC, GND, TXD, RXD, D+, D- | Power + actual data output (either serial or USB) |

Decoded barcode text only ever comes out the **UART/USB header**, specifically
the **TXD** pin (in serial mode) or over **D+/D-** (in USB mode).

Wire colors on cables are **not standardized** — always verify with a
multimeter continuity test against the board's printed silkscreen labels
before trusting a cable's color coding.

---

## 2. Two ways to get data out: UART vs USB

The GM805 can output decoded data in one of a few modes, set by an internal
configuration register (this is a *saved setting*, not just about which
wires you connect):

| Mode | How it behaves |
|---|---|
| **Serial / UART** | Streams plain text over TXD at 9600 baud (default), 8N1. This is what the Arduino sketch expects. |
| **USB HID Keyboard** | Acts like a USB keyboard — typing scanned text directly into whatever text field is focused on a PC. No code needed, but not usable by an Arduino directly. |
| **USB Virtual Serial** | Enumerates as a COM port over USB — readable with any serial terminal, no Arduino needed. |

**If you get zero output on TXD despite the scanner beeping/lighting on a
successful scan, the module is almost certainly in USB HID mode**, not
UART mode. This is a very common out-of-the-box state.

### Switching to Serial/UART output mode
1. Print the datasheet's setup barcodes (or generate them — see section 5).
2. Scan the **"Reset"** setup barcode (returns to factory defaults).
3. Scan the **"Serial Output"** setup barcode.
4. Once working, scan **"Save Current Settings as User Default Settings"**
   so it survives a power cycle — otherwise a future reset can silently put
   you back in USB mode.
5. If you need every barcode symbology enabled (not just the factory
   default subset), also scan the **"Support all"** barcode types setup code.

---

## 3. Testing with USB first (no Arduino needed)

This is the fastest way to confirm the scanner itself works before
introducing any wiring/code variables.

1. Take any spare USB-A cable (an old cable you can access the 4 internal
   wires on works fine). Standard internal USB wire colors:

   | USB wire color | Function |
   |---|---|
   | Red | VCC (5V) |
   | Black | GND |
   | Green | D+ |
   | White | D- |

2. Wire GM805 VCC/GND/D+/D- to the corresponding USB wires (matching
   *your* confirmed wire colors, not necessarily the table above — always
   verify with a multimeter first).
3. Plug into your PC.
4. If in **HID mode**: open Notepad, scan a barcode — the text should type
   itself in directly.
5. If in **Virtual Serial mode**: it'll show up as a new COM port; open it
   in any serial terminal at 9600 baud.

---

## 4. Wiring for Arduino (UART mode)

Once the module is confirmed in **Serial/UART output mode**:

```
GM805 GND -----> Arduino GND
GM805 VCC -----> Arduino 5V
GM805 TXD -----> Arduino Pin 10   (module -> Arduino, i.e. Arduino RX)
GM805 RXD -----> Arduino Pin 11   (Arduino -> module, optional, only needed to send commands)
GM805 D+  -----> not connected
GM805 D-  -----> not connected
```

Default UART parameters: **9600 baud, 8 data bits, no parity, 1 stop bit.**

Upload `GM805_Scanner.ino` (included in this project), open the Serial
Monitor at 9600 baud, and scan a code.

> **Uno/Nano users:** the sketch uses `SoftwareSerial` on pins 10/11 so it
> doesn't conflict with the USB-connected debug Serial port. If you want to
> use the hardware UART (pins 0/1) instead for reliability, you must
> disconnect the module while uploading new sketches, since pins 0/1 are
> shared with the USB programming interface.

---

## 5. Troubleshooting checklist

Work through these in order if you're not seeing data:

1. **Which LED lights up?**
   - White LED = illumination, lights on every scan *attempt* (not proof of success)
   - Blue LED = decode **success** indicator — this is the one that matters
   - No beep + no blue LED = it's not actually decoding; check focus/distance/lighting

2. **Confirm wiring with a multimeter**, continuity mode, wire-by-wire
   against the board's silkscreen pads — don't trust cable colors blindly.

3. **Confirm common ground** between the module and whatever it's wired to.

4. **Rule out USB HID mode** — this is the single most common cause of
   "scanner beeps/lights but nothing shows up on serial." See section 2.

5. **Bypass Arduino entirely** — wire directly to a USB-to-TTL/FTDI adapter
   and test in a serial terminal at 9600 baud. If data shows up there but
   not through the Arduino, the issue is Arduino-side (wrong pins, upload
   conflicts, SoftwareSerial quirk) — not the module or its wiring.

6. **Check the FTDI/adapter's voltage jumper** if using one — must be set
   to 5V, not 3.3V.

7. **Query the module directly over serial** to confirm it's alive, using
   the raw command to read its output-mode register (zone bit `0x000D`):
   ```
   7E 00 07 01 00 0D 01 AB CD
   ```
   A response starting `02 00 00 01 ...` confirms the UART link is active.

---

## 6. Notes on behavior

- **Continuous Mode** (the default) will keep re-reading the same code
  repeatedly while it stays in the scan area. The included sketch
  suppresses duplicate reads within a 1.5 second window — adjust
  `DUPLICATE_SUPPRESS_MS` as needed, or use the module's own
  "Same barcode reading delay" setup barcode to handle this at the
  hardware level instead.
- The module can read a wide range of 1D and 2D symbologies (EAN/UPC,
  Code128/39/93, Interleaved 2 of 5, QR, Data Matrix, PDF417, and more) —
  some are disabled by default and need the "Support all" or individual
  type-enable setup barcodes scanned first.
- Full command reference, zone-bit map, and every setup barcode image are
  in the official GM805 datasheet (sections 10 and appendices A-G).
