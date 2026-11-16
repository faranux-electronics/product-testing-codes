#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

// Pin definitions (from your wiring table)
#define TFT_CS   10
#define TFT_DC   9
#define TFT_RST  8
#define TFT_BL   7

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(9600);
  
  // Turn on backlight
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  
  // Initialize display (240x320)
  tft.init(240, 320);
  tft.setRotation(1);   // 1 = landscape (320x240)
  
  // Test colours
  tft.fillScreen(ST77XX_RED);
  delay(1000);
  tft.fillScreen(ST77XX_GREEN);
  delay(1000);
  tft.fillScreen(ST77XX_BLUE);
  delay(1000);
  tft.fillScreen(ST77XX_BLACK);
  
  // Print text
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(40, 60);
  tft.print("Display OK");
  
  Serial.println("Display test done.");
}

void loop() {
  // Nothing here
}