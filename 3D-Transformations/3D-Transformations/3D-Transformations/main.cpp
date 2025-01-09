#include <GL/glut.h>
#include <iostream>
#include <cmath>

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float currentMatrix[4][4];
int currentShape = 1;       // 1: cube, 2: Tetrahedron

// Function to multiply two 4x4 matrices
void matrixMultiplication(float mat1[4][4], float mat2[4][4], float result[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

// Draw the coordinate axes in 3D
void drawAxes() {
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    // X-axis
    glVertex3f(-5.0f, 0.0f, 0.0f);
    glVertex3f(5.0f, 0.0f, 0.0f);
    // Y-axis
    glVertex3f(0.0f, -5.0f, 0.0f);
    glVertex3f(0.0f, 5.0f, 0.0f);
    // Z-axis
    glVertex3f(0.0f, 0.0f, -5.0f);
    glVertex3f(0.0f, 0.0f, 5.0f);
    glEnd();
}

void renderBitmapString(float x, float y, float z, const char* string) {
    glRasterPos3f(x, y, z);
    for (const char* c = string; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
}

void printMatrix(const char* title, float matrix[4][4]) {
    cout << title << ":\n";
    for (int i = 0; i < 4; i++) {
        cout << "[ ";
        for (int j = 0; j < 4; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << "]\n";
    }
}

void applyTransformation(float transformation[4][4]) {
    float result[4][4];
    printMatrix("Transformation Matrix", transformation);
    matrixMultiplication(currentMatrix, transformation, result);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            currentMatrix[i][j] = result[i][j];
        }
    }
    printMatrix("Updated Matrix", currentMatrix);
    glutPostRedisplay();
}

void initIdentityMatrix() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            currentMatrix[i][j] = (i == j) ? 1 : 0;
        }
    }
}

void resetMatrix() {
    initIdentityMatrix();
    glutPostRedisplay();
}

// Draw a 3D Cube
/*
void drawCube() {
    glutWireCube(1.0);
}
*/

void drawCube() {
    glBegin(GL_QUADS);

    // Front face (Red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    // Back face (Green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    // Top face (Blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);

    // Bottom face (Yellow)
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    // Right face (Cyan)
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);

    // Left face (Magenta)
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    glEnd();
}

// Draw a 3D Tetrahedron
/*
void drawTetrahedron() {
    glutWireTetrahedron();
}
*/

void drawTetrahedron() {
    glBegin(GL_TRIANGLES);

    // Face 1  (Red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Face 2 (Green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(0.0f, -1.0f, -1.0f);

    // Face 3 (Blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // Base face (Yellow)
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(0.0f, -1.0f, -1.0f);

    glEnd();
}

// Display function to render shapes with the current transformation matrix
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    drawAxes();
    glPopMatrix();

    glPushMatrix();
    glMultMatrixf(&currentMatrix[0][0]);

    switch (currentShape) {
    case 1:
        drawCube();
        break;
    case 2:
        drawTetrahedron();
        break;
    default:
        break;
    }
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);
    renderBitmapString(0.8f, 1.9f, 0.0f, "Anchit Das-(SAP ID: 500107460)");
    glPopMatrix();

    glFlush();
}

// Context menu for 3D transformations and shape selection
void contextMenu(int choice) {
    float tx, ty, tz, angle, sx, sy, sz;
    float transformation[4][4] = { 0 };

    switch (choice) {
    case 1:  // Translation
        cout << "Enter translation (tx, ty, tz): ";
        cin >> tx >> ty >> tz;

        transformation[0][0] = 1; transformation[0][3] = tx;
        transformation[1][1] = 1; transformation[1][3] = ty;
        transformation[2][2] = 1; transformation[2][3] = tz;
        transformation[3][3] = 1;

        applyTransformation(transformation);
        break;

    case 2:  // Scaling
        cout << "Enter scaling factors (sx, sy, sz): ";
        cin >> sx >> sy >> sz;

        transformation[0][0] = sx;
        transformation[1][1] = sy;
        transformation[2][2] = sz;
        transformation[3][3] = 1;

        applyTransformation(transformation);
        break;

    case 3:  // Rotation around the Z-axis
        cout << "Enter angle of rotation (degrees): ";
        cin >> angle;
        angle = angle * M_PI / 180;  // convert to radians

        transformation[0][0] = cos(angle); transformation[0][1] = -sin(angle);
        transformation[1][0] = sin(angle); transformation[1][1] = cos(angle);
        transformation[2][2] = 1;
        transformation[3][3] = 1;

        applyTransformation(transformation);
        break;

    case 4:  // Reset matrix
        resetMatrix();
        break;

    case 5:  // Change Shape
        int shapeChoice;
        cout << "Enter shape number (1: Cube, 2: Tetrahedron): ";
        cin >> shapeChoice;
        if (shapeChoice == 1 || shapeChoice == 2) {
            currentShape = shapeChoice;
            glutPostRedisplay();
        }
        else {
            cout << "Invalid shape number. Please enter 1 or 2.\n";
        }
        break;

    default:
        break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(750, 750);
    glutCreateWindow("3D Transformation - Anchit Das");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.9f, 0.9f, 0.9f, 0.9f);
    glColor3f(0.0f, 0.0f, 0.0f);
    gluPerspective(45.0, 1.0, 1.0, 10.0);
    glTranslatef(0.0, 0.0, -5.0);

    initIdentityMatrix();
    glutDisplayFunc(display);

    // Create context menu
    glutCreateMenu(contextMenu);
    glutAddMenuEntry("Translate", 1);
    glutAddMenuEntry("Scale", 2);
    glutAddMenuEntry("Rotate", 3);
    glutAddMenuEntry("Reset", 4);
    glutAddMenuEntry("Change Shape", 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}
