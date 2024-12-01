#define inputA 2
#define inputB 3
#define PPR 100  // Pulse Per Rotation
#define PI 3.14159
#define wheelRadius 5
#define distancePerPulse (2 * PI * wheelRadius) / PPR

volatile int encoderCount = 0;  // Variable to store the pulse count

// ISR for encoder signal A
void encoderISR() {
  if (digitalRead(inputB) == HIGH) {  // Check the state of channel B
    encoderCount++;
  } else {
    encoderCount--;
  }
}

void setup ()  {

  Serial.begin(9600);

  pinMode(inputB, INPUT_PULLUP);
  
  // Set up interrupt on pin 2
  pinMode(inputA, INPUT_PULLUP);  // Enable internal pull-up resistor
  attachInterrupt(digitalPinToInterrupt(inputA), encoderISR, RISING);  // Trigger on rising edge
  
  // Using Signal B for direction:
  pinMode(inputB, INPUT_PULLUP);  //Direction detection
  
}

void loop () {
  // Calculate the total distance traveled
  float distanceTraveled = encoderCount * distancePerPulse;

  // Print the encoder count and the distance
  Serial.print("Encoder Count: ");
  Serial.print(encoderCount);
  Serial.print(" | Distance Traveled: ");
  Serial.print(distanceTraveled);
  Serial.println(" cm");

  delay(100);  // Adjust delay as needed
}
