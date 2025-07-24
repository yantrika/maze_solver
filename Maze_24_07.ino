#include <NewPing.h>

// ========== Motor Control Pins ==========
#define AIN1 14
#define AIN2 27
#define BIN1 26
#define BIN2 33

#define PWMA 25  // Motor A PWM
#define PWMB 32  // Motor B PWM
#define STBY 12

#define speed 50 // right motor
#define speedT 50 // left motor

// ========== Ultrasonic Sensor Pins ==========
#define TRIG_L 18
#define ECHO_L 13
#define TRIG_F 19
#define ECHO_F 35
#define TRIG_R 21
#define ECHO_R 34

#define MAX_DISTANCE 200  // in cm
#define sd 8  // wall threshold
#define ERROR_THRESHOLD 2.0  // in cm

// ========== NewPing Objects ==========
NewPing sonarLeft(TRIG_L, ECHO_L, MAX_DISTANCE);
NewPing sonarFront(TRIG_F, ECHO_F, MAX_DISTANCE);
NewPing sonarRight(TRIG_R, ECHO_R, MAX_DISTANCE);

// Global variables
long left, front, right;

void setup() {
  Serial.begin(9600);
  delay(1000);

  pinMode(AIN1, OUTPUT); pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT); pinMode(BIN2, OUTPUT);
  pinMode(PWMA, OUTPUT); pinMode(PWMB, OUTPUT);
  pinMode(STBY,OUTPUT);

  digitalWrite(STBY, HIGH);

  Serial.println("Self-Centering Left-Hand Maze Solver (NewPing)");
}

// ========== Movement Functions ==========
void moveForward() {
  digitalWrite(AIN1, HIGH); digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, HIGH); digitalWrite(BIN2, LOW);
  analogWrite(PWMA, speed); analogWrite(PWMB, speedT);
}

// void slightLeft() {
//   analogWrite(ENA, 180); analogWrite(ENB, 180);
//   digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
//   digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
// }

// void slightRight() {
//   analogWrite(ENA, 180); analogWrite(ENB, 180);
//   digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
//   digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
// }

void turnLeft() {
  digitalWrite(AIN1, HIGH); digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW); digitalWrite(BIN2, HIGH);
  analogWrite(PWMA, speed); analogWrite(PWMB, speedT);
}

void turnRight() {
  digitalWrite(AIN1, LOW); digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, HIGH); digitalWrite(BIN2, LOW);
  analogWrite(PWMA, speed); analogWrite(PWMB, speedT);
}

void stopMotors() {
  digitalWrite(AIN1, LOW); digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW); digitalWrite(BIN2, LOW);
  analogWrite(PWMA, 0); analogWrite(PWMB, 0);
}

// ========== Ultrasonic Readings ==========
void readings() {
  left = sonarLeft.ping_cm();
  front = sonarFront.ping_cm();
  right = sonarRight.ping_cm();

  if (left == 0) left = 0;
  if (front == 0) front = 0;
  if (right == 0) right = 0;

  Serial.print("L: "); Serial.print(left);
  Serial.print(" | F: "); Serial.print(front);
  Serial.print(" | R: "); Serial.println(right);
}

// ========== Maze-Solving Logic ==========
void loop() {
  readings();
  float error = left - right;

  // Left-hand rule priority: Left > Front > Right
  if ((right < sd && front < sd) || (left > sd && front > sd)) {
    Serial.println("Turn Left");
    turnLeft();
    delay(200);
  } 
  else if (front > sd) {
      Serial.println("Moving Forward (Centered)");
      moveForward();
      // readings();
  } 
  else if (left<sd && front<sd) {
    Serial.println("Turn Right");
    turnRight();
    delay(200);
    // readings();

  } 
  else {
    Serial.println("No path — Turn Left (default)");
    // turnLeft(); // Dead end; follow left-hand rule
  }

  delay(100);  // stabilize loop
}
