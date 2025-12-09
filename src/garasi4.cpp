#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION

//Variable global
bool isLightingOn = true; //buat lampu
//Posisi mobil
float carX = 0.0f;
float carZ = 6.0f;
float carAngle = 180.0f;
float carScale = 1.0f;
// Lokasi kamera dalam dunia
float cameraOffsetX = 15.0f;
float cameraOffsetY = 14.0f;
float cameraOffsetZ = 25.0f;
// Titik yang dilihat kamera
float cameraLookAtX = 0.0f;
float cameraLookAtY = 1.5f;
float cameraLookAtZ = 0.0f;
// Tekstur tanah
const char dirtFile[] = "src/dirt.png";
const GLenum format = GL_RGB;
int dirtWidth, dirtHeight;
int imageChannel = 4;
GLuint dirtTexture;

//function untuk import gambar tanah & membuat tekstur
void importDirt(){ 
  // import png tanah
  unsigned char* png = stbi_load(dirtFile, &dirtWidth, &dirtHeight, &imageChannel, 3);
  
  // cek error
  if (!png) {
    std::cout << "Tekstur gagal di load!"
              << "\nAlasan: " << stbi_failure_reason()
              << "\nPastikan file \"dirt.png\" terdapat di `/src` bersama dengan file UAS.cpp dan eksekusi kode di root projek (di atas /bin dan /src)!"
              << "\nLantai akan tidak bertekstur!";
    return;
  }
  
  // buat tekstur opengl
  glGenTextures(1, &dirtTexture);
  glBindTexture(GL_TEXTURE_2D, dirtTexture);
  
  // parameter tekstur
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
  // upload tekstur ke GPU
  glTexImage2D(GL_TEXTURE_2D, 0, format, dirtWidth, dirtHeight, 0, format, GL_UNSIGNED_BYTE, png);
    
  std::cout << "Texture loaded successfully!" << std::endl;
}

void drawGround() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, dirtTexture);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    float groundMat[] = {1, 1, 1, 1};   // putih → biar cahaya bereaksi
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, groundMat);
    // ==========================

    glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);
        glTexCoord2f(0, 0);
        glVertex3f(-50, 0, -50);
        glNormal3f(0, 1, 0);
        glTexCoord2f(50, 0); 
        glVertex3f(50, 0, -50);
        glNormal3f(0, 1, 0);
        glTexCoord2f(50, 50); 
        glVertex3f(50, 0, 50);
        glNormal3f(0, 1, 0);
        glTexCoord2f(0, 50); 
        glVertex3f(-50, 0, 50);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

// Fungsi bantu menggambar kotak
void drawBox(float sx, float sy, float sz) {
  glPushMatrix();
  glScalef(sx, sy, sz);
  glutSolidCube(1.0f);
  glPopMatrix();
}

void drawLamppost(float x, float z) {
  // Tiang
  float pole[] = {0.3f, 0.3f, 0.35f, 1.0f};
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, pole);

  glPushMatrix();
  glTranslatef(x, 3.0f, z);
  drawBox(0.2f, 6.0f, 0.2f);
  glPopMatrix();

  // Lengan lampu
  glPushMatrix();
  glTranslatef(x, 6.1f, z - 0.7f);
  drawBox(0.2f, 0.2f, 1.4f);
  glPopMatrix();

  // Bohlam (Sumber cahaya visual)
  float bulb[] = {1.0f, 0.9f, 0.6f, 1.0f};
  float emit[] = {0.9f, 0.85f, 0.6f, 1.0f}; // Membuat objek terlihat bersinar sendiri
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, bulb);
  glMaterialfv(GL_FRONT, GL_EMISSION, emit);

  glPushMatrix();
  glTranslatef(x, 5.7f, z - 1.0f);
  glutSolidSphere(0.25, 16, 16);
  glPopMatrix();

  // Reset emisi ke 0 agar objek lain tidak ikut bersinar
  float off[] = {0, 0, 0, 1};
  glMaterialfv(GL_FRONT, GL_EMISSION, off);
}

void drawGarage() {
  float wall[] = {0.45f, 0.45f, 0.5f, 1.0f};
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wall);

  //ini dinding Belakang
  glPushMatrix();
  glTranslatef(0.0f, 2.5f, -2.5f);
  drawBox(8.0f, 5.0f, 0.5f);
  glPopMatrix();

  //ini dinding Kiri
  glPushMatrix();
  glTranslatef(-4.25f, 2.5f, 1.0f);
  drawBox(0.5f, 5.0f, 7.0f);
  glPopMatrix();

  //ini dinding Kanan
  glPushMatrix();
  glTranslatef(4.25f, 2.5f, 1.0f);
  drawBox(0.5f, 5.0f, 7.0f);
  glPopMatrix();

  //Ini dinding atas
  glPushMatrix();
  glTranslatef(0.0f, 5.0f, 1.0f);
  drawBox(9.0f, 0.5f, 7.0f);
  glPopMatrix();

  // Lantai Garasi (full mengikuti dinding)
  float floorMat[] = {0.6f, 0.6f, 0.6f, 1.0f};
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, floorMat);

  glBegin(GL_QUADS);
  glNormal3f(0, 1, 0);
  glVertex3f(-4.0f, 0.01f, -2.5f);
  glVertex3f(4.0f, 0.01f, -2.5f);
  glVertex3f(4.0f, 0.01f, 4.5f);
  glVertex3f(-4.0f, 0.01f, 4.5f);
  glEnd();
}

void drawCar()
{
  float yellow[] = {1.0f, 1.0f, 0.0f, 1.0f}; //Warna mobil Kuning
  float spec[] = {1, 1, 1, 1}; //Kilap putih
  float shine[] = {10}; //Tingkat kekilapan
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
  glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
  glMaterialfv(GL_FRONT, GL_SHININESS, shine);

  // Transformasi global mobil
  glPushMatrix();
  glTranslatef(carX, 0.5f, carZ);         // Pindah mobil ke posisi X,Z
  glRotatef(carAngle, 0, 1, 0);           // Putar sesuai arah hadap
  glScalef(carScale, carScale, carScale); // Skala mobil

  // --- Body Mobil ---
  drawBox(2.0f, 1.0f, 3.6f);
  // --- Kabin Atas --- hilangin
  glPushMatrix();
  glTranslatef(0, 0.8f, 0.5f);
  drawBox(1.8f, 0.9f, 1.8f);
  glPopMatrix();

  // --- Material Roda ---
  float wheel[] = {0.1f, 0.1f, 0.1f, 1.0f};
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wheel);

  // --- Roda kiri-depan ---
  glPushMatrix();
  glTranslatef(-1.0f, -0.3f, 1.3f);
  glRotatef(90, 0, 1, 0);
  glutSolidTorus(0.1, 0.3, 10, 20);
  glPopMatrix();

  // --- Roda kanan-depan ---
  glPushMatrix();
  glTranslatef(1.0f, -0.3f, 1.3f);
  glRotatef(90, 0, 1, 0);
  glutSolidTorus(0.1, 0.3, 10, 20);
  glPopMatrix();

  // --- Roda kiri-belakang ---
  glPushMatrix();
  glTranslatef(-1.0f, -0.3f, -1.3f);
  glRotatef(90, 0, 1, 0);
  glutSolidTorus(0.1, 0.3, 10, 20);
  glPopMatrix();

  //ini roda kanan belakang
  glPushMatrix();
  glTranslatef(1.0f, -0.3f, -1.3f);
  glRotatef(90, 0, 1, 0);
  glutSolidTorus(0.1, 0.3, 10, 20);
  glPopMatrix();

  //ini Lampu Depan
  float lamp[] = {1, 1, 0.8f, 1};
  float lampEm[] = {0.8f, 0.8f, 0.5f, 1};
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, lamp);
  glMaterialfv(GL_FRONT, GL_EMISSION, lampEm);

  glPushMatrix();
  glTranslatef(0.6f, 0.0f, -1.9f);
  glutSolidSphere(0.2, 12, 12);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-0.6f, 0.0f, -1.9f);
  glutSolidSphere(0.2, 12, 12);
  glPopMatrix();

  // Matikan emisi agar objek lain tidak ikut bersinar
  float off[] = {0, 0, 0, 1};
  glMaterialfv(GL_FRONT, GL_EMISSION, off);

  glPopMatrix(); // Selesai menggambar mobil
}

void updateLampPostLight(GLenum lightID, float x, float z) {
    float lampPos[] = { x, 5.7f, z - 1.0f, 1.0f }; 
    float dir[] = { 0.0f, -1.0f, 0.0f }; 
    float color[] = { 1.0f, 0.9f, 0.6f, 1.0f };

    glLightfv(lightID, GL_POSITION, lampPos);
    glLightfv(lightID, GL_DIFFUSE, color);
    glLightfv(lightID, GL_SPOT_DIRECTION, dir);
    glLightf(lightID, GL_SPOT_CUTOFF, 35); 
    glLightf(lightID, GL_SPOT_EXPONENT, 20);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  // Posisi Kamera (Mata, Target, Up-Vector)
  gluLookAt(cameraOffsetX, cameraOffsetY, cameraOffsetZ, // Posisi kamera (serong atas)
            cameraLookAtX, cameraLookAtY, cameraLookAtZ, // Melihat ke tengah scene
            0, 1, 0); // Atas adalah sumbu Y positif

    updateLampPostLight(GL_LIGHT1, -6, 6);  // lampu kiri
    updateLampPostLight(GL_LIGHT2, 6, 6);   // lampu kanan

  if (isLightingOn) {
      glEnable(GL_LIGHT1);
      glEnable(GL_LIGHT2);
  } else {
      glDisable(GL_LIGHT1);
      glDisable(GL_LIGHT2);
  }
  drawGround();
  drawGarage();
  drawCar();
  drawLamppost(-6, 6); // Lampu kiri
  drawLamppost(6, 6); // Lampu kanan
  glutSwapBuffers();
}

void initLighting() {
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);

  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

  // Cahaya Ambience (Cahaya dasar malam hari, agak biru gelap)
  float amb[] = {0.1f, 0.1f, 0.25f, 1};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

  // Cahaya Bulan (Directional Light karena w=0)
  float l0p[] = {0, 10, 5, 0};
  float l0d[] = {0.4f, 0.4f, 0.5f, 1}; // Cahaya redup kebiruan
  glLightfv(GL_LIGHT0, GL_POSITION, l0p);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, l0d);
  glEnable(GL_LIGHT0);
}

void reshape(int w, int h) {
  if (h == 0)
    h = 1;
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(50, (float) w / h, 1, 100);
  glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
  float speed = 0.5f;
  float turn = 5;  // Konversi derajat ke radian
  float r = carAngle * 3.14159f / 180.0f;

  switch (key) {
    case 27:
      exit(0);

    //toggle lampu
    case 'v':
    case 'V':
      isLightingOn = !isLightingOn;
      break;

    // Pergerakan mobil
    case 'w':
    case 'W':
      carX -= sin(r) * speed;
      carZ -= cos(r) * speed;
      break;

    case 'a':
    case 'A':
      carAngle += turn;
      break;

    case 's':
    case 'S':
      carX += sin(r) * speed;
      carZ += cos(r) * speed;
      break;

    case 'd':
    case 'D':
      carAngle -= turn;
      break;

    case 'z':
    case 'Z':
      if (carScale > 0.03f)
        carScale -= 0.05f;
      break;

    case 'x':
    case 'X':
      carScale += 0.05f;
      break;

    // Pergerakan Kamera

    case 'i':
    case 'I':
      cameraOffsetY += 0.1f;
      break;

    case 'j':
    case 'J':
      cameraOffsetX -= 0.1f;
      break;

    case 'k':
    case 'K':
      cameraOffsetY -= 0.1f;
      break;

    case 'l':
    case 'L':
      cameraOffsetX += 0.1f;
      break;

    case 'u':
    case 'U':
      cameraOffsetZ -= 0.1f;
      break;

    case 'o':
    case 'O':
      cameraOffsetZ += 0.1f;
      break;

    // Titik yang dilihat kamera

    case 't':
    case 'T':
      cameraLookAtY += 0.1f;
      break;

    case 'f':
    case 'F':
      cameraLookAtX -= 0.1f;
      break;

    case 'g':
    case 'G':
      cameraLookAtY -= 0.1f;
      break;

    case 'h':
    case 'H':
      cameraLookAtX += 0.1f;
      break;

    case 'r':
    case 'R':
      cameraLookAtZ -= 0.1f;
      break;

    case 'y':
    case 'Y':
      cameraLookAtZ += 0.1f;
  }

  glutPostRedisplay();
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(0);
  glutInitWindowSize(1000, 700);
  glutCreateWindow("UAS GRAFIK KOMPUTER");
  // Inisialisasi GLEW (Wajib dicek errornya)
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    return 1;
  }
  glClearColor(0.02f, 0.02f, 0.1f, 1); // Langit malam biru tua
  //Load tekstur tanahoad dirt texture
  importDirt();
  initLighting(); // Nyalakan lampu
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);

  std::cout << "Kontrol: W/A/S/D untuk gerak, Z/X untuk scale, ESC untuk keluar." << std::endl;
  glutMainLoop();
  return 0;
}
