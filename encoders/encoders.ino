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

volatile int encoderCount1 = 0;  // Variable to store the pulse count
volatile int encoderCount2 = 0;  // Variable to store the pulse count
volatile int encoderCount3 = 0;  // Variable to store the pulse count

// ISR for encoder signal A
void encoderISR1() {
  if (digitalRead(inputB1) == HIGH) {  // Check the state of channel B
    encoderCount1++;
  } else {
    encoderCount1--;
  }
}

// ISR for encoder signal A
void encoderISR2() {
  if (digitalRead(inputB2) == HIGH) {  // Check the state of channel B
    encoderCount2++;
  } else {
    encoderCount2--;
  }
}

// ISR for encoder signal A
void encoderISR3() {
  if (digitalRead(inputB3) == HIGH) {  // Check the state of channel B
    encoderCount3++;
  } else {
    encoderCount3--;
  }
}

void setup ()  {

  Serial.begin(9600);

  pinMode(inputB1, INPUT_PULLUP);
  pinMode(inputB2, INPUT_PULLUP);
  pinMode(inputB3, INPUT_PULLUP);

  
  // Set up interrupt on pin 2
  pinMode(inputA1, INPUT_PULLUP);  // Enable internal pull-up resistor
  attachInterrupt(digitalPinToInterrupt(inputA1), encoderISR1, RISING);  // Trigger on rising edge

  // Set up interrupt on pin 2
  pinMode(inputA2, INPUT_PULLUP);  // Enable internal pull-up resistor
  attachInterrupt(digitalPinToInterrupt(inputA2), encoderISR2, RISING);  // Trigger on rising edge

  // Set up interrupt on pin 2
  pinMode(inputA3, INPUT_PULLUP);  // Enable internal pull-up resistor
  attachInterrupt(digitalPinToInterrupt(inputA3), encoderISR3, RISING);  // Trigger on rising edge
  
  
}

void loop () {
  // Calculate the total distance traveled
  float distanceTraveled1 = encoderCount1 * distancePerPulse;
  float distanceTraveled2 = encoderCount2 * distancePerPulse;
  float distanceTraveled3 = encoderCount3 * distancePerPulse;
  // Print the encoder count and the distance
  Serial.print("encoderCount1:");
  Serial.println(encoderCount1);
  Serial.print("encoderCount2:");
  Serial.println(encoderCount2);
  Serial.print("encoderCount3:");
  Serial.println(encoderCount3);

  delay(100);  // Adjust delay as needed
}
