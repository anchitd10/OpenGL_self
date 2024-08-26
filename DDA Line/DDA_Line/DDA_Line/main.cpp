/*
#include <GL/glut.h>
#include <iostream>
using namespace std;

int clickCount = 0;
int x1_coord, x2_coord, y1_coord, y2_coord;

void calculateSlope() {

    //slope = (y2-y1)/(x2-x1)

    if (x2_coord - x1_coord != 0) {
        float slope = static_cast<float>(y2_coord - y1_coord) / (x2_coord - x1_coord);
        cout << "Slope: " << slope << endl;
    }
}

//record mouse clicks
void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (clickCount == 0) {
            x1_coord = x;
            y1_coord = y;
            cout << "First Point: (" << x1_coord << ", " << y1_coord << ")" << endl;
            clickCount++;
        }
        else if (clickCount == 1) {
            x2_coord = x;
            y2_coord = y;
            cout << "Second Point: (" << x2_coord << ", " << y2_coord << ")" << endl;
            calculateSlope();
            clickCount = 0;
        }
        else {
            cout << "Slope is undefined (vertical line)." << endl;
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("DDA_Line(2 Points)");

    glutDisplayFunc(display);
    glutMouseFunc(mouseClick);

    glutMainLoop();
    return 0;
}
*/

#include <GL/glut.h>
#include <iostream>
#include <cmath>

// Global variables to store the clicked points
float x1_coord, y1_coord, x2_coord, y2_coord;
bool firstClick = true;
bool secondClick = false;

void drawLine() {
    // Calculate the difference between the points
    float dx = x2_coord - x1_coord;
    float dy = y2_coord - y1_coord;

    // Determine the number of steps needed
    int steps = std::max(abs(dx), abs(dy));

    // Calculate the increment for each step
    float xIncrement = dx / steps;
    float yIncrement = dy / steps;

    // Start with the first point
    float x = x1_coord;
    float y = y1_coord;

    // Draw each point of the line
    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++) {
        glVertex2f(x, y);
        x += xIncrement;
        y += yIncrement;
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (secondClick) {
        drawLine();
    }
    glFlush();
}

void mouse(int button, int state, int mouseX, int mouseY) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (firstClick) {
            x1_coord = mouseX;
            y1_coord = glutGet(GLUT_WINDOW_HEIGHT) - mouseY; // Invert Y coordinate
            firstClick = false;
            secondClick = true;
        }
        else {
            x2_coord = mouseX;
            y2_coord = glutGet(GLUT_WINDOW_HEIGHT) - mouseY; // Invert Y coordinate
            secondClick = true;
            glutPostRedisplay(); // Redraw the window with the new line
        }
    }
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // White background
    glColor3f(0.0, 0.0, 0.0); // Black points
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Line Drawing with Mouse Clicks");

    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
