#include "vex.h"
#include "robot-config.h"

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

// some autonomous functions that can be used with controller

void cursorPantallaControl()
{
  Controller1.Screen.setCursor(1,2);
  Controller1.Screen.clearLine();
}

void RegresarCatapulta()
{
  while(LimitSwitchA.pressing()== false)
  {
    catapulta.spin(forward,100,percent);
  }
  catapulta.stop(hold);
}
void RecogerRapido(int tiempo)
{
  recogedor.spin(forward);
  wait(tiempo,msec);
}

void lanzarPelotaconRecogedor(int tiempo)
{
  recogedor.spin(reverse);
  wait(tiempo,msec);
  recogedor.spin(forward);
  wait(tiempo,msec);     
}

void lanzarPelota()
{
  catapulta.spin(forward);
  wait(500,msec);
  catapulta.stop();
}

//funciones por timer
int velocidadAutonoma = 50;
int velocidadGiroAutonomo = 25;
float distanciaTiempo(int pulgadas)
{
  // cuadro = 24 inches;
  int tiempoporcuadro = 1150;
  int tiempopulgada = tiempoporcuadro/24;
  int tiempototal = pulgadas*tiempopulgada;
  return tiempototal;
}

float gradosTiempo(int grados)
{
  //tiempo por grado = 7.55 grados @ 50% power
  float tiempototal = 10*grados;
  return tiempototal;
}

void AvanzarTiempo(int tiempo, int velocidad)
{
  cursorPantallaControl();
  Controller1.Screen.print("Avanzando");
  tiempo = distanciaTiempo(tiempo);
  rightMotorA.spin(forward,velocidad,percent);
  leftMotorA.spin(forward,velocidad,percent);
  rightMotorB.spin(forward,velocidad,percent);
  leftMotorB.spin(forward,velocidad,percent);
  wait(tiempo,msec);
  rightMotorA.stop();
  leftMotorA.stop();
  rightMotorB.stop();
  leftMotorB.stop();
  wait(100,msec);
  Controller1.Screen.clearLine();
}

void RetrocederTiempo(int tiempo, int velocidad)
{
  Controller1.Screen.setCursor(1,2);
  Controller1.Screen.clearLine();
  Controller1.Screen.print("Retrocediendo");
  tiempo = distanciaTiempo(tiempo);
  rightMotorA.spin(reverse,velocidad,percent);
  leftMotorA.spin(reverse,velocidad,percent);
  rightMotorB.spin(reverse,velocidad,percent);
  leftMotorB.spin(reverse,velocidad,percent);
  wait(tiempo,msec);
  rightMotorA.stop();
  leftMotorA.stop();
  rightMotorB.stop();
  leftMotorB.stop();
  wait(100,msec);

  Controller1.Screen.clearLine();
}
void GirarDerechaTiempo(int tiempo)
{
  Controller1.Screen.setCursor(1,2);
  Controller1.Screen.print("Giro Derecha");
  tiempo = gradosTiempo(tiempo);
  rightMotorA.spin(reverse,velocidadGiroAutonomo,percent);
  leftMotorA.spin(forward,velocidadGiroAutonomo,percent);
  rightMotorB.spin(reverse,velocidadGiroAutonomo,percent);
  leftMotorB.spin(forward,velocidadGiroAutonomo,percent);
  wait(tiempo,msec);
  rightMotorA.stop();
  leftMotorA.stop();
  rightMotorB.stop();
  leftMotorB.stop();
  wait(100,msec);
  Controller1.Screen.clearLine();
}
void GirarIzquierdaTiempo(int tiempo)
{
  Controller1.Screen.setCursor(1,2);
  Controller1.Screen.print("Giro Izquierda");
  tiempo = gradosTiempo(tiempo);
  rightMotorA.spin(forward,velocidadGiroAutonomo,percent);
  leftMotorA.spin(reverse,velocidadGiroAutonomo,percent);
  rightMotorB.spin(forward,velocidadGiroAutonomo,percent);
  leftMotorB.spin(reverse,velocidadGiroAutonomo,percent);
  wait(tiempo,msec);
  rightMotorA.stop();
  leftMotorA.stop();
  rightMotorB.stop();
  leftMotorB.stop();
  wait(100,msec);
  Controller1.Screen.clearLine();
}


//funciones por distancia
//establecemos constantes con datos de nuestro motor

  const float diametroLlanta = 4; // pulgadas
  const float circunferenciaLlanta = diametroLlanta * 3.1416;
  float pulgadasPorGrado = circunferenciaLlanta / 360;

  void driveDistance(float pulgadas)
  {
    float grados = pulgadas/pulgadasPorGrado;
    LeftDriveSmart.spinFor(forward, grados, deg,false);
    RightDriveSmart.spinFor(forward, grados, deg);
  }
  void turnDistance(float giro)
  {
    float grados = giro / pulgadasPorGrado;
    LeftDriveSmart.spinFor(forward, grados, deg,false);
    RightDriveSmart.spinFor(reverse, grados, deg);
  }

  void detenerMotores()
  {
    LeftDriveSmart.stop();
    RightDriveSmart.stop();
  }
