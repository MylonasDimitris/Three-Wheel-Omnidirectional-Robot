#ifndef PINS_H
#define PINS_H

// Motors' PWM pins
#define motorL 9
#define motorB 6
#define motorR 5

// Motors' Direction pins
#define enableL 8
#define enableB 7
#define enableR 4

// Encoder pins
#define inputA1 3
#define inputB1 2
#define inputA2 18
#define inputB2 19
#define inputA3 21
#define inputB3 20


#define PPR 360  // Pulse Per Rotation
#define PI 3.14159
#define wheelRadius 5
#define distancePerPulse (2 * PI * wheelRadius) / PPR

#endif