// === Ultrasonic Sensor Pins ===
#define TRIG_L 26
#define ECHO_L 27
#define TRIG_F 33
#define ECHO_F 32
#define TRIG_R 12
#define ECHO_R 14

// === Motor Pins ===
#define IN1 22
#define IN2 23
#define IN3 19
#define IN4 21
#define ENA 2  // Left Motor PWM
#define ENB 4  // Right Motor PWM

// === Maze Parameters ===
#define MAZE_WIDTH 25  // cm
#define BOT_WIDTH 15   // cm
#define IDEAL_SIDE_DIST ((MAZE_WIDTH - BOT_WIDTH) / 2)  // 5 cm ideal
#define CENTER_TOLERANCE 1.5   // Acceptable ±1.5 cm
#define SAFE_DISTANCE 8        // Stop if anything is closer than 8 cm in front

volatile long left = 1000;
volatile long right = 1000;
volatile long front = 1000;

void setup() {
  Serial.begin(115200);

  // Motor Pin Modes
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);

  // Sensor Pin Modes
  pinMode(TRIG_L, OUTPUT); pinMode(ECHO_L, INPUT);
  pinMode(TRIG_F, OUTPUT); pinMode(ECHO_F, INPUT);
  pinMode(TRIG_R, OUTPUT); pinMode(ECHO_R, INPUT);

  // Tasks
  xTaskCreatePinnedToCore(readings, "SensorRead", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(main_loop, "DecisionMaking", 2048, NULL, 1, NULL, 1);

  // PWM Channels
  ledcAttach(ENA, 1000, 8);
  ledcAttach(ENB, 1000, 8);
}

// === Movement Functions ===
void moveForward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  ledcWrite(ENA, 150); ledcWrite(ENB, 150);
  Serial.println("Moving Forward");
}

void moveBackward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  ledcWrite(ENA, 150); ledcWrite(ENB, 150);
  Serial.println("Moving Backward");
}

void turnLeft() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  ledcWrite(ENA, 150); ledcWrite(ENB, 150);
  Serial.println("Hard Left Turn");
}

void turnRight() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  ledcWrite(ENA, 150); ledcWrite(ENB, 150);
  Serial.println("Hard Right Turn");
}

void slightLeft() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  ledcWrite(ENA, 150); ledcWrite(ENB, 150 * 0.6);
  Serial.println("Slight Left");
}

void slightRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  ledcWrite(ENA, 150 * 0.6); ledcWrite(ENB, 150);
  Serial.println("Slight Right");
}

void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  ledcWrite(ENA, 0); ledcWrite(ENB, 0);
  Serial.println("Motors Stopped");
}

// === Distance Measurement ===
long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);  // timeout 30ms = ~5m
  if (duration == 0) return 1000;
  return duration * 0.034 / 2;
}

// === Sensor Reading Task ===
void readings(void *pvParameters) {
  while (1) {
    left = getDistance(TRIG_L, ECHO_L);
    front = getDistance(TRIG_F, ECHO_F);
    right = getDistance(TRIG_R, ECHO_R);

    Serial.print("L:"); Serial.print(left);
    Serial.print(" F:"); Serial.print(front);
    Serial.print(" R:"); Serial.println(right);

    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

// === Navigation Task ===
void main_loop(void *pvParameters) {
  while (1) {
    float a = (float)left;
    float b = (float)right;
    float diff = a - b;

    if (front < SAFE_DISTANCE) {
      // Wall ahead – choose freer side
      if (a > b) {
        Serial.println("Wall Ahead - Turning Left");
        turnLeft();
      } else {
        Serial.println("Wall Ahead - Turning Right");
        turnRight();
      }
    } else {
      // Centering Logic
      if (abs(diff) <= CENTER_TOLERANCE) {
        moveForward();  // well-centered
      } else if (diff > CENTER_TOLERANCE) {
        slightRight();  // too close to left wall
      } else {
        slightLeft();   // too close to right wall
      }
    }

    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void loop() {
  // Nothing here; everything runs in FreeRTOS tasks
}