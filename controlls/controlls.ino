/*By inputting the desired vector at the array
 input the program outputs the desired ration 
 in which the motors should run in order for
  the robot to move in the correct direction.*/

//Libraries for gyroscope
#include "Wire.h"
#include <MPU6050_light.h>
#include "functions.h"
#include "pins.h"


MPU6050 mpu(Wire);
unsigned long timer = 0;

float input[3] = {0.0};  // {fx, fy, fw}


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


  pinMode(inputB1, INPUT_PULLUP);
  pinMode(inputB2, INPUT_PULLUP);
  pinMode(inputB3, INPUT_PULLUP);

  
  // Set up interrupt on pin 2
  pinMode(inputA1, INPUT_PULLUP);  // Enable internal pull-up resistor
  attachInterrupt(digitalPinToInterrupt(inputA1), encoderISR1, RISING);  // Trigger on rising edge

  // Set up interrupt on pin 19
  pinMode(inputA2, INPUT_PULLUP);  // Enable internal pull-up resistor
  attachInterrupt(digitalPinToInterrupt(inputA2), encoderISR2, RISING);  // Trigger on rising edge

  // Set up interrupt on pin 20
  pinMode(inputA3, INPUT_PULLUP);  // Enable internal pull-up resistor
  attachInterrupt(digitalPinToInterrupt(inputA3), encoderISR3, RISING);  // Trigger on rising edge


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


        input[2] = (tempz - rotz) / 20;
        movement(input);


        
      }
    }
  }
}
