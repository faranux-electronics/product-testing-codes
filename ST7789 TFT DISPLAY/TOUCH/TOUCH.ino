#include <XPT2046_Touchscreen.h>
#include <SPI.h>

// Touch pins (from your wiring)
#define TOUCH_CS  4
#define TOUCH_IRQ 3   // optional, but helps

XPT2046_Touchscreen ts(TOUCH_CS, TOUCH_IRQ);

void setup() {
  Serial.begin(115200);
  ts.begin();
  ts.setRotation(1);   // match your display orientation (1 = landscape)
  Serial.println("Touch screen – raw values will appear");
}

void loop() {
  if (ts.tirqTouched() && ts.touched()) {
    TS_Point p = ts.getPoint();
    // Filter by pressure (z) to avoid false touches
    if (p.z > 500) {
      Serial.print("X=");
      Serial.print(p.x);
      Serial.print(" Y=");
      Serial.print(p.y);
      Serial.print(" Z=");
      Serial.println(p.z);
    }
    delay(50);  // debounce
  }
}