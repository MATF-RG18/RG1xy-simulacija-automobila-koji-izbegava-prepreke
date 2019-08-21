#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define timer_id1 0

#define nitro_sirina 0.125
#define PREPREKA_NITRO (100)
#define PREPREKA_KAMEN (200)
#define PREPREKA_POEN  (300)
#define TRAJANJE_NITRO (200)

int SKOR = 0;


int obrisi = -100;
int u_nizu(float niz[],float prepreka);

float nitro_boost = 0.3;
float nitro_boost_levo = 0.05;
float nitro_boost_desno = 0.05;
int nitro_trajanje = 0;

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

typedef struct{
    float x,y,z;
    int vrsta_prepreke;
    int doslo_do_kolizije;
} Prepreka;

float nitro = 0;
float nitro_levo = 0;
float nitro_desno = 0;

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

void iscrtaj_nitro(float x, float y,float z);
void iscrtaj_kamen(float x, float y,float z);
void iscrtaj_poen(float x, float y,float z);
void iscrtaj_nebo();


int main(int argc, char **argv)
{
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
    case 'x':
    case 'X':
            timer_active = 0;
            break;
    }
}
      //mutacija - u smislu da prioritiziram neke prepreke od drugih
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
      //mutacija - u smislu da prioritiziram neke prepreke od drugih
    /* Postavlja se vidna tacka. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //2,2,-4
    //2, 0 , 2
    gluLookAt(x_auta, y_auta + 2, z_auta - 4,
                x_auta,y_auta,z_auta + 5,
                0.0,1.0,0.0);
    
    glutFullScreen();
    
//     SKOR PRINT
    glRasterPos3f(2.5,3.5,5);
    char skor_tekst[50] = "SCORE: ";
    char skor_vrednost[50];
    sprintf(skor_vrednost," %d ",SKOR);
    strcat(skor_tekst,skor_vrednost);

    int duzina = (int)strlen(skor_tekst);

    for(int i = 0; i < duzina ; i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,skor_tekst[i]);
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
    iscrtaj_nebo();
    iscrtaj_ivicnjak_levo();
    iscrtaj_ivicnjak_desno();
    

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
                if(pr.doslo_do_kolizije == 1){
                    iscrtaj_poen(pr.x,pr.y + obrisi,pr.z);
                }else{
                    iscrtaj_poen(pr.x,pr.y,pr.z);
                }
                break;
        }
    }

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
                if(pr.doslo_do_kolizije == 1){
                    iscrtaj_poen(pr.x,pr.y + obrisi,pr.z);
                }else{
                    iscrtaj_poen(pr.x,pr.y,pr.z);
                }
                break;
        }
    }

    /* Postavlja se nova slika u prozor. */
    glutSwapBuffers();
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

void iscrtaj_ivicnjak_levo(){
     glPushMatrix();
        glColor3f(1, 0, 0);
        glTranslatef(-12, 0.4, 0);
        glScalef(24, 1, 84);
        glutSolidCube(1);
    glPopMatrix();
}
void iscrtaj_ivicnjak_desno(){
      glPushMatrix();
        glColor3f(1, 0, 0);
        glTranslatef(16, 0.4, 0);
        glScalef(24, 1, 84);
        glutSolidCube(1);
    glPopMatrix();
}

void iscrtaj_put_drugi()
{
    glPushMatrix();
        glColor3f(.2, .2, .2);
        glTranslatef(x_puta_drugog, y_puta_drugog, z_puta_drugog);
        glScalef(4, .25, 42);
        glutSolidCube(1);
    glPopMatrix();
}

void iscrtaj_auto()
{
    
    glPushMatrix();
        glColor3f(1, 0, 0);
        glTranslatef(x_auta, y_auta, z_auta);
        glScalef(0.4, .25, 0.8);
        glutSolidCube(1);
    glPopMatrix();
    
      glPushMatrix();
        glColor3f(0, 1, 0);
        glTranslatef(x_auta, y_auta + 0.1, z_auta - 0.2);
        glScalef(0.4, 0.5, 0.3);
        glutSolidCube(1);
    glPopMatrix();
    
    
//     raniji kod
//     glPushMatrix();
//         glColor3f(1, 0, 0);
//         glTranslatef(x_auta, y_auta, z_auta);
//         glutSolidSphere(.25,100,100);
//     glPopMatrix();
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

    if(vrsta_kretanja[0] && x_auta < 3.8){
        x_auta += 0.07 + nitro_levo;
    }
    if(vrsta_kretanja[1] && x_auta > 0.2){
        x_auta -= 0.07 + nitro_desno;
    }
    
    
    if(nitro_trajanje != 0){
        nitro_trajanje--;
    }else if(nitro_trajanje == 0){
        nitro = 0;
        nitro_levo = 0;
        nitro_desno = 0;
    }

    z_puta_prvog -= 0.25 + nitro;
    z_puta_drugog -= 0.25 + nitro;
    printf("BRZINA:%f\n",0.25 + nitro);
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
    
    if(!treca_faza){
        printf("Prva faza: %d Druga faza: %d Treca faza: %d\n",prva_faza,druga_faza,treca_faza);
        printf("Trajanje 1.faza: %d Trajanje 2.faza: %d\n",prva_faza_trajanje,druga_faza_trajanje);
    }else{
        printf("TRECAN FAZA AKTIVNA!!! :D \n");
    }
    printf("NITRO ACTIVE:--%d--\n",nitro_trajanje);
    printf("**SKOR** ----> %d\n",SKOR);

    for (int i = 0 ; i < rb_prepreke_prva ; i++) {
        niz_prepreka_prvi[i].z -= 0.25 + nitro;
    }

    for (int i = 0 ; i < rb_prepreke_druga ; i++) {
        niz_prepreka_drugi[i].z -= 0.25 + nitro;
    }

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
    int broj_prepreka = 0;
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

// void iscrtaj_nitro(float x, float y, float z)
// {
//     glPushMatrix();
//         glColor3f(0, 0, 1);
//         glTranslatef(x, y, z);
//         glutSolidSphere(.125, 30, 30);
//     glPopMatrix();
// }

void iscrtaj_kamen(float x, float y,float z){
    glPushMatrix();
        glColor3f( 0.658824, 0.658824, 0.658824);
        glTranslatef(x, y - 0.125, z);
        glutSolidSphere(.125, 30, 30);
    glPopMatrix();
}

void iscrtaj_poen(float x, float y,float z){
    glPushMatrix();
        glColor3f(1, 1, 0);
        glTranslatef(x, y, z);
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
            if (udaljenost_pr_auto(niz_prepreka_prvi[i]) <= 0.325){
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
                        SKOR += 5;
                        niz_prepreka_prvi[i].doslo_do_kolizije = 1;
                        break;
                }
                
            }
        }
    }
    else{
        for(int i = 0 ; i < rb_prepreke_druga ; i++){
            if (udaljenost_pr_auto(niz_prepreka_drugi[i]) <= 0.325){
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
                    case PREPREKA_POEN:
                        SKOR += 5;
                        niz_prepreka_drugi[i].doslo_do_kolizije = 1;
                        break;
                }
            }
        }
    }

}
int da_postavim_prepreku(){
    int da_ili_ne = (int)rand() % 2;
    return da_ili_ne;
}

float generisi_broj(){
    float niz[] = {0.25,1.1,1.95,2.8,3.65};
    int broj_izbora = 5;
    int index_izbora = (int)rand() % broj_izbora;
    float izbor = niz[index_izbora];
    return izbor;
}

int izaberi_prepreku(){
    int br_prepreka = 3;
    int ran = (int)rand() % br_prepreka;
    return ran;
}

int u_nizu(float niz[],float prepreka){
    for(int i = 0;i < 3;i++){
        if(prepreka == niz[i]){
            return 1;
        }
    }
    return 0;
}

int mutiraj_nitro(){
    int posto = (int)rand() % 101;
    if(posto > 15){
        return 1;
    }
    return 0;
}

int mutiraj_poen(){
    int posto = (int)rand() % 101;
    if(posto > 70){
        return 1;
    }
    return 0;
}

void iscrtaj_nitro(float x,float y,float z){
    glPushMatrix();
        glColor3f(0, 0, 1);
        glTranslatef(x, y + 0.1, z);
        glScalef(0.2,0.4,0.1);
        glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
        glColor3f(0, 0, 1);
        glTranslatef(x, y + 0.3, z);
        glScalef(0.1,0.1,0.1);
        glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
        glColor3f(0, 0, 1);
        glTranslatef(x, y + 0.4, z);
        glScalef(0.25,0.1,0.1);
        glutSolidCube(1);
    glPopMatrix();
    
}
