#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <MPU6050_light.h>
#include <Wire.h>



void movement(float input[]); 
void movementFor(float input[], int givenDistance);
int MPUInit();
void encoderISR1();
void encoderISR2();
void encoderISR3();
void stop();
void rotate(float degrees);

#endif
