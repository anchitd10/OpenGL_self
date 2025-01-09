#include <GL/glut.h>
#include <iostream>
#include <cmath>

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float currentMatrix[3][3];
int currentShape = 1;       // 1: Square, 2: Triangle, 3: Pentagon

void matrixMultiplication(float mat1[3][3], float mat2[3][3], float result[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result[i][j] = 0;
            for (int k = 0; k < 3; k++) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

void drawAxes() {
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    // X-axis
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    // Y-axis
    glVertex2f(0.0f, -1.0f);
    glVertex2f(0.0f, 1.0f);
    glEnd();
}

void renderBitmapString(float x, float y, const char* string) {
    glRasterPos2f(x, y);
    for (const char* c = string; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
}
/*
void printMatrix(float matrix[3][3]) {
    cout << "Transformed Matrix:\n";
    for (int i = 0; i < 3; i++) {
        cout << "[ ";
        for (int j = 0; j < 3; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << "]\n";
    }
}
*/

void printMatrix(const char* title, float matrix[3][3]) {
    cout << title << ":\n";
    for (int i = 0; i < 3; i++) {
        cout << "[ ";
        for (int j = 0; j < 3; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << "]\n";
    }
}

void applyTransformation(float transformation[3][3]) {
    float result[3][3];

    printMatrix("Generic Transformation Matrix", transformation);

    matrixMultiplication(currentMatrix, transformation, result);

    //printMatrix("Applied Transformation Matrix", result);

    /*
    // Debug: Print transformation matrix
    cout << "Applied Transformation Matrix:\n";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << transformation[i][j] << " ";
        }
        cout << "\n";
    }
    */

    // update the current matrix
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            currentMatrix[i][j] = result[i][j];
        }
    }
    //printMatrix(currentMatrix); //transformed matrix
    printMatrix("Transformed Matrix", currentMatrix);
    glutPostRedisplay();
}

void initIdentityMatrix() {
    currentMatrix[0][0] = 1; currentMatrix[0][1] = 0; currentMatrix[0][2] = 0;
    currentMatrix[1][0] = 0; currentMatrix[1][1] = 1; currentMatrix[1][2] = 0;
    currentMatrix[2][0] = 0; currentMatrix[2][1] = 0; currentMatrix[2][2] = 1;
}

void resetMatrix() {
    initIdentityMatrix();
    glutPostRedisplay();
}

void drawSquare() {
    glBegin(GL_POLYGON);
    glVertex2f(-0.5, -0.5);
    glVertex2f(0.5, -0.5);
    glVertex2f(0.5, 0.5);
    glVertex2f(-0.5, 0.5);
    glEnd();
}

void drawTriangle() {
    glBegin(GL_POLYGON);
    glVertex2f(-0.5, -0.5);
    glVertex2f(0.5, -0.5);
    glVertex2f(0.0, 0.5);
    glEnd();
}

void drawPentagon() {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 5; i++) {
        float angle = 2.0f * M_PI * i / 5;  // 5 sides
        glVertex2f(0.5f * cos(angle), 0.5f * sin(angle));
    }
    glEnd();
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw axes without any transformations
    glPushMatrix();
    drawAxes();
    glPopMatrix();

    // Apply the transformation matrix for shapes
    glPushMatrix();
    glLoadIdentity();

    // Transpose the `currentMatrix` before passing it to OpenGL
    float transposeMatrix[16] = {
        currentMatrix[0][0], currentMatrix[1][0], 0, currentMatrix[2][0],
        currentMatrix[0][1], currentMatrix[1][1], 0, currentMatrix[2][1],
        0, 0, 1, 0,
        currentMatrix[0][2], currentMatrix[1][2], 0, currentMatrix[2][2]
    };

    // apply the transformation matrix
    glMultMatrixf(transposeMatrix);

    switch (currentShape) {
    case 1:
        drawSquare();
        break;
    case 2:
        drawTriangle();
        break;
    case 3:
        drawPentagon();
        break;
    }
    glPopMatrix();

    // render text without transformations
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);
    renderBitmapString(0.4f, 0.9f, "Anchit Das-(SAP ID: 500107460)");
    glPopMatrix();

    glFlush();
}

// Context menu
void contextMenu(int choice) {
    float tx, ty, angle, sx, sy, shx, shy;
    float transformation[3][3];
    float translateToOrigin[3][3];
    float translateBack[3][3];

    switch (choice) {
    case 1:  // Translation
        cout << "Enter translation (tx, ty): ";
        cin >> tx >> ty;

        transformation[0][0] = 1;
        transformation[0][1] = 0;
        transformation[0][2] = tx;

        transformation[1][0] = 0;
        transformation[1][1] = 1;
        transformation[1][2] = ty;

        transformation[2][0] = 0;
        transformation[2][1] = 0;
        transformation[2][2] = 1;

        applyTransformation(transformation);
        break;

    case 2:  // Scaling
        cout << "Enter scaling factors (sx, sy): ";
        cin >> sx >> sy;
        transformation[0][0] = sx;
        transformation[0][1] = 0;
        transformation[0][2] = 0;

        transformation[1][0] = 0;
        transformation[1][1] = sy;
        transformation[1][2] = 0;

        transformation[2][0] = 0;
        transformation[2][1] = 0;
        transformation[2][2] = 1;

        applyTransformation(transformation);
        break;

    case 3:  // Rotation
        cout << "Enter angle of rotation (degrees): ";
        cin >> angle;
        angle = angle * M_PI / 180;  // convert to radians
        transformation[0][0] = cos(angle);
        transformation[0][1] = -sin(angle);
        transformation[0][2] = 0;

        transformation[1][0] = sin(angle);
        transformation[1][1] = cos(angle);
        transformation[1][2] = 0;

        transformation[2][0] = 0;
        transformation[2][1] = 0;
        transformation[2][2] = 1;

        applyTransformation(transformation);
        break;

    case 4:  // Reflection
        cout << "Reflect over x-axis (1), y-axis (2), or origin (3)?: ";
        int axis;
        cin >> axis;

        // Translation to the origin
        translateToOrigin[0][0] = 1;
        translateToOrigin[0][1] = 0;
        translateToOrigin[0][2] = -currentMatrix[0][2];

        translateToOrigin[1][0] = 0;
        translateToOrigin[1][1] = 1;
        translateToOrigin[1][2] = -currentMatrix[1][2];

        translateToOrigin[2][0] = 0;
        translateToOrigin[2][1] = 0;
        translateToOrigin[2][2] = 1;

        applyTransformation(translateToOrigin);

        // Reflection matrix based on user input
        if (axis == 1) {  // Reflect over x-axis
            transformation[0][0] = 1;
            transformation[0][1] = 0;
            transformation[0][2] = 0;
            
            transformation[1][0] = 0;
            transformation[1][1] = -1;
            transformation[1][2] = 0;
        }
        else if (axis == 2) {  // Reflect over y-axis
            transformation[0][0] = -1;
            transformation[0][1] = 0;
            transformation[0][2] = 0;
            
            transformation[1][0] = 0;
            transformation[1][1] = 1;
            transformation[1][2] = 0;
        }
        else if (axis == 3) {  // Reflect over origin
            transformation[0][0] = -1;
            transformation[0][1] = 0;
            transformation[0][2] = 0;
            
            transformation[1][0] = 0;
            transformation[1][1] = -1;
            transformation[1][2] = 0;
        }
        transformation[2][0] = 0;
        transformation[2][1] = 0;
        transformation[2][2] = 1;

        applyTransformation(transformation);

        // Translate back to original position
        translateBack[0][0] = 1;
        translateBack[0][1] = 0;
        translateBack[0][2] = currentMatrix[0][2];

        translateBack[1][0] = 0;
        translateBack[1][1] = 1;
        translateBack[1][2] = currentMatrix[1][2];

        translateBack[2][0] = 0;
        translateBack[2][1] = 0;
        translateBack[2][2] = 1;

        applyTransformation(translateBack);
        break;

    case 5:  // Shearing
        cout << "Enter shearing factors (shx, shy): ";
        cin >> shx >> shy;
        transformation[0][0] = 1;
        transformation[0][1] = shx;
        transformation[0][2] = 0;

        transformation[1][0] = shy;
        transformation[1][1] = 1;
        transformation[1][2] = 0;

        transformation[2][0] = 0;
        transformation[2][1] = 0;
        transformation[2][2] = 1;

        applyTransformation(transformation);
        break;

    case 6:  // Change shape
        cout << "Select shape (1: Square, 2: Triangle, 3: Pentagon): ";
        cin >> currentShape;
        break;

    case 7:  // Reset matrix
        resetMatrix();
        break;

    default:
        break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(750, 750);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("2D Transformation-Anchit Das");
    glClearColor(0.9f, 0.9f, 0.9f, 0.9f);
    glColor3f(0.0f, 0.0f, 0.0f);
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    initIdentityMatrix();

    glutCreateMenu(contextMenu);
    glutAddMenuEntry("Translate", 1);
    glutAddMenuEntry("Scale", 2);
    glutAddMenuEntry("Rotate", 3);
    glutAddMenuEntry("Reflect", 4);
    glutAddMenuEntry("Shear", 5);
    glutAddMenuEntry("Change Shape", 6);
    glutAddMenuEntry("Reset Matrix", 7);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
