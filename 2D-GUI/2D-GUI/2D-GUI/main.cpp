#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float currentMatrix[3][3];
float tempMatrix[3][3];
float position[2] = { 0.0f, 0.0f };
float rotation = 0.0f;
float scale[2] = { 1.0f, 1.0f };

const int MENU_TRANSLATE = 1;
const int MENU_SHEAR_XY = 2;
const int MENU_SHEAR_XZ = 3;
const int MENU_SHEAR_YZ = 4;
const int MENU_REFLECT_X = 5;
const int MENU_REFLECT_Y = 6;
const int MENU_REFLECT_Z = 7;
const int MENU_REFLECT_ORIGIN = 8;
const int MENU_RESET = 9;
const int MENU_ROTATE_X = 10;
const int MENU_ROTATE_Y = 11;
const int MENU_ROTATE_Z = 12;
const int MENU_SCALE = 13;

void renderText(const char* text, float x, float y) {
    glColor3f(0.0f, 0.0f, 0.0f);
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

void printMatrix(const float matrix[3][3], const string& transformationType) {
    cout << "\nApplied Transformation: " << transformationType << "\nTransformed Matrix:\n";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << setw(10) << fixed << setprecision(2) << matrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << "\n----------------------------------------\n";
}

void matrixMultiplication(float mat1[3][3], float mat2[3][3], float result[3][3]) {
    float temp[3][3] = { 0 };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            temp[i][j] = 0;
            for (int k = 0; k < 3; k++) {
                temp[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }

    memcpy(result, temp, 9 * sizeof(float));
}

void initIdentityMatrix() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            currentMatrix[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
    printMatrix(currentMatrix, "Initial Identity Matrix");
}

void createTranslationMatrix(float tx, float ty) {
    float translationMatrix[3][3] = {
        {1, 0, tx},
        {0, 1, ty},
        {0, 0, 1}
    };
    matrixMultiplication(translationMatrix, currentMatrix, currentMatrix);
    printMatrix(currentMatrix, "Translation");
}

void createRotationMatrix(float angle) {
    float rad = angle * M_PI / 180.0f;
    float rotationMatrix[3][3] = {
        {cos(rad), -sin(rad), 0},
        {sin(rad), cos(rad),  0},
        {0,        0,         1}
    };
    matrixMultiplication(rotationMatrix, currentMatrix, currentMatrix);
    printMatrix(currentMatrix, "Rotation");
}

void applyRotation() {
    float angle;
    cout << "Enter the angle for rotation: ";
    cin >> angle;
    createRotationMatrix(angle);
    glutPostRedisplay();
}

void createScaleMatrix(float sx, float sy) {
    float scaleMatrix[3][3] = {
        {sx, 0,  0},
        {0,  sy, 0},
        {0,  0,  1}
    };
    matrixMultiplication(scaleMatrix, currentMatrix, currentMatrix);
    printMatrix(currentMatrix, "Scale");
}

void applyScale() {
    cout << "Enter scale factors for X, Y: ";
    cin >> scale[0] >> scale[1];
    createScaleMatrix(scale[0], scale[1]);
    glutPostRedisplay();
}

void createShearMatrix(float shx, float shy) {
    float shearMatrix[3][3] = {
        {1,   shx, 0},
        {shy, 1,   0},
        {0,   0,   1}
    };
    matrixMultiplication(shearMatrix, currentMatrix, currentMatrix);
    printMatrix(currentMatrix, "Shear");
}

void createReflectionMatrix(char axis) {
    float reflectionMatrix[3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };

    switch (axis) {
    case 'X':
        reflectionMatrix[1][1] = -1;
        break;
    case 'Y':
        reflectionMatrix[0][0] = -1;
        break;
    case 'O': // Origin
        reflectionMatrix[0][0] = -1;
        reflectionMatrix[1][1] = -1;
        break;
    }

    matrixMultiplication(reflectionMatrix, currentMatrix, currentMatrix);
    printMatrix(currentMatrix, string("Reflection ") + (axis == 'O' ? "through origin" : string("about ") + axis + "-axis"));
}

void drawAxes() {
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-10.0f, 0.0f);
    glVertex2f(10.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(0.0f, -10.0f);
    glVertex2f(0.0f, 10.0f);
    glEnd();
}


void drawSquare() {
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f);  // Red color
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(-0.5f, 0.5f);
    glEnd();

    /*
    // Draw outline
    glColor3f(0.0f, 0.0f, 0.0f);  // Black outline
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(-0.5f, 0.5f);
    glEnd();
    */
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawAxes();

    // save current matrix
    glPushMatrix();

    // apply transformation matrix
    GLfloat transformMatrix[16] = {
        currentMatrix[0][0], currentMatrix[1][0], 0.0f, currentMatrix[2][0],
        currentMatrix[0][1], currentMatrix[1][1], 0.0f, currentMatrix[2][1],
        0.0f, 0.0f, 1.0f, 0.0f,
        currentMatrix[0][2], currentMatrix[1][2], 0.0f, currentMatrix[2][2]
    };

    glMultMatrixf(transformMatrix);

    drawSquare();

    // restore matrix
    glPopMatrix();

    glLoadIdentity();  // reset matrix before drawing text
    renderText("Anchit Das (SAP ID: 500107460)", -9.0f, -9.0f);
    renderText("(WASD) for translation", -9.5f, 9.0f);
    renderText("(NumPad-82,46) for rotation", -9.5f, 8.5f);
    renderText("(+-) for scaling", -9.5f, 8.0f);
    renderText("Right-Click Context Menu for Shearing and Reflection", -9.5f, 7.5f);

    renderxaxis("X-Axis", 8.0f, 9.0f);
    renderyaxis("Y-Axis", 8.0f, 8.0f);

    glutSwapBuffers();
}


void processMenuEvents(int option) {
    float tx, ty;
    float shx, shy;

    switch (option) {
    case MENU_TRANSLATE:
        cout << "Enter translation values (tx, ty): ";
        cin >> tx >> ty;
        createTranslationMatrix(tx, ty);
        break;

    case MENU_SCALE:
        applyScale();
        break;

    case MENU_SHEAR_XY:
        cout << "Enter shear factors for X and Y: ";
        cin >> shx >> shy;
        createShearMatrix(shx, shy);
        break;

    case MENU_REFLECT_X:
        createReflectionMatrix('X');
        break;

    case MENU_REFLECT_Y:
        createReflectionMatrix('Y');
        break;

    case MENU_REFLECT_ORIGIN:
        createReflectionMatrix('O');
        break;

    case MENU_RESET:
        initIdentityMatrix();
        break;
    }

    glutPostRedisplay();
}

void createMenu() {
    int menu = glutCreateMenu(processMenuEvents);
    glutAddMenuEntry("Translate", MENU_TRANSLATE);
    glutAddMenuEntry("Scale", MENU_SCALE);
    glutAddMenuEntry("Shear", MENU_SHEAR_XY);
    glutAddMenuEntry("Reflect X", MENU_REFLECT_X);
    glutAddMenuEntry("Reflect Y", MENU_REFLECT_Y);
    glutAddMenuEntry("Reflect Origin", MENU_REFLECT_ORIGIN);
    glutAddMenuEntry("Reset", MENU_RESET);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void keyboard(unsigned char key, int x, int y) {
    const float moveSpeed = 0.1f;
    const float scaleSpeed = 0.1f;
    const float rotateSpeed = 5.0f;

    switch (key) {
        // Translation
    case 'w':
        createTranslationMatrix(0.0f, moveSpeed);
        break;
    case 's':
        createTranslationMatrix(0.0f, -moveSpeed);
        break;
    case 'a':
        createTranslationMatrix(-moveSpeed, 0.0f);
        break;
    case 'd':
        createTranslationMatrix(moveSpeed, 0.0f);
        break;

        // Scaling
    case '+':
        createScaleMatrix(1.0f + scaleSpeed, 1.0f + scaleSpeed);
        break;
    case '-':
        createScaleMatrix(1.0f - scaleSpeed, 1.0f - scaleSpeed);
        break;

        // Rotation
    case '8':
        createRotationMatrix(rotateSpeed);
        break;
    case '2':
        createRotationMatrix(-rotateSpeed);
        break;
    case '4':
        createRotationMatrix(-rotateSpeed);
        break;
    case '6':
        createRotationMatrix(rotateSpeed);
        break;
    }
    glutPostRedisplay();
}

void reshape(int w, int h) {
    // prevent division by zero
    if (h == 0) h = 1;

    glViewport(0, 0, w, h);
    float aspect = (float)w / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Keep square aspect ratio
    if (w <= h) {
        gluOrtho2D(-10.0, 10.0, -10.0 / aspect, 10.0 / aspect);
    }
    else {
        gluOrtho2D(-10.0 * aspect, 10.0 * aspect, -10.0, 10.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("2D Transformations-Anchit Das");

    // Initialize the matrix before creating menu
    initIdentityMatrix();
    createMenu();

    // Set white background
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}