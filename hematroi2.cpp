#include <GL/glut.h>
#include <cmath>
#include <cstring>
#include <cstdlib> // For random generation

// Góc quay cua mat troi và các hành tinh
float sunAngle = 0.0f;
float planetOrbitAngles[9] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};  
float planetSelfAngles[9] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; 

// Du lieu hành tinh
struct Planet {
    float orbitRadius;
    float planetRadius;
    float orbitSpeed;
    float selfSpeed;
    float color[3];
    const char* name;
} planets[] = {
    {1.5f, 0.3f, 0.9f, 1.5f, {0.5f, 0.5f, 0.5f}, ""}, 
    {3.0f, 0.3f, 0.7f, 1.0f, {0.9f, 0.5f, 0.2f}, "Venus"},   
    {5.0f, 0.4f, 0.5f, 1.0f, {0.0f, 0.9f, 0.0f}, "Earth"},   
    {7.0f, 0.3f, 0.4f, 0.9f, {1.0f, 0.0f, 0.0f}, "Mars"},     
    {9.0f, 0.8f, 0.45f, 0.5f, {1.0f, 0.9f, 0.5f}, "Jupiter"},  
    {11.0f, 0.7f, 0.2f, 0.4f, {1.0f, 0.5f, 0.0f}, "Saturn"}, 
    {13.0f, 0.6f, 0.1f, 0.3f, {0.9f, 0.8f, 0.4f}, "Uranus"}, 
    {15.0f, 0.5f, 0.05f, 0.2f, {0.0f, 0.5f, 1.0f}, "Neptune"}, 
    {17.0f, 1.0f, 0.01f, 0.1f, {0.0f, 0.0f, 0.5f}, "Pluto"} 
};

// Các bien dieu khien góc nhìn
float cameraX = 0.0f, cameraY = 10.0f, cameraZ = 20.0f; 
float cameraDistance = 20.0f; 
float theta = 45.0f, phi = 45.0f; 

// Hàm ve (hành tinh)
void drawSphere(float radius, float r, float g, float b) {
    glColor3f(r, g, b);  
    glutSolidSphere(radius, 50, 50);
}

// Hàm ve quy dao cua hành tinh

void drawOrbit(float radius) {
    glColor3f(0.3f, 0.3f, 0.3f);  
    glLineWidth(2.0f); 

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0f;
        glVertex3f(radius * cos(angle), 0.0f, radius * sin(angle));
    }
    glEnd();

    glLineWidth(1.0f); 
}


// Hàm ve chu
void drawText(float x, float y, const char* text) {
    glRasterPos3f(x, y, 0.0f); 
    for (int i = 0; i < strlen(text); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]); 
    }
}


struct Star {
    float x, y, z;
} stars[500];  

// Hàm kh?i t?o ngôi sao
void initStars() {
    for (int i = 0; i < 500; i++) {
        stars[i].x = (rand() % 200 - 100) / 8.0f;  
        stars[i].y = (rand() % 200 - 100) / 8.0f;  
        stars[i].z = (rand() % 200 - 100) / 8.0f;  
    }
}

//quay quanh truc Oy
void rotateYMatrix(GLfloat angle) {
    GLfloat radian = angle * M_PI / 180.0f;
    GLfloat rotationMatrix[16] = {
        cos(radian), 0.0f, -sin(radian), 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        sin(radian), 0.0f, cos(radian), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    glMultMatrixf(rotationMatrix); // Áp d?ng ma tr?n quay quanh tr?c Y
}

// Hàm ve ngôi sao
void drawStars() {
    glPointSize(2.0f); 
    glBegin(GL_POINTS); 
    for (int i = 0; i < 500; i++) {
        glColor3f(1.0f, 1.0f, 1.0f); 
        glVertex3f(stars[i].x, stars[i].y, stars[i].z);  
    }
    glEnd();
}

// Hàm UpdateCameraPosition 
void UpdateCameraPosition(GLfloat *x0, GLfloat *y0, GLfloat *z0, GLfloat theta, GLfloat phi, GLfloat R) {
    *x0 = R * cos(phi * M_PI / 180.0f) * sin(theta * M_PI / 180.0f);
    *y0 = R * sin(phi * M_PI / 180.0f);
    *z0 = R * cos(phi * M_PI / 180.0f) * cos(theta * M_PI / 180.0f);
}
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Cap nhat vi trí camera theo he toa do cau
    UpdateCameraPosition(&cameraX, &cameraY, &cameraZ, theta, phi, cameraDistance);

    // Thiet lap vi trí camera
    gluLookAt(cameraX, cameraY, cameraZ,  
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);

    glPushMatrix();

    // Ve Mat Troi
    glPushMatrix();
    glRotatef(sunAngle, 0.0f, 1.0f, 0.0f);
    drawSphere(0.8f, 1.0f, 0.8f, 0.0f);  
    glPopMatrix();

    // Ve các hành tinh
    for (int i = 0; i < 9; i++) {
        Planet planet = planets[i];

        // Ve quy dao hành tinh
        drawOrbit(planet.orbitRadius);

        // Ve hành tinh
        glPushMatrix();
        rotateYMatrix(planetOrbitAngles[i]);
        
        glTranslatef(planet.orbitRadius, 0.0f, 0.0f); 

        // Hành tinh tu quay
        glPushMatrix();
        rotateYMatrix(planetOrbitAngles[i]);
        drawSphere(planet.planetRadius, planet.color[0], planet.color[1], planet.color[2]);
        glPopMatrix();

        // Ve tên hành tinh
        glPushMatrix();
        rotateYMatrix(planetOrbitAngles[i]);
        drawText(planet.planetRadius + 0.8f, 0.2f, planet.name);
        glPopMatrix();
        glPopMatrix();
    }

    // Ve Mat Trang cua Trái Ðat
    glPushMatrix();
    rotateYMatrix(planetOrbitAngles[2]); 
    glTranslatef(planets[2].orbitRadius, 0.0f, 0.0f); 
    
    //quay quanh tdat
    glPushMatrix();
    rotateYMatrix(sunAngle * 20);  
    glTranslatef(1.0f, 0.0f, 0.0f);  
    drawSphere(0.2f, 0.7f, 0.7f, 0.7f);  
 //   drawText(0.8f, 0.2f, "Moon");
    glPopMatrix();
    glPopMatrix();

    // Ve các ngôi sao
    glPushMatrix(); 
    drawStars();  
    glPopMatrix();  

    glutSwapBuffers();
}
// Hàm cap nhat góc quay cua các hành tinh và Mat Troi
void update(int value) {
    sunAngle += 0.1f; 

    for (int i = 0; i < 9; i++) {  
        planetOrbitAngles[i] += planets[i].orbitSpeed;
        if (planetOrbitAngles[i] > 360.0f) {
            planetOrbitAngles[i] -= 360.0f;
        }

        planetSelfAngles[i] += planets[i].selfSpeed;
        if (planetSelfAngles[i] > 360.0f) {
            planetSelfAngles[i] -= 360.0f;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// Hàm khoi tao môi truong
void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 1.0f, 0.1f, 100.0f);  //goc nhin
    glMatrixMode(GL_MODELVIEW);
}


void keyboard(unsigned char key, int x, int y) {
    if (key == 'w') {
        phi += 1.0f;
    }
    if (key == 's') {
        phi -= 1.0f;
    }
    if (key == 'a') {
        theta += 1.0f;
    }
    if (key == 'd') {
        theta -= 1.0f;
    }
    if (key == '+') {
        cameraDistance -= 1.0f;
    }
    if (key == '-') {
        cameraDistance += 1.0f;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Solar System with Moon and Stars");
    init();
    initStars(); 
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(25, update, 0);
    glutMainLoop();
    return 0;
}

