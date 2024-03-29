#include "vex.h"
#include "funcionesautonomas.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);

motor brazoRecogedor = motor(PORT15, ratio36_1, false);
motor catapulta = motor(PORT16, ratio36_1, false);
motor recogedor = motor(PORT14, ratio6_1, true);
motor leftMotorA = motor(PORT19, ratio18_1, false);
motor leftMotorB = motor(PORT20, ratio18_1, false);
motor_group LeftDriveSmart = motor_group(leftMotorA, leftMotorB);
motor rightMotorA = motor(PORT17, ratio18_1, true);
motor rightMotorB = motor(PORT18, ratio18_1, true);
motor_group RightDriveSmart = motor_group(rightMotorA, rightMotorB);
drivetrain Drivetrain = drivetrain(LeftDriveSmart, RightDriveSmart, 319.19, 295-40, 40, mm, 1);
limit LimitSwitchA = limit(Brain.ThreeWirePort.A);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
// define variables used for controlling motors based on controller inputs
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;
bool DrivetrainNeedsToBeStopped_Controller1 = true;
bool Controller1LeftShoulderControlMotorsStopped = true;
bool Controller1RightShoulderControlMotorsStopped = true;
bool pickerArmNeedsToBeStopped = true;

bool catapultaTrabada = true;

void RescatarCatapulta()
{
  if(catapultaTrabada)
  {
    cursorPantallaControl();
    Controller1.Screen.print("destrabar ");
    catapultaTrabada=false;
  }
  else
  {
    cursorPantallaControl();
    Controller1.Screen.print("no trabado");
    catapultaTrabada=true;
  }
}

// define a task that will handle monitoring inputs from Controller1
int rc_auto_loop_function_Controller1() 
{
  // process the controller input every 20 milliseconds
  // update the motors based on the input values
  
  while(true) 
  {
    if(RemoteControlCodeEnabled) 
    {
      // calculate the drivetrain motor velocities from the controller joystick axies
      // left = Axis3 + Axis1
      // right = Axis3 - Axis1
      int drivetrainLeftSideSpeed = Controller1.Axis3.position() + Controller1.Axis1.position();
      int drivetrainRightSideSpeed = Controller1.Axis3.position() - Controller1.Axis1.position();
      
      // check if the value is inside of the deadband range
      if (drivetrainLeftSideSpeed < 5 && drivetrainLeftSideSpeed > -5) 
      {
        // check if the left motor has already been stopped
        if (DrivetrainLNeedsToBeStopped_Controller1) 
        {
          // stop the left drive motor
          LeftDriveSmart.stop(brake);
          // tell the code that the left motor has been stopped
          DrivetrainLNeedsToBeStopped_Controller1 = false;
        }
      } 
      else 
      {
        // reset the toggle so that the deadband code knows to stop the left motor nexttime the input is in the deadband range
        DrivetrainLNeedsToBeStopped_Controller1 = true;
      }
      // check if the value is inside of the deadband range
      if (drivetrainRightSideSpeed < 5 && drivetrainRightSideSpeed > -5) 
      {
        // check if the right motor has already been stopped
        if (DrivetrainRNeedsToBeStopped_Controller1) 
        {
          // stop the right drive motor
          RightDriveSmart.stop(brake);
          // tell the code that the right motor has been stopped
          DrivetrainRNeedsToBeStopped_Controller1 = false;
        }
      } 
      else 
      {
        // reset the toggle so that the deadband code knows to stop the right motor next time the input is in the deadband range
        DrivetrainRNeedsToBeStopped_Controller1 = true;
      }
      
      // only tell the left drive motor to spin if the values are not in the deadband range
      if (DrivetrainLNeedsToBeStopped_Controller1) 
      {
        LeftDriveSmart.setVelocity(drivetrainLeftSideSpeed, percent);
        LeftDriveSmart.spin(forward);
      }
      // only tell the right drive motor to spin if the values are not in the deadband range
      if (DrivetrainRNeedsToBeStopped_Controller1) 
      {
        RightDriveSmart.setVelocity(drivetrainRightSideSpeed, percent);
        RightDriveSmart.spin(forward);
      }
       // check the ButtonL1/ButtonL2 status to control catapulta
      if (Controller1.ButtonL1.pressing()) 
      {
        catapulta.spin(forward,100,percent);
        //cambiamos la instruccion de avanzar mientras se presiona el boton por avanzar mientras revisa si el limit esta activado
        Controller1LeftShoulderControlMotorsStopped = false;
      }
      else if (!Controller1LeftShoulderControlMotorsStopped) {
        catapulta.stop(hold);
        // set the toggle so that we don't constantly tell the motor to stop when the buttons are released
        Controller1LeftShoulderControlMotorsStopped = true;
      }
      if (Controller1.ButtonR1.pressing()) 
      {
        recogedor.spin(forward,100,percent);
        Controller1RightShoulderControlMotorsStopped = false;
      } 
      else if (Controller1.ButtonR2.pressing()) 
      {
        recogedor.spin(reverse,100,percent);
        Controller1RightShoulderControlMotorsStopped = false;
      } 
      else if(Controller1.ButtonA.pressing())
      {
        RecogerRapido(500);
        Controller1RightShoulderControlMotorsStopped = false;
      }
      else if (!Controller1RightShoulderControlMotorsStopped) 
      {
        recogedor.stop(hold);
        // set the toggle so that we don't constantly tell the motor to stop when the buttons are released
        Controller1RightShoulderControlMotorsStopped = true;
      }
      //control brazo

      if(Controller1.ButtonUp.pressing())
      {
        brazoRecogedor.spin(forward,80,percent);
        pickerArmNeedsToBeStopped = false;
      }
      else if(Controller1.ButtonDown.pressing())
      {
        brazoRecogedor.spin(reverse,80,percent);
        pickerArmNeedsToBeStopped = false;
      }
      else if(!pickerArmNeedsToBeStopped)
      {
        brazoRecogedor.stop(hold);
        pickerArmNeedsToBeStopped = true;
      }
      if(Controller1.ButtonX.pressing())
      {
        RescatarCatapulta();
        if(catapultaTrabada)
      {
       if(LimitSwitchA.pressing()==0)
        {
        catapulta.spin(forward,100,percent);
        }
      else if (Controller1.ButtonL1.pressing()==0) 
        {
        catapulta.stop(hold);
        }
      }
      else
        {
          catapulta.stop(coast);
        }
      }
    }
    // wait before repeating the process
    wait(20, msec);
  }
  return 0;
}

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  task rc_auto_loop_task_Controller1(rc_auto_loop_function_Controller1);
}