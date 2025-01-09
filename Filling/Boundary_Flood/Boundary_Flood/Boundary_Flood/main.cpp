#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <string>
using namespace std;


void renderBitmapString(float x, float y, void* font, const string& str) {
    glRasterPos2f(x, y);
    for (char c : str) {
        glutBitmapCharacter(font, c);
    }
}

//----------ellipse-----------------
int a = 0, b = 0; // x-length, y-length or rx and ry
int centerX_ellipse = 300, centerY_ellipse = 350; // Center of ellipse
bool isClickedA = false, isClickedB = false;
int clickedQuadrantX = 1, clickedQuadrantY = 1;

//-----------circle----------------
int centerX_circle = 900, centerY_circle = 350; // Center of circle
int secondClickX = 0, secondClickY = 0;
int clickCount = 0;
int radius_circle = 0;
bool isCircleClicked = false;

//------------filling algo-------------
int fillX = -1, fillY = -1;
float currentFillColor[3] = { 1.0f, 0.0f, 0.0f }; //red
float boundaryColor[3] = { 0.0f, 0.0f, 0.0f };

void getpixel(int x, int y, float* color) {
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, color);
}

void putpixel(int x, int y, const float* color) {
    glColor3fv(color);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

void drawEllipse(int a, int b) {
    int x = 0;
    int y = b;

    //store squares of a and b
    int a2 = a * a;
    int b2 = b * b;

    //store 2 times squares of a and b
    int twoA2 = 2 * a2;
    int twoB2 = 2 * b2;

    //initial decision parameter for region 1
    int p = b2 - (a2 * b) + (0.25 * a2);

    //region 1
    while (a2 * (y - 0.5) > b2 * (x + 1)) {
        //all quadrants
        glVertex2i(centerX_ellipse + x, centerY_ellipse + y); // 1st
        glVertex2i(centerX_ellipse - x, centerY_ellipse + y); // 2nd
        glVertex2i(centerX_ellipse - x, centerY_ellipse - y); // 3rd
        glVertex2i(centerX_ellipse + x, centerY_ellipse - y); // 4th

        x++;

        if (p < 0) {
            // x-increment
            p += twoB2 * x + b2;
        }
        else {
            // x-y increment
            y--;
            p += twoB2 * x - twoA2 * y + b2;
        }
    }

    //initial decision parameter(region 2)
    p = b2 * (x + 0.5) * (x + 0.5) + a2 * (y - 1) * (y - 1) - a2 * b2;

    // Region 2
    while (y >= 0) {
        //all quadrants
        glVertex2i(centerX_ellipse + x, centerY_ellipse + y); // 1st
        glVertex2i(centerX_ellipse - x, centerY_ellipse + y); // 2nd
        glVertex2i(centerX_ellipse - x, centerY_ellipse - y); // 3rd
        glVertex2i(centerX_ellipse + x, centerY_ellipse - y); // 4th

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

void drawCircle(int radius_circle) {
    int x = 0;
    int y = radius_circle;
    int p = 1 - radius_circle; // initial decision parameter

    glBegin(GL_POINTS);

    while (x <= y) {
        if (p < 0) {
            p += 2 * x + 3;
        }
        else {
            p += 2 * (x - y) + 5;
            y--;
        }
        x++;

        //octants
        glVertex2i(300 + x, 350 + y);
        glVertex2i(300 - x, 350 + y);
        glVertex2i(300 + x, 350 - y);
        glVertex2i(300 - x, 350 - y);

        glVertex2i(300 + y, 350 + x);
        glVertex2i(300 - y, 350 + x);
        glVertex2i(300 + y, 350 - x);
        glVertex2i(300 - y, 350 - x);
    }

    glEnd();
}

/* -----------------display function for each window------------------ */
void displayMainWindow() {
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}

void displaySubWindow1() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Quadrants
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 0.0);

    // X-axis
    glVertex2i(0, 350);
    glVertex2i(600, 350);

    // Y-axis
    glVertex2i(300, 0);
    glVertex2i(300, 700);

    glEnd();

    glBegin(GL_POINTS);

    if (isClickedA && isClickedB) {
        drawEllipse(a, b);
    }

    glEnd();
    glFlush();


    if (isClickedA) {
        string coordA = "(" + to_string(a) + ", 0)";
        glColor3f(0.0, 0.0, 0.0);
        //renderBitmapString(centerX_ellipse + a, centerY_ellipse, GLUT_BITMAP_HELVETICA_12, coordA);
        renderBitmapString(centerX_ellipse + clickedQuadrantX * a, centerY_ellipse, GLUT_BITMAP_HELVETICA_12, coordA);
    }
    if (isClickedB) {
        string coordB = "(0, " + to_string(b) + ")";
        glColor3f(0.0, 0.0, 0.0);
        //renderBitmapString(centerX_ellipse, centerY_ellipse + b, GLUT_BITMAP_HELVETICA_12, coordB);
        renderBitmapString(centerX_ellipse, centerY_ellipse + clickedQuadrantY * b, GLUT_BITMAP_HELVETICA_12, coordB);
    }


    glColor3f(0.0, 0.0, 0.0);
    renderBitmapString(50, 30, GLUT_BITMAP_HELVETICA_18, "Ellipse Mid Point Algo");
    glutSwapBuffers();
}

void displaySubWindow2() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Quadrants
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 0.0);

    // x-axis
    glVertex2i(0, 350);
    glVertex2i(600, 350);

    // y-axis
    glVertex2i(300, 0);
    glVertex2i(300, 700);

    glEnd();

    if (isCircleClicked) {
        glColor3f(0.0, 0.0, 0.0);
        drawCircle(radius_circle);

        string centerCoord = "(300, 350)";
        glColor3f(0.0, 0.0, 0.0);
        renderBitmapString(310, 340, GLUT_BITMAP_HELVETICA_12, centerCoord);

        string secondClickCoord = "(" + to_string(secondClickX) + ", " + to_string(secondClickY) + ")";
        renderBitmapString(secondClickX + 10, secondClickY + 10, GLUT_BITMAP_HELVETICA_12, secondClickCoord);
    }

    glColor3f(0.0, 0.0, 0.0);
    renderBitmapString(50, 30, GLUT_BITMAP_HELVETICA_18, "Circle Mid Point Algo");

    renderBitmapString(450, 675, GLUT_BITMAP_HELVETICA_12, "Anchit Das(500107460)");

    glutSwapBuffers();
}


// ---------------------filling algo----------------------

//boundary fill
//1200x700
void boundaryFill4(int x, int y) {
    if (x < 0 || x > 1200 || y < 0 || y > 700) {
        return;
    }

    float pixelColor[3];
    getpixel(x, y, pixelColor);

    //check if the current pixel is not the boundary color and not already filled
    if ((pixelColor[0] != boundaryColor[0] || pixelColor[1] != boundaryColor[1] || pixelColor[2] != boundaryColor[2]) &&
        (pixelColor[0] != currentFillColor[0] || pixelColor[1] != currentFillColor[1] || pixelColor[2] != currentFillColor[2])) {

        putpixel(x, y, currentFillColor);

        boundaryFill4(x + 1, y);
        boundaryFill4(x, y + 1);
        boundaryFill4(x - 1, y);
        boundaryFill4(x, y - 1);
    }
}


// ----------------mouse calback functions------------------
void mouseSubWindow1(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (!isClickedA) {
            a = abs(x - centerX_ellipse);
            clickedQuadrantX = (x >= centerX_ellipse) ? 1 : -1;
            centerY_ellipse = 700 - y;
            isClickedA = true;
            cout << "X-length_ellipse(a): " << a << endl;
        }
        else if (!isClickedB) {
            int x2 = x;
            int y2 = 700 - y;
            b = abs(y2 - centerY_ellipse);
            clickedQuadrantY = (y2 >= centerY_ellipse) ? 1 : -1;
            isClickedB = true;
            cout << "Y-length_ellipse(b): " << b << endl;
            cout << "Ellipse dimensions: a = " << a << ", b = " << b << endl;
        }
        else {
            isClickedA = false;
            isClickedB = false;
            a = 0;
            b = 0;
        }
        glutPostRedisplay();
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        fillX = x;
        fillY = 700 - y;
        cout << "Right Clicked at: (" << fillX << ", " << fillY << ")" << endl;
    }
}

void mouseSubWindow2(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (clickCount == 0) {
            int dx = abs(x - 300);
            int dy = abs((700 - y) - 350);
            radius_circle = sqrt(dx * dx + dy * dy);
            secondClickX = x;
            secondClickY = 700 - y;

            cout << "Second Clicked Coordinate: (" << secondClickX << ", " << secondClickY << ")" << endl;
            cout << "Calculated Radius: " << radius_circle << endl;

            isCircleClicked = true;
            clickCount++;
        }
        else if (clickCount == 1) {
            //reset
            isCircleClicked = false;
            radius_circle = 0;
            clickCount++;
            glutPostRedisplay();
        }
        else if (clickCount == 2) {
            int dx = abs(x - 300);
            int dy = abs((700 - y) - 350);
            radius_circle = sqrt(dx * dx + dy * dy);
            secondClickX = x;
            secondClickY = 700 - y; // Adjust for coordinate system
            isCircleClicked = true;

            cout << "Circle Clicked Coordinate: (" << secondClickX << ", " << secondClickY << ")" << endl;
            cout << "Calculated Radius: " << radius_circle << endl;

            clickCount = 1;
            glutPostRedisplay();
        }
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        fillX = x;
        fillY = 700 - y;  // Adjust for OpenGL coordinate system
        cout << "Right Clicked at: (" << fillX << ", " << fillY << ")" << endl;
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key == ' ') { // If space is pressed
        if (fillX != -1 && fillY != -1) {
            boundaryFill4(fillX, fillY); // Call the boundary fill algorithm
        }
    }
}

// ------------------create subwindows----------------------
void createSubWindows(int mainWindow) {
    // First subwindow
    int subWindow1 = glutCreateSubWindow(mainWindow, 0, 0, 600, 700); // Left half
    //glClearColor(1.0, 0.0, 0.0, 1.0); // Red
    glClearColor(0.9, 0.9, 0.9, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 600, 0, 700);
    glutDisplayFunc(displaySubWindow1);
    glutMouseFunc(mouseSubWindow1);

    // Second subwindow
    int subWindow2 = glutCreateSubWindow(mainWindow, 600, 0, 600, 700); // Right half
    //glClearColor(0.0, 1.0, 0.0, 1.0); // Green
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 600, 0, 700);
    glutDisplayFunc(displaySubWindow2);
    glutMouseFunc(mouseSubWindow2);
}


void mainDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glutKeyboardFunc(keyboard);
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1200, 700);
    glutInitWindowPosition(100, 100);
    int mainWindow = glutCreateWindow("VIVA(13-09-2024)-Anchit Das");

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1200, 0, 700);
    glMatrixMode(GL_MODELVIEW);
    glutDisplayFunc(mainDisplay);

    createSubWindows(mainWindow);
    glutKeyboardFunc(keyboard);

    //glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
