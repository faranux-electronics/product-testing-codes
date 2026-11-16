/*
 * Sensor: MPU-9250/6500 (6-Axis IMU)
 * Axes: 3-axis Accelerometer + 3-axis Gyroscope (NO Magnetometer)
 * WhoAmI Register: 0x70
 * I2C Address: 0x68 (ADO=GND) or 0x69 (ADO=3.3V)
 * Library: MPU9250_WE v1.2.17
 * Board: Arduino Uno
 * 
 * Wiring:
 *   VCC → 5V
 *   GND → GND
 *   SDA → A4
 *   SCL → A5
 * 
 * Data Output:
 *   Accelerometer : g-force (X, Y, Z) — flat = (0, 0, 1)
 *   Gyroscope     : degrees/second (X, Y, Z) — still = (~0, ~0, ~0)
 *   Pitch & Roll  : tilt angles in degrees
 *   Temperature   : internal chip temperature in °C
 */

#include <MPU9250_WE.h>
#include <Wire.h>

#define MPU9250_ADDR 0x68
MPU9250_WE myMPU9250 = MPU9250_WE(MPU9250_ADDR);

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  if(!myMPU9250.init()){
    Serial.println("MPU9250 does not respond");
  } else {
    Serial.println("MPU6500 connected!");
  }

  // Optional: calibrate gyro (keep sensor still for ~3s)
  Serial.println("Calibrating gyro, keep still...");
  myMPU9250.autoOffsets();
  Serial.println("Done.");
}

void loop() {
  xyzFloat acc = myMPU9250.getGValues();     // acceleration in g
  xyzFloat gyr = myMPU9250.getGyrValues();   // gyro in degrees/sec
  float temp   = myMPU9250.getTemperature(); // °C

  Serial.print("AccX: ");  Serial.print(acc.x);
  Serial.print(" | AccY: "); Serial.print(acc.y);
  Serial.print(" | AccZ: "); Serial.print(acc.z);
  Serial.print(" | GyroX: "); Serial.print(gyr.x);
  Serial.print(" | GyroY: "); Serial.print(gyr.y);
  Serial.print(" | GyroZ: "); Serial.print(gyr.z);
  Serial.print(" | Temp: "); Serial.println(temp);

  delay(100);
}