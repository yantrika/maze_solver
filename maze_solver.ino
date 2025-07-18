#include <Arduino.h>
#include <NewPing.h>
#include "src/Motors.hpp"
#include "src/PID.hpp"
#include "src/Sensors.hpp"


// PID parameters can be adjusted as needed
Motors motor_obj;
PID pid_obj;
Sensors sens_obj;

void setup() {
  Serial.begin(9600);
  delay(1000);

  Serial.println("Self-Centering Left-Hand Maze Solver (NewPing)");
}

void loop() {

  //Read and then update the sensors
  sens_obj.update_vals();

  //Apply PID
  pid_obj.motor_change(sens_obj.left, sens_obj.right, &motor_obj);

  delay(100);  // stabilize loop
}
