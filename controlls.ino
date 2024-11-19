// Motors' PWM pins
const int motorL = 3;
const int motorB = 6;
const int motorR = 5;
// Motors' Direction pins
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


/**
 * Initializes the motor and enable pins as output and starts the serial communication at a baud rate of 9600.
 * Introduces a short delay to ensure the system is ready before proceeding with operations.
 */
void setup() {
  // Set the motor pins as output
  pinMode(motorL, OUTPUT);
  pinMode(motorB, OUTPUT);
  pinMode(motorR, OUTPUT);

  // Set the enable pins as output
  pinMode(enableL, OUTPUT);
  pinMode(enableB, OUTPUT);
  pinMode(enableR, OUTPUT);

  // Start the serial communication at a baud rate of 9600
  Serial.begin(9600);

  // Introduce a short delay to ensure the system is ready before proceeding with operations
  delay(20);
}


/**
 * Function to control the movement of a three-wheel omnidirectional robot.
 * It calculates the motor outputs based on the input forces and normalizes
 * the outputs to ensure the motors operate within their limits.
 *
 * @param input A float array containing the input forces: {fx, fy, fw}.
 */
void movement(float input[]) {
  // Calculate the output for each motor based on input forces
  for (int i = 0; i < 3; i++) {
    output[i] = 0;
    for (int j = 0; j < 3; j++) {
      output[i] += calculations[i][j] * input[j];
    }
  }

  // Find the maximum absolute value in the output array for normalization
  float max = 0.0;
  for (int i = 0; i < 3; i++) {
    if (abs(output[i]) > max) max = abs(output[i]);
  }

  // Check if the maximum value is zero to avoid division by zero
  if (max == 0) {
    Serial.println("Error: Max value is zero, cannot normalize.");
    return;
  }

  // Calculate the ratio for each motor output relative to the maximum value
  float ratioL = output[0] / max;
  float ratioB = output[1] / max;
  float ratioR = output[2] / max;

  // Set PWM values for each motor and update the direction based on the ratio
  analogWrite(motorL, (int)(constrain(180 * abs(ratioL), 0, 180)));
  digitalWrite(enableL, ratioL > 0 ? HIGH : LOW);

  analogWrite(motorR, (int)(constrain(180 * abs(ratioB), 0, 180)));
  digitalWrite(enableR, ratioB > 0 ? HIGH : LOW);

  analogWrite(motorB, (int)(constrain(180 * abs(ratioR), 0, 180)));
  digitalWrite(enableB, ratioR > 0 ? HIGH : LOW);
}

int a = 0;

/**
 * Continuously checks for incoming serial data.
 * If an incoming byte is available, it reads the byte.
 * If the byte is 'S', it sets a specific input vector and calls the movement function
 * to perform the movement once, ensuring the action is executed only once by setting a flag.
 */
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
