#include <Stepper.h>

const int stepsPerRevolution = 2048;  
const int sensorPin = 33;
const int laserPin = 2;
const int lightThreshold = 10;
const int numAvgMeasurements = 20;

// photoresistor ADC readings
int lightVal;
int initVal;

// shows total steps taken up to a certain point, not an indicator of actual location
int currentStep = 0;

// ULN2003 Motor Driver pins
#define IN1 23
#define IN2 22
#define IN3 18
#define IN4 19

// initialize the stepper library
Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);

void setup() {
  // Turn on the laser diode and set the stepper motor speed to 5rpm
  pinMode(laserPin, OUTPUT);
  digitalWrite(laserPin, HIGH);
  myStepper.setSpeed(5);

  // initialize the serial port
  Serial.begin(115200);
  Serial.println("Starting laser/photoresistor scan...");
}

void loop() {
  // grab an initial photoresistor reading to start the upcoming loop
  initVal = readAverageIntensity();
  lightVal = initVal;

  Serial.print("Initial light value: ");
  Serial.println(initVal);

  // rotate counterclockwise until threshold is reached (in other words, rotate until blade fully blocks the photoresistor)
  // collect photoresistor ADC averages for each step
  while (lightVal > lightThreshold) {
    turnBlade(-1);
  }

  // turn 80 steps clockwise while collecting averages
  for (int i = 0; i < 80; i++) {
    turnBlade(1);
  }
  
  // terminate loop
  while (true) {
    delay(1000);
  }
}

// averages numAvgMeasurements (ex. 20) photoresistor readings to output a more accurate value
int readAverageIntensity() {
  long sum = 0;
  for (int i = 0; i < numAvgMeasurements; i++) {
    sum += analogRead(sensorPin); // no need for extra delay as analogRead takes roughly 100 microseconds
  }

  return sum / numAvgMeasurements;
}

// sends a turning signal to the stepper motor, increments currentStep and prints out currentStep and photoresistor average
void turnBlade(int step) {
  myStepper.step(step);
  delay(200);
  currentStep++; // NOTE: currentStep shows how many total steps were taken. In the data analysis portion, I made sure to only analyze the data of the blade going down
  lightVal = readAverageIntensity();
  Serial.print(currentStep);
  Serial.print(" ");
  Serial.println(lightVal);
}
