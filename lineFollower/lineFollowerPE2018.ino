/**
 * Global constanta to store total input sensor and pwm value
 */
#include <RobotikPE2019.h>
 
#define INPUT_SENSOR 5
#define PWM_VALUE 150
#define MAX_PWM 255

mikon kiri;
mikon kanan;

int thresholdValues[] = {800, 800, 200, 800, 800}; 

/*
 * Pre-define declared method
 */
int getState(void);
void handleState(int state);

void setup() {
  /*
   * Declare to Serial output to see the result via arduino serial monitor
   * with default baud rate 9600
   */
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  Serial.begin(9600);

  /*
   * Setting pinmode output pin 4, 5, 6, and 7
   * Pin 5 and 6 are used for PWM
   * Left motor use pin 4 and 5
   * Right motor use pin 6 and 7
   */
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
   
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);  
}

void loop() {
  //determine state of sensor array
  int state = getState();

  //determined sensor array convert to movement
  handleState(state);

}

/*
 * Method declaration to ease the programming process
 */

/*
 * Method to generate input state for DC motors based on sensor condition
 * return type is integer
 */
int getState(void) {
  //declare return variable
  int result = 0;
  
  //input sensor declaration
  int sensorVal1 = analogRead(A1);
  int sensorVal2 = analogRead(A2);
  int sensorVal3 = analogRead(A3);
  int sensorVal4 = analogRead(A4);
  int sensorVal5 = analogRead(A5);

  //store input sensor to array default 6 input, can be changed based on needed
  int sensorArray[INPUT_SENSOR] = {
    sensorVal1, sensorVal2,
    sensorVal3, sensorVal4,
    sensorVal5
  };

  //array to store analog to digital value conversion with int 500 threshold based number of sensor
  int digitalArray[INPUT_SENSOR];

  //looping sensor value to store result of convertion
  for (int i = 0; i < INPUT_SENSOR; i++) {
    //conditional statement with threshold limit
    //if data <= 500, convert to 0
    //if data > 500, conver to 1
    if (sensorArray[i] <= thresholdValues[i]) {
      digitalArray[i] = 0;
    } else {
      digitalArray[i] = 1;
    }
  }

  //array of differential sensor value (x+1 compare to x) to define motor movement
  int difSensorArray[INPUT_SENSOR - 1];

  //looping to store different data between member of array to define motor movement
  for (int i = 0; i < INPUT_SENSOR - 1; i++) {
    difSensorArray[i] = digitalArray[i + 1] - digitalArray[i];
  }

  //boolean value to define motor movement
  bool to_left = false;
  bool to_right = false;

  //looping difSensor array to determine final value of the boolean
  //if difSensor contains 1, to_right = true
  //if difSensor contains -1, to_left = true;
  for (int i = 0; i < INPUT_SENSOR - 1; i++) {
    if (difSensorArray[i] == 1) {
      to_right = true;
    } else if (difSensorArray[i] == -1) {
      to_left = true;
    }
  }

  //final result determination to determine DC motor movement based on value
  //0 = move_straight, 1 = trun_right, 2 = turn_left, 3 = turn_back, 4 = back
  //for value 3 and value 4 is determined by first value of difSensorArray
  //if first value = 0, value = 3
  //if first value = 1, value = 4
  if (to_right == true && to_left == true) {
    result = 0;
  } else if (to_right == true && to_left == false) {
    result = 1;
  } else if (to_right == false && to_left == true) {
    result = 2;
  } else {
    if (difSensorArray[0] == 0) {
      result = 3;
    } else if (difSensorArray[0] == 1) {
      result = 4;
    }
  }

  return result;
}

/*
 * Method to handle DC motor movement based on input state
 */
void handleState(int state) {
  if (state == 0) {
    //move straight
    digitalWrite(4, LOW); analogWrite(5, PWM_VALUE);
    digitalWrite(7, LOW); analogWrite(6, PWM_VALUE);
  } else if (state == 1) {
    //move right
    digitalWrite(4, LOW); analogWrite(5, PWM_VALUE);
    digitalWrite(7, HIGH); analogWrite(6, MAX_PWM - PWM_VALUE);
  } else if (state == 2) {
    //move left
    digitalWrite(4, HIGH); analogWrite(5, MAX_PWM - PWM_VALUE);
    digitalWrite(7, LOW); analogWrite(6, PWM_VALUE);
  } else if (state == 3) {
    //move back
    digitalWrite(4, HIGH); analogWrite(5, MAX_PWM - PWM_VALUE);
    digitalWrite(7, HIGH); analogWrite(6, MAX_PWM - PWM_VALUE);
  } else if (state == 4) {
    //move stop
    digitalWrite(4, LOW); digitalWrite(5, LOW);
    digitalWrite(7, LOW); digitalWrite(6, LOW);
  } else {
    //circle around
    digitalWrite(4, LOW); analogWrite(5, HIGH);
    digitalWrite(7, HIGH); analogWrite(6, LOW);
  }
}
