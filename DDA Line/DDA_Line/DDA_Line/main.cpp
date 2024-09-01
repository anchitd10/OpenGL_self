#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <string>

using namespace std;

// Global variables to store the clicked points
float x1_coord, y1_coord, x2_coord, y2_coord;
bool isFirstClick = true;
bool isSecondClick = false;
float slope = 0.0;

void renderBitmapString(float x, float y, void* font, const char* string) {
    const char* c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void drawLine() {
    float dx = x2_coord - x1_coord;
    float dy = y2_coord - y1_coord;

    int steps = max(abs(dx), abs(dy));

    //float xIncrement = dx / steps;
    //float yIncrement = dy / steps;

    float x = x1_coord;
    float y = y1_coord;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++) {
        glVertex2f(x, y);
        if (slope < 1) {
            x += 1.0;
            y += slope;
        }
        else if (slope > 1) {
            x -= 1 / slope;
            y -= 1;
        }
    }
    glEnd();

    string coords1 = "(" + to_string((int)x1_coord) + "," + to_string((int)y1_coord) + ")";
    string coords2 = "(" + to_string((int)x2_coord) + "," + to_string((int)y2_coord) + ")";
    renderBitmapString(x1_coord + 5, y1_coord + 5, GLUT_BITMAP_HELVETICA_12, coords1.c_str());
    renderBitmapString(x2_coord + 5, y2_coord + 5, GLUT_BITMAP_HELVETICA_12, coords2.c_str());
}

void displayName() {
    renderBitmapString(300, 20, GLUT_BITMAP_HELVETICA_18, "Anchit Das");
    renderBitmapString(300, 5, GLUT_BITMAP_HELVETICA_18, "SAP ID: 500107460");
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (isSecondClick) {
        drawLine();
    }
    displayName();
    glFlush();
}

void mouse(int button, int state, int mouseX, int mouseY) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (isFirstClick) {
            x1_coord = mouseX;
            y1_coord = glutGet(GLUT_WINDOW_HEIGHT) - mouseY;
            isFirstClick = false;
            isSecondClick = false;
        }
        else {
            x2_coord = mouseX;
            y2_coord = glutGet(GLUT_WINDOW_HEIGHT) - mouseY;
            isSecondClick = true;

            slope = (y2_coord - y1_coord) / (x2_coord - x1_coord);
            cout << "First Point: (" << x1_coord << ", " << y1_coord << ")" << endl;
            cout << "Second Point: (" << x2_coord << ", " << y2_coord << ")" << endl;
            cout << "Slope: " << slope << endl;

            glutPostRedisplay();

            isFirstClick = true;
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
    glutCreateWindow("Line Draw DDA");

    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
