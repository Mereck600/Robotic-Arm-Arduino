// #include <Servo.h>

// // Servo control pins
// int onePin = 12;
// int twoPin = 13;
// int threePin = 11;
// int microPin = 10;

// // Button input pins
// int prevMotorPin = 2;
// int nextMotorPin = 3;
// int adjustPin = 5;
// int revPin = 4;

// // Servo objects
// Servo servos[4];

// // Servo positions
// int servoPositions[4] = {90, 90, 90, 90};

// // Selected motor index
// int currentMotor = 0;


#include <Servo.h>

// Servo control pins
int onePin = 12;
int twoPin = 13;
int threePin = 11;
int microPin = 10;

// Button input pins
int prevMotorPin = 2;
int nextMotorPin = 3;
int adjustPin = 5;
int revPin = 4;

// Servo objects
Servo servos[4];

// Servo positions
int servoPositions[4] = {20, 20, 20, 20};

// Selected motor index
int currentMotor = 0;

// Direction and debounce tracking
bool revMode = false;
bool prevRevState = LOW;
bool prevNextState = LOW;
bool prevPrevState = LOW;

void setup() {
  Serial.begin(9600);

  servos[0].attach(onePin);
  servos[1].attach(twoPin);
  servos[2].attach(threePin);
  servos[3].attach(microPin);

  pinMode(prevMotorPin, INPUT);
  pinMode(nextMotorPin, INPUT);
  pinMode(adjustPin, INPUT);
  pinMode(revPin, INPUT);
}

void loop() {
  // Read button states
  //servos[0]
  bool prevMotorPressed = digitalRead(prevMotorPin);
  bool nextMotorPressed = digitalRead(nextMotorPin);
  bool adjustPressed = digitalRead(adjustPin);
  bool revCurrent = digitalRead(revPin);

  // Reverse toggle
  if (revCurrent == HIGH && prevRevState == LOW) {
    Serial.println("Toggled reverse mode.");
    revMode = !revMode;
    delay(50);
  }
  prevRevState = revCurrent;

  // Previous motor
  if (prevMotorPressed == HIGH && prevPrevState == LOW) {
    currentMotor = (currentMotor - 1 + 4) % 4;
    Serial.print("Selected motor: ");
    Serial.println(currentMotor);
    delay(200); // Slightly longer debounce for selection
  }
  prevPrevState = prevMotorPressed;

  // Next motor
  if (nextMotorPressed == HIGH && prevNextState == LOW) {
    currentMotor = (currentMotor + 1) % 4;
    Serial.print("Selected motor: ");
    Serial.println(currentMotor);
    delay(200);
  }
  prevNextState = nextMotorPressed;

  // Adjust angle continuously while button is held
  if (adjustPressed == HIGH) {
    servoPositions[currentMotor] += revMode ? -1 : 1;
    servoPositions[currentMotor] = constrain(servoPositions[currentMotor], 0, 180);
    servos[currentMotor].write(servoPositions[currentMotor]);
    
    Serial.print("Adjusting servo ");
    Serial.print(currentMotor);
    Serial.print(" to angle: ");
    Serial.println(servoPositions[currentMotor]);
    
    delay(70); // Adjust this delay to control speed
  }
}

