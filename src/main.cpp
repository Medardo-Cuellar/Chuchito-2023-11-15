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
#include "pid.h"

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

void lanzamientoPelotaAutonomo(int distancia)
{
  recogedor.spin(reverse,100,percent);
  AvanzarTiempo(distancia, 40);
  recoger();
  RetrocederTiempo(distancia, 40);
  lanzarPelota();
  RegresarCatapulta();
  wait(400,msec);
}

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
  
  //moverBrazoRecogedor(600);
  RegresarCatapulta();
  int variablelanzamiento = 3;
  for(int i=1; i<=10; i++)
  {
    if(i==4){variablelanzamiento++;}
    else if(i==7){variablelanzamiento++;}    
    lanzamientoPelotaAutonomo(variablelanzamiento);
  }
  

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
