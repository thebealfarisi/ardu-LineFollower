#ifndef rizkimikon
#define rizkimikon

#include <Arduino.h>


//command

#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4

//motor

#define RIGHT 3
#define LEFT 2



class mikon {

	
	private:
	
	public:
	mikon();
	~mikon();
	void motor(int nMotor, int command, int speed);
	
	
	};
class cekSensor{
	public:
	cekSensor();
	~cekSensor();
	void value();
	
};	
#endif