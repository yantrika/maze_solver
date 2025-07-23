#define TRIG_PIN 25
#define ECHO_PIN 26

unsigned long lastReadTime = 0;
const unsigned long interval = 200; // ms

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  .
  Serial.println("Ultrasonic Sensor Test Started");
}

long readUltrasonicCM(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH, 30000);  // 30 ms timeout
  if (duration == 0) return -1;                // Timeout
  return duration * 0.034 / 2;                 // Convert to cm
}

void loop() {
  unsigned long currentTime = millis();
  if (currentTime - lastReadTime >= interval) {
    lastReadTime = currentTime;

    long distance = readUltrasonicCM(TRIG_PIN, ECHO_PIN);
    if (distance == -1) {
      Serial.println("No echo received.");
    } else {
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.println(" cm");
    }
  }

  // Your robot logic can run here without being blocked
}
