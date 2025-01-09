#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float currentMatrix[4][4];
float tempMatrix[4][4];
float position[3] = { 0.0f, 0.0f, 0.0f };
float rotation[3] = { 0.0f, 0.0f, 0.0f };
float scale[3] = { 1.0f, 1.0f, 1.0f };


// menu ids
const int MENU_SHEAR_XY = 1;
const int MENU_SHEAR_XZ = 2;
const int MENU_SHEAR_YZ = 3;
const int MENU_REFLECT_X = 4;
const int MENU_REFLECT_Y = 5;
const int MENU_REFLECT_Z = 6;
const int MENU_REFLECT_ORIGIN = 7;
const int MENU_RESET = 8;
const int MENU_ROTATE_X = 9;
const int MENU_ROTATE_Y = 10;
const int MENU_ROTATE_Z = 11;
const int MENU_SCALE = 12;

// text render functions
void renderText(const char* text, float x, float y) {
    glColor3f(0.0f, 0.0f, 0.0f); // Set text color to black
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
    }
}

void renderxaxis(const char* text, float x, float y) {
    glColor3f(1.0f, 0.0f, 0.0f);
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
    }
}

void renderyaxis(const char* text, float x, float y) {
    glColor3f(0.0f, 1.0f, 0.0f);
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
    }
}

void renderzaxis(const char* text, float x, float y) {
    glColor3f(0.0f, 0.0f, 1.0f);
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
    }
}

void printMatrix(const float matrix[4][4], const string& transformationType) {
    cout << "\nApplied Transformation: " << transformationType << "\nTransformed Matrix:\n";
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cout << setw(10) << fixed << setprecision(2) << matrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << "\n----------------------------------------\n";
}

void matrixMultiplication(float mat1[4][4], float mat2[4][4], float result[4][4]) {
    float temp[4][4] = { 0 };

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                temp[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }

    // Copy the result back
    memcpy(result, temp, 16 * sizeof(float));
}

void initIdentityMatrix() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            currentMatrix[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
    printMatrix(currentMatrix, "Initial Identity Matrix");
}

void createTranslationMatrix(float tx, float ty, float tz) {
    float translationMatrix[4][4] = {
        {1, 0, 0, tx},
        {0, 1, 0, ty},
        {0, 0, 1, tz},
        {0, 0, 0, 1}
    };
    matrixMultiplication(translationMatrix, currentMatrix, currentMatrix);
    printMatrix(currentMatrix, "Translation");
}

void createRotationMatrix(float angle, char axis) {
    float rad = angle * M_PI / 180.0f;
    float rotationMatrix[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };

    switch (axis) {
    case 'X':
        rotationMatrix[1][1] = cos(rad);
        rotationMatrix[1][2] = -sin(rad);
        rotationMatrix[2][1] = sin(rad);
        rotationMatrix[2][2] = cos(rad);
        break;
    case 'Y':
        rotationMatrix[0][0] = cos(rad);
        rotationMatrix[0][2] = sin(rad);
        rotationMatrix[2][0] = -sin(rad);
        rotationMatrix[2][2] = cos(rad);
        break;
    case 'Z':
        rotationMatrix[0][0] = cos(rad);
        rotationMatrix[0][1] = -sin(rad);
        rotationMatrix[1][0] = sin(rad);
        rotationMatrix[1][1] = cos(rad);
        break;
    }

    matrixMultiplication(rotationMatrix, currentMatrix, currentMatrix);
    printMatrix(currentMatrix, string("Rotation around ") + axis + "-axis");
}

void applyRotation(char axis) {
    float angle;
    cout << "Enter the angle for rotation around " << axis << "-axis: ";
    cin >> angle;
    createRotationMatrix(angle, axis);
    glutPostRedisplay();
}

void createScaleMatrix(float sx, float sy, float sz) {
    float scaleMatrix[4][4] = {
        {sx, 0,  0,  0},
        {0,  sy, 0,  0},
        {0,  0,  sz, 0},
        {0,  0,  0,  1}
    };
    matrixMultiplication(scaleMatrix, currentMatrix, currentMatrix);
    printMatrix(currentMatrix, "Scale");
}

void applyScale() {
    cout << "Enter scale factors for X, Y, Z: ";
    cin >> scale[0] >> scale[1] >> scale[2];
    createScaleMatrix(scale[0], scale[1], scale[2]);
    glutPostRedisplay();
}

void createShearMatrix(float shx, float shy, float shz, const string& plane) {
    float shearMatrix[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };

    if (plane == "XY") {
        shearMatrix[0][1] = shx;
    }
    else if (plane == "XZ") {
        shearMatrix[0][2] = shx;
    }
    else if (plane == "YZ") {
        shearMatrix[1][2] = shy;
    }

    matrixMultiplication(shearMatrix, currentMatrix, currentMatrix);
    printMatrix(currentMatrix, "Shear " + plane);
}

void createReflectionMatrix(char axis) {
    float reflectionMatrix[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };

    switch (axis) {
    case 'X':
        reflectionMatrix[0][0] = -1;
        break;
    case 'Y':
        reflectionMatrix[1][1] = -1;
        break;
    case 'Z':
        reflectionMatrix[2][2] = -1;
        break;
    case 'O': // Origin
        reflectionMatrix[0][0] = -1;
        reflectionMatrix[1][1] = -1;
        reflectionMatrix[2][2] = -1;
        break;
    }

    matrixMultiplication(reflectionMatrix, currentMatrix, currentMatrix);
    printMatrix(currentMatrix, string("Reflection ") + (axis == 'O' ? "through origin" : string("about ") + axis + "-axis"));
}

void drawAxes() {
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-10.0f, 0.0f, 0.0f);
    glVertex3f(10.0f, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -10.0f, 0.0f);
    glVertex3f(0.0f, 10.0f, 0.0f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -10.0f);
    glVertex3f(0.0f, 0.0f, 10.0f);
    glEnd();
}

void drawCube() {
    // Front face
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glEnd();

    // Back face
    glBegin(GL_QUADS);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glEnd();

    // Top face
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glEnd();

    // Bottom face
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glEnd();

    // Right face
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glEnd();

    // Left face
    glBegin(GL_QUADS);
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set up camera
    //gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
    gluLookAt(3.0f, 3.0f, 3.0f,  // Eye position
        0.0f, 0.0f, 0.0f,   // Look at point
        0.0f, 1.0f, 0.0f);  // Up vector

    drawAxes();

    // apply transformation matrix
    float transformMatrix[16];
    //transpose the matrix
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            transformMatrix[i * 4 + j] = currentMatrix[j][i];
        }
    }
    glMultMatrixf(transformMatrix);

    drawCube();
    // Draw the text
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // Set up an orthographic projection for 2D text

    // Disable depth testing for 2D text
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    renderText("Anchit Das (SAP ID: 500107460)", -0.9f, -0.9f);
    renderText("(AD,WS,QE) for translation", -0.95f, 0.9f);
    renderText("(NumPad-82,46,79) for rotation", -0.95f, 0.85f);
    renderText("(+-) for scaling", -0.95f, 0.8);
    renderText("Right-Click Context Menu for Shearing and Reflection", -0.95f, 0.75f);

    renderxaxis("X-Axis", 0.8f, 0.9f);
    renderyaxis("Y-Axis", 0.8f, 0.8f);
    renderzaxis("Z-Axis", 0.8f, 0.7f);

    // Re-enable depth testing for 3D rendering
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glutSwapBuffers();
}

void processMenuEvents(int option) {
    float shx, shy, shz;

    switch (option) {
    case MENU_SCALE:
        applyScale();
        break;

    case MENU_SHEAR_XY:
        cout << "Enter shear factor for XY: ";
        cin >> shx;
        createShearMatrix(shx, 0, 0, "XY");
        break;

    case MENU_SHEAR_XZ:
        cout << "Enter shear factor for XZ: ";
        cin >> shx;
        createShearMatrix(shx, 0, 0, "XZ");
        break;

    case MENU_SHEAR_YZ:
        cout << "Enter shear factor for YZ: ";
        cin >> shy;
        createShearMatrix(0, shy, 0, "YZ");
        break;

    case MENU_REFLECT_X:
        createReflectionMatrix('X');
        break;

    case MENU_REFLECT_Y:
        createReflectionMatrix('Y');
        break;

    case MENU_REFLECT_Z:
        createReflectionMatrix('Z');
        break;

    case MENU_REFLECT_ORIGIN:
        createReflectionMatrix('O');
        break;

    case MENU_ROTATE_X:
        applyRotation('X');
        break;

    case MENU_ROTATE_Y:
        applyRotation('Y');
        break;

    case MENU_ROTATE_Z:
        applyRotation('Z');
        break;

    case MENU_RESET:
        initIdentityMatrix();
        glutPostRedisplay();
        break;
    }

    glutPostRedisplay();
}

void createMenu() {
    int menu = glutCreateMenu(processMenuEvents);
    glutAddMenuEntry("Scale", MENU_SCALE);
    glutAddMenuEntry("Rotate around X-axis", MENU_ROTATE_X);
    glutAddMenuEntry("Rotate around Y-axis", MENU_ROTATE_Y);
    glutAddMenuEntry("Rotate around Z-axis", MENU_ROTATE_Z);
    glutAddMenuEntry("Shear XY", MENU_SHEAR_XY);
    glutAddMenuEntry("Shear XZ", MENU_SHEAR_XZ);
    glutAddMenuEntry("Shear YZ", MENU_SHEAR_YZ);
    glutAddMenuEntry("Reflect X", MENU_REFLECT_X);
    glutAddMenuEntry("Reflect Y", MENU_REFLECT_Y);
    glutAddMenuEntry("Reflect Z", MENU_REFLECT_Z);
    glutAddMenuEntry("Reflect Origin", MENU_REFLECT_ORIGIN);
    glutAddMenuEntry("Reset", MENU_RESET);  
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void keyboard(unsigned char key, int x, int y) {
    const float moveSpeed = 0.1f;
    const float scaleSpeed = 0.1f;
    //const float rotateSpeed = 0.1f;
    const float rotateSpeed = 5.0f;

    switch (key) {
        // Translation
    case 'w': 
        createTranslationMatrix(0.0f, moveSpeed, 0.0f);
        break;
    
    case 's':
        createTranslationMatrix(0.0f, -moveSpeed, 0.0f);
        break;
    
    case 'a':
        createTranslationMatrix(-moveSpeed, 0.0f, 0.0f);
        break;
    
    case 'd':
        createTranslationMatrix(moveSpeed, 0.0f, 0.0f);
        break;
    
    case 'q':
        createTranslationMatrix(0.0f, 0.0f, moveSpeed);
        break;
    
    case 'e':
        createTranslationMatrix(0.0f, 0.0f, -moveSpeed);
        break;

        // Scaling
    case '+':
        createScaleMatrix(1.0f + scaleSpeed, 1.0f + scaleSpeed, 1.0f + scaleSpeed);
        break;
    
    case '-':
        createScaleMatrix(1.0f - scaleSpeed, 1.0f - scaleSpeed, 1.0f - scaleSpeed);
        break;

        // Rotation
    case '8':
        createRotationMatrix(rotateSpeed, 'X');
        break;
    
    case '2':
        createRotationMatrix(-rotateSpeed, 'X');
        break;
    
    case '4':
        createRotationMatrix(-rotateSpeed, 'Y');
        break;
    
    case '6':
        createRotationMatrix(rotateSpeed, 'Y');
        break;
    
    case '7':
        createRotationMatrix(-rotateSpeed, 'Z');
        break;
    
    case '9':
        createRotationMatrix(rotateSpeed, 'Z');
        break;
    }
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Transformations-Anchit Das");

    glEnable(GL_DEPTH_TEST);
    initIdentityMatrix();
    createMenu();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}