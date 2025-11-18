#include <GL/freeglut.h>

void drawScene(void)
{
    // glClear(GL_COLOR_BUFFER_BIT);
    // glColor3f(0.0, 0.0, 0.5);

    // glPointSize(5.0);
    // glBegin(GL_POINTS); //GL_POLYGON, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_POINTS
    //     glVertex3f(20.0, 20.0, 0.0); //xy nyas 20, 
    //     glVertex3f(80.0, 20.0, 0.0);
    //     glVertex3f(80.0, 80.0, 0.0);
    //     glVertex3f(20.0, 80.0, 0.0);
    // glEnd();
    // glFlush();

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0); //warna hitam
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(10.0, 90.0, 0.0); 
        glVertex3f(10.0, 10.0, 0.0);
        glVertex3f(35.0, 75.0, 0.0);
        glVertex3f(30.0, 20.0, 0.0);
        glVertex3f(90.0, 90.0, 0.0);
        glVertex3f(80.0, 40.0, 0.0);
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