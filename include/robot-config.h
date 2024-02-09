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

void cursorPantallaControl();
void RecogerRapido(int tiempo);
void lanzarPelota();
void lanzarPelotaconRecogedor(int tiempo);
void RegresarCatapulta();

float distanciaTiempo(int pulgadas);
float gradosTiempo(int grados);
void driveDistance(float pulgadas);
void turnDistance(float pulgadas);
void detenerMotores();


void AvanzarTiempo(int tiempo, int velocidad);
void RetrocederTiempo(int tiempo, int velocidad);
void GirarDerechaTiempo(int tiempo);
void GirarIzquierdaTiempo(int tiempo);



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );