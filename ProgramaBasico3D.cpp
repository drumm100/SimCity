
#ifdef WIN32
#include <windows.h>
#include "gl\glut.h"
    static DWORD last_idle_time;
#else
    #include <sys/time.h>
    static struct timeval last_idle_time;
#endif
#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int NUM_COLORS;
ifstream inFile;
int x;

typedef struct{
    float minX, minY, minZ;
    float maxX, maxY, maxZ;
    int altura, largura, profundidade;
    int color;
}Quadrilateral;

int colors[10][3];
Quadrilateral quadrilaterals[10];

GLfloat AspectRatio, AngY=0;

void readColors(){
    inFile.open("colors.txt");
    if(!inFile){
        cout << "N�o consegui abrir o arquivo das cores";
        exit(1);
    }else{
        cout << "Arquivos das cores aberto" << endl;
    }

    while(inFile >> x){
        NUM_COLORS = x;
        for(int i = 0; i < NUM_COLORS; i++){
            int id, r, g, b;
            inFile >> id >> r >> g >> b;
            colors[i][0] = r;
            colors[i][1] = g;
            colors[i][2] = b;
        }
    }

    inFile.close();
}

void readObjects(){
    //Example
    Quadrilateral cubo01;

    inFile.open("predio01.txt");
    if(!inFile){
        cout << "N�o consegui abrir o arquivo do predio01" << endl;
        exit(1);
    }else{
        cout << "Arquivo do predio01 aberto" << endl;
    }

    inFile >> cubo01.minX >> cubo01.minY >> cubo01.minZ;
    inFile >> cubo01.maxX >> cubo01.maxY >> cubo01.maxZ;
    inFile >> cubo01.color;

    quadrilaterals[0] = cubo01;
    inFile.close();

    cout << "Cubo 01" << endl;
    cout << cubo01.minX << "|" << cubo01.minY << "|" << cubo01.minZ << endl;
    cout << cubo01.maxX<< "|" << cubo01.maxY << "|" << cubo01.maxZ << endl;
}

// **********************************************************************
//  void DefineLuz(void)
// **********************************************************************
void DefineLuz(void)
{
  // Define cores para um objeto dourado
  GLfloat LuzAmbiente[]   = {0.24725f, 0.1995f, 0.07f } ;
  GLfloat LuzDifusa[]   = {0.75164f, 0.60648f, 0.22648f, 1.0f };
  GLfloat LuzEspecular[] = {0.626281f, 0.555802f, 0.366065f, 1.0f };
  GLfloat PosicaoLuz0[]  = {3.0f, 3.0f, 0.0f, 1.0f };
  GLfloat PosicaoLuz1[]  = {-3.0f, -3.0f, 0.0f, 1.0f };
  GLfloat Especularidade[] = {1.0f, 1.0f, 1.0f, 1.0f };

  // ****************  Fonte de Luz 0
  glEnable(GL_COLOR_MATERIAL);

  // Habilita o uso de ilumina��o
  glEnable(GL_LIGHTING);

  // Ativa o uso da luz ambiente
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LuzAmbiente);

  // Define os parametros da Luz n�mero Zero
  glLightfv(GL_LIGHT0, GL_AMBIENT, LuzAmbiente);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, LuzDifusa  );
  glLightfv(GL_LIGHT0, GL_SPECULAR, LuzEspecular  );
  glLightfv(GL_LIGHT0, GL_POSITION, PosicaoLuz0 );
  glEnable(GL_LIGHT0);

  // Ativa o "Color Tracking"
  glEnable(GL_COLOR_MATERIAL);

  // Define a reflectancia do material
  glMaterialfv(GL_FRONT,GL_SPECULAR, Especularidade);

  // Define a concentra��o do brilho.
  // Quanto maior o valor do Segundo parametro, mais
  // concentrado ser� o brilho. (Valores v�lidos: de 0 a 128)
  glMateriali(GL_FRONT,GL_SHININESS,51);

  // ****************  Fonte de Luz 1

  // Ativa o uso da luz ambiente
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LuzAmbiente);
  // Define os parametros da Luz n�mero Zero
  glLightfv(GL_LIGHT1, GL_AMBIENT, LuzAmbiente);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, LuzDifusa  );
  glLightfv(GL_LIGHT1, GL_SPECULAR, LuzEspecular  );
  glLightfv(GL_LIGHT1, GL_POSITION, PosicaoLuz1 );
  glEnable(GL_LIGHT1);

  // Ativa o "Color Tracking"
  glEnable(GL_COLOR_MATERIAL);

  // Define a reflectancia do material
  glMaterialfv(GL_FRONT,GL_SPECULAR, Especularidade);

  // Define a concentra��o do brilho.
  // Quanto maior o valor do Segundo parametro, mais
  // concentrado ser� o brilho. (Valores v�lidos: de 0 a 128)
  glMateriali(GL_FRONT,GL_SHININESS,20);

}


// **********************************************************************
//  void init(void)
//	Inicializa os par�metros globais de OpenGL
// **********************************************************************
void init(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Fundo de tela preto

	glShadeModel(GL_SMOOTH);
	glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_DEPTH_TEST);
	glEnable (GL_CULL_FACE);

    // Obtem o tempo inicial
    #ifdef WIN32
        last_idle_time = GetTickCount();
    #else
        gettimeofday (&last_idle_time, NULL);
    #endif

    readColors();
    readObjects();

}


// **********************************************************************
//  void PosicUser()
// **********************************************************************
void PosicUser()
{
	// Set the clipping volume
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90,AspectRatio,0.01,200);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 0,
		      0,0,-8,
			  0.0f,1.0f,0.0f);
}


// **********************************************************************
//  void reshape( int w, int h )
//  trata o redimensionamento da janela OpenGL
// **********************************************************************
void reshape( int w, int h )
{

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	AspectRatio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	PosicUser();

}

// **********************************************************************
//  void DesenhaCubo()
// **********************************************************************
void DesenhaCubo()
{
    int minX = quadrilaterals[0].minX;
    int minY = quadrilaterals[0].minY;
    int minZ = quadrilaterals[0].minZ;
    int maxX = quadrilaterals[0].maxX;
    int maxY = quadrilaterals[0].maxY;
    int maxZ = quadrilaterals[0].maxZ;
	glBegin ( GL_QUADS );
		// Front Face
		glNormal3f(0,0,1);
		glVertex3f(minX, minY, minZ); // 0 0 0
		glVertex3f(minX, maxY,  minZ);
		glVertex3f(maxX,  maxY,  minZ);
		glVertex3f(maxX,  minY,  minZ);
		// Back Face
		glNormal3f(0,0,-1);
		glVertex3f(minX, minY, maxZ);
		glVertex3f(minX, maxY,  maxZ);
		glVertex3f(maxX,  maxY,  maxZ);
		glVertex3f(maxX,  minY,  maxZ);
		// Top Face
		glNormal3f(0,1,0);
		glVertex3f(minX,  maxY, minZ); // 0 10 0
		glVertex3f(minX,  maxY,  maxZ); // 0 10 10
		glVertex3f(maxX,  maxY,  maxZ); // 10 10 10
		glVertex3f(maxX,  maxY, minZ); // 10 10 0
		// Bottom Face
		glNormal3f(0,-1,0);
		glVertex3f(minX,  minY, minZ);
		glVertex3f(minX,  minY,  maxZ);
		glVertex3f(maxX,  minY,  maxZ);
		glVertex3f(maxX,  minY, minZ);
		// Right face
		glNormal3f(1,0,0);
		glVertex3f(maxX, minY, minZ);
		glVertex3f(maxX, maxY, minZ);
		glVertex3f(maxX, maxY,  maxZ);
		glVertex3f(maxX, minY,  minZ);
		// Left Face
		glNormal3f(-1,0,0);
        glVertex3f(minX, minY, minZ);
		glVertex3f(minX, maxY, minZ);
		glVertex3f(minX, maxY,  maxZ);
		glVertex3f(minX, minY,  minZ);
	glEnd();
}

// **********************************************************************
//  void display( void )
// **********************************************************************
void display( void )
{

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	DefineLuz();

	PosicUser();

	glMatrixMode(GL_MODELVIEW);


	glPushMatrix();
		glTranslatef ( 1.0f, 0.0f, -5.0f );
        glRotatef(AngY,0,1,0);
		glColor3f(0.5f,0.3f,0.0f);
		DesenhaCubo();
	glPopMatrix();


	glPushMatrix();
		glTranslatef ( -1.0f, 2.0f, -8.0f );
		glRotatef(AngY,0,1,0);
		glColor3f(0.5f,0.3f,0.0f);
		DesenhaCubo();
	glPopMatrix();


	glutSwapBuffers();
}

// **********************************************************************
//  void animate ( unsigned char key, int x, int y )
//
//
// **********************************************************************
void animate()
{
    static float dt;
    static float AccumTime=0;

#ifdef _WIN32
    DWORD time_now;
    time_now = GetTickCount();
    dt = (float) (time_now - last_idle_time) / 1000.0;
#else
    // Figure out time elapsed since last call to idle function
    struct timeval time_now;
    gettimeofday(&time_now, NULL);
    dt = (float)(time_now.tv_sec  - last_idle_time.tv_sec) +
    1.0e-6*(time_now.tv_usec - last_idle_time.tv_usec);
#endif
    AccumTime +=dt;
    if (AccumTime >=3) // imprime o FPS a cada 3 segundos
    {
        cout << 1.0/dt << " FPS"<< endl;
        AccumTime = 0;
    }
    //cout << "AccumTime: " << AccumTime << endl;
    // Anima cubos
    //AngY++;
    // Sa;va o tempo para o pr�ximo ciclo de rendering
    last_idle_time = time_now;

        //if  (GetAsyncKeyState(32) & 0x8000) != 0)
          //  cout << "Espaco Pressionado" << endl;

    // Redesenha
    glutPostRedisplay();
}

// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
//
//
// **********************************************************************
void keyboard ( unsigned char key, int x, int y )
{
	switch ( key )
	{
    case 27:        // Termina o programa qdo
      exit ( 0 );   // a tecla ESC for pressionada
      break;
    case ' ':
        ++AngY;
        glutPostRedisplay();

    default:
            cout << key;
      break;
  }
}

// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
//
//
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )
{
	switch ( a_keys )
	{
		case GLUT_KEY_UP:       // When Up Arrow Is Pressed...
			glutFullScreen ( ); // Go Into Full Screen Mode
			break;
	    case GLUT_KEY_DOWN:     // When Down Arrow Is Pressed...
			glutInitWindowSize  ( 700, 500 );
			break;
		default:
			break;
	}
}

// **********************************************************************
//  void main ( int argc, char** argv )
//
//
// **********************************************************************
int main ( int argc, char** argv )
{
	glutInit            ( &argc, argv );
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );// | GLUT_STEREO);// | GLUT_DOUBLE | GLUT_RGBA );
	//glutInitDisplayMode (GLUT_RGB | GLUT_DEPTH | GLUT_STEREO);// | GLUT_DOUBLE | GLUT_RGBA );

	glutInitWindowPosition (0,0);
	glutInitWindowSize  ( 700, 500 );
	glutCreateWindow    ("SimCity" );

	init ();

	glutDisplayFunc ( display );
	glutReshapeFunc ( reshape );
	glutKeyboardFunc ( keyboard );
	glutSpecialFunc ( arrow_keys );
	glutIdleFunc ( animate );

	glutMainLoop ( );
	return 0;
}


