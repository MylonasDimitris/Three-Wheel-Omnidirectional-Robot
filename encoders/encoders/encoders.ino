#define outputA 9
#define outputB 10
#define motor 5

int counter = 0;
int aState;
int aLastState;

unsigned long previousMillis = 0;  // Stores the last time an action was performed
const long interval = 5000;  // 5 seconds interval

void setup ()  {
  pinMode (outputA,INPUT);
  pinMode (outputB,INPUT);

  Serial.begin (9600);

  aLastState = digitalRead(outputA);

  analogWrite(motor, 180);
  previousMillis = millis();
}

void loop () {
  

  unsigned long currentMillis = millis();  // Get the current time

  // Check if 5 seconds have passed
  if (currentMillis - previousMillis >= interval) {
    // If 5 seconds have passed, write 0 to the analog pin
    analogWrite(motor, 0);
  }
  aState = digitalRead(outputA);
  if (aState != aLastState) {
   if (digitalRead (outputB) != aState) {
      counter++;
     } else {
       counter --;
     }
     Serial.print("Position: ");
     Serial.println(counter);
     
}

 aLastState = aState;
}