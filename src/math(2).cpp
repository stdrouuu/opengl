#include <GL/freeglut.h>
#include <cstdlib>
#include <cmath>
#include <iostream>

#define PI 3.14159265
#define N 40.0

static int isWire = 0;
static long font = (long)GLUT_BITMAP_8_BY_13;

void writeBitmapString(void* font, const char* string)
{
    const char* c;
    for(c = string; *c != '\0'; c++)
        glutBitmapCharacter(font, *c);
}

void drawDisc(float R, float X, float Y, float Z)
{
    float t;
    int i;
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(X, Y, Z);
    for(i = 0; i <= N; ++i)
    {
        t = 2 * PI * i / N;
        glVertex3f(X + cos(t) * R, Y + sin(t) * R, Z);
    }
    glEnd();
}


void drawScene(void)
{
    float angle;
    int i;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT, GL_FILL);
    
    //annulus kiri
    glColor3f(1.0, 1.0, 0.5); // warna kuning pastel
    drawDisc(20.0, 25.0, 75.0, 0.0);
    glColor3f(1.0, 1.0, 1.0); 
    drawDisc(10.0, 25.0, 75.0, 0.0);
    
    //annulus kanan 
    glEnable(GL_DEPTH_TEST);
    glColor3f(1.0, 0.5, 1.0); // warna pink
    drawDisc(20.0, 75.0, 75.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    drawDisc(10.0, 75.0, 75.0, 0.5);
    glDisable(GL_DEPTH_TEST);

    //annulus bawah 
    if(isWire)
        glPolygonMode(GL_FRONT, GL_LINE);
    else 
        glPolygonMode(GL_FRONT, GL_FILL);
    glColor3f(0.0, 1.0, 1.0); // warna biru muda
    glBegin(GL_TRIANGLE_STRIP);
    for(i = 0; i <= N; ++i)
    {
        angle = 2 * PI * i / N;
        glVertex3f(50 + cos(angle) * 10.0, 30 + sin(angle) * 10.0, 0.0);
        glVertex3f(50 + cos(angle) * 20.0, 30 + sin(angle) * 20.0, 0.0);
    }
    glEnd();

    glColor3f(0.0, 0.0, 0.0); 
    glRasterPos3f(15.0, 51.0, 0.0);
    writeBitmapString((void*)font, "Overwritten");
    glRasterPos3f(69.0, 51.0, 0.0);
    writeBitmapString((void*)font, "Floating");
    glRasterPos3f(38.0, 6.0, 0.0);
    writeBitmapString((void*)font, "The Real Deal");
    glFlush();
}


void keyInput(unsigned char key, int x, int y)
{
    switch(key)
    {
    case ' ':
        if(isWire == 0)
            isWire = 1;
        else
            isWire = 0;
        glutPostRedisplay();
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
    
}

void resize(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void printInteraction(void)
{
    std::cout << "Interaction:" << std::endl;
    std::cout << "Press the space bar to toggle between wireframe and filled for lower" << std::endl;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(500, 500);
    // glutInitWindowSize(1920, 1080);
    glutCreateWindow("Drawing Circular Annuluses");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    setUp();
    glutMainLoop();
    return 0;
}