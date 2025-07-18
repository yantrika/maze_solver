#include <NewPing.h>
#include "src/Motors.cpp"
#include "src/PID.h"
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

  

  Serial.println("Self-Centering Left-Hand Maze Solver (NewPing)");
}

// ========== Movement Functions ==========
Motors motor_obj;
PID pid_obj;
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
  pid_obj.motor_change(left, right, &motor_obj);

//  // Left-hand rule priority: Left > Front > Right
//  if ((right < sd && front < sd) || (left > sd && front > sd)) {
//    Serial.println("Turn Left");
//    motor_obj.turnLeft();
//    delay(200);
//  } 
//  else if (front > sd) {
//      Serial.println("Moving Forward (Centered)");
//      motor_obj.moveForward();
//      // readings();
//  } 
//  else if (left<sd && front<sd) {
//    Serial.println("Turn Right");
//   motor_obj.turnRight();
//    delay(200);
//    // readings();
//
//  } 
//  else {
//    Serial.println("No path — Turn Left (default)");
//    // turnLeft(); // Dead end; follow left-hand rule
//  }

  delay(100);  // stabilize loop
}
