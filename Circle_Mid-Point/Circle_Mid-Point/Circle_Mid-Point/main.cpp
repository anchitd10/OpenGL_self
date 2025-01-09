#include <GL/glut.h>
#include <cmath>
#include <string>

int centerX_circle = 250, centerY_circle = 250; // Center of circle
int radius_circle = 0;
bool isCircleClicked = false;

void renderBitmapString(float x, float y, void* font, const char* string) {
    const char* c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void drawCircle(int radius) {
    int x = 0;
    int y = radius;
    int p = 1 - radius; // Initial decision parameter

    glBegin(GL_POINTS);

    // Draw the initial set of points
    glVertex2i(centerX_circle + x, centerY_circle + y);
    glVertex2i(centerX_circle - x, centerY_circle + y);
    glVertex2i(centerX_circle + x, centerY_circle - y);
    glVertex2i(centerX_circle - x, centerY_circle - y);
    glVertex2i(centerX_circle + y, centerY_circle + x);
    glVertex2i(centerX_circle - y, centerY_circle + x);
    glVertex2i(centerX_circle + y, centerY_circle - x);
    glVertex2i(centerX_circle - y, centerY_circle - x);

    while (x <= y) {
        if (p < 0) {
            p += 2 * x + 3;
        }
        else {
            p += 2 * (x - y) + 5;
            y--;
        }
        x++;

        // Draw points in all eight octants
        glVertex2i(centerX_circle + x, centerY_circle + y);
        glVertex2i(centerX_circle - x, centerY_circle + y);
        glVertex2i(centerX_circle + x, centerY_circle - y);
        glVertex2i(centerX_circle - x, centerY_circle - y);
        glVertex2i(centerX_circle + y, centerY_circle + x);
        glVertex2i(centerX_circle - y, centerY_circle + x);
        glVertex2i(centerX_circle + y, centerY_circle - x);
        glVertex2i(centerX_circle - y, centerY_circle - x);
    }

    glEnd();
}

void mouseFunc(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (!isCircleClicked) {
            centerX_circle = x;
            centerY_circle = 500 - y; // Adjust for coordinate system
            isCircleClicked = true;
        }
        else {
            int x2 = x;
            int y2 = 500 - y; // Adjust for coordinate system
            radius_circle = sqrt((x2 - centerX_circle) * (x2 - centerX_circle) +
                (y2 - centerY_circle) * (y2 - centerY_circle));
        }
        glutPostRedisplay();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw quadrant axes
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 0.0); // Black

    // x-axis
    glVertex2i(0, 250); // Start of x-axis
    glVertex2i(500, 250); // End of x-axis

    // y-axis
    glVertex2i(250, 0); // Start of y-axis
    glVertex2i(250, 500); // End of y-axis

    glEnd();

    if (isCircleClicked) {
        glColor3f(0.0, 0.0, 1.0); // Blue for circle
        drawCircle(radius_circle);
    }

    glColor3f(0.0, 0.0, 0.0); // Black for text
    //renderBitmapString(25, 15, GLUT_BITMAP_HELVETICA_18, "Circle Mid Point Algo");
    renderBitmapString(350, 475, GLUT_BITMAP_HELVETICA_12, "Anchit Das(500107460)");

    glutSwapBuffers();
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Circle Mid-Point Algo");

    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouseFunc);
    glutMainLoop();
    return 0;
}
