#define PROYECTO "ISGI::S5E02::Control del tiempo"
#include <iostream> // Biblioteca de entrada salida
#include <sstream> // Biblioteca de manejo de strings
#include <cmath> // Biblioteca matematica de C
#include <time.h>
#include <math.h>
#include <Utilidades.h>
#include <gl\freeglut.h> // Biblioteca grafica
using namespace std;
static const float radio = 5.0; // Radio de giro de la camara
static float angulo = 0.0; // Angulo de travelling inicial de la camara
static float ojo[] = {0,0,radio}; // Posicion inicial de la camara
static const float velocidad = 24.0*3.1415926/180; // radianes/segundo
static const int tasaFPS = 40; // tasa que se quiere como maximo
# define PI           3.14159265358979323846  /* pi */
//static float alfa = 0;
double hora;
static float rx, ry, rz, alfa;
struct tm *newtime;
time_t ltime;

double Spin;
void animate( int ms )
{  
    Spin = Spin + ms*0.02;      
    if(Spin > 360.0){
      Spin = Spin - 360.0;
    }

}

int old_t;

void idle(void) {
  int t = glutGet(GLUT_ELAPSED_TIME);
  int passed = t - old_t;
  old_t = t;
  animate( passed );
  glutPostRedisplay();
}


void muestraFPS()
// Calcula la frecuencia y la muestra en el título de la ventana
// cada segundo
{
	int ahora, tiempo_transcurrido;
	static int antes = 0;
	static int FPS = 0;
	stringstream titulo;
	//Cuenta de llamadas a esta función en el último segundo
	FPS++;
	ahora = glutGet(GLUT_ELAPSED_TIME); //Tiempo transcurrido desde el inicio
	tiempo_transcurrido = ahora - antes; //Tiempo transcurrido desde antes
	if(tiempo_transcurrido>1000){ //Acaba de rebasar el segundo
		titulo << "FPS: " << FPS; //Se muestra una vez por segundo
		glutSetWindowTitle(titulo.str().c_str());
		antes = ahora; //Ahora ya es antes
		FPS = 0; //Reset del conteo
	}
}

void init()
// Funcion propia de inicializacion
{
	cout << "Version: OpenGL " << glGetString(GL_VERSION) << endl;
	glClearColor(1,1,1,1.0); // Color de fondo a blanco
	glEnable(GL_DEPTH_TEST); // Habilita visibilidad
}

void display()
// Funcion de atencion al dibujo
{
	time (&ltime);
	newtime = localtime(&ltime);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Borra la pantalla
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,1,5.5,0,0,0,0,1,0); // Situa la camara



	//Cubo grande del centro(rotacion y movimiento continuo)
	glPushMatrix();
	glRotatef(Spin,0,0,1);
	glColor3f(0.4,0.4,0.9);
	//Es va modificant el escalat
	glScalef(0.5,0.5,0.5);
	glutWireCube(0.5);
	glColor3f(0,0,1);
	glPopMatrix();

	//Cubo pequeño (horas)
	glPushMatrix();
	glRotatef((360/12)* newtime -> tm_hour, 0, 0, -1);
	glScalef();
	glTranslatef(0,0.9,0);
	//glTranslatef(0.9*sin(2*3.1415926/12), 0.9*cos(2*3.1415926/12),0);
	glColor3f(0.2,0.8,0.2);
	glutWireCube(0.08);
	glPopMatrix();

	//Cono(minutos)
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glRotatef((360/60)* newtime -> tm_min, 0, 1, 0);
	glColor3f(0.8,0.8,0.8);
	glutWireCone(0.08, 0.8,8,8);
	glColor3f(0.2,0.2,1);
	glutSolidCone(0.08, 0.8,10,10);
	glPopMatrix();

	//Esfera exterior(segundos)
	glPushMatrix();
	glRotatef((360/60)* newtime -> tm_sec, 0, 0, -1);
	glTranslatef(0,1,0);
	glColor3f(0,0,0); 
	glutWireSphere(0.05,10,10);
	glColor3f(0.8,0,0.8);
	glutSolidSphere(0.05,10,10);
	glPopMatrix();

	for (int i=0;i<12;i++){
		glPushMatrix();
		glTranslatef(0.9*cos(i*2*3.1415926/12), 0.9*sin(i*2*3.1415926/12),0);
		glColor3f(0.08*i,0.03*i,0.2*i);
		glutSolidSphere(0.04, 20, 10);
		glPopMatrix();
	}


	//muestraFPS(); // Hemos dibujado un frame
	glutSwapBuffers(); // Intercambia los buffers
}

// Funcion de atencion al redimensionamiento
void reshape(GLint w, GLint h)
{
	// Usamos toda el area de dibujo
	glViewport(0,0,w,h);
	// Definimos la camara (matriz de proyeccion)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float razon = (float) w / h;
	float d=(2*asin(1/sqrt((1.3*1.3)+(3*3)+(4*4)))); 
	gluPerspective((d*180)/PI,razon,1,10);
}

//OnTimer
void onTimer(int tiempo)
// Funcion de atencion al timer periodico
{
	rx = 30*cos(alfa);
	ry = 30*sin(alfa);
	rz = 30*cos(alfa);

	alfa += 0.01;
	if (alfa>2*PI)alfa=0;

	glutPostRedisplay(); // Se manda el dibujo
	glutTimerFunc(100, onTimer,1);
}


void main(int argc, char** argv)
// Programa principal
{
	glutInit(&argc, argv); // Inicializacion de GLUT
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH); // Alta de buffers a usar
	glutInitWindowSize(500,500); // Tamanyo inicial de la ventana
	glutCreateWindow("Reloj Analógico"); // Creacion de la ventana con su titulo
	std::cout << PROYECTO << " running" << std::endl; // Mensaje por consola
	init();
	glutDisplayFunc(display); // Alta de la funcion de atencion a display
	glutReshapeFunc(reshape); // Alta de la funcion de atencion a reshape
	glutIdleFunc(idle);
	glutTimerFunc(100, onTimer,1);
	//glutTimerFunc(1000/tasaFPS,onTimer,tasaFPS); // Alta de la funcion de atencion al timer
	 // Inicializacion propia
	glutMainLoop(); // Puesta en marcha del programa
}