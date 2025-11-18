#include <GL/freeglut.h>

void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.75, 0.8);

    // gambar kotak -----------------------------
    glBegin(GL_POLYGON);
        glVertex3f(20.0, 20.0, 0.0); //xy nya 20, 
        glVertex3f(80.0, 20.0, 0.0);
        glVertex3f(80.0, 80.0, 0.0);
        glVertex3f(20.0, 80.0, 0.0);
    glEnd();

    // gambar jajar genjang ---------------------
    // glBegin(GL_POLYGON);
    //cara1
        // glVertex3f(20.0, 5.0, 0.0);
        // glVertex3f(80.0, 20.0, 0.0);
        // glVertex3f(80.0, 80.0, 0.0);
        // glVertex3f(20.0, 65.0, 0.0);

    //cara2
        // glVertex3f(20.0, 5.0, 0.0);
        // glVertex3f(80.0, 20.0, 0.0);
        // glVertex3f(80.0, 85.0, 0.0);
        // glVertex3f(20.0, 70.0, 0.0);
    glEnd();


    glFlush();
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
    
    // left right bottom top znear zfar
    // glOrtho(0.0, 200.0, 0.0, 200.0, -1.0, 1.0);
    // glOrtho(20.0, 80.0, 20.0, 90.0, -1.0, 1.0);
    // glOrtho(0.0, 100.0, 0.0, 100.0, -2.0, 5.0);
    // vs
    glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(500, 500);
    // glutInitWindowSize(1920, 1080);
    glutCreateWindow("Drawing a Square");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    setUp();
    glutMainLoop();
    return 0;
}