#include "functions.h"
#include <Arduino.h>
#include "pins.h"
#include <Wire.h>
#include <MPU6050_light.h>

MPU6050 mpu(Wire);

int MPUInit() {
  Wire.begin();
  mpu.begin();
  mpu.upsideDownMounting = true; // uncomment this line if the MPU6050 is mounted upside-down
  mpu.calcOffsets(); // gyro and accelero
  return 1;
}


float calculations[3][3] = {
  {1.0 / 3.0, -1.0 / sqrt(3.0), 1.0 / 3.0},
  {-2.0 / 3.0, 0.0, 1.0 / 3.0},
  {1.0 / 3.0, 1.0 / sqrt(3.0), 1.0 / 3.0}
};

float output[3] = {0.0};




volatile int encoderCount1 = 0;  // Variable to store the pulse count
volatile int encoderCount2 = 0;  // Variable to store the pulse count
volatile int encoderCount3 = 0;  // Variable to store the pulse count

// Interrupt Service Routine for encoder signal A
/**
 * This ISR handles the encoder signal for channel A.
 * It increments or decrements the encoder count based on the state of channel B.
 */
void encoderISR1() {
  if (digitalRead(inputB1) == HIGH) {  // Channel B is HIGH, increment count
    encoderCount1++;
  } else {  // Channel B is LOW, decrement count
    encoderCount1--;
  }
}


void encoderISR2() {
  if (digitalRead(inputB2) == HIGH) {  // Channel B is HIGH, increment count
    encoderCount2++;
  } else {  // Channel B is LOW, decrement count
    encoderCount2--;
  }
}


void encoderISR3() {
  if (digitalRead(inputB3) == HIGH) {  // Channel B is HIGH, increment count
    encoderCount3++;
  } else {  // Channel B is LOW, decrement count
    encoderCount3--;
  }
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

  if (max == 0.0){
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
  stop();
  float input[3] = {0.0};
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


void movementFor(float input[], int givenDistance){
  mpu.update();

  float rotz = mpu.getAngleZ();
  float tempz = rotz;



  encoderCount1 = 0;
  encoderCount2 = 0;
  encoderCount3 = 0;

  float distanceTraveled1 = encoderCount1 * distancePerPulse;
  float distanceTraveled2 = encoderCount2 * distancePerPulse;
  float distanceTraveled3 = encoderCount3* distancePerPulse;

  float distanceX = distanceTraveled1 - (distanceTraveled2 / 2) + (distanceTraveled3 * sqrt(3)) / 2;
  float distanceY = - (distanceTraveled2 / 2) - ((distanceTraveled3 * sqrt(3)) / 2);

  float distance = sqrt(distanceX * distanceX + distanceY * distanceY);

  while(distance < givenDistance){

    //Get the updated gyroscope data
    mpu.update();

    //Read the current rotation around the z-axis
    rotz = mpu.getAngleZ();

    //adjust the rotation
    input[2] = (tempz - rotz) / 20;

    distanceTraveled1 = encoderCount1 * distancePerPulse;
    distanceTraveled2 = encoderCount2 * distancePerPulse;
    distanceTraveled3 = encoderCount3 * distancePerPulse;

    distanceX = distanceTraveled1 - (distanceTraveled2 / 2) + (distanceTraveled3 * sqrt(3)) / 2;
    distanceY = - (distanceTraveled2 / 2) - ((distanceTraveled3 * sqrt(3)) / 2);

    distance = sqrt(distanceX * distanceX + distanceY * distanceY);
    movement(input);
  }
  stop();
  return 1;
}
