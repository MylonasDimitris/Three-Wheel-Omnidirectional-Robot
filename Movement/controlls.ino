//Libraries
#include "functions.h"
#include "pins.h"
#include <EnableInterrupt.h>


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
  enableInterrupt(inputA1, encoderISR1, RISING);  // Trigger on rising edge

  // Set up interrupt on pin 19
  pinMode(inputA2, INPUT_PULLUP);  // Enable internal pull-up resistor
  enableInterrupt(inputA2, encoderISR2, RISING);  // Trigger on rising edge

  // Set up interrupt on pin 20
  pinMode(inputA3, INPUT_PULLUP);  // Enable internal pull-up resistor
  enableInterrupt(inputA3, encoderISR3, RISING);  // Trigger on rising edge


  // Start the serial communication at a baud rate of 9600
  Serial.begin(9600);


  Serial.print("Starting");

  int status = MPUInit();

  if (status == 1){
    Serial.println("MPU INITIALIZED");
  }
  else{
    Serial.println("MPU FAILED");
  }
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
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
  // Check if there is any incoming serial data
  if (Serial.available() > 0) {
    // Read the incoming byte
    char incomingByte = Serial.read();

    // If the incoming byte is 'S', perform the movement
    if (incomingByte == 'S') {
      delay(100);
      
      // Set the input vector for the movement function
      input[0] = 0.0; input[1] = 1.0; input[2] = 0.0;


      // Perform the movement
      while (a == 0) {
        movementFor(input, 30);

        a = 1;
        
      }
    }
  }
}
