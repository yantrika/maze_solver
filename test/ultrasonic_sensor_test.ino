#include <Arduino.h>
#include <NewPing.h>

#define TRIG_PIN 10   
#define ECHO_PIN 11   
#define MAX_DIST 200  

// Create NewPing object 
NewPing obj(TRIG_PIN, ECHO_PIN, MAX_DIST);

void setup() {

  Serial.begin(9600);
  delay(1000);  
  
  
}

void loop() {

  Serial.print("Distance: ");
  Serial.print(obj.ping_cm());
  Serial.println(" cm");

  delay(1000);
}

