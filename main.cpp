#ifdef _APPLE_
#include <GLUT/glut.h>
#else
#include <windows.h>
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include<conio.h>
#include<math.h>
#include <gl/glut.h>

#include <iostream>

using namespace std;

//Alvarado Torres Alexandra Luisa
//Paz Guillen Jose Alonso

//VENTANA DE VISUALIZACION DE UN MODELO 3D DE UN ATOMO
//--En la version alambrica se usan primitivas simples como puntos y lineas--
//--En la version solida se usan primitivas mas complejas como esferas y cilindros de creacion propia--

//Metodo de creacion de menus
void creacionDeMenus(void);
void dibujar(int);

//Metodos de dibujo
void ejes(int);
void dibujarEjes(int);
void dibujarCilindro(int);
void dibujarModelo(int);
void cilindroAlambrico(int,float,float,float);
void cilindroSolido(int,float,float,float);

//Metodos de manejo de camara
void avanza(void);
void retro(void);
void rotacamara(void);
void rotacamara2(void);

//Variables de cilindro virtual
float R=0.5;
float H=4;

//Cantidad de filas de atomos
float numAtom=30;
int numAtomInt=30;

//Variables para movimiento de camara
GLdouble angulo=0, incremento_angulo=0.1;
GLdouble const radio=0.5;

//Coordenadas de posicion de camara y punto de visualizacion
GLfloat px0=0,py0=0,pz0=4;
GLfloat px1=0,py1=0,pz1=2;

//Vector para las rotaciones
static GLfloat theta[] = {0.0,0.0,0.0};

//Angulo de la rotacion
GLfloat incremento = 0.15;
//Variacion de la velocidad de rotaciones
GLfloat velocidad = 0.01;
//Sentido de la rotacion
float sentido=-1;

//Eje del plano sobre el que rotar
//Se coloca en -1 para empezar sin rotacion sobre ningun eje
static GLint axis = -1;


//Opciones para los menus y submenus
int opcion;
bool modoRot = true;
int opRepren=1;
bool opEje=false;
bool opVir=false;
bool opModelo=false;
int modelo=0;

//----------------------------------------------------------------//
//-----------Metodos de dibujo para el ADN -----------------------//
//----------------------------------------------------------------//

//Dibujo del cilindro virtual
void cilindroVirtual()
{
    cilindroAlambrico(15,15,H,R);

}

//Dibujo del modelo de ADN alambrico completo.
//Usa POINTS y LINES como indican los enunciados
void modeloAlambrico()
{
    float x1,y1,z1;
    float x2,y2,z2;
    float ys1[numAtomInt],xs1[numAtomInt],zs1[numAtomInt];
    float ys2[numAtomInt],xs2[numAtomInt],zs2[numAtomInt];
    int i=0;
    glPointSize(18);
    glColor3f(1,0,0);
    //Se dibujan las moleculas
    glBegin(GL_POINTS);
    for (float ang = 0; ang < 2 * M_PI; ang += 2*M_PI/numAtom)
    {
        x1 = R * cos(ang);
        z1 = R * sin(ang);
        y1 = H*i/numAtom;

        xs1[i]=x1;
        ys1[i]=y1;
        zs1[i]=z1;

        glVertex3f(x1,y1,z1);

        x2 = R * cos(ang+ M_PI);
        z2 = R * sin(ang+ M_PI);
        y2 = y1;

        xs2[i]=x2;
        ys2[i]=y2;
        zs2[i]=z2;

        glVertex3f(x2,y2,z2);
        i++;
    }
    glEnd();

    glColor3f(0,0,1);

      //Se dibujan los enlaces
    glLineWidth(6);
    glBegin(GL_LINES);
    i=0;
    for (float ang = 0; ang < 2 * M_PI; ang += 2*M_PI/numAtom)
    {
        x1 = R * cos(ang);
        z1 = R * sin(ang);
        y1 = H*i/numAtom;
        glVertex3f(x1,y1,z1);

        x2 = R * cos(ang+ M_PI);
        z2 = R * sin(ang+ M_PI);

        glVertex3f(x2,y1,z2);
        i++;
    }
    glEnd();

    glColor3f(0,1,0);

    for (int i=0; i < numAtom; i++)
    {
        glBegin(GL_LINES);

        glVertex3f(xs1[i],ys1[i],zs1[i]);
        glVertex3f(xs1[i+1],ys1[i+1],zs1[i+1]);

        glVertex3f(xs2[i],ys2[i],zs2[i]);
        glVertex3f(xs2[i+1],ys2[i+1],zs2[i+1]);

        glEnd();
    }
}

//Dibujo del modelo solido de ADN
//Usa esferas de alambre y una funcion cilindro propia

void modeloSolido()
{
    glLineWidth(1);
    float ys[30];
    float x,y,z;
    int i=0;
    glColor3f(1,0,0);
    for (float ang = 0; ang < 2 * M_PI; ang += 2*M_PI/numAtom)
    {
        glColor3f(1,0,0);
        glPushMatrix();
        x = R * cos(ang);
        z = R * sin(ang);
        y = H*i/numAtom;
        ys[i]=y;
        //Dibujo de molecula
        glTranslatef(x,y,z);
        glutWireSphere(0.075,8,8);

        //Dibujo del cilindro de enlace
        glPushMatrix();
            glColor3f(0,0,1);
            glTranslatef(0,ys[0],0);
            glRotatef(-ang*180/M_PI,0,1,0);
            glRotatef(90,0,0,1);
            cilindroAlambrico(6,10,2*R,0.025);
        glPopMatrix();

        glPopMatrix();

        glPushMatrix();

        glColor3f(1,0,0);
        x = R * cos(ang+ M_PI);
        z = R * sin(ang+ M_PI);

        //Dibujo de molecula opuesta
        glTranslatef(x,y,z);
        glutWireSphere(0.075,8,8);
        glPopMatrix();
        i++;

    }

}

//----------------------------------------------------------------//
//-------------Metodos de dibujo generales------------------------//
//----------------------------------------------------------------//

void ejes(int longitud)
{
    glLineWidth(1);

    //Eje Z Rojo
    glColor3f(1,0,0);
    glBegin(GL_LINES);
    glVertex3d(0,0,longitud);
    glVertex3d(0,0,0);
    glEnd();

    //Eje Y Verde
    glColor3f(0,1,0);
    glBegin(GL_LINES);
    glVertex3d(0,0,0);
    glVertex3d(0,longitud,0);
    glEnd();

    //Eje X Azul
    glColor3f(0,0,1);
    glBegin(GL_LINES);
    glVertex3d(0,0,0);
    glVertex3d(longitud,0,0);
    glEnd();

}

//Dibujo de cilindro de creacion propia
//Usa vectores para reducir calculo numerico
void cilindroAlambrico(int Npara, float Ncortes,float altura,float radio)
{
    GLfloat posX[Npara],posZ[Npara];
    int i=0;

    float x,y=0,z;
    glBegin(GL_LINE_LOOP);
    for (float ang = 0; ang < 2 * M_PI; ang += 2*M_PI/Npara)
    {
        x = radio * cos(ang);
        z = radio * sin(ang);
        posX[i] = x;
        posZ[i] = z;
        i++;
        glVertex3f(x,y,z);
    }
    glEnd();

    for(int i=0; i<Ncortes+1; i++)
    {
        glBegin(GL_LINE_LOOP);
        for(int j=0; j<Npara; j++)
        {
            x=posX[j];
            z=posZ[j];
            y=altura*i/Ncortes;
            glVertex3f(x,y,z);
        }
        glEnd();
    }

    glBegin(GL_LINES);
    for(int i=0; i<Npara; i++)
    {
        x=posX[i];
        z=posZ[i];
        glVertex3f(x,0,z);
        glVertex3f(x,altura,z);
    }
    glEnd();


}

//----------------------------------------------------------------//
//---------------------FUNCION DISPLAY----------------------------//
//----------------------------------------------------------------//

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glLoadIdentity();

    // la camara se desplaza sobre el plano xz
    gluLookAt(px0,py0,pz0,px1,py1,pz1,0,1,0);

    //Se mueve hacia abajo la imagen completa para que quepa
    glTranslatef(0,-2,0);

    //Rotacion de toda la imagen
    glRotatef(theta[1]*sentido,0.0,1.0,0.0);

    //Metodos para dibujar segun opcion
    dibujarEjes(opEje);
    dibujarCilindro(opVir);
    dibujarModelo(modelo);

    glFlush();
    glPopMatrix();
    glutSwapBuffers();
}

//Metodo para las rotaciones de la imagen
void rotaciones()
{
    theta[axis] += incremento;
    if(theta[axis]>360)
        theta[axis] -= 360.0;
    display();
}

//----------------------------------------------------------------//
//-------------------Manejo con teclado---------------------------//
//----------------------------------------------------------------//

void teclado(unsigned char tecla,int x,int y)
{
    switch(tecla)
    {
    case 'I' :
        avanza();
        break;
    case 'M' :
        retro();
        break;
    case 'J' :
        angulo=angulo+incremento_angulo;
        rotacamara();
        break;
    case 'K' :
        angulo=angulo-incremento_angulo;
        rotacamara();
        break;
    case 'L' :
        angulo=angulo-incremento_angulo;
        rotacamara2();
        break;
    case 'P' :
        angulo=angulo+incremento_angulo;
        rotacamara2();
        break;
    case 'R' :
        if(axis == -1)
        {
            axis = 1;
        }
        else if(axis == 1)
        {
            axis = -1;
        }
        break;
    case 'Z' :
            incremento=incremento+velocidad;
        break;
    case 'X' :
        if(incremento>0)
            incremento=incremento-velocidad;
        break;
    case 'F' :
        exit(0);
        break;
    }
}

//----------------------------------------------------------------//
//-------------------Creacion de menus----------------------------//
//----------------------------------------------------------------//


//Metodo para dibujar ejes segun el bool opEje
void dibujarEjes(int opEje)
{
    if(opEje==true)
    {
        ejes(4);
    }
}

//Menu que activa y desactiva rotaciones
void menu_rota(int opcion)
{
    switch(opcion)
    {
    case 1:
        sentido=-1;
        break;
    case 2:
        sentido=1;
        break;

    }

}

//Menu que habilita o desactiva los ejes
void menu_referencia(int opcion)
{
    switch(opcion)
    {
    case 1:
        opEje = true;
        break;
    case 2:
        opEje = false;
        break;
    }

}

//Menu que habilita el pintado del cilindro virtual
void menu_virtual(int opcion)
{
    switch(opcion)
    {
    case 1:
        opVir = true;
        break;
    case 2:
        opVir = false;
        break;
    }

}

//Metodo que dibuja el cilindro segun la variable opVir
void dibujarCilindro(int opVir)
{
    if(opVir==true)
    {
        glLineWidth(1);
        glColor3f(0,0,0.5);
        cilindroVirtual();
    }

}

//Metodo que dibuja el modelo segun la opcion
void dibujarModelo(int opcion)
{
    switch(opcion)
    {
    case 1:
        modeloAlambrico();
        break;
    case 2:
        modeloSolido();
        break;
    }
}


void menu_principal(int opcion)
{
    switch(opcion)
    {
    case 3:
        modelo=1;
        break;
    case 4:
        modelo=2;
        break;
    case 5:
        exit(-1);
    }
    glutPostRedisplay();
}

//----------------------------------------------------------------//
//--------------Llamado del init, reshape y main------------------//
//----------------------------------------------------------------//

void iniciar(void)
{
    glClearColor (1.0, 1.0, 1.0, 0.0);
    glShadeModel (GL_SMOOTH);

}

void myReshape(int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h,0.1, 30.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(750,750);
    glutCreateWindow("Modelo de ADN 3D");

    creacionDeMenus();

    iniciar();
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutIdleFunc(rotaciones);
    glutKeyboardFunc(teclado);
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 0;
}

//Metodos de manejo de camara
void rotacamara()
{
    px1=px0+radio*sin(angulo);
    pz1=pz0-radio*cos(angulo);
}

void rotacamara2()
{
    py1=py0+radio*sin(angulo);
    pz1=pz0-radio*cos(angulo);
}

void avanza()
{
    px0=px1;
    pz0=pz1;
    px1=px0+radio*sin(angulo);
    pz1=pz0-radio*cos(angulo);
}

void retro()
{
    px1=px0;
    pz1=pz0;
    px0=px0-radio*sin(angulo);
    pz0=pz0+radio*cos(angulo);
}

//Metodo para creacion de menus
void creacionDeMenus()
{

    int submenu_rota=glutCreateMenu(menu_rota);
    glutAddMenuEntry("Horario",1);
    glutAddMenuEntry("Antihorario",2);

    int submenu_referencia=glutCreateMenu(menu_referencia);
    glutAddMenuEntry("Activado",1);
    glutAddMenuEntry("Desactivado",2);


    int submenu_virtual=glutCreateMenu(menu_virtual);
    glutAddMenuEntry("Activado",1);
    glutAddMenuEntry("Desactivado",2);


    int menu=glutCreateMenu(menu_principal);
    glutAddSubMenu("Cilindro virtual",submenu_virtual);
    glutAddSubMenu("Ejes",submenu_referencia);
    glutAddMenuEntry("Modelo Alambrico",3);
    glutAddMenuEntry("Modelo Solido",4);
    glutAddSubMenu("Rotaciones",submenu_rota);

    glutAddMenuEntry("Salir",5);

    glutAttachMenu(GLUT_RIGHT_BUTTON);

}
