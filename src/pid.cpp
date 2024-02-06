#include "vex.h"

//pid control for autonomous

//pid settings

double kP = 0.2;
double kI = 0.005;
double kD =0.1;

double turnkP = 0.0;
double turnkI = 0.0;
double turnkD =0.0;


// autonomous settings
int desiredValue = 0;

int error; //sensorValue - DesiredValue : Position
int prevError = 0; //position 20 msec ago
int derivative; // error - previous error :  speed control
int totalError = 0; //total error = totalerror + error

int desiredTurnValue = 0;

int turnError; //sensorValue - DesiredValue : Position
int turnPrevError = 0; //position 20 msec ago
int turnDerivative; // error - previous error :  speed control
int turnTotalError = 0; //total error = totalerror + error


bool resetDriveSensors = true;

int drivePID(int avance, int vuelta, bool enableDrivePID)
{
  desiredValue = avance;
  desiredTurnValue = vuelta;

  while(enableDrivePID)
  {
    if (resetDriveSensors)
    {
      resetDriveSensors = false;
      LeftDriveSmart.setPosition(0,degrees);
      RightDriveSmart.setPosition(0,degrees);
    }
    int leftMotorPosition = LeftDriveSmart.position(degrees);
    int rightMotorPosition = RightDriveSmart.position(degrees);
    ///////////////////////////////////
    //lateral movement pid
    ///////////////////////////////////
    int averagePosition = (leftMotorPosition+rightMotorPosition)/2;

    //Proportional
    error = averagePosition - desiredValue;
    //derivative
    derivative = error - prevError;

    //Integral (position in time)
    totalError += error;

    double lateralMotorPower = (error * kP + derivative * kD + totalError * kI);

    ///////////////////////////////////
    //turning movement pid
    ///////////////////////////////////
      int turnDifference = (leftMotorPosition-rightMotorPosition);

    //Proportional
    turnError = turnDifference - desiredTurnValue;
    //derivative
    turnDerivative = turnError - turnPrevError;

    //Integral (position in time)
    turnTotalError += turnError;

    double turnMotorPower = (error * turnkP + derivative * turnkI + totalError * turnkD);

    LeftDriveSmart.spin(forward,lateralMotorPower+turnMotorPower,voltageUnits::volt);
    RightDriveSmart.spin(forward,lateralMotorPower+turnMotorPower,voltageUnits::volt);
    
    prevError = error;
    turnPrevError = turnError;
    vex::task::sleep(20);

  }
  return 1;
}