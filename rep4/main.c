#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#define timer_id1 0

/* Fleg koji odredjuje stanje tajmera. */
static int timer_active;

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_release(unsigned char key,int x,int y);
static void on_reshape(int width, int height);
static void on_display(void);

double x_auta = 2;
double y_auta = 0.25;
double z_auta = 0;

float x_puta_prvog = 2;
float y_puta_prvog = -0.125;
float z_puta_prvog = 20;

float x_puta_drugog = 2;
float y_puta_drugog = -0.125;
float z_puta_drugog = 60;

// niz koji sluzi za kretanje
static int vrsta_kretanja[] = {0,0};

void iscrtaj_put_prvi();
void iscrtaj_put_drugi();
void iscrtaj_auto();
void kretanje_auta(int value);

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
    glutKeyboardUpFunc(on_release);

    timer_active = 0;

    glClearColor(1, 1, 1, 0);
    glEnable(GL_DEPTH_TEST);

    glutMainLoop();

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        exit(0);break;
    case 'S':
        case 's':
            if(!timer_active){
                    glutTimerFunc(30, kretanje_auta ,timer_id1);
                    timer_active = 1;
            };break;
    case 'a':
    case 'A':
        vrsta_kretanja[0] = 1;
        glutPostRedisplay();
        break;
    //skretanje u desno
    case 'd':
    case 'D':
        vrsta_kretanja[1] = 1;
        glutPostRedisplay();
        break;
    }
}

static void on_reshape(int width, int height)
{
    /* Postavlja se viewport. */
    glViewport(0, 0, width, height);

    /* Postavljaju se parametri projekcije. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) width / height, 1, 1500);
}

static void on_release(unsigned char key,int x,int y)
{
    switch(key){
        case 'a':
        case 'A':
            vrsta_kretanja[0] -= 1;break;
        case 'd':
        case 'D': vrsta_kretanja[1] -= 1;break;
    }
}

static void on_display(void)
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Postavlja se vidna tacka. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //2,2,-4
    //2, 0 , 2
    gluLookAt(x_auta, y_auta + 2, z_auta - 4,
                x_auta,y_auta,z_auta + 5,
                0.0,1.0,0.0);

    glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex3f(0,0,0);
        glVertex3f(100,0,0);
        glVertex3f(0,0,0);
        glVertex3f(-100,0,0);
        
        glColor3f(0,1,0);
        glVertex3f(0,0,0);
        glVertex3f(0,100,0);
        glVertex3f(0,0,0);
        glVertex3f(0,-100,0);

        glColor3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,100);
        glVertex3f(0,0,0);
        glVertex3f(0,0,-100);
    glEnd();

    iscrtaj_put_prvi();
    iscrtaj_put_drugi();
    iscrtaj_auto();

    /* Postavlja se nova slika u prozor. */
    glutSwapBuffers();
}

void iscrtaj_put_prvi()
{
    glPushMatrix();
        glColor3f(.2, .2, .2);
        glTranslatef(x_puta_prvog, y_puta_prvog, z_puta_prvog);
        glScalef(4, .25, 40);
        glutSolidCube(1);
    glPopMatrix();
}

void iscrtaj_put_drugi()
{
    glPushMatrix();
        glColor3f(.2, .2, .2);
        glTranslatef(x_puta_drugog, y_puta_drugog, z_puta_drugog);
        glScalef(4, .25, 40);
        glutSolidCube(1);
    glPopMatrix();
}

void iscrtaj_auto()
{
    glPushMatrix();
        glColor3f(1, 0, 0);
        glTranslatef(x_auta, y_auta, z_auta);
        glutSolidSphere(.25,100,100);
    glPopMatrix();
}

void kretanje_auta(int value)
{
    if(value)
        return;

    if(vrsta_kretanja[0] && x_auta < 3.5){
        x_auta += 0.1;
    }
    if(vrsta_kretanja[1] && x_auta > 0.5){
        x_auta -= 0.1;
    }

    z_puta_prvog -= 0.2;
    z_puta_drugog -= 0.2;

    if (z_puta_prvog <= -20){
        z_puta_prvog = 60;
    }

    if (z_puta_drugog <= -20){
        z_puta_drugog = 60;
    }

    glutPostRedisplay();
    if(timer_active) {
        glutTimerFunc(30,kretanje_auta,0);
    }
}