/********************************************************/
/*                     cube.cpp                                                 */
/********************************************************/
/*                Affiche a l'ecran un cube en 3D                      */
/********************************************************/

/* inclusion des fichiers d'en-tete freeglut */

#ifdef __APPLE__
#include <GLUT/glut.h> /* Pour Mac OS X */
#else
#include <GL/freeglut.h>   /* Pour les autres systemes */
#endif
#include <cstdlib>
#include <math.h>
int mv=0;

/*class Point*/
class Point
{
public :
    //coordonnées x, y et z du point
    double x;
    double y;
    double z;
    // couleur r, v et b du point
    float r;
    float g;
    float b;
};
//Tableau pour stocker les sommets du cube et leur couleur
Point pCube[8]=
{
    {-0.5,-0.5, 0.5,1.0,0.0,0.0},
    { 0.5, -0.5, 0.5,0.0,1.0,0.0},
    { 0.5, -0.5, -0.5,0.0,0.0,1.0},
    { -0.5, -0.5, -0.5,1.0,1.0,1.0},
    { -0.5,0.5, 0.5,1.0,0.0,0.0},
    { 0.5, 0.5, 0.5,0.0,1.0,0.0},
    { 0.5, 0.5, -0.5,0.0,0.0,1.0},
    { -0.5, 0.5, -0.5,1.0,1.0,1.0}
};

//Tableau pour stocker les indices des sommets par face pour le cube
int fCube[6][4]=
{
    {0,3,2,1},
    {0,1,5,4},
    {1,2,6,5},
    {2,3,7,6},
    {0,4,7,3},
    {4,5,6,7}
};

char presse;
int anglex,angley,x,y,xold,yold;

/* Prototype des fonctions */
void affichage();
void clavier(unsigned char touche,int x,int y);
void reshape(int x,int y);
void idle();
void mouse(int bouton,int etat,int x,int y);
void mousemotion(int x,int y);
void drawHalfSphere(int scalex,int scaley,GLfloat rayon);
void torus(int numc,int numt);
void DrawPartialCylindre(float rHaut, float rbase,float height, float partial, int numberSegment);

int main(int argc,char **argv)
{
//glEnable(GL_LIGHTING);
//glEnable(GL_COLOR_MATERIAL);
    /* initialisation de glut et creation
       de la fenetre */
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(200,200);
    glutInitWindowSize(500,500);
    glutCreateWindow("cube");

    /* Initialisation d'OpenGL */
    glClearColor(0.0,0.0,0.0,0.0);
    glColor3f(1.0,1.0,1.0);
    glPointSize(2.0);
    glEnable(GL_DEPTH_TEST);

    /* enregistrement des fonctions de rappel */
    glutDisplayFunc(affichage);
    glutKeyboardFunc(clavier);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mousemotion);
    glutIdleFunc(idle);

    /* Entree dans la boucle principale glut */
    glutMainLoop();
    return 0;
}
int incr=1;
void idle(void)
{
    if(mv>180)
        incr=-1;
    if(mv<-180)
        incr=1;
    mv+=incr;
    glutPostRedisplay();
}


void affichage()
{

//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, {255,255,0});
    int i,j;
    /* effacement de l'image avec la couleur de fond */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);

    glLoadIdentity();
    glRotatef(angley,1.0,0.0,0.0);
    glRotatef(anglex,0.0,1.0,0.0);

    glRotatef(90,2.0,0.0,0.0);

    //glRotatef(mv,0.0,0.0,1.0);

    int v=50;
    int ecartEntrePied = 3;
    glOrtho(-v, v,-v, v, -v-2,v-2);
    glColor3f(0.5,0.5,0.5);

    glPushMatrix();
    /*glScalef(2,1,0.5);
    glutSolidCube(1);
    glPopMatrix();*/

    //right foot
    glRotatef(90,1,0,0);
    glTranslatef(ecartEntrePied,0,0);
    drawHalfSphere(24,24,2.5);
    glPopMatrix();

    //left foot
    glPushMatrix();
    glRotatef(90,1,0,0);
    glTranslatef(-ecartEntrePied,0,0);
    drawHalfSphere(24,24,2.5);

    //first part left leg
    glTranslatef(0,1,0);
    glRotatef(-90,1,0,0);
    glutSolidCylinder(1,7,30,20);

    //knee
    glColor3f(0.6,0.6,0.6);
    glTranslatef(0,0,8);
    glutSolidSphere(1,30,30);

    //second part left leg
    glColor3f(0.5,0.5,0.5);
    glTranslatef(0,0,1);
    glutSolidCylinder(1,7,30,20);
    glPopMatrix();

    glPushMatrix();
    //first part right leg
    glTranslatef(ecartEntrePied,0,1);
    glutSolidCylinder(1,7,30,20);

    //knee
    glColor3f(0.6,0.6,0.6);
    glTranslatef(0,0,8);
    glutSolidSphere(1,30,30);

    //second part right leg
    glColor3f(0.5,0.5,0.5);
    glTranslatef(0,0,1);
    glutSolidCylinder(1,7,30,20);
    glPopMatrix();

    //chest
    //glPushMatrix();
    glTranslatef(0,0,17); //19=1+2+8+8
    DrawPartialCylindre(5.5,4.5,15,1,50);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.6,0.6,0.6);
    glTranslatef(0,-0.3,2.5);
    glScalef(0.85,0.95,1);
    DrawPartialCylindre(5.8,5,9.5,1,50);
    glPopMatrix();

    //right shoulder
    glPushMatrix();
    glTranslatef(5,0,12.5); //5.5 = rayon du premier cylindre et 12.9=hauteurCylindre-rayonSphere;
    glRotatef(-90,0,0,1);
    drawHalfSphere(30,30,1.4);
    glRotatef(90,0,0,1);

    //first part right arm
    glColor3f(0.5,0.5,0.5);
    glRotatef(135,0,1,0);
    glutSolidCylinder(1,6,30,30);

    //right elbow
    glColor3f(0.6,0.6,0.6);
    glTranslatef(0,0,7);
    glutSolidSphere(1,30,30);

    //Seconde part right arm
    glColor3f(0.5,0.5,0.5);
    glRotatef(45,0,1,0);
    glTranslatef(0,0,1);
    glutSolidCylinder(1,6,30,30);

    //hand right
    glTranslatef(0,0,8);
    glRotatef(180,1,0,0);
    DrawPartialCylindre(1,1.5,2,1,30);
    glTranslatef(0,-0.8,-2);
    glRotatef(90,0,0,1);
    glutSolidCylinder(0.3,2,30,30);
    glTranslatef(1.5,1,0);
    glutSolidCylinder(0.3,2,30,30);
    glTranslatef(0,-2,0);
    glutSolidCylinder(0.3,2,30,30);

    glPopMatrix();

    //left shoulder

    glPushMatrix();
    glColor3f(0.6,0.6,0.6);
    glTranslatef(-5,0,12.5); //5.5 = rayon du premier cylindre et 12.9=hauteurCylindre-rayonSphere;
    glRotatef(90,0,0,1);
    drawHalfSphere(30,30,1.4);
    glRotatef(-90,0,0,1);

    //first part left arm
    glColor3f(0.5,0.5,0.5);
    glRotatef(-135,0,1,0);
    glutSolidCylinder(1,6,30,30);

    //left elbow
    glColor3f(0.6,0.6,0.6);
    glTranslatef(0,0,7);
    glutSolidSphere(1,30,30);

    //Seconde part left arm
    glColor3f(0.5,0.5,0.5);
    glRotatef(-45,0,1,0);
    glTranslatef(0,0,1);
    glutSolidCylinder(1,6,30,30);

    //hand left
    glTranslatef(0,0,8);
    glRotatef(180,1,0,0);
    DrawPartialCylindre(1,1.5,2,1,30);
    glTranslatef(0,-0.8,-2);
    glRotatef(90,0,0,1);
    glutSolidCylinder(0.3,2,30,30);
    glTranslatef(1.5,1,0);
    glutSolidCylinder(0.3,2,30,30);
    glTranslatef(0,-2,0);
    glutSolidCylinder(0.3,2,30,30);

    glPopMatrix();

    //head
    glPushMatrix();
    glTranslatef(0,0,16);
    DrawPartialCylindre(3,3,6.5,2,30);
    glRotatef(180,0,0,1);
    DrawPartialCylindre(3,3,1,2,30);
    glColor3f(1,1,1);
    DrawPartialCylindre(2,2,3,2,30);
    glColor3f(0.5,0.5,0.5);
    glTranslatef(0,0,3);
    DrawPartialCylindre(3,3,3.5,2,30);


    //feeler
    glRotatef(-180,0,0,1);
    glRotatef(90,1,0,0);
    glTranslatef(0,3.5,0);
    drawHalfSphere(24,24,3);
    glTranslatef(0,3,0);
    drawHalfSphere(24,24,1);
    glRotatef(90,1,0,0);
    glTranslatef(0,0,-3);
    glutSolidCylinder(0.3,2.8,30,30);
    glutSolidSphere(0.6,30,30);
    glRotatef(-90,1,0,0);
    glTranslatef(-2,-6.5,3.8);


    //masque
    glPushMatrix();
    glPushMatrix();
    glColor3f(0.6,0.6,0.6);
    glTranslatef(1.8,1,-0.5);
    glScalef(2.5,0.3,2.2);
    glutSolidCube(1);
    glTranslatef(0,-7,0);
    glutSolidCube(1);
    glPopMatrix();

    glTranslatef(0.8,0,-0.8);
    glScalef(1.5,1,15);
    glRotatef(90,0,0,1);
    torus(20,20);
    glRotatef(180,0,0,1);
    glTranslatef(0,1.5,0);
    torus(20,20);
    glPopMatrix();
    //

    //eyes
    glColor3f(1,1,1);
    glTranslatef(1.1,0,0);
    glutSolidSphere(0.9,24,24);
    glTranslatef(1.8,0,0);
    glutSolidSphere(0.9,24,24);
    glPopMatrix();

    //nec
    glColor3f(0.5,0.5,0.5);
    glTranslatef(0,0,15);
    DrawPartialCylindre(3,5.5,1.5,1,25);
    /*
        //Repère
        //axe x en rouge
        glBegin(GL_LINES);
            glColor3f(1.0,0.0,0.0);
        	glVertex3f(0, 0,0.0);
        	glVertex3f(1, 0,0.0);
        glEnd();
        //axe des y en vert
        glBegin(GL_LINES);
        	glColor3f(0.0,1.0,0.0);
        	glVertex3f(0, 0,0.0);
        	glVertex3f(0, 1,0.0);
        glEnd();
        //axe des z en bleu
        glBegin(GL_LINES);
        	glColor3f(0.0,0.0,1.0);
        	glVertex3f(0, 0,0.0);
        	glVertex3f(0, 0,1.0);
        glEnd();
    */
    glFlush();

    //On echange les buffers
    glutSwapBuffers();
}
void PointPartialDisk(Point* tab,float r, int numSegments, float partial, float z)
{
    for(int i = 0; i < (numSegments/partial)+1; i++)
    {
        Point tmp;
        float theta = 2.0f * M_PI * float(i)/float(numSegments);//get the current angle

        tmp.x = r * cosf(theta);//calculate the x component
        tmp.y = r * sinf(theta);//calculate the y component
        tmp.z = z;
        tab[i] = tmp;
    }
}


void DrawPartialCylindre(float rHaut, float rbase,float height, float partial, int numberSegment)
{
    Point DiskHaut[int((numberSegment/partial)+1)]= {};
    Point DiskBas[int((numberSegment/partial)+1)]= {};
    PointPartialDisk(DiskHaut,rHaut,numberSegment,partial,height);
    PointPartialDisk(DiskBas,rbase,numberSegment,partial,0);
    glBegin(GL_POLYGON);
    for(int i = 0; i < (numberSegment/partial)+1; i++)
    {
        glVertex3f(DiskHaut[i].x,DiskHaut[i].y,DiskHaut[i].z);
    }
    glEnd();
    glBegin(GL_POLYGON);
    for(int i = 0; i < (numberSegment/partial)+1; i++)
    {
        glVertex3f(DiskBas[i].x,DiskBas[i].y,DiskBas[i].z);
    }
    glEnd();
    glBegin(GL_TRIANGLE_STRIP);
    for(int i = 0; i < (numberSegment/partial)+1; i++)
    {
        glVertex3f(DiskBas[i].x,DiskBas[i].y,DiskBas[i].z);
        glVertex3f(DiskHaut[i].x,DiskHaut[i].y,DiskHaut[i].z);
    }
    glEnd();
}

void clavier(unsigned char touche,int x,int y)
{
    switch (touche)
    {
    case 'p': /* affichage du carre plein */
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        glutPostRedisplay();
        break;
    case 'f': /* affichage en mode fil de fer */
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        glutPostRedisplay();
        break;
    case 's' : /* Affichage en mode sommets seuls */
        glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
        glutPostRedisplay();
        break;
    case 'd':
        glEnable(GL_DEPTH_TEST);
        glutPostRedisplay();
        break;
    case 'D':
        glDisable(GL_DEPTH_TEST);
        glutPostRedisplay();
        break;
    case 'q' : /*la touche 'q' permet de quitter le programme */
        exit(0);
    }
}

void reshape(int x,int y)
{
    if (x<y)
        glViewport(0,(y-x)/2,x,x);
    else
        glViewport((x-y)/2,0,y,y);
}

void mouse(int button, int state,int x,int y)
{
    /* si on appuie sur le bouton gauche */
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        presse = 1; /* le booleen presse passe a 1 (vrai) */
        xold = x; /* on sauvegarde la position de la souris */
        yold=y;
    }
    /* si on relache le bouton gauche */
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        presse=0; /* le booleen presse passe a 0 (faux) */
}

void mousemotion(int x,int y)
{
    if (presse) /* si le bouton gauche est presse */
    {
        /* on modifie les angles de rotation de l'objet
        en fonction de la position actuelle de la souris et de la derniere
         position sauvegardee */
        anglex=anglex+(x-xold);
        angley=angley+(y-yold);
        glutPostRedisplay(); /* on demande un rafraichissement de l'affichage */
    }

    xold=x; /* sauvegarde des valeurs courante de le position de la souris */
    yold=y;
}


void torus(int numc, int numt)
{
    int i, j, k;
    double s, t, x, y, z, twopi;

    twopi = 2 * M_PI;
    for (i = 0; i < numc; i++)
    {
        glBegin(GL_QUAD_STRIP);
        for (j = 0; j <= numt/2; j++)
        {
            for (k = 1; k >= 0; k--)
            {
                s = (i + k) % numc + 0.5;
                t = j % numt;

                x = (1+.1*cos(s*twopi/numc))*cos(t*twopi/numt);
                y = (1+.1*cos(s*twopi/numc))*sin(t*twopi/numt);
                z = .1 * sin(s * twopi / numc);
                glVertex3f(x, y, z);
            }
        }
        glEnd();
    }
}

void drawHalfSphere(int scaley, int scalex, GLfloat r)
{
    int i, j;
    GLfloat v[scalex*scaley][3];

    for (i=0; i<scalex; ++i)
    {
        for (j=0; j<scaley; ++j)
        {
            v[i*scaley+j][0]=r*cos(j*2*M_PI/scaley)*cos(i*M_PI/(2*scalex));
            v[i*scaley+j][1]=r*sin(i*M_PI/(2*scalex));
            v[i*scaley+j][2]=r*sin(j*2*M_PI/scaley)*cos(i*M_PI/(2*scalex));
        }
    }

    glBegin(GL_QUADS);
    for (i=0; i<scalex-1; ++i)
    {
        for (j=0; j<scaley; ++j)
        {
            glVertex3fv(v[i*scaley+j]);
            glVertex3fv(v[i*scaley+(j+1)%scaley]);
            glVertex3fv(v[(i+1)*scaley+(j+1)%scaley]);
            glVertex3fv(v[(i+1)*scaley+j]);
        }
    }
    glEnd();
}
