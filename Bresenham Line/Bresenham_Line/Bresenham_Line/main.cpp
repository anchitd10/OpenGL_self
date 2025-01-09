#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <string>

using namespace std;

int x1_coord, y1_coord, x2_coord, y2_coord;
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
    int dx = x2_coord - x1_coord; // Difference in x
    int dy = y2_coord - y1_coord; // Difference in y

    int x = x1_coord;
    int y = y1_coord;

    int xIncrement, yIncrement;
    int p;

    if (dx >= 0) { //line moving right or horizontal
        xIncrement = 1;
    }
    else { //line is moving left (dx<0)
        xIncrement = -1;
        dx = -dx; //take the positive difference for algorithm simplicity
    }

    if (dy >= 0) { //line moving up
        yIncrement = 1;
    }
    else { //line moving down (dy<0)
        yIncrement = -1;
        dy = -dy; //taking positive difference for algorithm simplicity
    }

    glBegin(GL_LINES);
    if (dx >= dy) { //more horizontal
        p = 2 * dy - dx;

        for (int k = 0; k <= dx; k++) {
            glVertex2i(x, y);

            if (p < 0) {
                x += xIncrement;
                p += 2 * dy;
            }
            else {
                x += xIncrement;
                y += yIncrement;
                p += 2 * dy - 2 * dx;
            }
        }
    }
    else {
        //dy>dx(steep or vertical)
        p = 2 * dx - dy;

        for (int k = 0; k <= dy; k++) {
            glVertex2i(x, y);

            if (p < 0) {
                y += yIncrement;
                p += 2 * dx;
            }
            else {
                x += xIncrement;
                y += yIncrement;
                p += 2 * dx - 2 * dy;
            }
        }
    }
    glEnd();

    string coords1 = "(" + to_string(x1_coord) + "," + to_string(y1_coord) + ")";
    string coords2 = "(" + to_string(x2_coord) + "," + to_string(y2_coord) + ")";
    renderBitmapString(x1_coord + 5, y1_coord + 5, GLUT_BITMAP_HELVETICA_12, coords1.c_str());
    renderBitmapString(x2_coord + 5, y2_coord + 5, GLUT_BITMAP_HELVETICA_12, coords2.c_str());
}

void displayName() {
    renderBitmapString(300, 20, GLUT_BITMAP_HELVETICA_18, "Anchit Das");
    renderBitmapString(300, 5, GLUT_BITMAP_HELVETICA_18, "SAP ID: 500107460");
}


void drawQuadrants() {
    glColor3f(0.0, 0.0, 0.0);

    glBegin(GL_LINES);
    glVertex2i(250, 0);
    glVertex2i(250, 500);
    glEnd();

    glBegin(GL_LINES);
    glVertex2i(0, 250);
    glVertex2i(500, 250);
    glEnd();
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawQuadrants();

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
    glutCreateWindow("Line Draw Bresenham");

    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}