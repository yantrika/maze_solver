#line 1 "/home/bhaskar/study/maze_solver/src/Motors.cpp"
#include "Motors.hpp"
#include <Arduino.h>

Motors::Motors(){
        pinMode(IN1, OUTPUT); 
        pinMode(IN2, OUTPUT);
        pinMode(IN3, OUTPUT); 
        pinMode(IN4, OUTPUT);
        pinMode(ENA, OUTPUT); 
        pinMode(ENB, OUTPUT);
}

void Motors::moveForward(int pid = 0) {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    analogWrite(ENA, currspeedL -pid); analogWrite(ENB, currspeedR + pid);
  }
  
  void Motors::slightLeft() {
    analogWrite(ENA, 180); analogWrite(ENB, 180);
    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  }
  
  void Motors::slightRight() {
    analogWrite(ENA, 180); analogWrite(ENB, 180);
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  }
  
  void Motors::turnLeft() {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
    analogWrite(ENA, currspeedL); analogWrite(ENB, 100);
  }
  
  void Motors::turnRight() {
    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    analogWrite(ENA, 100); analogWrite(ENB, currspeedR);
  }
  
  void Motors::stopMotors() {
    digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
    analogWrite(ENA, 0); analogWrite(ENB, 0);
  }

  