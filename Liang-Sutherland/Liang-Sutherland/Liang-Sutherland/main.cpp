#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <string>
using namespace std;

//variables for DDA and Bresenham
int xmin = 100, ymin = 200, xmax = 500, ymax = 500; //for clipping window

bool clipLine = false;

// Cohen-Sutherland region codes
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

const int TOP_LEFT = 9;  //1001
const int TOP_RIGHT = 10; //1010
const int BOTTOM_LEFT = 5; //0101
const int BOTTOM_RIGHT = 6; //0110


int x1_coord_Cohen, y1_coord_Cohen, x2_coord_Cohen, y2_coord_Cohen;
bool isFirstClick_Cohen = true, isSecondClick_Cohen = false;


int x1_coord_Bresenham, y1_coord_Bresenham, x2_coord_Bresenham, y2_coord_Bresenham;
bool isFirstClick_Bresenham = true, isSecondClick_Bresenham = false;


void renderBitmapString(float x, float y, void* font, const char* string) {
    glRasterPos2f(x, y);
    for (const char* c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}


//-----------------------Cohen-Sutherland---------------------------

int computeCode(float x, float y) {
    int code = INSIDE; // Initial code

    if (x < xmin)      // to the left of clip window
        code |= LEFT;
    else if (x > xmax) // to the right of clip window
        code |= RIGHT;
    if (y < ymin)      // below the clip window
        code |= BOTTOM;
    else if (y > ymax) // above the clip window
        code |= TOP;

    return code;
}

/*
int computeCode(float x, float y) {
    int code = INSIDE; // Initial code is INSIDE

    if (x < xmin)      //left of clip window
        code &= LEFT;
    if (x > xmax)      //right of clip window
        code &= RIGHT;
    if (y < ymin)      //below clip window
        code &= BOTTOM;
    if (y > ymax)      //above clip window
        code &= TOP;

    //combining conditions to represent regions for corners
    if ((code & LEFT) && (code & TOP))
        code &= TOP_LEFT;
    else if ((code & RIGHT) && (code & TOP))
        code &= TOP_RIGHT;
    else if ((code & LEFT) && (code & BOTTOM))
        code &= BOTTOM_LEFT;
    else if ((code & RIGHT) && (code & BOTTOM))
        code &= BOTTOM_RIGHT;

    return code;
}
*/



void cohenSutherlandClip(int x1, int y1, int x2, int y2) {
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);

    bool accept = false;

    while (true) {
        if ((code1 == 0) && (code2 == 0)) {
            // Trivial acceptance
            accept = true;
            break;
        }
        else if (code1 & code2) {
            // Trivial rejection
            break;
        }
        else { //partial accept/reject
            int codeOut;
            int x, y;

            //endpoint that is outside the clip window
            if (code1 != 0)
                codeOut = code1;
            else
                codeOut = code2;

            //intersection point
            if (codeOut & TOP) {           //above the clip window
                x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
                y = ymax;
            }
            else if (codeOut & BOTTOM) {  //below the clip window
                x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
                y = ymin;
            }
            else if (codeOut & RIGHT) {   //right of the clip window
                y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
                x = xmax;
            }
            else if (codeOut & LEFT) {    //left of the clip window
                y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
                x = xmin;
            }

            //replace with the intersection point
            if (codeOut == code1) {
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1);
            }
            else {
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2);
            }
        }
    }

    if (accept) {
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex2i(x1, y1);
        glVertex2i(x2, y2);
        glEnd();

        string clipped_point1_text = "(" + to_string(x1) + ", " + to_string(y1) + ")";
        string clipped_point2_text = "(" + to_string(x2) + ", " + to_string(y2) + ")";

        renderBitmapString(x1 + 10, y1 + 10, GLUT_BITMAP_HELVETICA_10, clipped_point1_text.c_str());
        renderBitmapString(x2 + 10, y2 + 10, GLUT_BITMAP_HELVETICA_10, clipped_point2_text.c_str());
    }
}


//------------------Liang-Barsky---------------------------

bool clipTest(float p, float q, float& t1, float& t2) {
    float r;
    bool result = true;

    if (p < 0.0) {
        r = q / p;
        if (r > t2) result = false;
        else if (r > t1) t1 = r;
    }
    else if (p > 0.0) {
        r = q / p;
        if (r < t1) result = false;
        else if (r < t2) t2 = r;
    }
    else if (q < 0.0) {
        result = false;
    }

    return result;
}



//Bresenham
void drawLineBresenham() {
    //int p1 = 0, p2 = 0, p3 = 0, p4 = 0;
    //int q1 = 0, q2 = 0, q3 = 0, q4 = 0;

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

void drawLineBresenhamClipped() {
    int dx = x2_coord_Bresenham - x1_coord_Bresenham;
    int dy = y2_coord_Bresenham - y1_coord_Bresenham;

    float t1 = 0.0, t2 = 1.0;

    if (clipTest(-(x2_coord_Bresenham - x1_coord_Bresenham), x1_coord_Bresenham - xmin, t1, t2)) // Left
        if (clipTest((x2_coord_Bresenham - x1_coord_Bresenham), xmax - x1_coord_Bresenham, t1, t2)) // Right
            if (clipTest(-(y2_coord_Bresenham - y1_coord_Bresenham), y1_coord_Bresenham - ymin, t1, t2)) // Bottom
                if (clipTest((y2_coord_Bresenham - y1_coord_Bresenham), ymax - y1_coord_Bresenham, t1, t2)) { // Top
                    if (t2 < 1.0) {
                        x2_coord_Bresenham = x1_coord_Bresenham + t2 * dx;
                        y2_coord_Bresenham = y1_coord_Bresenham + t2 * dy;
                    }
                    if (t1 > 0.0) {
                        x1_coord_Bresenham = x1_coord_Bresenham + t1 * dx;
                        y1_coord_Bresenham = y1_coord_Bresenham + t1 * dy;
                    }

                    drawLineBresenham(); //clipped line
                }
}

void mouseCohen(int button, int state, int mouseX, int mouseY) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (isFirstClick_Cohen) {
            x1_coord_Cohen = mouseX;
            y1_coord_Cohen = glutGet(GLUT_WINDOW_HEIGHT) - mouseY;

            isFirstClick_Cohen = false;
            isSecondClick_Cohen = false;

            int code1 = computeCode(x1_coord_Cohen, y1_coord_Cohen);
            cout << "Point 1 (" << x1_coord_Cohen << ", " << y1_coord_Cohen << ") Region Code: " << code1 << endl;
        }
        else {
            x2_coord_Cohen = mouseX;
            y2_coord_Cohen = glutGet(GLUT_WINDOW_HEIGHT) - mouseY;
            isSecondClick_Cohen = true;

            int code1 = computeCode(x1_coord_Cohen, y1_coord_Cohen);
            int code2 = computeCode(x2_coord_Cohen, y2_coord_Cohen);
            cout << "Point 2 (" << x2_coord_Cohen << ", " << y2_coord_Cohen << ") Region Code: " << code2 << endl;

            // Check for trivial acceptance, rejection, or partial acceptance
            if ((code1 == 0) && (code2 == 0)) {
                cout << "The line is trivially accepted." << endl;
                // Call the Cohen-Sutherland clipping function to draw the line
                cohenSutherlandClip(x1_coord_Cohen, y1_coord_Cohen, x2_coord_Cohen, y2_coord_Cohen);
            }
            else if (code1 & code2) {
                cout << "The line is trivially rejected." << endl;
            }
            else {
                cout << "The line is partially accepted." << endl;
                // Call the Cohen-Sutherland clipping function to draw the line
                cohenSutherlandClip(x1_coord_Cohen, y1_coord_Cohen, x2_coord_Cohen, y2_coord_Cohen);
            }

            // Reset clicks for the next line
            isFirstClick_Cohen = true;
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

void keyboard(unsigned char key, int x, int y) {
    if (key == ' ') {
        clipLine = !clipLine; //enable clipping
        cout << "Clipping is now " << (clipLine ? "enabled" : "disabled") << endl;
        glutPostRedisplay(); //redraw the window with clipping applied
    }
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

    // Clipping window
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(xmin, ymin);
    glVertex2i(xmax, ymin);
    glVertex2i(xmax, ymax);
    glVertex2i(xmin, ymax);
    glEnd();

    // Draw the line based on mouse clicks
    /*
    if (isSecondClick_Cohen) {
        if (clipLine) {
            cohenSutherlandClip(); // Clipped line
        }
        else {
            glColor3f(1.0, 0.0, 0.0); // Color of the original line
            glBegin(GL_LINES);
            glVertex2i(x1_coord_Cohen, y1_coord_Cohen);
            glVertex2i(x2_coord_Cohen, y2_coord_Cohen);
            glEnd();
        }
    }
    */

    if (isSecondClick_Cohen) {
        if (!clipLine) {
            //original line
            glColor3f(0.0, 0.0, 0.0);
            glBegin(GL_LINES);
            glVertex2i(x1_coord_Cohen, y1_coord_Cohen);
            glVertex2i(x2_coord_Cohen, y2_coord_Cohen);
            glEnd();

            string point1_text = "(" + to_string(x1_coord_Cohen) + ", " + to_string(y1_coord_Cohen) + ")";
            string point2_text = "(" + to_string(x2_coord_Cohen) + ", " + to_string(y2_coord_Cohen) + ")";
            renderBitmapString(x1_coord_Cohen + 10, y1_coord_Cohen + 10, GLUT_BITMAP_HELVETICA_10, point1_text.c_str());
            renderBitmapString(x2_coord_Cohen + 10, y2_coord_Cohen + 10, GLUT_BITMAP_HELVETICA_10, point2_text.c_str());
        }
        else {
            cohenSutherlandClip(x1_coord_Cohen, y1_coord_Cohen, x2_coord_Cohen, y2_coord_Cohen);
        }
    }
    glFlush();

    glColor3f(0.0, 0.0, 0.0);
    renderBitmapString(50, 30, GLUT_BITMAP_HELVETICA_18, "Cohen-Sutherland Clipping");
    renderBitmapString(450, 675, GLUT_BITMAP_HELVETICA_12, "Anchit Das(500107460)");
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

    //clipping window
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(xmin, ymin);
    glVertex2i(xmax, ymin);
    glVertex2i(xmax, ymax);
    glVertex2i(xmin, ymax);
    glEnd();

    // Draw the line based on mouse clicks
    if (isSecondClick_Bresenham) {
        if (clipLine) {
            drawLineBresenhamClipped(); //clipped Bresenham line
        }
        else {
            drawLineBresenham();
        }
    }

    glFlush();

    /*
    glBegin(GL_POINTS);
    glColor3f(0.0, 0.0, 1.0);
    if (isSecondClick_Bresenham) {
        drawLineBresenham();
    }
    glEnd();
    */

    glColor3f(0.0, 0.0, 0.0);
    renderBitmapString(50, 30, GLUT_BITMAP_HELVETICA_18, "Bresenham / Liang-Barsky Algo");
    renderBitmapString(450, 675, GLUT_BITMAP_HELVETICA_12, "Anchit Das(500107460)");
    glutSwapBuffers();
}

//create subwindows
void createSubWindows(int mainWindow) {
    int subWindow1 = glutCreateSubWindow(mainWindow, 0, 0, 600, 700); //left half
    glClearColor(1.0, 1.0, 1.0, 1.0); //red
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 600, 0, 700);
    glutDisplayFunc(displaySubWindow1);
    glutMouseFunc(mouseCohen);
    glutKeyboardFunc(keyboard);

    int subWindow2 = glutCreateSubWindow(mainWindow, 600, 0, 600, 700); //right half
    glClearColor(0.0, 1.0, 0.0, 1.0); //green
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 600, 0, 700);
    glutDisplayFunc(displaySubWindow2);
    glutMouseFunc(mouseBresenham);
    glutKeyboardFunc(keyboard);
}

void mainDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);
    /*
    if (clipLine) {
        drawLineBresenhamClipped();
    }
    else {
        drawLineBresenham();
    }
    */

    cout << "Region (Inside): 0:       ---> 0000" << endl;
    cout << "Region (Left): 1:         ---> 0001" << endl;
    cout << "Region (Bottom): 2:       ---> 0010" << endl;
    cout << "Region (Bottom): 4:          ---> 0100" << endl;
    cout << "Region (Top): 8:       ---> 1000" << endl;
    cout << "Region (Top-Left): 9:     ---> 1001" << endl;
    cout << "Region (Top-Right): 10:   ---> 1010" << endl;
    cout << "Region (Bottom-Left): 5:  ---> 0101" << endl;
    cout << "Region (Bottom-Right): 6: ---> 0110" << endl;
    cout << "-------------------------------------------" << endl;
    cout << "" << endl;

    glutSwapBuffers();
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1200, 700);
    glutInitWindowPosition(100, 100);
    int mainWindow = glutCreateWindow("Liang-Barsky/Cohen Sutherland");

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
