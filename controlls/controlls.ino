/*By inputting the desired vector at the array
 input the program outputs the desired ration 
 in which the motors should run in order for
  the robot to move in the correct direction.*/

//Libraries for gyroscope
#include "Wire.h"
#include <MPU6050_light.h>

// Motors' encoders pins
#define motorAchanA 9
#define motorAchanA 10

// Motors' PWM pins
#define motorL 3
#define motorB 6
#define motorR 5

// Motors' Direction pins
#define enableL 2
#define enableB 7
#define enableR 4

MPU6050 mpu(Wire);
unsigned long timer = 0;


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
  Wire.begin();

  Serial.print("Starting");

  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  // mpu.upsideDownMounting = true; // uncomment this line if the MPU6050 is mounted upside-down
  //mpu.calcOffsets(); // gyro and accelero
  Serial.println("Done!\n");

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


/**
 * Stops all motors by setting the PWM values to zero.
 */
void stop() {
  // Set PWM values for all motors to zero to stop them
  analogWrite(motorL, 0);
  analogWrite(motorR, 0);
  analogWrite(motorB, 0);
}


/**
 * Rotates the robot by a specified number of degrees.
 *
 * @param degrees The number of degrees to rotate the robot by.
 */
void rotate(float degrees) {

  // Store the initial orientation of the robot
  mpu.update();
  float initialRotation = mpu.getAngleZ();

  // Set the input forces to zero
  input[0] = 0.0; input[1] = 0.0;

  // If the rotation is clockwise
  if (degrees > 0.0) {
    // Set the input force to rotate clockwise
    input[2] = 1.0;

    // Rotate until the robot reaches the desired orientation
    do {
      // Update the gyroscope data
      mpu.update();
      // Move the robot based on the input forces
      movement(input);
      // Print a message to indicate that the robot is rotating
      Serial.println("BRUH");
    } 
    // Check if the robot has reached the desired orientation
    while (abs(abs(mpu.getAngleZ()) - abs(initialRotation)) < abs(degrees));
  }
  // If the rotation is counter-clockwise
  else {
    // Set the input force to rotate counter-clockwise
    input[2] = -1.0;

    // Rotate until the robot reaches the desired orientation
    do {
      // Update the gyroscope data
      mpu.update();
      // Move the robot based on the input forces
      movement(input);
    } 
    // Check if the robot has reached the desired orientation
    while (abs(abs(initialRotation) - abs(mpu.getAngleZ())) < abs(degrees));

  }
  // Stop the motors
  input[2] = 0.0;
  stop();

}



int a = 0;

/**
 * Continuously checks for incoming serial data.
 * If an incoming byte is available, it reads the byte.
 * If the byte is 'S', it sets a specific input vector and calls the movement function
 * to perform the movement once, ensuring the action is executed only once by setting a flag.
 */
void loop() {
  // Get gyroscope data
  mpu.update();

  float rotx = mpu.getAngleX();
  float roty = mpu.getAngleY();
  float rotz = mpu.getAngleZ();

  // Print the gyroscope data for debugging purposes
  Serial.print("rotx:");
  Serial.println(rotx);
  Serial.print("roty:");
  Serial.println(roty);
  Serial.print("rotz:");
  Serial.println(rotz);
  delay(10);

  // Check if there is any incoming serial data
  if (Serial.available() > 0) {
    // Read the incoming byte
    char incomingByte = Serial.read();

    // Read the current rotation around the z-axis
    rotz = mpu.getAngleZ();

    // If the incoming byte is 'S', perform the movement
    if (incomingByte == 'S') {
      float tempz = rotz;

      delay(100);
      
      // Set the input vector for the movement function
      input[0] = 0.0; input[1] = 1.0; input[2] = 0.0;

      // Call the movement function to perform the movement
      movement(input);

      // Perform the movement in a loop
      while (a == 0) {
        // Get the updated gyroscope data
        mpu.update();

        // Read the current rotation around the z-axis
        rotz = mpu.getAngleZ();

        if (rotz - tempz){
          input[2] = (rotz - tempz) / 10;
          movement(input);
        }
        else{
          input[2] = (tempz - rotz) / 10;
          movement(input);
        }

        
      }
    }
  }
}
