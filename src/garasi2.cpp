#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cstdlib>
#include <cmath>
#include <iostream>

// --- GLOBALS ---
float carX = 0.0f, carZ = 15.0f; // Mobil mulai di luar garasi
float carAngle = 0.0f;
bool headLightOn = true;

// Variabel Pintu Garasi
float doorY = 0.0f;      // Posisi ketinggian pintu
bool isDoorOpen = false; // Status pintu

GLuint texID;

// Ukuran checkerboard
#define checkImageWidth 64
#define checkImageHeight 64
GLubyte checkImage[checkImageHeight][checkImageWidth][4];

// --- TEXTURE GENERATION ---
void makeCheckImage(void)
{
    int i, j, c;
    for (i = 0; i < checkImageHeight; i++)
    {
        for (j = 0; j < checkImageWidth; j++)
        {
            // Pola hitam putih
            c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
            checkImage[i][j][0] = (GLubyte)c;
            checkImage[i][j][1] = (GLubyte)c;
            checkImage[i][j][2] = (GLubyte)c;
            checkImage[i][j][3] = (GLubyte)255;
        }
    }
}

// --- HELPER: Draw Box ---
void drawBox(float sx, float sy, float sz)
{
    glPushMatrix();
    glScalef(sx, sy, sz);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// --- HELPER: Draw Tire ---
void drawTire()
{
    glPushMatrix();
    glColor3f(0.1f, 0.1f, 0.1f);
    glutSolidTorus(0.1, 0.25, 10, 20);
    glPopMatrix();
}

// --- OBJEK: GARASI & MAP ---
void drawEnvironment()
{
    // 1. TANAH LUAS (Rumput Hijau)
    glDisable(GL_TEXTURE_2D);
    float matGrass[] = {0.0f, 0.5f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matGrass);

    glPushMatrix();
    glTranslatef(0.0f, -0.1f, 0.0f); // Sedikit di bawah lantai garasi
    drawBox(100.0f, 0.1f, 100.0f);
    glPopMatrix();

    // 2. LANTAI GARASI (Papan Catur)
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID);
    float matWhite[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matWhite);

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    // Lantai ukuran 8x10
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.0f, 0.0f, 5.0f);
    glTexCoord2f(5.0f, 0.0f);
    glVertex3f(4.0f, 0.0f, 5.0f);
    glTexCoord2f(5.0f, 5.0f);
    glVertex3f(4.0f, 0.0f, -5.0f);
    glTexCoord2f(0.0f, 5.0f);
    glVertex3f(-4.0f, 0.0f, -5.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    // 3. DINDING & ATAP GARASI
    float matGrey[] = {0.6f, 0.6f, 0.7f, 1.0f}; // Warna tembok beton
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matGrey);

    // Dinding Kiri
    glPushMatrix();
    glTranslatef(-4.25f, 2.5f, 0.0f);
    drawBox(0.5f, 5.0f, 10.0f);
    glPopMatrix();

    // Dinding Kanan
    glPushMatrix();
    glTranslatef(4.25f, 2.5f, 0.0f);
    drawBox(0.5f, 5.0f, 10.0f);
    glPopMatrix();

    // Dinding Belakang
    glPushMatrix();
    glTranslatef(0.0f, 2.5f, -5.25f);
    drawBox(9.0f, 5.0f, 0.5f);
    glPopMatrix();

    // Atap
    glPushMatrix();
    glTranslatef(0.0f, 5.25f, 0.0f);
    drawBox(9.0f, 0.5f, 11.0f);
    glPopMatrix();

    // 4. PINTU GARASI (ANIMASI)
    float matDoor[] = {0.4f, 0.2f, 0.0f, 1.0f}; // Coklat Kayu
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matDoor);

    glPushMatrix();
    // Posisi pintu di depan (Z=5), Naik turun berdasarkan doorY
    glTranslatef(0.0f, 2.5f + doorY, 5.0f);
    drawBox(8.0f, 5.0f, 0.2f); // Pintu tipis
    glPopMatrix();
}

// --- OBJEK: MOBIL ---
void drawCar()
{
    float matBody[] = {0.8f, 0.1f, 0.1f, 1.0f}; // Merah
    float matSpec[] = {0.9f, 0.9f, 0.9f, 1.0f};
    float matShine[] = {50.0f};

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matBody);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

    glPushMatrix();
    glTranslatef(carX, 0.5f, carZ);
    glRotatef(carAngle, 0.0f, 1.0f, 0.0f);

    // Lampu Sorot Mobil
    if (headLightOn)
    {
        glEnable(GL_LIGHT1);
        GLfloat light1_pos[] = {0.0f, 0.5f, -1.0f, 1.0f};
        GLfloat light1_dir[] = {0.0f, -0.2f, -1.0f};
        glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_dir);
    }
    else
    {
        glDisable(GL_LIGHT1);
    }

    // Body Utama
    drawBox(2.0f, 1.0f, 4.0f);

    // Atap
    glPushMatrix();
    glTranslatef(0.0f, 0.8f, -0.5f);
    drawBox(1.8f, 0.8f, 2.0f);
    glPopMatrix();

    // Roda
    float matBlack[] = {0.1f, 0.1f, 0.1f, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matBlack);

    glPushMatrix();
    glTranslatef(-1.0f, -0.5f, -1.2f);
    glRotatef(90, 0, 1, 0);
    drawTire();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(1.0f, -0.5f, -1.2f);
    glRotatef(90, 0, 1, 0);
    drawTire();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-1.0f, -0.5f, 1.2f);
    glRotatef(90, 0, 1, 0);
    drawTire();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(1.0f, -0.5f, 1.2f);
    glRotatef(90, 0, 1, 0);
    drawTire();
    glPopMatrix();

    // Lampu Depan (Visual)
    float matYellow[] = {1.0f, 1.0f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matYellow);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

    glPushMatrix();
    glTranslatef(0.6f, 0.0f, -2.01f);
    glutSolidSphere(0.2, 10, 10);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.6f, 0.0f, -2.01f);
    glutSolidSphere(0.2, 10, 10);
    glPopMatrix();

    glPopMatrix();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Kamera Isometric dari atas serong
    gluLookAt(15.0, 15.0, 25.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    drawEnvironment(); // Gambar Garasi & Pintu
    drawCar();         // Gambar Mobil

    glutSwapBuffers();
}

void setUp(void)
{
    glClearColor(0.1, 0.1, 0.2, 0.0); // Malam hari
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);

    // Texture Setup
    makeCheckImage();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

    // Light 0: Lampu Garasi (Kuning hangat di dalam)
    float lightPos0[] = {0.0, 4.0, 0.0, 1.0};
    float lightColor0[] = {0.6f, 0.6f, 0.5f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glEnable(GL_LIGHT0);

    // Light 1: Lampu Mobil (Setup awal)
    float lightDif1[] = {1.0, 1.0, 0.8, 1.0};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDif1);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
}

// Timer untuk animasi pintu halus (60 FPS)
void timer(int value)
{
    if (isDoorOpen)
    {
        if (doorY < 4.0f)
            doorY += 0.1f; // Naikkan pintu
    }
    else
    {
        if (doorY > 0.0f)
            doorY -= 0.1f; // Turunkan pintu
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void KeyInput(unsigned char key, int x, int y)
{
    float speed = 0.5f;
    float turnSpeed = 5.0f;
    float rad = carAngle * 3.14159f / 180.0f;

    switch (key)
    {
    case 27:
        exit(0);
        break;
    case 'w':
        carX -= sin(rad) * speed;
        carZ -= cos(rad) * speed;
        break;
    case 's':
        carX += sin(rad) * speed;
        carZ += cos(rad) * speed;
        break;
    case 'a':
        carAngle += turnSpeed;
        break;
    case 'd':
        carAngle -= turnSpeed;
        break;
    case ' ':
        headLightOn = !headLightOn;
        break;
    case 'g':
        isDoorOpen = !isDoorOpen;
        break; // Toggle Pintu
    }
}

void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w / (float)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Mobil Masuk Garasi");

    GLenum err = glewInit();
    if (GLEW_OK != err)
        return -1;

    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(KeyInput);
    glutTimerFunc(0, timer, 0); // Start timer

    setUp();

    std::cout << "--- KONTROL ---" << std::endl;
    std::cout << "[W/A/S/D] : Setir Mobil" << std::endl;
    std::cout << "[Spasi]   : Lampu Mobil On/Off" << std::endl;
    std::cout << "[G]       : Buka/Tutup Pintu Garasi" << std::endl;

    glutMainLoop();
    return 0;
}