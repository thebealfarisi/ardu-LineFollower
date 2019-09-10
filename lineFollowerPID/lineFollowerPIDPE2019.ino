#include <RobotikPE2019.h>

const int DELAY = 10;

mikon kiri;
mikon kanan;

int thresholdValues[] = {800, 800, 200, 800, 800};
//int readValues[5];
int digitalValues[5];

float pTerm = 0, iTerm = 0, dTerm = 0;

int error = 0;
int prevError = 0;

float kp = 15;
float ki = 0;
float kd = 15;

float output = 0;
int integral = 0, derivative = 0;

const int motor1Speed = 85;
const int motor2Speed = 85;

int motor1NewSpeed = 0, motor2NewSpeed = 0;

int counter = 0;
int errorT[]={
  -3, // kanan
  3
};

void setup() {
  dataInit();
  Serial.begin(9600);  
}

void loop() {
  readValue();
  getError();
  pidCalculation();
  setMotorSpeed();
  Serial.println(error);
  delay(DELAY);
  
}

void dataInit() {
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
}

void readValue() {
  digitalValues[0] = valueConversion(thresholdValues[0], analogRead(A1));
  digitalValues[1] = valueConversion(thresholdValues[1], analogRead(A2));
  digitalValues[2] = valueConversion(thresholdValues[2], analogRead(A3));
  digitalValues[3] = valueConversion(thresholdValues[3], analogRead(A4));
  digitalValues[4] = valueConversion(thresholdValues[4], analogRead(A5));
}

void getError() {
  // 1 INPUT
  if (digitalValues[0] == 1 && digitalValues[1] == 0 && digitalValues[2] == 0 && digitalValues[3] == 0 && digitalValues[4] == 0) {    
    error = -2;
  } else if (digitalValues[0] == 0 && digitalValues[1] == 1 && digitalValues[2] == 0 && digitalValues[3] == 0 && digitalValues[4] == 0) {
    error = -1;
  } else if (digitalValues[0] == 0 && digitalValues[1] == 0 && digitalValues[2] == 1 && digitalValues[3] == 0 && digitalValues[4] == 0) {
    error = 0;
  } else if (digitalValues[0] == 0 && digitalValues[1] == 0 && digitalValues[2] == 0 && digitalValues[3] == 1 && digitalValues[4] == 0) {
    error = 1;
  } else if (digitalValues[0] == 0 && digitalValues[1] == 0 && digitalValues[2] == 0 && digitalValues[3] == 0 && digitalValues[4] == 1) {
    error = 2;
  } 
  // 2 INPUT
  else if (digitalValues[0] == 1 && digitalValues[1] == 1 && digitalValues[2] == 0 && digitalValues[3] == 0 && digitalValues[4] == 0) {
    error = -2;
  } else if (digitalValues[0] == 0 && digitalValues[1] == 1 && digitalValues[2] == 1 && digitalValues[3] == 0 && digitalValues[4] == 0) {
    error = -1;
  } else if (digitalValues[0] == 0 && digitalValues[1] == 0 && digitalValues[2] == 1 && digitalValues[3] == 1 && digitalValues[4] == 0) {
    error = 1;
  } else if (digitalValues[0] == 0 && digitalValues[1] == 0 && digitalValues[2] == 0 && digitalValues[3] == 1 && digitalValues[4] == 1) {
    error = 2;
  } 
  // 3 INPUT
  else if (digitalValues[0] == 1 && digitalValues[1] == 1 && digitalValues[2] == 1 && digitalValues[3] == 0 && digitalValues[4] == 0) {
    error = -1;
  } else if (digitalValues[0] == 0 && digitalValues[1] == 1 && digitalValues[2] == 1 && digitalValues[3] == 1 && digitalValues[4] == 0) {
    error = 0;
  } else if (digitalValues[0] == 0 && digitalValues[1] == 0 && digitalValues[2] == 1 && digitalValues[3] == 1 && digitalValues[4] == 1) {
    error = 1;
  } 
  // 4 INPUT
  else if (digitalValues[0] == 1 && digitalValues[1] == 1 && digitalValues[2] == 1 && digitalValues[3] == 1 && digitalValues[4] == 0) {
    error = -1;
  } else if (digitalValues[0] == 0 && digitalValues[1] == 1 && digitalValues[2] == 1 && digitalValues[3] == 1 && digitalValues[4] == 1) {
    error = 1;
  } else if (digitalValues[0] == 1 && digitalValues[1] == 1 && digitalValues[2] == 1 && digitalValues[3] == 1 && digitalValues[4] == 1) {
//    if (error < 0) {
//      error = -3; /// kanan
//    } else {
//      error = 3; /// kiri
//    }

      error = errorT[counter];
      counter++;
      counter = counter % (sizeof(errorT)/sizeof(int));
      
  } else if (digitalValues[0] == 0 && digitalValues[1] == 0 && digitalValues[2] == 0 && digitalValues[3] == 0 && digitalValues[4] == 0) {
    error = prevError;
  } else {
    error = prevError;
  }
  
}

int valueConversion(int threshold, int sensorValue) {
  int result = 0;
  if (threshold > sensorValue) {
    result = 1;
  }

  return result;
}

void pidCalculation() {
  pTerm = kp * error;
  integral = error + prevError;
  iTerm = ki * integral;
  derivative = error - prevError;
  dTerm = kd * derivative;
  output = pTerm + iTerm + dTerm;
  prevError = error;
}

void setMotorSpeed() {

  motor1NewSpeed = motor1Speed - output; // motor kiri

  if(motor1NewSpeed > 255){motor1NewSpeed = 255;}
  else if(motor1NewSpeed < 0){motor1NewSpeed = 0;}
  
  motor2NewSpeed = motor2Speed + output; // motor kanan

  if(motor2NewSpeed > 255){motor2NewSpeed = 255;}
  else if(motor2NewSpeed < 0){motor2NewSpeed = 0;}
  
  if (error == 0) {
    
    kiri.motor(LEFT, FORWARD, motor1NewSpeed);
    kanan.motor(RIGHT, FORWARD, motor2NewSpeed);
    
  } else if (error > 0) {

    kiri.motor(LEFT, FORWARD, motor1NewSpeed);
    kanan.motor(RIGHT, FORWARD, motor2NewSpeed);
    
  } else if (error < 0) {
    
    kiri.motor(LEFT, FORWARD, motor1NewSpeed);
    kanan.motor(RIGHT, FORWARD, motor2NewSpeed);
    
  }
}
