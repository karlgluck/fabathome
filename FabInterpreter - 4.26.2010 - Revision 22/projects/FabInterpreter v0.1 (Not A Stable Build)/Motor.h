#ifndef MOTOR_H
#define MOTOR_H

#include "util/Util.h"
using namespace System::Runtime::InteropServices; //Needed for DLLs
//PIC-SERVO base header files
#include "sio_util.h"
#include "nmccom.h"
#include "picservo.h"
#include "picio.h"
//PIC-SERVO path header files
#include "picServoPath.h"

class Motor
{
//CONSTANTS/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	string NAME;
	byte ADDRESS; //The number of the motor port that this motor is plugged in.
     double COUNTS_PER_U; //Number of counts for 1 U of motion.
	double TICKS_PER_SECOND; //Number of ticks that a motor makes per second.
     double MIN_COMMANDED_POSITION;
	double MAX_COMMANDED_POSITION;
	double MIN_COMMANDED_VELOCITY;
	double MAX_COMMANDED_VELOCITY;
	double MIN_COMMANDED_ACCELERATION;
	double MAX_COMMANDED_ACCELERATION;
	short KP, KD, KI, IL, EL;
	byte OL, CL, SR, DB;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
     double commandedPosition;
     bool initialized;
     bool isReversed;

     //Tell hardware to send updated data for this motor to the DLL.
     void updateData(void);

public:
	Motor(void);

	Motor(string name,const byte& address, const double countsPerU, const double ticksPerSecond, short kp, short kd, short ki, short il, byte ol, byte cl, short int el, byte sr, byte db);
	
	//Returns: The current position of the motor.  The unit of position is U.
	double getPosition(void);
	
	//Returns: The most recent position that the motor was commanded to move to by moveAbsolute, moveRelative, and resetPosition functions.  The unit of position is U.
	double getCommandedPosition(void);
	
	//Move to a position at a specified velocity and acceleration.
	//The unit of position is U, the unit of velocity is U/second, and the unit of acceleration is U/(second^2).
	//Requires MIN_COMMANDED_POSITION <= position <= MAX_COMMANDED_POSITION, MIN_COMMANDED_VELOCITY <= velocity <= MAX_COMMANDED_VELOCITY, MIN_COMMANDED_ACCELERATION <= acceleration <= MAX_COMMANDED_ACCELERATION
	//Returns true iff command was issued successfully.
	bool moveAbsolute(double position, double velocity, double acceleration);
	
	//Move relative to the current position at a specified velocity and acceleration.
	//Returns true iff command was issued successfully.
	bool moveRelative(double positionDelta, double velocity, double acceleration);

     //Returns: True iff motor is currently moving.
     bool moving(void);

	//This function returns as soon as possible after the motor has finished moving.
	void waitMove(void);
	
	bool stop(void);
	
     //Set whether this motor is reversed.  If a motor is reversed, movement commands will cause the motor to move in the opposite direction.
     void setReversed(bool);

	bool setGroup(byte groupAddress, bool isLeader);
	
     //The motors current position is considered to be 0 and the motors commanded position is set to 0.
	bool resetPosition(void);
	
	//Initialize the motor.
	//Returns: "" if successful or an error message if failed.
     string initialize(void);
	
     byte getAddress(void);

     double getCountsPerU(void);

     //Perform cleanup when done using motor.
	//Returns: True iff successful.
	bool cleanUp(void);

     //The destructor.
     ~Motor(void);

};

#endif //ndef MOTOR_H