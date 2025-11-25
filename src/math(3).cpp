#include <GL/freeglut.h>
#include<cstdlib>
#include<cmath>
#include<iostream>

#define PI 3.14159256
#define N 40.0

static int runMode = 1;
float currAngle = 0.0f;
float animateStep = 3.0f;

const double Xmin = 0.0, Xmax = 3.0;
const double Ymin = 0.0, Ymax = 3.0;

void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (runMode == 1)
    {
        currAngle += animateStep;
        if (currAngle > 360.0)
        currAngle -= 360.0 * floor(currAngle / 360.0);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(1.5, 1.5, 0.0);
    glRotatef(currAngle, 0.0, 0.0, 1.0);
    glTranslatef(-1.5, -1.5, 0.0);

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.3, 1.0, 0.5);
    glVertex3f(2.7, 0.85, 0.0);
    glVertex3f(2.7, 1.15, 0.0);

    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(2.53, 0.71, 0.5);
    glVertex3f(1.46, 2.86, 0.0);
    glVertex3f(1.2, 2.71, 0.0);

    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(1.667, 2.79, 0.5);
    glVertex3f(0.337, 0.786, 0.0);
    glVertex3f(0.597, 0.636, 0.0);
    glEnd();

    glFlush();
    glutSwapBuffers();
    if (runMode == 1)
        glutPostRedisplay();
}

void keyInput (unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'r':
        runMode = 1 - runMode;
        if (runMode == 1) glutPostRedisplay();
        break;
    case 's':
        runMode = 1;
        drawScene();
        runMode = 0;
        break;
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
    double scale, center;
    double winXmin, winXmax, winYmin, winYmax;
    glViewport(0, 0, w, h);

    w = (w == 0) ? 1 : w;
    h = (h == 0) ? 1 : h;
    if((Xmax - Xmin) / w < (Ymax - Ymin) / h) {
        scale = ((Ymax - Ymin) / h) / ((Xmax -Xmin) / w);
        center = (Xmax + Xmin) / 2;
        winXmin = center - (center - Xmin) * scale;
        winXmax = center + (Xmax - center) * scale;
        winYmin = Ymin;
        winXmax = Ymax;
    }
    else
    {
        scale = ((Xmax - Xmin) / w) / ((Ymax -Ymin) / h);
        center = (Ymax + Ymin) /2;
        winYmin = center - (center - Ymin) * scale;
        winYmax = center + (Ymax - center) * scale;
        winXmin = Xmin;
        winXmax = Xmax;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(winXmin, winXmax, winYmin, winYmax, -1, 1);
}

void specialInput(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_UP:
            if (animateStep < 1.0e3) animateStep *= sqrt(2.0);
            break;
        case GLUT_KEY_DOWN:
            if (animateStep> 1.0e-6) animateStep /= sqrt(2.0);
            break;
    }
}

int main (int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(10, 60);
    glutCreateWindow("Simple Animation");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialInput);
    setUp();
    glutMainLoop();
    
}