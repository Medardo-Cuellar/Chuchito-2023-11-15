/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Clawbot Competition Template                              */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// Catapulta            motor         16              
// Recogedor            motor         15              
// Drivetrain           drivetrain    19, 20, 17, 18  
// LimitSwitchA         limit         A               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/


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


bool enableDrivePID = true;
bool resetDriveSensors = true;

int drivePID(int avance, int vuelta)
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


//funciones autonomas//
void avanzar(double distancia_en_pulgadas) {
  // Verifica la dirección del movimiento
  if (distancia_en_pulgadas >= 0.0) {
    // Avanza hacia adelante
    Drivetrain.driveFor(forward,distancia_en_pulgadas,inches);
  }else if (distancia_en_pulgadas <= 0.0){
    distancia_en_pulgadas = distancia_en_pulgadas*-1;
    // Avanza hacia atrás
    Drivetrain.driveFor(reverse,distancia_en_pulgadas,inches);

    
  }

}

// Función para girar
void giro(double grados) {
  // Verifica la dirección del giro
  if (grados >= 0.0) {
    // Gira a la derecha
    Drivetrain.turnFor(right,grados,degrees);
  } else {
    grados=grados*-1;
    // Gira a la izquierda
    Drivetrain.turnFor(left,grados,degrees);
  }

  }



void soltarPrecarga()
{
catapulta.spin(forward,100,percent);
wait(3000,msec);
catapulta.stop();
}


void recoger(){
   recogedor.spin(reverse,100,percent); 
   wait(1500,msec);
   recogedor.stop(); 
}
int detenerRecogedor (){
  while(LimitSwitchA.pressing()==0){
    recogedor.stop();
  }
  return 0;
}
void devolverBola(int tiempo)
{
recogedor.spin(forward,100,percent);
wait(tiempo,msec);
recogedor.stop();
}

void autonomoskills(int tiempo)
{
  recogedor.spin(reverse,100,percent);
  AvanzarTiempo(tiempo, 40);
  recoger();
  RetrocederTiempo(tiempo, 40);
  lanzarPelota();
  RegresarCatapulta();
  wait(400,msec);
}

int catapultaTrabada = 0;
void RescatarCatapulta()
{
  if(catapultaTrabada==0)
  {
    catapultaTrabada++;
    Controller1.Screen.setCursor(1,1);
    Controller1.Screen.print("destraba ");
  }
  else if(catapultaTrabada==1)
  {
    catapultaTrabada++;
    Controller1.Screen.setCursor(1,1);
    Controller1.Screen.print("trabado");
  }
  else
  { catapultaTrabada=0;
  Controller1.Screen.setCursor(1,1);
  Controller1.Screen.print("destraba");}
}

void autonomobasico()
{
  recogedor.spin(forward,100,percent);
  RegresarCatapulta();
  recogedor.stop();
  RetrocederTiempo(15,100);
  RetrocederTiempo(25,50);
  GirarDerechaTiempo(135);
  AvanzarTiempo(10, 50);
  GirarDerechaTiempo(50);
  AvanzarTiempo(13,50);
  task recogeTask(detenerRecogedor);
  wait(1000,msec);
  GirarDerechaTiempo(140);
  devolverBola(1000);
  GirarDerechaTiempo(180);
  RetrocederTiempo(23,50);
  
}
void moverBrazoRecogedor(int tiempo)
{
  brazoRecogedor.spin(reverse);
  wait(tiempo, msec);
  brazoRecogedor.stop(brake);
}

void regresarBrazoRecogedor(int tiempo)
{
  brazoRecogedor.spin(forward);
  wait(tiempo, msec);
  brazoRecogedor.stop(brake);
}

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  
  //AvanzarTiempo(5, 100);
  //RetrocederTiempo(5, 100);
  //drivePID(10,0);

 moverBrazoRecogedor(600);
  RegresarCatapulta();
  autonomoskills(4);
  autonomoskills(4);
  autonomoskills(4);
  autonomoskills(4);
  autonomoskills(3);
  autonomoskills(3);
  autonomoskills(3);
  autonomoskills(3);
  autonomoskills(3);
  autonomoskills(3); 
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
    enableDrivePID = false;
  while (1) {
      
      Controller1.ButtonX.pressed(RescatarCatapulta);
      if(catapultaTrabada==1)
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

    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(20, msec); 
    // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    
    wait(100, msec);
  }
}
