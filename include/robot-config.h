using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor catapulta;
extern motor recogedor;
extern motor_group RightDriveSmart;
extern motor_group LeftDriveSmart;
extern drivetrain Drivetrain;
extern limit LimitSwitchA;
extern motor brazoRecogedor;
extern motor rightMotorA;
extern motor leftMotorA;
extern motor rightMotorB;
extern motor leftMotorB;


void RegresarCatapulta();




/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );