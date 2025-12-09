#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include <iostream>
#include <vector>

// Variabel Global untuk rotasi kamera dan animasi
float angleX = 20.0f, angleY = -45.0f;
float walkAngle = 0.0f;
float headAngle = 0.0f;
GLuint textureCreeper;
GLuint textureFloor;

// --- 1. TEXTURE GENERATION (Supaya tidak perlu file gambar eksternal) ---
// Membuat tekstur noise hijau (untuk Creeper) dan checkerboard (untuk lantai)
GLuint generateTexture(int type)
{
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    int width = 64, height = 64;
    std::vector<unsigned char> data(width * height * 3);

    for (int i = 0; i < width * height; ++i)
    {
        if (type == 1)
        { // CREEPER SKIN (Green Noise)
            unsigned char noise = rand() % 50;
            data[i * 3 + 0] = 50 + noise;      // R (Dark Green)
            data[i * 3 + 1] = 150 + noise * 2; // G (Bright Green)
            data[i * 3 + 2] = 50 + noise;      // B
        }
        else
        { // FLOOR (Checkerboard)
            int row = (i / width) / 8;
            int col = (i % width) / 8;
            bool white = (row + col) % 2 == 0;
            data[i * 3 + 0] = white ? 200 : 100;
            data[i * 3 + 1] = white ? 200 : 100;
            data[i * 3 + 2] = white ? 200 : 100;
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // Nearest biar kelihatan kotak-kotak (pixelated)
    return texID;
}

// --- 2. CUSTOM CUBE DRAWING WITH NORMALS & UV ---
// Fungsi manual menggambar kubus agar bisa pasang tekstur dan normal lighting
void drawTexturedCube(float size)
{
    float s = size / 2.0f;
    glBegin(GL_QUADS);
    // Front Face
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-s, -s, s);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(s, -s, s);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(s, s, s);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-s, s, s);
    // Back Face
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-s, -s, -s);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-s, s, -s);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(s, s, -s);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(s, -s, -s);
    // Top Face
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-s, s, -s);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-s, s, s);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(s, s, s);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(s, s, -s);
    // Bottom Face
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-s, -s, -s);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(s, -s, -s);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(s, -s, s);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-s, -s, s);
    // Right Face
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(s, -s, -s);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(s, s, -s);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(s, s, s);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(s, -s, s);
    // Left Face
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-s, -s, -s);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-s, -s, s);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-s, s, s);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-s, s, -s);
    glEnd();
}

// --- 3. HIERARCHICAL MODELING ---
void drawCreeper()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureCreeper);

    // Material properties for Creeper
    GLfloat mat_ambient[] = {0.2f, 0.8f, 0.2f, 1.0f};
    GLfloat mat_diffuse[] = {0.3f, 0.9f, 0.3f, 1.0f};
    GLfloat mat_specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat mat_shininess[] = {20.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // -- BODY (Parent) --
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.0f); // Geser seluruh creeper ke atas lantai

    // Gambar Badan (Body)
    glPushMatrix();
    glScalef(1.0f, 2.0f, 0.5f); // Scale Y=2 (Badan panjang)
    drawTexturedCube(1.0f);
    glPopMatrix();

    // -- HEAD (Child of Body) --
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.0f);                      // Naik ke atas leher
    glRotatef(sin(headAngle) * 15.0f, 0.0f, 1.0f, 0.0f); // Animasi Geleng Kepala
    glScalef(1.0f, 1.0f, 1.0f);
    drawTexturedCube(1.0f); // Gambar Kepala
    glPopMatrix();

    // -- LEGS (Children of Body) --
    // Kaki Depan Kiri
    glPushMatrix();
    glTranslatef(-0.5f, -1.5f, 0.5f);
    glRotatef(sin(walkAngle) * 30.0f, 1.0f, 0.0f, 0.0f); // Animasi jalan
    glScalef(0.5f, 1.5f, 0.5f);
    drawTexturedCube(1.0f);
    glPopMatrix();

    // Kaki Depan Kanan
    glPushMatrix();
    glTranslatef(0.5f, -1.5f, 0.5f);
    glRotatef(sin(walkAngle + 3.14f) * 30.0f, 1.0f, 0.0f, 0.0f); // Gerak berlawanan
    glScalef(0.5f, 1.5f, 0.5f);
    drawTexturedCube(1.0f);
    glPopMatrix();

    // Kaki Belakang Kiri
    glPushMatrix();
    glTranslatef(-0.5f, -1.5f, -0.5f);
    glRotatef(sin(walkAngle + 3.14f) * 30.0f, 1.0f, 0.0f, 0.0f);
    glScalef(0.5f, 1.5f, 0.5f);
    drawTexturedCube(1.0f);
    glPopMatrix();

    // Kaki Belakang Kanan
    glPushMatrix();
    glTranslatef(0.5f, -1.5f, -0.5f);
    glRotatef(sin(walkAngle) * 30.0f, 1.0f, 0.0f, 0.0f);
    glScalef(0.5f, 1.5f, 0.5f);
    drawTexturedCube(1.0f);
    glPopMatrix();

    glPopMatrix(); // End of Whole Creeper
    glDisable(GL_TEXTURE_2D);
}

void drawFloor()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureFloor);

    // Material lantai (sedikit redup)
    GLfloat floor_diffuse[] = {0.6f, 0.6f, 0.6f, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, floor_diffuse);

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    // Buat lantai besar 20x20
    for (int x = -10; x < 10; x++)
    {
        for (int z = -10; z < 10; z++)
        {
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(x, 0.0f, z);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(x + 1, 0.0f, z);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(x + 1, 0.0f, z + 1);
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(x, 0.0f, z + 1);
        }
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

// --- 4. LIGHTING SETUP ---
void initLighting()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE); // Penting untuk scaling object

    // LIGHT 0: Directional Light (Cahaya Matahari - Putih)
    glEnable(GL_LIGHT0);
    GLfloat light0_pos[] = {5.0f, 10.0f, 5.0f, 0.0f}; // 0.0f di akhir artinya directional
    GLfloat light0_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);

    // LIGHT 1: Point Light (Cahaya Merah di samping - Efek Dramatis)
    glEnable(GL_LIGHT1);
    GLfloat light1_pos[] = {-5.0f, 2.0f, 0.0f, 1.0f};    // 1.0f artinya point light
    GLfloat light1_diffuse[] = {0.8f, 0.2f, 0.2f, 1.0f}; // Merah
    GLfloat light1_specular[] = {1.0f, 0.5f, 0.5f, 1.0f};
    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);

    // Global Ambient
    GLfloat global_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Setup Kamera (Orbit)
    float camX = 10.0f * sin(angleX * 0.01745f);
    float camZ = 10.0f * cos(angleX * 0.01745f);
    gluLookAt(camX, 5.0f + angleY / 10.0f, camZ,
              0.0f, 1.5f, 0.0f,
              0.0f, 1.0f, 0.0f);

    // Draw Objects
    drawFloor();
    drawCreeper();

    // Visualisasi posisi Lampu Merah (Objek ke-3 biar jelas)
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(-5.0f, 2.0f, 0.0f);
    glutSolidSphere(0.2f, 10, 10);
    glPopMatrix();
    glEnable(GL_LIGHTING);

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    if (h == 0)
        h = 1;
    float aspect = (float)w / (float)h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int value)
{
    // Update animasi
    walkAngle += 0.1f;
    headAngle += 0.05f;
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // 60 FPS
}

void keyboard(unsigned char key, int x, int y)
{
    // Interaksi Rotasi Kamera
    if (key == 'a')
        angleX -= 5.0f;
    if (key == 'd')
        angleX += 5.0f;
    if (key == 'w')
        angleY += 5.0f;
    if (key == 's')
        angleY -= 5.0f;
    if (key == 27)
        exit(0); // ESC
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Tugas Creeper OpenGL - Bang Jago");

    GLenum err = glewInit();
    if (GLEW_OK != err)
        return -1;

    initLighting();
    textureCreeper = generateTexture(1);
    textureFloor = generateTexture(2);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);

    std::cout << "Kontrol: W/A/S/D untuk putar kamera." << std::endl;

    glutMainLoop();
    return 0;
}