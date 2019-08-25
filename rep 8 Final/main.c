#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#define timer_id1 0
#define nitro_sirina 0.125
#define PREPREKA_NITRO (100)
#define PREPREKA_KAMEN (200)
#define PREPREKA_POEN  (300)
#define TRAJANJE_NITRO (60)

/* Fleg koji odredjuje stanje tajmera. */
static int timer_active;

float boja_r_nitro;
float boja_g_nitro;
float boja_b_nitro;

int SKOR = 0;
int HIGH_SCORE = 0;

int ugao_rotacije_nitra = 0;
int ugao_rotacije_poena = 0;
int ugao_rotacije_tocka = 0;

float nitro_boost = 0.09;
float nitro_boost_levo = 0.02;
float nitro_boost_desno = 0.02;
int nitro_trajanje = 0;
float nitro = 0;
float nitro_levo = 0;
float nitro_desno = 0;

double x_auta = 2;
double y_auta = 0.25;
double z_auta = 0;

float x_puta_prvog = 2;
float y_puta_prvog = -0.125;
float z_puta_prvog = 20;

float x_puta_drugog = 2;
float y_puta_drugog = -0.125;
float z_puta_drugog = 60;

int obrisi = -100;


/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_release(unsigned char key,int x,int y);
static void on_reshape(int width, int height);
static void on_display(void);



typedef struct{
    float x,y,z;
    int vrsta_prepreke;
    int doslo_do_kolizije;
} Prepreka;


Prepreka niz_prepreka_prvi[50];
int rb_prepreke_prva = 0;
Prepreka niz_prepreka_drugi[50];
int rb_prepreke_druga = 0;

// niz koji sluzi za kretanje
static int vrsta_kretanja[] = {0,0};

// PUT
void prepreke_prvi_put();
void prepreke_drugi_put();
// IVICNJAK
void iscrtaj_ivicnjak_levo();
void iscrtaj_ivicnjak_desno();

void iscrtaj_pom_put();
void iscrtaj_put_prvi();
void iscrtaj_put_drugi();
void iscrtaj_auto();
void kretanje_auta(int value);
float udaljenost_pr_auto(Prepreka pr);
float udaljenost_prepreka_prepreka(Prepreka pr1,Prepreka pr2);
void kolizija();

// PREPREKE POMOCNE FJE I PROMENJIVE
int prva_faza = 1;
int prva_faza_trajanje = 900;
int druga_faza = 0;
int druga_faza_trajanje = 500;
int treca_faza = 0;
float generisi_broj();
int da_postavim_prepreku();
int vrsta_prepreke[] = {PREPREKA_NITRO,PREPREKA_KAMEN,PREPREKA_POEN};
int izaberi_prepreku();

int mutiraj_nitro();
int mutiraj_poen();
int u_nizu(float niz[],float prepreka);

void iscrtaj_nitro(float x, float y,float z);
void iscrtaj_kamen(float x, float y,float z);
void iscrtaj_poen(float x, float y,float z);
void iscrtaj_nebo();


FILE *skor_f;

int main(int argc, char **argv)
{
    
//     AKO ZELIMO DA PREBRISEMO FAJL SVREMENA NA VREME MOZEMO OTKOMENTARISATI OVU LINIJU
//     int fd = open("HighScore.txt",O_TRUNC);
//     close(fd);
    skor_f = fopen("HighScore.txt","r");
    int tmp;
    while(fscanf(skor_f,"%d",&tmp) != EOF){
        if(tmp > HIGH_SCORE){
            HIGH_SCORE = tmp;
        }
    }
    fclose(skor_f);
    
    
    GLfloat light_position[] = { 2, 5, 20, 0};
    
    GLfloat light_ambient[] = { 0, 0, 0, 1 };

    GLfloat light_diffuse[] = { 1, 1, 1, 1 };

    GLfloat light_specular[] = { 1, 1, 1, 1 };

    GLfloat model_ambient[] = { 0.4, 0.4, 0.4, 1 };
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    
    srand(time(NULL));

    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
    glutKeyboardUpFunc(on_release);
    
    timer_active = 0;

    glClearColor(1, 1, 1, 0);
    glEnable(GL_DEPTH_TEST);
    
//     Postavljamo osvetljenje
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
    

    glutMainLoop();
    
    

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
//         Upis High score-a u datoteku
        skor_f = fopen("HighScore.txt","a");
        fprintf(skor_f,"%d ",SKOR);
        fclose(skor_f);
        exit(0);
        break;
    case 'S':
    case 's':
            if(!timer_active){
                    glutTimerFunc(30, kretanje_auta ,timer_id1);
                    timer_active = 1;
            };
            break;
//      skretanje u levo
    case 'a':
    case 'A':
        vrsta_kretanja[0] = 1;
        glutPostRedisplay();
        break;
//      skretanje u desno
    case 'd':
    case 'D':
        vrsta_kretanja[1] = 1;
        glutPostRedisplay();
        break;
    case 'x':
    case 'X':
            timer_active = 0;
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
    gluLookAt(x_auta, y_auta + 2, z_auta - 4,
                x_auta,y_auta,z_auta + 5,
                0.0,1.0,0.0);
    
//     iscrtavamo scenu u full screnu
    glutFullScreen();
    
//     SKOR PRINT
    glRasterPos3f(2.5,3.5,5);
    char skor_tekst[50] = "SCORE: ";
    char skor_vrednost[50];
    sprintf(skor_vrednost," %d ",SKOR);
    strcat(skor_tekst,skor_vrednost);

    int duzina = (int)strlen(skor_tekst);

    for(int i = 0; i < duzina ; i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,skor_tekst[i]);
    }
    
// NITRO PRINT
    glRasterPos3f(2.5,4,5);
    char nitro_tekst[50] = "NITRO: ";
    char nitro_vrednost[50];
    sprintf(nitro_vrednost," %d ",nitro_trajanje);
    strcat(nitro_tekst,nitro_vrednost);

    duzina = (int)strlen(nitro_tekst);

    for(int i = 0; i < duzina ; i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,nitro_tekst[i]);
    }
    
//     HIGH SCORE PRINT
    glRasterPos3f(8,1,5);
    char high_skor_tekst[50] = "HIGH SCORE: ";
    char high_skor_vrednost[50];
    sprintf(high_skor_vrednost," %d ",HIGH_SCORE);
    strcat(high_skor_tekst,high_skor_vrednost);

    duzina = (int)strlen(high_skor_tekst);

    for(int i = 0; i < duzina ; i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,high_skor_tekst[i]);
    }
    
//     Kordinatne ose , pomoc pri orjentaciji
//     glBegin(GL_LINES);
//         glColor3f(1,0,0);
//         glVertex3f(0,0,0);
//         glVertex3f(100,0,0);
//         glVertex3f(0,0,0);
//         glVertex3f(-100,0,0);
//         
//         glColor3f(0,1,0);
//         glVertex3f(0,0,0);
//         glVertex3f(0,100,0);
//         glVertex3f(0,0,0);
//         glVertex3f(0,-100,0);
// 
//         glColor3f(0,0,1);
//         glVertex3f(0,0,0);
//         glVertex3f(0,0,100);
//         glVertex3f(0,0,0);
//         glVertex3f(0,0,-100);
//     glEnd();

    iscrtaj_ivicnjak_levo();
    iscrtaj_ivicnjak_desno();
    
    glDisable(GL_LIGHTING);
    iscrtaj_put_prvi();
    iscrtaj_put_drugi();
    iscrtaj_pom_put();
    
    iscrtaj_auto();
    iscrtaj_nebo();
    
    
//     Prolazak kroz niz prepreka na prvoj ravni i njihovo iscrtavanje
    for (int i = 0 ; i < rb_prepreke_prva ; i ++ ) {
        Prepreka pr = niz_prepreka_prvi[i];
        switch(pr.vrsta_prepreke){
            case PREPREKA_NITRO: 
                if(pr.doslo_do_kolizije == 1){
                    iscrtaj_nitro(pr.x,pr.y + obrisi,pr.z);
                }else{
                    iscrtaj_nitro(pr.x,pr.y,pr.z);
                }
                break;
            case PREPREKA_KAMEN:
                iscrtaj_kamen(pr.x,pr.y,pr.z);
                break;
            case PREPREKA_POEN:
                glEnable(GL_LIGHTING);
                if(pr.doslo_do_kolizije == 1){
                    iscrtaj_poen(pr.x,pr.y + obrisi,pr.z);
                }else{
                    iscrtaj_poen(pr.x,pr.y,pr.z);
                }
                 glDisable(GL_LIGHTING);
                break;
        }
    }
    
//     Prolazak kroz niz prepreka na drugoj ravni i njihovo iscrtavanje
    for (int i = 0 ; i < rb_prepreke_druga ; i ++ ) {
        Prepreka pr = niz_prepreka_drugi[i];
        switch(pr.vrsta_prepreke){
            case PREPREKA_NITRO: 
                if(pr.doslo_do_kolizije == 1){
                    iscrtaj_nitro(pr.x,pr.y + obrisi,pr.z);
                }else{
                    iscrtaj_nitro(pr.x,pr.y,pr.z);
                }
                break;
            case PREPREKA_KAMEN:
                iscrtaj_kamen(pr.x,pr.y,pr.z);
                break;
            case PREPREKA_POEN:
                glEnable(GL_LIGHTING);
                if(pr.doslo_do_kolizije == 1){
                    iscrtaj_poen(pr.x,pr.y + obrisi,pr.z);
                }else{
                    iscrtaj_poen(pr.x,pr.y,pr.z);
                }
                glDisable(GL_LIGHTING);
                break;
        }
    }
    glEnable(GL_LIGHTING);
    /* Postavlja se nova slika u prozor. */
    glutSwapBuffers();
}

void iscrtaj_pom_put(){
    
    glPushMatrix();
        glColor3f(.2, .2, .2);
        glTranslatef(x_puta_prvog, y_puta_prvog, z_puta_prvog-25);
        glScalef(4, .25, 10);
        glutSolidCube(1);
    glPopMatrix();
    
}

void iscrtaj_put_prvi()
{
    glPushMatrix();
        glColor3f(.2, .2, .2);
        glTranslatef(x_puta_prvog, y_puta_prvog, z_puta_prvog);
        glScalef(4, .25, 42);
        glutSolidCube(1);
    glPopMatrix();
}

void iscrtaj_ivicnjak_desno(){
    
     glPushMatrix();
     
        glTranslatef(-12, 0.1, 0);
        glScalef(24, 1, 84);
        
        GLfloat material_ambient[] = { 0.0215, 0.1745, 0.0215, 1 };
        GLfloat material_specular[] = { 0.633, 0.727811, 0.633, 1 };
        GLfloat material_diffuse[] = { 0.07568, 0.61424, 0.07568, 1 };
        GLfloat low_shininess[] = { 5 };
        GLfloat material_emission[] = { 0.3, 0.2, 0.2, 0 };
        
        if(nitro != 0){
            material_ambient[0] = 0.8;
            material_ambient[1] =0;
            material_ambient[2] =0;
            material_ambient[3] = 1;
            material_specular[0] =0.7;
            material_specular[1] =0.7;
            material_specular[2] =0.7;
            material_specular[3] =1;
            material_diffuse[0] =0.9;
            material_diffuse[1] =0;
            material_diffuse[2] =0;
            material_diffuse[3] =1;
            
        }
        
        glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);
        
        glutSolidCube(1);
        
    glPopMatrix();
}
void iscrtaj_ivicnjak_levo(){
      glPushMatrix();
      
        glTranslatef(16, 0.1, 0);
        glScalef(24, 1, 84);
        
        GLfloat material_ambient[] = { 0.0215, 0.1745, 0.0215, 1 };
        GLfloat material_specular[] = { 0.633, 0.727811, 0.633, 1 };
        GLfloat material_diffuse[] = { 0.07568, 0.61424, 0.07568, 1 };
        GLfloat low_shininess[] = { 5 };
        GLfloat material_emission[] = { 0.3, 0.2, 0.2, 0 };
        
        if(nitro != 0){
                   material_ambient[0] = 0.8;
            material_ambient[1] =0;
            material_ambient[2] =0;
            material_ambient[3] = 1;
            material_specular[0] =0.7;
            material_specular[1] =0.7;
            material_specular[2] =0.7;
            material_specular[3] =1;
            material_diffuse[0] =0.9;
            material_diffuse[1] =0;
            material_diffuse[2] =0;
            material_diffuse[3] =1;
            
        }
        
        glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);
        
        glutSolidCube(1);
        
    glPopMatrix();
}

void iscrtaj_put_drugi()
{
    glPushMatrix();
        glTranslatef(x_puta_drugog, y_puta_drugog, z_puta_drugog);
        glScalef(4, .25, 42);
        glutSolidCube(1);
    glPopMatrix();
}

void iscrtaj_auto()
{
//     Postavljanje rotacije tocka
    if(nitro){
        ugao_rotacije_tocka += 4;
        ugao_rotacije_tocka = ugao_rotacije_tocka % 360;
    }else{
        ugao_rotacije_tocka += 2;
        ugao_rotacije_tocka = ugao_rotacije_tocka % 360;
    }
    
//     kabina
    glPushMatrix();
    
        if(nitro){
            glColor3f(0.05,0.03,0.1);
        }else{
             glColor3f(0.9, 0.9, 0.95);
        }
        
        glTranslatef(x_auta , y_auta + 0.1, z_auta - 0.3);
        glutSolidSphere(0.1,20,5);
        
    glPopMatrix();
    
//     skoljka
    glPushMatrix();
    
        if(nitro){
            glColor3f(0.92,0.1,0.15);
        }else{
             glColor3f(0.05,0.03,0.1);
        }
        
        glTranslatef(x_auta, y_auta, z_auta);
        glScalef(0.2, .15, 0.8);
        glutSolidCube(1);
        
    glPopMatrix();
    
//     hauba 
    glPushMatrix();
    
        glColor3f(0,0,0);
        glTranslatef(x_auta, y_auta, z_auta + 0.2);
        glScalef(0.12, .15, 0.3);
        glutSolidCube(1);
        
    glPopMatrix();
    
//     spojler
    glPushMatrix();
    
        glColor3f(0,0,0);
        glTranslatef(x_auta , y_auta + 0.2, z_auta - 0.8);
        glScalef(0.5, .01, 0.1);
        glutSolidCube(1);
        
    glPopMatrix();
    
//     spojler levo krilce
     glPushMatrix();
     
       if(nitro){
            glColor3f(0.92,0.1,0.15);
        }else{
             glColor3f(0.05,0.03,0.1);
        }
        
        glTranslatef(x_auta+0.2 , y_auta + 0.3, z_auta - 1.04);
        glScalef(0.1, .01, 0.2);
        glutSolidCube(1);
        
    glPopMatrix();
    
//     spojler desno krilce
     glPushMatrix();
     
        if(nitro){
            glColor3f(0.92,0.1,0.15);
        }else{
             glColor3f(0.05,0.03,0.1);
        }
        
        glTranslatef(x_auta-0.2 , y_auta + 0.3, z_auta - 1.04);
        glScalef(0.1, .01, 0.2);
        glutSolidCube(1);
        
    glPopMatrix();

    
//     prednji levi delic
    glPushMatrix();
    
        glColor3f(0,0,0);
        glTranslatef(x_auta - 0.1, y_auta, z_auta + 0.2);
        glScalef(0.12, 0.04, 0.04);
        glutSolidCube(1);
        
    glPopMatrix();
    
//     prednji levi
    glPushMatrix();
    
        glColor3f(0.05,0.03,0.1);
        glTranslatef(x_auta + 0.2, y_auta, z_auta + 0.2);
        glRotatef(ugao_rotacije_tocka,1,0,0);
        glutWireSphere(0.06,35,5);
        
    glPopMatrix();
    
//     prednji desni delic
    glPushMatrix();
    
        glColor3f(0,0,0);
        glTranslatef(x_auta + 0.1, y_auta, z_auta + 0.2);
        glScalef(0.12, 0.04, 0.04);
        glutSolidCube(1);
        
    glPopMatrix();
    
//     prednji desni
    glPushMatrix();
    
        glColor3f(0.05,0.03,0.1);
        glTranslatef(x_auta-0.2, y_auta, z_auta+0.2);
        glRotatef(ugao_rotacije_tocka,1,0,0);
        glutWireSphere(0.06,35,5);
        
    glPopMatrix();
    
//     zadnji desni delic
    glPushMatrix();
    
        glColor3f(0,0,0);
        glTranslatef(x_auta - 0.1, y_auta, z_auta - 0.2);
        glScalef(0.12, 0.04, 0.04);
        glutSolidCube(1);
        
    glPopMatrix();
    
//     zadnji desni
     glPushMatrix();
     
        glColor3f(0.05,0.03,0.1);
        glTranslatef(x_auta-0.25, y_auta, z_auta-0.2);
        glRotatef(ugao_rotacije_tocka,1,0,0);
        glutWireSphere(0.1,70,10);
        
    glPopMatrix();
    
    
    //     zadnji levi delic
    glPushMatrix();
    
        glColor3f(0,0,0);
        glTranslatef(x_auta + 0.1, y_auta, z_auta - 0.2);
        glScalef(0.12, 0.04, 0.04);
        glutSolidCube(1);
        
    glPopMatrix();
    
//     zadnji levi
    glPushMatrix();
    
        glColor3f(0.05,0.03,0.1);
        glTranslatef(x_auta+0.25, y_auta, z_auta-0.2);
        glRotatef(ugao_rotacije_tocka,1,0,0);
        glutWireSphere(0.1,70,10);
        
    glPopMatrix();

   
    
}

void iscrtaj_nebo(){
    glPushMatrix();
        glColor3f(0.439216, 0.858824, 0.576471);
        glTranslatef(2, 0, 0);
        glutSolidSphere(40, 30, 30);
    glPopMatrix();
}

void kretanje_auta(int value)
{
    if(value)
        return;
    
//     sprecavanje da auto moze da probije postavljene margine
    if(vrsta_kretanja[0] && x_auta < 3.58){
        x_auta += 0.07 + nitro_levo;
    }
    if(vrsta_kretanja[1] && x_auta > 0.42){
        x_auta -= 0.07 + nitro_desno;
    }
    
//     regulisanje trajanja nitra
    if(nitro_trajanje != 0){
        nitro_trajanje--;
    }else if(nitro_trajanje == 0){
        nitro = 0;
        nitro_levo = 0;
        nitro_desno = 0;
    }
    
//     simulacija kretanja auta
    z_puta_prvog -= 0.25 + nitro;
    z_puta_drugog -= 0.25 + nitro;
    
//     regulisanje I i II faze
    if(prva_faza){
        prva_faza_trajanje--;
        if(prva_faza_trajanje == 0){
            prva_faza = 0;
            druga_faza = 1;
        }
    }
    
    if(druga_faza == 1){
        druga_faza_trajanje--;
        if(druga_faza_trajanje == 0){
            druga_faza = 0;
            treca_faza = 1;
        }
    }
    
//     ispis u terminalu: trajanje I, II i pocetak III faze , skora i trajanja nitra
    if(!treca_faza){
        printf("Prva faza: %d Druga faza: %d Treca faza: %d\n",prva_faza,druga_faza,treca_faza);
        printf("Trajanje 1.faza: %d Trajanje 2.faza: %d\n",prva_faza_trajanje,druga_faza_trajanje);
    }else{
        printf("TRECAN FAZA AKTIVNA!!! :D \n");
    }
    printf("NITRO ACTIVE:--%d--\n",nitro_trajanje);
    printf("**SKOR** ----> %d\n",SKOR);
    
//     kretanje prepreka
    for (int i = 0 ; i < rb_prepreke_prva ; i++) {
        niz_prepreka_prvi[i].z -= 0.25 + nitro;
    }

    for (int i = 0 ; i < rb_prepreke_druga ; i++) {
        niz_prepreka_drugi[i].z -= 0.25 + nitro;
    }
    
//     naizmenicno crtanje puteva
    if (z_puta_prvog <= -20){
        z_puta_prvog = 60;
        prepreke_prvi_put();
    }

    if (z_puta_drugog <= -20){
        z_puta_drugog = 60;
        prepreke_drugi_put();
    }
    
    kolizija();

    glutPostRedisplay();
    if(timer_active) {
        glutTimerFunc(20,kretanje_auta,0);
    }
}

void prepreke_prvi_put()
{
    //broj prepreka koji moze da se generise
    int broj_prepreka = 0;
    //broj prepreka u nizu struktura
    rb_prepreke_prva = 0; 
    
    for(int i = 7 ; i < 42 ; i += 7){
        
        if(prva_faza){
            if(!da_postavim_prepreku()){
                continue;
            }
        }
        
        if(druga_faza){
            if(!da_postavim_prepreku()){
                continue;
            }
        }
        
        broj_prepreka = ((int)rand() % 3) + 1;
        int trenutna_prepreka = 0;
        
//        UBACENO ZA SPRECAVANJE DA SE 2 PREPREKE NADJU NA ISTOM MESTU
        float niz_pozicija[] = {0,0,0};

        while(trenutna_prepreka < broj_prepreka){
            Prepreka pr;
            
            int prepreka_broj = izaberi_prepreku();
            pr.vrsta_prepreke = vrsta_prepreke[prepreka_broj];
    //mutacija - u smislu da prioritiziram neke prepreke od drugih
            if(pr.vrsta_prepreke == PREPREKA_NITRO){
                if(mutiraj_nitro()){
                    pr.vrsta_prepreke = PREPREKA_POEN;
                }
            }
            
            if(pr.vrsta_prepreke == PREPREKA_POEN){
                if(mutiraj_poen()){
                    pr.vrsta_prepreke = PREPREKA_KAMEN;
                }
            }
            

            float tmp = 0 + generisi_broj();
            
//             SPRECAVA DA SE 2 PREPREKE NADJU NA ISTOM MESTU
                while(u_nizu(niz_pozicija,tmp)){
                    tmp = 0 + generisi_broj();
                }

            pr.x = tmp;
            pr.y = 0.125;
            pr.z = z_puta_prvog + i - 19.5;
            pr.doslo_do_kolizije = 0;
            niz_prepreka_prvi[rb_prepreke_prva++] = pr;
            niz_pozicija[trenutna_prepreka] = tmp;
            trenutna_prepreka++;
        }
    }
}

void prepreke_drugi_put()
{
    
//broj prepreka koji moze da se generise
    int broj_prepreka = 0;
//broj prepreka u nizu struktura
    rb_prepreke_druga = 0;
    
    for(int i = 7 ; i < 42 ; i += 7){
        
         if(prva_faza){
            if(!da_postavim_prepreku()){
                continue;
            }
        }
        
        broj_prepreka = ((int)rand() % 3) + 1;
        int trenutna_prepreka = 0;
        
//        UBACENO ZA SPRECAVANJE DA SE 2 PREPREKE NADJU NA ISTOM MESTU
        float niz_pozicija[] = {0,0,0};

        
        while(trenutna_prepreka < broj_prepreka){
            Prepreka pr;
            
             int prepreka_broj = izaberi_prepreku();
            pr.vrsta_prepreke = vrsta_prepreke[prepreka_broj];
            
            
            //mutacija - u smislu da prioritiziram neke prepreke od drugih
            if(pr.vrsta_prepreke == PREPREKA_NITRO){
                if(mutiraj_nitro()){
                    pr.vrsta_prepreke = PREPREKA_POEN;
                }
            }
              if(pr.vrsta_prepreke == PREPREKA_POEN){
                if(mutiraj_poen()){
                    pr.vrsta_prepreke = PREPREKA_KAMEN;
                }
            }
            
            float tmp = 0 + generisi_broj();
            
                
//             SPRECAVA DA SE 2 PREPREKE NADJU NA ISTOM MESTU
            while(u_nizu(niz_pozicija,tmp)){
                tmp = 0 + generisi_broj();
            }
            
            pr.x = tmp;
            pr.y = 0.125;
            pr.z = z_puta_drugog + i - 19.5;
            pr.doslo_do_kolizije = 0;
            niz_prepreka_drugi[rb_prepreke_druga++] = pr;
            niz_pozicija[trenutna_prepreka] = tmp;
            trenutna_prepreka++;
        }
    }
}

void iscrtaj_kamen(float x, float y,float z){
    glPushMatrix();
    
        if(nitro){
            glColor3f(0.92,0.1,0.15);
        }else{
            glColor3f( 0.658824, 0.658824, 0.658824);
        }
        
        glTranslatef(x, y - 0.125, z);
        glutSolidSphere(.125, 30, 30);
        
    glPopMatrix();
}

void iscrtaj_poen(float x, float y,float z){
    
        ugao_rotacije_poena += 3;
        ugao_rotacije_poena = ugao_rotacije_poena % 360;
        
    glPushMatrix();
    
        glTranslatef(x, y, z);
        glRotatef(ugao_rotacije_poena,0,1,0);
        
        GLfloat material_ambient[] = { 0.4, 0.4, 0.4, 1 };
        GLfloat material_specular[] = { 0.05, 0.05, 0.05, 1 };
        GLfloat material_diffuse[] = {0.75164, 0.60648, 0.22648, 1 };
        GLfloat low_shininess[] = { 100 };
        GLfloat material_emission[] = { 0.3, 0.2, 0.2, 0 };
        
        glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);
        
        glutSolidSphere(.125, 30, 30);
        
    glPopMatrix();
}

float udaljenost_pr_auto(Prepreka pr)
{
    float x = powf((pr.x - x_auta), 2);
    float y = powf((pr.y - y_auta), 2);
    float z = powf((pr.z - z_auta), 2);

    return sqrtf(x + y + z);
}

float udaljenost_prepreka_prepreka(Prepreka pr1,Prepreka pr2){
    
    float x = powf((pr1.x - pr2.x), 2);
    float y = powf((pr1.y - pr2.y), 2);
    float z = powf((pr1.z - pr2.z), 2);
    
    return sqrtf(x + y + z);
}

void kolizija()
{
    if (z_puta_prvog < z_puta_drugog){
        for(int i = 0 ; i < rb_prepreke_prva ; i++){
            if (udaljenost_pr_auto(niz_prepreka_prvi[i]) <= 0.4){
                Prepreka pr = niz_prepreka_prvi[i];
                switch(pr.vrsta_prepreke){
                    
                    case PREPREKA_NITRO:
                        niz_prepreka_prvi[i].doslo_do_kolizije = 1;
                        if(nitro_trajanje){
                            nitro += nitro_boost;
                            nitro_levo += nitro_boost_levo;
                            nitro_desno += nitro_boost_desno;
                            nitro_trajanje += TRAJANJE_NITRO;
                            break;
                        }
                        nitro = nitro_boost;
                        nitro_levo = nitro_boost_levo;
                        nitro_desno = nitro_boost_desno;
                        nitro_trajanje = TRAJANJE_NITRO;
                        break;
                        
                    case PREPREKA_KAMEN:
                        printf("Udario si u kamen: %d\n",pr.vrsta_prepreke);
                        timer_active = 0;
                        break;
                        
                    case PREPREKA_POEN:
                        if(nitro){
                            SKOR += 20;
                        }else{
                            SKOR += 5;
                        }
                        niz_prepreka_prvi[i].doslo_do_kolizije = 1;
                        break;
                }
                
            }
        }
    }
    else{
        for(int i = 0 ; i < rb_prepreke_druga ; i++){
            if (udaljenost_pr_auto(niz_prepreka_drugi[i]) <= 0.4){
                Prepreka pr = niz_prepreka_drugi[i];
                switch(pr.vrsta_prepreke){
                    
                    case PREPREKA_NITRO:
                        niz_prepreka_drugi[i].doslo_do_kolizije = 1;
                        if(nitro_trajanje){
                            nitro += nitro_boost;
                            nitro_levo += nitro_boost_levo;
                            nitro_desno += nitro_boost_desno;
                            nitro_trajanje += TRAJANJE_NITRO;
                            break;
                        }
                        nitro = nitro_boost;
                        nitro_levo = nitro_boost_levo;
                        nitro_desno = nitro_boost_desno;
                        nitro_trajanje = TRAJANJE_NITRO;
                        break;
                        
                    case PREPREKA_KAMEN:
                        printf("Udario si u kamen: %d\n",pr.vrsta_prepreke);
                        timer_active = 0;
                        break;
                    case PREPREKA_POEN:
                        SKOR += 5;
                        niz_prepreka_drugi[i].doslo_do_kolizije = 1;
                        break;
                }
            }
        }
    }

}

// pomocna fja sluzi za spontano generisanje prepreka
int da_postavim_prepreku(){
    int da_ili_ne = (int)rand() % 2;
    return da_ili_ne;
}

// pomocna fja sluzi za spontano generisanje prepreka
float generisi_broj(){
    float niz[] = {0.25,1.1,1.95,2.8,3.65};
    int broj_izbora = 5;
    int index_izbora = (int)rand() % broj_izbora;
    float izbor = niz[index_izbora];
    return izbor;
}

// pomocna fja sluzi za spontano generisanje prepreka
int izaberi_prepreku(){
    int br_prepreka = 3;
    int ran = (int)rand() % br_prepreka;
    return ran;
}

// pomocna fja sluzi za spontano generisanje prepreka
int u_nizu(float niz[],float prepreka){
    for(int i = 0;i < 3;i++){
        if(prepreka == niz[i]){
            return 1;
        }
    }
    return 0;
}

// fja koja sluzi za prioritizaciju poena prepreke
int mutiraj_nitro(){
    int posto = (int)rand() % 101;
    if(posto > 15){
        return 1;
    }
    return 0;
}

// fja koja sluzi za prioritizaciju prepreka
int mutiraj_poen(){
    int posto = (int)rand() % 101;
    if(posto > 70){
        return 1;
    }
    return 0;
}

void iscrtaj_nitro(float x,float y,float z){
    
    ugao_rotacije_nitra += 2;
    ugao_rotacije_nitra = ugao_rotacije_nitra % 360;
    
    glPushMatrix();
    
        glColor3f(0, 0, 1);
        glTranslatef(x, y + 0.1, z);
        glRotatef(ugao_rotacije_nitra,0,1,0);
        glScalef(0.2,0.4,0.13);
        glutSolidCube(1);
        
    glPopMatrix();
    
    glPushMatrix();
    
        glColor3f(0, 0, 1);
        glTranslatef(x, y + 0.3, z);
        glRotatef(ugao_rotacije_nitra,0,1,0);
        glScalef(0.1,0.3,0.13);
        glutSolidCube(1);
        
    glPopMatrix();
    
    glPushMatrix();
    
        glColor3f(0, 0, 1);
        glTranslatef(x, y + 0.5, z);
        glRotatef(ugao_rotacije_nitra,0,1,0);
        glScalef(0.25,0.1,0.13);
        glutSolidCube(1);
        
    glPopMatrix();
    
}
