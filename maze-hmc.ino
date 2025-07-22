#include <NewPing.h>
#include <Wire.h>
#include <math.h>

// ========== Compass ==========
#define HMC5883L_Address 0x1E
float headingOffset = 0;
float headingTolerance = 5.0;

// ========== Motors ==========
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11
#define ENA 5
#define ENB 6
#define speed 200
#define speedT 255

// ========== Ultrasonic ==========
#define TRIG_L 12
#define ECHO_L A0
#define TRIG_F 4
#define ECHO_F 7
#define TRIG_R 2
#define ECHO_R 3
#define MAX_DISTANCE 200
#define sd 5

NewPing sonarLeft(TRIG_L, ECHO_L, MAX_DISTANCE);
NewPing sonarFront(TRIG_F, ECHO_F, MAX_DISTANCE);
NewPing sonarRight(TRIG_R, ECHO_R, MAX_DISTANCE);

long left, front, right;

// ========== Setup ==========
void setup() {
  Wire.begin();
  Serial.begin(9600);

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);

  initializeHMC5883L();
  delay(500);

  int x, y;
  readHMC5883L(x, y);
  headingOffset = calculateHeading(x, y);

  Serial.println("Compass-MazeBot Ready");
}

// ========== Motion ==========
void moveForward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, speed); analogWrite(ENB, speedT);
}

void slightLeft() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, 150); analogWrite(ENB, 200);
}

void slightRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, 200); analogWrite(ENB, 150);
}

void turnRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, speed); analogWrite(ENB, 100);
  delay(350);
  stopMotors();
  resetHeading();
}

void turnLeft() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, 100); analogWrite(ENB, speedT);
  delay(350);
  stopMotors();
  resetHeading();
}

void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0); analogWrite(ENB, 0);
}

// ========== Compass ==========
void initializeHMC5883L() {
  Wire.beginTransmission(HMC5883L_Address);
  Wire.write(0x02);
  Wire.write(0x00);
  Wire.endTransmission();
}

void readHMC5883L(int &x, int &y) {
  Wire.beginTransmission(HMC5883L_Address);
  Wire.write(0x03);
  Wire.endTransmission();

  Wire.requestFrom(HMC5883L_Address, 6);
  if (Wire.available() >= 6) {
    x = Wire.read() << 8 | Wire.read();
    Wire.read(); Wire.read();
    y = Wire.read() << 8 | Wire.read();
  }
}

float calculateHeading(int x, int y) {
  float heading = atan2((float)y, (float)x) * (180.0 / PI);
  if (heading < 0) heading += 360;
  return heading;
}

float getRelativeHeading() {
  int x, y;
  readHMC5883L(x, y);
  float currentHeading = calculateHeading(x, y);
  float rel = currentHeading - headingOffset;
  if (rel < 0) rel += 360;
  if (rel >= 360) rel -= 360;
  return rel;
}

void resetHeading() {
  int x, y;
  readHMC5883L(x, y);
  headingOffset = calculateHeading(x, y);
  Serial.println("Heading Reset to 0°");
}

// ========== U-Turn ==========
void turnAround() {
  Serial.println("U-turn Initiated");
  while (true) {
    float rh = getRelativeHeading();
    Serial.print("Turning... Heading: "); Serial.println(rh);

    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
    analogWrite(ENA, 180); analogWrite(ENB, 180);

    if (abs(rh - 180.0) <= 5.0) break;
    delay(10);
  }

  stopMotors();
  resetHeading();
  Serial.println("U-turn Complete");
}

// ========== Sensor ==========
void readings() {
  left = sonarLeft.ping_cm();
  front = sonarFront.ping_cm();
  right = sonarRight.ping_cm();

  if (left == 0) left = 0;
  if (front == 0) front = 0;
  if (right == 0) right = 0;

  float rh = getRelativeHeading();
  Serial.print("L: "); Serial.print(left);
  Serial.print(" | F: "); Serial.print(front);
  Serial.print(" | R: "); Serial.print(right);
  Serial.print(" | Heading: "); Serial.println(rh);
}

// ========== Maze Solver ==========
void loop() {
  readings();
  float relHeading = getRelativeHeading();

  if ((right < sd && front < sd) || (left > sd && front > sd) || (left > sd && right > sd)) {
    Serial.println("Turn Left");
    turnLeft();
  } 
  else if (front > sd) {
    Serial.print("Moving Forward | Heading: "); Serial.println(relHeading);
    if (relHeading <= headingTolerance || relHeading >= 360 - headingTolerance) {
      moveForward();
    } else if (relHeading > headingTolerance && relHeading < 180) {
      slightRight();
    } else {
      slightLeft();
    }
  } 
  else if (left < sd && front < sd) {
    Serial.println("Turn Right");
    turnRight();
  } 
  else {
    Serial.println("Dead-end Detected — Executing U-turn");
    turnAround();
  }

  delay(100);
}
