#include <Wire.h>
#include <math.h>

#define HMC5883L_ADDRESS 0x1E

// Motor Pins
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11
#define ENA 5
#define ENB 6

float heading = 0.0;
float magOffsetX = 0, magOffsetY = 0;

int motorSpeed = 255;
float Kp = 3.0, Ki = 0.005, Kd = 1.2;
float integral = 0, lastError = 0;
unsigned long lastTime = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // HMC5883L Initialization
  Wire.beginTransmission(HMC5883L_ADDRESS);
  Wire.write(0x00); Wire.write(0x70); Wire.endTransmission();
  Wire.beginTransmission(HMC5883L_ADDRESS);
  Wire.write(0x01); Wire.write(0xA0); Wire.endTransmission();
  Wire.beginTransmission(HMC5883L_ADDRESS);
  Wire.write(0x02); Wire.write(0x00); Wire.endTransmission();

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);

  delay(1000);
  Serial.println("Starting self-calibration...");
  rotateAndCalibrateCompass();
  delay(1000);
}

void loop() {
  float currentHeading = getHeading();
  float targetRight = fmod(currentHeading + 90 + 360, 360);
  Serial.println("Turning 90° Right...");
  turnToAngle(targetRight);
  delay(2000);

  currentHeading = getHeading();
  float targetLeft = fmod(currentHeading - 90 + 360, 360);
  Serial.println("Turning 90° Left...");
  turnToAngle(targetLeft);
  delay(2000);
}

float getHeading() {
  int16_t x, y;
  Wire.beginTransmission(HMC5883L_ADDRESS);
  Wire.write(0x03);
  Wire.endTransmission();
  Wire.requestFrom(HMC5883L_ADDRESS, 6);
  if (Wire.available() == 6) {
    x = Wire.read() << 8 | Wire.read();
    Wire.read(); Wire.read();  // skip Z
    y = Wire.read() << 8 | Wire.read();
  }

  float x_cal = x - magOffsetX;
  float y_cal = y - magOffsetY;
  float angle = atan2(y_cal, x_cal);
  if (angle < 0) angle += 2 * PI;
  return angle * 180 / PI;
}

void turnToAngle(float targetAngle) {
  targetAngle = fmod(targetAngle + 360.0, 360.0);
  integral = 0; lastError = 0;
  lastTime = millis();

  unsigned long start = millis();
  while (millis() - start < 5000) {
    float current = getHeading();
    float error = targetAngle - current;
    if (error > 180) error -= 360;
    if (error < -180) error += 360;

    if (abs(error) < 1.0) break;

    unsigned long now = millis();
    float dt = (now - lastTime) / 1000.0;
    lastTime = now;

    integral += error * dt;
    float derivative = (error - lastError) / dt;
    lastError = error;

    float output = Kp * error + Ki * integral + Kd * derivative;
    output = constrain(output, -motorSpeed, motorSpeed);

    int pwm = abs(output);
    if (pwm < 60) pwm = 60;

    if (output > 0) {
      digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    } else {
      digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
    }

    analogWrite(ENA, pwm);
    analogWrite(ENB, pwm);

    Serial.print("Heading: "); Serial.print(current);
    Serial.print(" | Target: "); Serial.print(targetAngle);
    Serial.print(" | Error: "); Serial.print(error);
    Serial.print(" | PWM: "); Serial.println(pwm);

    delay(20);
  }

  stopMotors();
  Serial.println("Turn complete.");
}

void rotateAndCalibrateCompass() {
  int16_t x, y;
  int16_t minX = 32767, maxX = -32768;
  int16_t minY = 32767, maxY = -32768;

  unsigned long start = millis();

  while (millis() - start < 4250) {
    Wire.beginTransmission(HMC5883L_ADDRESS);
    Wire.write(0x03);
    Wire.endTransmission();
    Wire.requestFrom(HMC5883L_ADDRESS, 6);

    if (Wire.available() == 6) {
      x = Wire.read() << 8 | Wire.read();
      Wire.read(); Wire.read();
      y = Wire.read() << 8 | Wire.read();

      if (x < minX) minX = x;
      if (x > maxX) maxX = x;
      if (y < minY) minY = y;
      if (y > maxY) maxY = y;
    }

    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    analogWrite(ENA, 180); analogWrite(ENB, 180);

    delay(50);
  }

  stopMotors();
  magOffsetX = (maxX + minX) / 2.0;
  magOffsetY = (maxY + minY) / 2.0;

  Serial.println("Self-calibration complete.");
  Serial.print("X offset: "); Serial.println(magOffsetX);
  Serial.print("Y offset: "); Serial.println(magOffsetY);
}

void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0); analogWrite(ENB, 0);
}
