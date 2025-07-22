#include <Wire.h>
#include <math.h>

#define HMC5883L_Address 0x1E

float headingOffset = 0;  // Store initial heading

void setup() {
  Wire.begin();
  Serial.begin(9600);
  initializeHMC5883L();

  delay(500);  // Allow sensor to stabilize

  int x, y;
  readHMC5883L(x, y);
  headingOffset = calculateHeading(x, y);  // Set startup direction as 0°
}

void loop() {
  int x, y;
  readHMC5883L(x, y);
  float currentHeading = calculateHeading(x, y);

  float relativeHeading = currentHeading - headingOffset;

  // Normalize angle to [0, 360)
  if (relativeHeading < 0) relativeHeading += 360;
  if (relativeHeading >= 360) relativeHeading -= 360;

  Serial.print("Relative Heading: ");
  Serial.println(relativeHeading);

  delay(200);
}

void initializeHMC5883L() {
  Wire.beginTransmission(HMC5883L_Address);
  Wire.write(0x02);  // Mode register
  Wire.write(0x00);  // Continuous measurement mode
  Wire.endTransmission();
}

void readHMC5883L(int &x, int &y) {
  Wire.beginTransmission(HMC5883L_Address);
  Wire.write(0x03);  // Data output register
  Wire.endTransmission();

  Wire.requestFrom(HMC5883L_Address, 6);
  if (Wire.available() >= 6) {
    x = Wire.read() << 8 | Wire.read();  // X-axis
    Wire.read(); Wire.read();            // Z-axis (ignored)
    y = Wire.read() << 8 | Wire.read();  // Y-axis
  }
}

float calculateHeading(int x, int y) {
  float heading = atan2((float)y, (float)x) * (180.0 / PI);
  if (heading < 0) heading += 360;
  return heading;
}