#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <string>
using namespace std;

//variables for DDA and Bresenham
int x1_coord_DDA, y1_coord_DDA, x2_coord_DDA, y2_coord_DDA;
bool isFirstClick_DDA = true, isSecondClick_DDA = false;

int x1_coord_Bresenham, y1_coord_Bresenham, x2_coord_Bresenham, y2_coord_Bresenham;
bool isFirstClick_Bresenham = true, isSecondClick_Bresenham = false;


void renderBitmapString(float x, float y, void* font, const char* string) {
    glRasterPos2f(x, y);
    for (const char* c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}


//DDA
void drawLineDDA() {
    int dx = x2_coord_DDA - x1_coord_DDA;
    int dy = y2_coord_DDA - y1_coord_DDA;

    float m = static_cast<float>(dy) / dx;  //slope

    float x = x1_coord_DDA;
    float y = y1_coord_DDA;

    glBegin(GL_POINTS);  //

    //slope <= 1, x increment uniform
    if (m <= 1) {
        float yIncrement = m;
        if (x1_coord_DDA > x2_coord_DDA) {  //right to left
            x = x2_coord_DDA;
            y = y2_coord_DDA;
        }
        for (int i = 0; i <= abs(dx); i++) { //no of points to be plotted so absolute value
            glVertex2i(x, y);
            x++;
            y += yIncrement;
        }
    }
    //slope > 1, y increment uniform
    else {
        float xIncrement = 1 / m;
        if (y1_coord_DDA > y2_coord_DDA) {  //top to bottom
            x = x2_coord_DDA;
            y = y2_coord_DDA;
        }
        for (int i = 0; i <= abs(dy); i++) { //no of points to be plotted so absolute value
            glVertex2i(x, y);
            y++; 
            x += xIncrement;
        }
    }
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    string point1_text = "(" + to_string(x1_coord_DDA) + ", " + to_string(y1_coord_DDA) + ")";
    string point2_text = "(" + to_string(x2_coord_DDA) + ", " + to_string(y2_coord_DDA) + ")";

    renderBitmapString(x1_coord_DDA + 10, y1_coord_DDA + 10, GLUT_BITMAP_HELVETICA_10, point1_text.c_str());
    renderBitmapString(x2_coord_DDA + 10, y2_coord_DDA + 10, GLUT_BITMAP_HELVETICA_10, point2_text.c_str());
}

//Bresenham
void drawLineBresenham() {
    int dx = x2_coord_Bresenham - x1_coord_Bresenham;
    int dy = y2_coord_Bresenham - y1_coord_Bresenham;

    int x = x1_coord_Bresenham;
    int y = y1_coord_Bresenham;

    int xIncrement, yIncrement;
    int p;

    if (dx >= 0) { //right or horizontal
        xIncrement = 1;
    }
    else { //left (dx<0)
        xIncrement = -1;
        dx = -dx; //positive difference
    }

    if (dy >= 0) { //line moving up
        yIncrement = 1;
    }
    else { //moving down (dy<0)
        yIncrement = -1;
        dy = -dy;
    }

    glBegin(GL_POINTS);
    if (dx >= dy) { //x increment is uuniform
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
    else { //dy > dx --> y increment is uniform
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

    glColor3f(0.0, 0.0, 0.0);
    string point1_text = "(" + to_string(x1_coord_Bresenham) + ", " + to_string(y1_coord_Bresenham) + ")";
    string point2_text = "(" + to_string(x2_coord_Bresenham) + ", " + to_string(y2_coord_Bresenham) + ")";

    renderBitmapString(x1_coord_Bresenham + 10, y1_coord_Bresenham + 10, GLUT_BITMAP_HELVETICA_10, point1_text.c_str());
    renderBitmapString(x2_coord_Bresenham + 10, y2_coord_Bresenham + 10, GLUT_BITMAP_HELVETICA_10, point2_text.c_str());
}

//DDA mouse func
void mouseDDA(int button, int state, int mouseX, int mouseY) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (isFirstClick_DDA) {
            x1_coord_DDA = mouseX;
            y1_coord_DDA = glutGet(GLUT_WINDOW_HEIGHT) - mouseY;

            isFirstClick_DDA = false;
            isSecondClick_DDA = false;
        }
        else {
            x2_coord_DDA = mouseX;
            y2_coord_DDA = glutGet(GLUT_WINDOW_HEIGHT) - mouseY;
            isSecondClick_DDA = true;

            float slope = static_cast<float>(y2_coord_DDA - y1_coord_DDA) / (x2_coord_DDA - x1_coord_DDA);
            cout << "First Point(DDA): (" << x1_coord_DDA << ", " << y1_coord_DDA << ")" << endl;
            cout << "Second Point(DDA): (" << x2_coord_DDA << ", " << y2_coord_DDA << ")" << endl;
            cout << "Slope(DDA): " << slope << endl;

            glutPostRedisplay();

            isFirstClick_DDA = true;
        }
    }
}


//bresenham mouse func
void mouseBresenham(int button, int state, int mouseX, int mouseY) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (isFirstClick_Bresenham) {
            x1_coord_Bresenham = mouseX;
            y1_coord_Bresenham = glutGet(GLUT_WINDOW_HEIGHT) - mouseY;

            isFirstClick_Bresenham = false;
            isSecondClick_Bresenham = false;
        }
        else {
            x2_coord_Bresenham = mouseX;
            y2_coord_Bresenham = glutGet(GLUT_WINDOW_HEIGHT) - mouseY;
            isSecondClick_Bresenham = true;

            float slope = static_cast<float>(y2_coord_Bresenham - y1_coord_Bresenham) / (x2_coord_Bresenham - x1_coord_Bresenham);
            cout << "First Point(Bresenham): (" << x1_coord_Bresenham << ", " << y1_coord_Bresenham << ")" << endl;
            cout << "Second Point(Bresenham): (" << x2_coord_Bresenham << ", " << y2_coord_Bresenham << ")" << endl;
            cout << "Slope(Bresenham): " << slope << endl;

            glutPostRedisplay();

            isFirstClick_Bresenham = true;
        }
    }
}


// Display function for the first subwindow (DDA)
void displaySubWindow1() {
    glClear(GL_COLOR_BUFFER_BIT);

    //quadrants
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 0.0);
    //x-axis
    glVertex2i(0, 350);
    glVertex2i(600, 350);
    //y-axis
    glVertex2i(300, 0);
    glVertex2i(300, 700);
    glEnd();

    if (isSecondClick_DDA) {
        drawLineDDA();
    }

    glColor3f(0.0, 0.0, 0.0);
    renderBitmapString(50, 30, GLUT_BITMAP_HELVETICA_18, "DDA Line Algo");

    glutSwapBuffers();
}

//bresenham display func
void displaySubWindow2() {
    glClear(GL_COLOR_BUFFER_BIT);

    //quadrants
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 0.0);
    //x-axis
    glVertex2i(0, 350);
    glVertex2i(600, 350);
    //y-axis
    glVertex2i(300, 0);
    glVertex2i(300, 700);
    glEnd();

    glBegin(GL_POINTS);
    glColor3f(0.0, 0.0, 1.0);
    if (isSecondClick_Bresenham) {
        drawLineBresenham();
    }
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    renderBitmapString(50, 30, GLUT_BITMAP_HELVETICA_18, "Bresenham Line Algo");
    renderBitmapString(450, 675, GLUT_BITMAP_HELVETICA_12, "Anchit Das(500107460)");
    glutSwapBuffers();
}

//create subwindows
void createSubWindows(int mainWindow) {
    int subWindow1 = glutCreateSubWindow(mainWindow, 0, 0, 600, 700); //left half
    glClearColor(1.0, 0.0, 0.0, 1.0); //red
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 600, 0, 700);
    glutDisplayFunc(displaySubWindow1);
    glutMouseFunc(mouseDDA);

    int subWindow2 = glutCreateSubWindow(mainWindow, 600, 0, 600, 700); //right half
    glClearColor(0.0, 1.0, 0.0, 1.0); //green
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 600, 0, 700);
    glutDisplayFunc(displaySubWindow2);
    glutMouseFunc(mouseBresenham);
}

void mainDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1200, 700);
    glutInitWindowPosition(100, 100);
    int mainWindow = glutCreateWindow("Bresenham_DDA");

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1200, 0, 700);
    glMatrixMode(GL_MODELVIEW);
    glutDisplayFunc(mainDisplay);

    createSubWindows(mainWindow);
    glutMainLoop();
    return 0;
}
