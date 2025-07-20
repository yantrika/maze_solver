#include <AFMotor.h>

AF_DCMotor motorL(1);
AF_DCMotor motorR(4);
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  motorL.setSpeed(100);
  motorR.setSpeed(100);
  motorL.run(FORWARD);
  motorR.run(FORWARD);
  delay(2000);
  motorL.run(RELEASE);
  motorR.run(RELEASE);
  delay(1000); 
  motorL.run(BACKWARD);
  motorR.run(BACKWARD);
  delay(2000);
  motorL.run(RELEASE);
  motorR.run(RELEASE);  
  delay(1000);
}
