void avanzar(double distancia_en_pulgadas);
void giro(double grados);
void soltarPrecarga();
void recoger();
int detenerRecogedor ();
void devolverBola(int tiempo);
extern int distancia;

void lanzamientoPelotaAutonomo(int distancia);;
void moverBrazoRecogedor(int tiempo);
void regresarBrazoRecogedor(int tiempo);
void cursorPantallaControl();

void RegresarCatapulta();

void RecogerRapido(int tiempo);

void lanzarPelotaconRecogedor(int tiempo);

void lanzarPelota();

float distanciaTiempo(int pulgadas);

float gradosTiempo(int grados);

void AvanzarTiempo(int tiempo, int velocidad);

void RetrocederTiempo(int tiempo, int velocidad);

void GirarDerechaTiempo(int tiempo);

void GirarIzquierdaTiempo(int tiempo);

//funciones por distancia

void driveDistance(float pulgadas);

void turnDistance(float giro);

void detenerMotores();
