// Motors' PWM pins
const int motorL = 3;
const int motorB = 6;
const int motorR = 5;
const int trigPin = 8;   // Trigger Pin for Ultrasonic Sensor
const int echoPin = 9;   // Echo Pin for Ultrasonic Sensor
const int LEFT_TRIG = 10;
const int LEFT_ECHO = 11;
const int RIGHT_TRIG = 12;
const int RIGHT_ECHO = 13;
const int enableL = 2;
const int enableB = 7;
const int enableR = 4;

float output[3] = {0.0};
float input[3] = {0.0};  // {fx, fy, fw}
float calculations[3][3] = {
  {1.0 / 3.0, -1.0 / sqrt(3.0), 1.0 / 3.0},
  {-2.0 / 3.0, 0.0, 1.0 / 3.0},
  {1.0 / 3.0, 1.0 / sqrt(3.0), 1.0 / 3.0}
};

void setup() {
  pinMode(motorL, OUTPUT);
  pinMode(motorB, OUTPUT);
  pinMode(motorR, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LEFT_TRIG, OUTPUT);
  pinMode(LEFT_ECHO, INPUT);
  pinMode(RIGHT_TRIG, OUTPUT);
  pinMode(RIGHT_ECHO, INPUT);
  pinMode(enableL, OUTPUT);
  pinMode(enableB, OUTPUT);
  pinMode(enableR, OUTPUT);

  Serial.begin(9600);
  delay(20);
}

long measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.0343 / 2;  // Distance in cm
}

void movement(float input[]) {
  for (int i = 0; i < 3; i++) {
    output[i] = 0;
    for (int j = 0; j < 3; j++) {
      output[i] += calculations[i][j] * input[j];
    }
  }

  float max = 0.0;
  for (int i = 0; i < 3; i++) {
    if (abs(output[i]) > max) max = abs(output[i]);
  }

  if (max == 0) {
    Serial.println("Error: Max value is zero, cannot normalize.");
    return;
  }

  float ratioL = output[0] / max;
  float ratioB = output[1] / max;
  float ratioR = output[2] / max;

  analogWrite(motorL, (int)(constrain(180 * abs(ratioL), 0, 180)));
  digitalWrite(enableL, ratioL > 0 ? HIGH : LOW);

  analogWrite(motorR, (int)(constrain(180 * abs(ratioB), 0, 180)));
  digitalWrite(enableR, ratioB > 0 ? HIGH : LOW);

  analogWrite(motorB, (int)(constrain(180 * abs(ratioR), 0, 180)));
  digitalWrite(enableB, ratioR > 0 ? HIGH : LOW);
}

int a = 0;

void loop() {
  if (Serial.available() > 0) {
    // Read the incoming byte
    char incomingByte = Serial.read();
    
    if (incomingByte == 'S'){
      while (a == 0){
        input[0] = 0.0; input[1] = 1.0; input[2] = 0.0;
        movement(input);
        a = 1;
      }
    }
  }
}
