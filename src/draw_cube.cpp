#include <GL/freeglut.h>
#include <cstdlib>
#include <cmath>
#include <iostream>

#define PI 3.14159265
#define N 40.0

int runMode = 1;
float currAngle = 0.0f;
float animateStep = 3.0f;

const double Xmin = 0.0, Xmax = 3.0;
const double Ymin = 0.0, Ymax = 3.0;

void specialInput(int key, int x, int y);
void drawScene(void);
void keyInput(unsigned char key, int x, int y);
void resize(int w, int h);

void specialInput(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP:
        if(animateStep < 1.0e3) animateStep *= sqrt(2.0);
        break;
    case GLUT_KEY_DOWN:
        if(animateStep > 1.0e-6) animateStep /= sqrt(2.0);
        break;
    case 27:
        exit(0);
        break;
    default:
        break;
    }
}

void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    glLoadIdentity();
    gluLookAt(1.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glScalef(1.0, 2.0, 1.0);
    glutWireCube(1.0);
    // glutWireTeacup(5.0);
    glutWireTeaspoon(5.0);
    glFlush();
}

void keyInput(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 27:
        exit(0);
        break;
    default:
        break;
    }
}

void setUp(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
}

void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowPosition(10,60);
    glutInitWindowSize(500, 500);
    // glutInitWindowSize(1920, 1080);
    glutCreateWindow("Simple Wire Cube");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    setUp();
    glutMainLoop();
    return 0;
}