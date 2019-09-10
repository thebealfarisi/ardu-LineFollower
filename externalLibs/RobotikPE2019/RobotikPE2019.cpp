#include "RobotikPE2019.h"
#include <Arduino.h>

#define MOTORLATCH 12
#define MOTORCLK 4
#define MOTORENABLE 7
#define MOTORDATA 8
#define MOTOR1_A 2
#define MOTOR1_B 3
#define MOTOR2_A 1
#define MOTOR2_B 4
#define MOTOR3_A 5
#define MOTOR3_B 7
#define MOTOR4_A 0
#define MOTOR4_B 6
#define MOTOR1_PWM 11
#define MOTOR2_PWM 3
#define MOTOR3_PWM 6
#define MOTOR4_PWM 5


/////////////////////////////////////////////////
//											   //
//					MOTOR					   //
//											   //
/////////////////////////////////////////////////


mikon::mikon()
{
	Serial.println("Selamat datang di Kompetisi Robot Line Follower Physics Expo 2019");
	Serial.println(" ");
	Serial.println("Ini adalah library untuk kontrol motor dan cek sensor");
	Serial.println("class untuk motor DC adalah mikon");
	Serial.println("Fungsi-fungsinya:");
	Serial.println("mikon() untuk menampilkan bantuan");
	Serial.println("motor(roda, command, speed) untuk menggerakan motor");
	Serial.println("parameter roda tersedia RIGHT atau LEFT ");
	Serial.println("parameter command tersedia FORWARD, BACKWARD, BRAKE, dan RELEASE ");
	Serial.println("parameter speed merupakan nilai analog 0-255");
	Serial.println("value() untuk cek sensor fotodioda");
	Serial.println(" ");
	
	Serial.println("SELAMAT BERSENANG-SENANG");
	

	
}

mikon::~mikon()
{
}

void shiftWrite(int output, int high_low)
{
static int latch_copy;
static int shift_register_initialized = false;
// Do the initialization on the fly,
// at the first time it is used.
if (!shift_register_initialized)
{
// Set pins for shift register to output
pinMode(MOTORLATCH, OUTPUT);
pinMode(MOTORENABLE, OUTPUT);
pinMode(MOTORDATA, OUTPUT);
pinMode(MOTORCLK, OUTPUT);
// Set pins for shift register to default value (low);
digitalWrite(MOTORDATA, LOW);
digitalWrite(MOTORLATCH, LOW);
digitalWrite(MOTORCLK, LOW);
// Enable the shift register, set Enable pin Low.
digitalWrite(MOTORENABLE, LOW);
// start with all outputs (of the shift register) low
latch_copy = 0;
shift_register_initialized = true;
}
// The defines HIGH and LOW are 1 and 0.
// So this is valid.
bitWrite(latch_copy, output, high_low);
shiftOut(MOTORDATA, MOTORCLK, MSBFIRST, latch_copy);
delayMicroseconds(5); // For safety, not really needed.
digitalWrite(MOTORLATCH, HIGH);
delayMicroseconds(5); // For safety, not really needed.
digitalWrite(MOTORLATCH, LOW);
}


void motor_output (int output, int high_low, int speed)
{
int motorPWM;
switch (output)
{
case MOTOR1_A:
case MOTOR1_B:
motorPWM = MOTOR1_PWM;
break;
case MOTOR2_A:
case MOTOR2_B:
motorPWM = MOTOR2_PWM;
break;
case MOTOR3_A:
case MOTOR3_B:
motorPWM = MOTOR3_PWM;
break;
case MOTOR4_A:
case MOTOR4_B:
motorPWM = MOTOR4_PWM;
break;
default:
speed = -3333;
break;
}if (speed != -3333)
{
shiftWrite(output, high_low);
// set PWM only if it is valid
if (speed >= 0 && speed <= 255)
{
analogWrite(motorPWM, speed);
}
}
}

void mikon::motor(int nMotor, int command, int speed)
{
int motorA, motorB;
if (nMotor >= 1 && nMotor <= 4)
{
switch (nMotor)
{
case 1:
motorA = MOTOR1_A;
motorB = MOTOR1_B;
break;
case 2:
motorA = MOTOR2_A;
motorB = MOTOR2_B;
break;
case 3:
motorA = MOTOR3_A;
motorB = MOTOR3_B;
break;
case 4:
motorA = MOTOR4_A;
motorB = MOTOR4_B;
break;
default:
break;
}
switch (command)
{
case FORWARD:
motor_output (motorA, 1, speed);

motor_output (motorB, 0, -1); // -1: no PWM set
break;
case BACKWARD:
motor_output (motorA, 0, speed);
motor_output (motorB, 1, -1); // -1: no PWM set
break;
case BRAKE:
motor_output (motorA, 0, 255); // 255: fully on.
motor_output (motorB, 0, -1); // -1: no PWM set
break;
case RELEASE:
motor_output (motorA, 0, 0); // 0: output floating.
motor_output (motorB, 0, -1); // -1: no PWM set
break;
default:
break;
}
}
}




/////////////////////////////////////////////////
//											   //
//					Sensor					   //
//											   //
/////////////////////////////////////////////////

cekSensor::cekSensor()
{
}
cekSensor::~cekSensor()
{
}

void cekSensor::value()
{
	int x1 = analogRead(A1);
	int x2 = analogRead(A2);
	int x3 = analogRead(A3);
	int x4 = analogRead(A4);
	int x5 = analogRead(A5);
	Serial.print(" || ");
	Serial.print(x1); Serial.print(" || ");
	Serial.print(x2); Serial.print(" || ");
	Serial.print(x3); Serial.print(" || ");
	Serial.print(x4); Serial.print(" || ");
	Serial.print(x5); Serial.print(" || ");
	Serial.println(" ");
}
