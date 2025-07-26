// Motor A connections
#define AIN1 8
#define AIN2 9
#define PWMA 5

// Motor B connections
#define BIN1 10
#define BIN2 11
#define PWMB 6

// Standby pin
#define STBY 7

// Motor speed (0-255)
int speedVal = 200;

void setup() {
  // Set all pins as output
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  
  pinMode(STBY, OUTPUT);
  
  digitalWrite(STBY, HIGH); // Disable standby
}

void loop() {
  // Motor A forward
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, speedVal);

  // Motor B forward
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, speedVal);

  delay(2000);

  // Stop motors
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
  delay(1000);

  // Motor A backward
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, speedVal);

  // Motor B backward
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB, speedVal);

  delay(2000);

  // Stop motors
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
  delay(1000);
}
