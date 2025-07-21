#include <NewPing.h>

// ========== Motor Control Pins ==========
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

#define ENA 5  // Motor A PWM
#define ENB 6  // Motor B PWM

#define speed 200  // right motor
#define speedT 255  // left motor

// ========== Ultrasonic Sensor Pins ==========
#define TRIG_L 12
#define ECHO_L A0
#define TRIG_F 4
#define ECHO_F 7
#define TRIG_R 2
#define ECHO_R 3

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

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);

  Serial.println("Self-Centering Left-Hand Maze Solver (NewPing)");
}

// ========== Movement Functions ==========
void moveForward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, speed); analogWrite(ENB, speedT);
}

void slightLeft() {
  analogWrite(ENA, 180); analogWrite(ENB, 180);
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void slightRight() {
  analogWrite(ENA, 180); analogWrite(ENB, 180);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void turnLeft() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, speed); analogWrite(ENB, 100);
}

void turnRight() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, 100); analogWrite(ENB, speedT);
}

void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0); analogWrite(ENB, 0);
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
