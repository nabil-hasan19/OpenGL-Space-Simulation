#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <ctime>

#define PI 3.14159265358979323846

//--------------------------------------
// ENUM STATES
//--------------------------------------

enum AppState
{
    WELCOME_SCREEN,
    STATIC_VIEW,
    DYNAMIC_VIEW
};

enum SatelliteState
{
    SAT_IDLE,
    SAT_DEPLOYMENT,
    SAT_TRANSLATION,
    SAT_ORBIT
};

//--------------------------------------
// GLOBAL VARIABLES
//--------------------------------------

AppState currentState = WELCOME_SCREEN;
SatelliteState satelliteState = SAT_IDLE;

float earthOrbitAngle = 0.0f;
float moonOrbitAngle = 0.0f;
float sunRotation = 0.0f;

float satelliteX = 0.0f;
float satelliteY = 0.0f;

float satelliteOrbitAngle = 0.0f;

bool animate = false;

//--------------------------------------
// STAR STRUCTURE
//--------------------------------------

struct Star
{
    float x;
    float y;
};

std::vector<Star> stars;

//--------------------------------------
// INITIALIZATION
//--------------------------------------

void initStars()
{
    for (int i = 0; i < 300; i++)
    {
        Star s;
        s.x = rand() % 200 - 100;
        s.y = rand() % 200 - 100;
        stars.push_back(s);
    }
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glEnable(GL_DEPTH_TEST);

    initStars();
}

//--------------------------------------
// DRAW TEXT
//--------------------------------------

void drawText(float x, float y, const char* text)
{
    glRasterPos2f(x, y);

    while (*text)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        text++;
    }
}

//--------------------------------------
// DRAW STARS
//--------------------------------------

void drawStars()
{
    glPointSize(2);

    glBegin(GL_POINTS);

    for (int i = 0; i < stars.size(); i++)
    {
        glColor3f(1.0, 1.0, 1.0);
        glVertex3f(stars[i].x, stars[i].y, -50);
    }

    glEnd();
}

//--------------------------------------
// DRAW ORBIT
//--------------------------------------

void drawOrbit(float radius)
{
    glColor3f(0.5, 0.5, 0.5);

    glBegin(GL_LINE_LOOP);

    for (int i = 0; i < 360; i++)
    {
        float theta = i * PI / 180.0;

        glVertex3f(radius * cos(theta),
                   radius * sin(theta),
                   0);
    }

    glEnd();
}

//--------------------------------------
// DRAW SUN
//--------------------------------------

void drawSun()
{
    glPushMatrix();

    glRotatef(sunRotation, 0, 1, 0);

    glColor3f(1.0, 0.8, 0.0);
    glutSolidSphere(4, 40, 40);

    glPopMatrix();
}

//--------------------------------------
// DRAW EARTH
//--------------------------------------

void drawEarth()
{
    glColor3f(0.0, 0.4, 1.0);
    glutSolidSphere(1.5, 30, 30);
}

//--------------------------------------
// DRAW MOON
//--------------------------------------

void drawMoon()
{
    glColor3f(0.8, 0.8, 0.8);
    glutSolidSphere(0.5, 20, 20);
}

//--------------------------------------
// DRAW ROCKET
//--------------------------------------

void drawRocket()
{
    GLUquadric* quad = gluNewQuadric();

    // BODY
    glColor3f(0.8, 0.8, 0.8);

    glPushMatrix();
    glRotatef(-90, 1, 0, 0);

    gluCylinder(quad, 0.3, 0.3, 1.5, 20, 20);

    glPopMatrix();

    // NOSE CONE
    glColor3f(1.0, 0.0, 0.0);

    glPushMatrix();

    glTranslatef(0, 1.5, 0);

    glutSolidCone(0.3, 0.7, 20, 20);

    glPopMatrix();

    // LEFT FIN
    glBegin(GL_TRIANGLES);

    glColor3f(1, 0, 0);

    glVertex3f(-0.3, 0.2, 0);
    glVertex3f(-0.7, -0.2, 0);
    glVertex3f(-0.3, -0.2, 0);

    glEnd();

    // RIGHT FIN
    glBegin(GL_TRIANGLES);

    glVertex3f(0.3, 0.2, 0);
    glVertex3f(0.7, -0.2, 0);
    glVertex3f(0.3, -0.2, 0);

    glEnd();

    gluDeleteQuadric(quad);
}

//--------------------------------------
// DRAW SATELLITE
//--------------------------------------

void drawSatellite()
{
    glPushMatrix();

    glColor3f(0.7, 0.7, 0.7);

    glutSolidCube(0.6);

    // SOLAR PANELS

    glBegin(GL_QUADS);

    glColor3f(0.0, 0.3, 1.0);

    // LEFT PANEL
    glVertex3f(-1.2, 0.3, 0);
    glVertex3f(-0.3, 0.3, 0);
    glVertex3f(-0.3, -0.3, 0);
    glVertex3f(-1.2, -0.3, 0);

    // RIGHT PANEL
    glVertex3f(1.2, 0.3, 0);
    glVertex3f(0.3, 0.3, 0);
    glVertex3f(0.3, -0.3, 0);
    glVertex3f(1.2, -0.3, 0);

    glEnd();

    glPopMatrix();
}

//--------------------------------------
// DRAW WELCOME SCREEN
//--------------------------------------

void drawWelcomeScreen()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    drawStars();

    glColor3f(0.0, 1.0, 1.0);

    drawText(-20, 10, "OPENGL SPACE SIMULATION");
    drawText(-18, 2, "Press SPACE to Start");
    drawText(-28, -5, "Right Click for Menu Options");

    glutSwapBuffers();
}

//--------------------------------------
// DRAW SCENE
//--------------------------------------

void drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    gluLookAt(0, 20, 40,
              0, 0, 0,
              0, 1, 0);

    drawStars();

    //----------------------------------
    // SUN
    //----------------------------------

    drawSun();

    //----------------------------------
    // EARTH ORBIT
    //----------------------------------

    drawOrbit(15);

    glPushMatrix();

    glRotatef(earthOrbitAngle, 0, 1, 0);
    glTranslatef(15, 0, 0);

    drawEarth();

    //----------------------------------
    // ROCKET ON EARTH
    //----------------------------------

    if (satelliteState == SAT_IDLE)
    {
        glPushMatrix();

        glTranslatef(0, 2.5, 0);

        drawRocket();

        glPopMatrix();
    }

    //----------------------------------
    // SATELLITE LAUNCH
    //----------------------------------

    if (satelliteState != SAT_IDLE)
    {
        glPushMatrix();

        if (satelliteState == SAT_DEPLOYMENT)
        {
            glTranslatef(0, satelliteY, 0);
        }
        else if (satelliteState == SAT_TRANSLATION)
        {
            glTranslatef(satelliteX, satelliteY, 0);
        }
        else if (satelliteState == SAT_ORBIT)
        {
            glRotatef(satelliteOrbitAngle, 0, 1, 0);
            glTranslatef(4, 0, 0);
        }

        drawSatellite();

        glPopMatrix();
    }

    //----------------------------------
    // MOON
    //----------------------------------

    drawOrbit(4);

    glPushMatrix();

    glRotatef(moonOrbitAngle, 0, 1, 0);
    glTranslatef(4, 0, 0);

    drawMoon();

    glPopMatrix();

    glPopMatrix();

    glutSwapBuffers();
}

//--------------------------------------
// DISPLAY
//--------------------------------------

void display()
{
    if (currentState == WELCOME_SCREEN)
        drawWelcomeScreen();
    else
        drawScene();
}

//--------------------------------------
// ANIMATION
//--------------------------------------

void update(int value)
{
    if (currentState == DYNAMIC_VIEW)
    {
        earthOrbitAngle += 0.2f;
        moonOrbitAngle += 1.0f;
        sunRotation += 0.5f;

        //----------------------------------
        // SATELLITE STATES
        //----------------------------------

        if (satelliteState == SAT_DEPLOYMENT)
        {
            satelliteY += 0.05f;

            if (satelliteY >= 5)
            {
                satelliteState = SAT_TRANSLATION;
            }
        }
        else if (satelliteState == SAT_TRANSLATION)
        {
            satelliteX += 0.05f;

            if (satelliteX >= 4)
            {
                satelliteState = SAT_ORBIT;
            }
        }
        else if (satelliteState == SAT_ORBIT)
        {
            satelliteOrbitAngle += 2.0f;
        }
    }

    glutPostRedisplay();

    glutTimerFunc(16, update, 0);
}

//--------------------------------------
// KEYBOARD
//--------------------------------------

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case ' ':
        currentState = STATIC_VIEW;
        break;

    case 'd':
        currentState = DYNAMIC_VIEW;
        break;

    case 's':
        currentState = STATIC_VIEW;
        break;

    case 'l':
        if (satelliteState == SAT_IDLE)
        {
            satelliteState = SAT_DEPLOYMENT;

            satelliteX = 0;
            satelliteY = 2.5;
        }
        break;

    case 27:
        exit(0);
        break;
    }

    glutPostRedisplay();
}

//--------------------------------------
// MENU
//--------------------------------------

void menu(int option)
{
    switch (option)
    {
    case 1:
        currentState = STATIC_VIEW;
        break;

    case 2:
        currentState = DYNAMIC_VIEW;
        break;

    case 3:
        if (satelliteState == SAT_IDLE)
        {
            satelliteState = SAT_DEPLOYMENT;

            satelliteX = 0;
            satelliteY = 2.5;
        }
        break;

    case 4:
        exit(0);
        break;
    }

    glutPostRedisplay();
}

//--------------------------------------
// RESHAPE
//--------------------------------------

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    gluPerspective(60,
                   (float)w / (float)h,
                   1,
                   500);

    glMatrixMode(GL_MODELVIEW);
}

//--------------------------------------
// MAIN
//--------------------------------------

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(1000, 700);

    glutCreateWindow("Space Simulation");

    init();

    glutDisplayFunc(display);

    glutReshapeFunc(reshape);

    glutKeyboardFunc(keyboard);

    glutTimerFunc(0, update, 0);

    //----------------------------------
    // RIGHT CLICK MENU
    //----------------------------------

    glutCreateMenu(menu);

    glutAddMenuEntry("Static View", 1);
    glutAddMenuEntry("Dynamic View", 2);
    glutAddMenuEntry("Launch Satellite", 3);
    glutAddMenuEntry("Exit", 4);

    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();

    return 0;
}
