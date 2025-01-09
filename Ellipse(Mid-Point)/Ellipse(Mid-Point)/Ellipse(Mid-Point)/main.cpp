#include <GL/glut.h>
#include <iostream>
#include<string>
#include <cmath>

using namespace std;

int a = 0, b = 0; //x-length, y-length or rx and ry
int centerX = 250, centerY = 250;
bool isClickedA = false, isClickedB = false;
int centerX_ellipse = 250, centerY_ellipse = 250;
int clickedQuadrantX = 1, clickedQuadrantY = 1;
int clickedX = 0, clickedY = 0;

/*
void renderBitmapString(float x, float y, void* font, const char* string) {
    const char* c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}
*/

void renderBitmapString(float x, float y, void* font, const string& str) {
    glRasterPos2f(x, y);
    for (char c : str) {
        glutBitmapCharacter(font, c);
    }
}

void drawEllipse(int a, int b) {
    int x = 0;
    int y = b;

    // Store squares of a and b
    int a2 = a * a;
    int b2 = b * b;

    // Store 2 * squares of a and b
    int twoA2 = 2 * a2;
    int twoB2 = 2 * b2;

    //initial decision parameter region 1
    int p = b2 - (a2 * b) + (0.25 * a2);

    // Region 1
    while (a2 * (y - 0.5) > b2 * (x + 1)) {
        glVertex2i(centerX + x, centerY + y); // 1st
        glVertex2i(centerX - x, centerY + y); // 2nd
        glVertex2i(centerX - x, centerY - y); // 3rd
        glVertex2i(centerX + x, centerY - y); // 4th

        x++;

        if (p < 0) {
            //x-increment
            p += twoB2 * x + b2;
        }
        else {
            //x-y increment
            y--;
            p += twoB2 * x - twoA2 * y + b2;
        }
    }

    //initial decision parameter region 2
    p = b2 * (x + 0.5) * (x + 0.5) + a2 * (y - 1) * (y - 1) - a2 * b2;

    // Region 2
    while (y >= 0) {
        // For all quadrants
        glVertex2i(centerX + x, centerY + y); // 1st
        glVertex2i(centerX - x, centerY + y); // 2nd
        glVertex2i(centerX - x, centerY - y); // 3rd
        glVertex2i(centerX + x, centerY - y); // 4th

        y--;

        if (p > 0) {
            //y-decrement
            p -= twoA2 * y + a2;
        }
        else {
            //x-y increment
            x++;
            p += twoB2 * x - twoA2 * y + a2;
        }
    }
}

void displayName() {
    renderBitmapString(300, 20, GLUT_BITMAP_HELVETICA_18, "Anchit Das");
    renderBitmapString(300, 5, GLUT_BITMAP_HELVETICA_18, "SAP ID: 500107460");
    renderBitmapString(10, 10, GLUT_BITMAP_HELVETICA_12, "Ellipse(Mid-Point)");
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);

    //quadrants
    glColor3f(0.0, 0.0, 0.0);
    for (int i = 0; i < 500; i++) {
        glVertex2i(i, 250); // x-axis
        glVertex2i(250, i); // y-axis
    }

    if (isClickedA && isClickedB) {
        drawEllipse(a, b);
    }

    glEnd();

    displayName();

    if (isClickedA) {
        glColor3f(1.0, 0.0, 0.0);
        string coordA = "(" + to_string(clickedX) + ", " + to_string(centerY) + ")";
        renderBitmapString(clickedX, centerY, GLUT_BITMAP_HELVETICA_12, coordA);
    }

    if (isClickedB) {
        glColor3f(0.0, 0.0, 1.0);
        string coordB = "(" + to_string(centerX) + ", " + to_string(clickedY) + ")";
        renderBitmapString(centerX, clickedY, GLUT_BITMAP_HELVETICA_12, coordB);
    }

    glFlush();
}


void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int clickY = 500 - y;

        if (!isClickedA) {
            a = abs(x - centerX);
            clickedX = x;
            isClickedA = true;
            cout << "X-length(a): " << a << endl;
        }
        else if (!isClickedB) {
            b = abs(clickY - centerY);
            clickedY = clickY;
            isClickedB = true;
            cout << "Y-length(b): " << b << endl;
            glutPostRedisplay();
        }
        else {
            isClickedA = false;
            isClickedB = false;
            a = 0;
            b = 0;
        }
    }
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Ellipse Midpoint Algorithm");

    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}