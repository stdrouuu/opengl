#include <GL/freeglut.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include "getBMP.h"

#define PI 3.141592653589793238
#define R 12.0

static int p = 20;
static int q = 20;
static float* vertices = NULL;
static float* textureCoor = NULL;

static unsigned int texture[1];
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0;

void loadTexture()
{
    imageFile* image[1];
    image[0] = getBMP("Texture/earth.bmp");

    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->width,
        image[0]->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        image[0]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

float f(int i, int j)
{
    return (R * cos(-PI / 2.0 + (float)j / q * PI)
        * cos(2.0 * (float)i / p * PI));
}

float g(int i, int j)
{
    return (R * sin(-PI / 2.0 + (float)j / q * PI));
}

float h(int i, int j)
{
    return (-R * cos(-PI / 2.0 + (float)j / q * PI)
    * sin(2.0 * (float)i / p * PI));
}

void fillVertexArr(void)
{
    int i, j, k;
    k = 0;
    for(j = 0; j <= q; j++)
        for(i = 0; i <= p; i++)
        {
            vertices[k++] = f(i, j);
            vertices[k++] = g(i, j);
            vertices[k++] = h(i, j);
        }
}

void fillTextureCoorArr(void)
{
    int i, j, k;
    k = 0;
    for (j = 0; j <= q; j++)
        for (i = 0; i <= p; i++)
        {
            textureCoor[k++] = (float)i / p;
            textureCoor[k++] = (float)j / q;
        }
}

void setup(void)
{
    glEnable(GL_DEPTH_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glClearColor(1.0, 1.0, 1.0, 0.0);

    glGenTextures(1, texture);
    loadTexture();

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glEnable(GL_TEXTURE_2D);

    vertices = new float[3 * (p + 1) * (q + 1)];
    textureCoor = new float[2 * (p + 1) * (q + 1)];

    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, textureCoor);

    fillVertexArr(); 
    fillTextureCoorArr();
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void drawScene(void) {
    int i, j;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0, 0.0, 25.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glRotatef(Xangle, 0.0, 0.0, 1.0);
    glRotatef(Yangle, 0.0, 1.0, 0.0);
    glRotatef(Zangle, 1.0, 0.0, 0.0);

    glBindTexture(GL_TEXTURE_2D, texture[0]);
    for (j = 0; j < q; j++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (i = 0; i <= p; i++) {
            glArrayElement((j + 1) * (p + 1) + i);
            glArrayElement(j * (p + 1) + i);
        }
        glEnd();
    }
    glutSwapBuffers();
}
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyInput(unsigned char key, int x, int y) {
    switch (key) {
        case 27:
            exit(0);
            break;
        case 'x':
            Xangle += 5.0;
            if (Xangle > 360.0) Xangle -= 360.0;
            glutPostRedisplay();
            break;
        case 'X':
            Xangle -= 5.0;
            if (Xangle < 0.0) Xangle += 360.0;
            glutPostRedisplay();
            break;
        case 'y':
            Yangle += 5.0;
            if (Yangle > 360.0) Yangle -= 360.0;
            glutPostRedisplay();
            break;
        case 'Y':
            Yangle -= 5.0;
            if (Yangle < 0.0) Yangle += 360.0;
            glutPostRedisplay();
            break;
        case 'z':
            Zangle += 5.0;
            if (Zangle > 360.0) Zangle -= 360.0;
            glutPostRedisplay();
            break;
        case 'Z':
            Zangle -= 5.0;
            if (Zangle < 0.0) Zangle += 360.0;
            glutPostRedisplay();
            break;
        default:
            break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Texture Sphere");
    glutReshapeFunc(resize);
    glutDisplayFunc(drawScene);
    glutKeyboardFunc(keyInput);
    setup();
    glutMainLoop();
    return 0;
}