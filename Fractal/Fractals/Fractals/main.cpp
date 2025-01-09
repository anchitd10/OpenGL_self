#include <GL/glut.h>
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// parameters
float zoomFactor = 1.0f;
float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraZ = 5.0f;
float rotationAngle = 0.0f;
bool isAutoRotating = true;

void renderText(const char* text) {
    // disable lighting for text rendering
    glDisable(GL_LIGHTING);
    
    // projection mode to render 2D text
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    
    // Set up orthographic projection
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    gluOrtho2D(0, w, h, 0);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    
    glRasterPos2i(40, 60);
    
    // Draw each character
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *text++);
    }
    
    // Restore previous matrices
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    // re-enable lighting
    glEnable(GL_LIGHTING);
}

void drawSphere(float x, float y, float z, float radius) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glutSolidSphere(radius, 20, 20);
    glPopMatrix();
}

// generate the foam fractal
void generateFoam(float x, float y, float z, float radius, int depth) {
    if (depth == 0) return;

    // draw the current sphere
    glColor3f(0.2f + 0.1f * depth, 0.3f + 0.1f * depth, 0.5f + 0.05f * depth); // gradient colors
    drawSphere(x, y, z, radius);

    // create child spheres around the current sphere
    float newRadius = radius * 0.5; // smaller spheres
    for (int i = 0; i < 12; i++) { // spread in 3D space
        float angle1 = (i / 6.0) * 2 * M_PI;   // circular pattern on X-Y plane
        float angle2 = ((i + 1) % 6) * M_PI / 3; // alternating Z-depth
        float dx = newRadius * 2 * cos(angle1);
        float dy = newRadius * 2 * sin(angle1);
        float dz = newRadius * 2 * cos(angle2);
        generateFoam(x + dx, y + dy, z + dz, newRadius, depth - 1);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // reset model view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // camera
    gluLookAt(cameraX, cameraY, cameraZ,   // eye position
        0.0, 0.0, 0.0,               // look at the origin
        0.0, 1.0, 0.0);              // up vector

    if (isAutoRotating) {
        glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);
    }

    // Apply zoom
    glPushMatrix();
    glScalef(zoomFactor, zoomFactor, zoomFactor);

    // call recursion
    generateFoam(0.0f, 0.0f, 0.0f, 0.8f, 5); // Depth 5 foam with radius 0.8
    generateFoam(2.0f, 0.0f, 1.0f, 0.8f, 5);
    generateFoam(-2.0f, 0.0f, -1.0f, 0.8f, 5);

    glPopMatrix();

    // Render text as static 2D overlay
    renderText("Anchit(50010746)");

    glutSwapBuffers();
}

// continuous rotation
void idle() {
    if (isAutoRotating) {
        rotationAngle += 0.5f;
        if (rotationAngle >= 360.0f) {
            rotationAngle -= 360.0f;
        }
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '+':
        zoomFactor *= 1.1f;
        break;
    case '-':
        zoomFactor /= 1.1f;
        break;
    case 'r':
    case 'R':
        isAutoRotating = !isAutoRotating;
        break;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    float moveSpeed = 0.1f;
    switch (key) {
    case GLUT_KEY_LEFT:
        cameraX -= moveSpeed;
        break;

    case GLUT_KEY_RIGHT:
        cameraX += moveSpeed;
        break;

    case GLUT_KEY_UP:
        cameraY += moveSpeed;
        break;

    case GLUT_KEY_DOWN:
        cameraY -= moveSpeed;
        break;
    }
    glutPostRedisplay();
}

void init() {
    glEnable(GL_DEPTH_TEST); // depth testing
    glEnable(GL_LIGHTING);   // lighting
    glEnable(GL_LIGHT0);     // default light
    glEnable(GL_COLOR_MATERIAL); // use material properties
    glShadeModel(GL_SMOOTH); // Smooth shading

    // Light position and properties
    GLfloat lightPos[] = { 2.0f, 2.0f, 2.0f, 1.0f };
    GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 1.0, 20.0);
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Foam/Bubbles Fractal(Anchit-500107460)");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}