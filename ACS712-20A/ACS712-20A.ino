const int sensorPin = A0;
const float sensitivity = 0.100; // V/A for ACS712-20A
float Vref = 2.5;                // nominal zero-current midpoint

void setup() {
  Serial.begin(9600);
  // Calibrate zero point — make sure NO current is flowing when this runs
  long sum = 0;
  for (int i = 0; i < 500; i++) {
    sum += analogRead(sensorPin);
    delay(2);
  }
  Vref = (sum / 500.0) * (5.0 / 1023.0);
  Serial.print("Calibrated zero offset: ");
  Serial.println(Vref, 4);
}

void loop() {
  long sum = 0;
  for (int i = 0; i < 200; i++) {
    sum += analogRead(sensorPin);
    delayMicroseconds(500);
  }
  float voltage = (sum / 200.0) * (5.0 / 1023.0);
  float current = (voltage - Vref) / sensitivity;

  Serial.print("Voltage: ");
  Serial.print(voltage, 4);
  Serial.print(" V  Current: ");
  Serial.print(current, 4);
  Serial.println(" A");

  delay(500);
}