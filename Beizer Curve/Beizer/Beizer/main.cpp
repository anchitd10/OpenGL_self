#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

string formatCoords(float x, float y) {
    stringstream ss;
    ss << fixed << setprecision(2) << "(" << x << ", " << y << ")";
    return ss.str();
}

void drawPoint(float x, float y, float size) {
    glBegin(GL_POLYGON);
    for (float angle = 0; angle < 2 * M_PI; angle += 0.1f) {
        glVertex2f(x + size * cos(angle), y + size * sin(angle));
    }
    glEnd();
}

void renderText(const char* text, float x, float y, void* font) {
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(font, text[i]);
    }
}

void renderCoordinates(float x, float y) {
    string coords = formatCoords(x, y);
    // text offset
    float offsetX = 0.02f;
    float offsetY = 0.02f;
    renderText(coords.c_str(), x + offsetX, y + offsetY, GLUT_BITMAP_HELVETICA_10);
}

//                 parameters,     basis function/matrix,      control points,       store in result
void matrixMultiply(float t[1][4], float bezierMatrix[4][4], float controlPoints[4][3], float result[1][3]) {
    float temp[1][4];
    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < 4; j++) {
            temp[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                temp[i][j] += t[i][k] * bezierMatrix[k][j];
            }
        }
    }
    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < 3; j++) {
            result[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                result[i][j] += temp[i][k] * controlPoints[k][j];
            }
        }
    }
}

void drawBezierCurve(float controlPoints[4][3], int segments) {
    float bezierMatrix[4][4] = {
        {-1,  3, -3, 1},
        { 3, -6,  3, 0},
        {-3,  3,  0, 0},
        { 1,  0,  0, 0}
    };

    for (int i = 0; i <= segments; i++) {
        float t = (float)i / segments;
        float tMatrix[1][4] = { {t * t * t, t * t, t, 1} };
        float result[1][3];
        matrixMultiply(tMatrix, bezierMatrix, controlPoints, result);
        glVertex3f(result[0][0], result[0][1], result[0][2]);
    }
}

void drawLantern() {
    float size = 0.02f;

    // Lantern body - using a simple oval shape for the lantern body (Bezier curves)
    
}

void drawAppleLogo() {
    float size = 0.02f;

    // left curve
    glBegin(GL_LINE_STRIP);
    float leftCurve[4][3] = {
        {0.0f, 0.0f, 0.0f},
        {-0.2f, -0.2f, 0.0f},
        {-0.3f, -0.3f, 0.0f},
        {-0.5f, -0.2f, 0.0f}
    };
    drawBezierCurve(leftCurve, 30);
    glEnd();

    for (int i = 0; i < 4; i++) {
        drawPoint(leftCurve[i][0], leftCurve[i][1], size);
        renderCoordinates(leftCurve[i][0], leftCurve[i][1]);
    }

    // right curve
    glBegin(GL_LINE_STRIP);
    float rightCurve[4][3] = {
        {0.0f, 0.0f, 0.0f},
        {0.2f, -0.2f, 0.0f},
        {0.3f, -0.3f, 0.0f},
        {0.5f, -0.2f, 0.0f}
    };
    drawBezierCurve(rightCurve, 30);
    glEnd();

    for (int i = 0; i < 4; i++) {
        drawPoint(rightCurve[i][0], rightCurve[i][1], size);
        renderCoordinates(rightCurve[i][0], rightCurve[i][1]);
    }

    // stripe 1
    glBegin(GL_LINE_STRIP);
    float stripe1Curve[4][3] = {
        {-0.38f, -0.4f, 0.0f},
        {-0.2f, -0.3f, 0.0f},
        {0.2f, -0.3f, 0.0f},
        {0.38f, -0.4f, 0.0f}
    };
    drawBezierCurve(stripe1Curve, 30);
    glEnd();

    for (int i = 0; i < 4; i++) {
        drawPoint(stripe1Curve[i][0], stripe1Curve[i][1], size);
        renderCoordinates(stripe1Curve[i][0], stripe1Curve[i][1]);
    }


    // stripe 2
    glBegin(GL_LINE_STRIP);
    float stripe2Curve[4][3] = {
        {-0.38f, -0.4f, 0.0f},
        {-0.25f, -0.6f, 0.0f},
        {-0.14f, -0.46f, 0.0f},
        {0.0f, -0.32f, 0.0f}
    };
    drawBezierCurve(stripe2Curve, 30);
    glEnd();

    for (int i = 0; i < 4; i++) {
        drawPoint(stripe2Curve[i][0], stripe2Curve[i][1], size);
        renderCoordinates(stripe2Curve[i][0], stripe2Curve[i][1]);
    }

    // stripe 3
    glBegin(GL_LINE_STRIP);
    float stripe3Curve[4][3] = {
        {0.38f, -0.4f, 0.0f},
        {0.25f, -0.6f, 0.0f},
        {0.14f, -0.46f, 0.0f},
        {0.0f, -0.32f, 0.0f}
    };
    drawBezierCurve(stripe3Curve, 30);
    glEnd();

    for (int i = 0; i < 4; i++) {
        drawPoint(stripe3Curve[i][0], stripe3Curve[i][1], size);
        renderCoordinates(stripe3Curve[i][0], stripe3Curve[i][1]);
    }


    // lower curve
    glBegin(GL_LINE_STRIP);
    float lowerCurve[4][3] = {
        {0.5f, -0.2f, 0.0f},
        {0.25f, -0.75f, 0.0f},
        {-0.25f, -0.75f, 0.0f},
        {-0.5f, -0.2f, 0.0f}
    };
    drawBezierCurve(lowerCurve, 20);
    glEnd();

    for (int i = 0; i < 4; i++) {
        drawPoint(lowerCurve[i][0], lowerCurve[i][1], size);
        renderCoordinates(lowerCurve[i][0], lowerCurve[i][1]);
    }

    // flame right curve
    glColor3f(0.8f, 0.5f, 0.1f);
    glBegin(GL_LINE_STRIP);
    float flameRightCurve[4][3] = {
        {0.0f, 0.0f, 0.0f},
        {0.15f, 0.3f, 0.0f},
        {0.25f, 0.2f, 0.0f},
        {0.0f, 0.5f, 0.0f}
    };
    drawBezierCurve(flameRightCurve, 30);
    glEnd();

    for (int i = 0; i < 4; i++) {
        drawPoint(flameRightCurve[i][0], flameRightCurve[i][1], size);
        renderCoordinates(flameRightCurve[i][0], flameRightCurve[i][1]);
    }

    // left flame
    glColor3f(0.8f, 0.5f, 0.1f);
    glBegin(GL_LINE_STRIP);
    float bottomBump[4][3] = {
        {0.0f, 0.0f, 0.0f},
        {-0.05f, 0.3f, 0.0f},
        {-0.15f, 0.2f, 0.0f},
        {0.0f, 0.5f, 0.0f}
    };
    drawBezierCurve(bottomBump, 20);
    glEnd();

    for (int i = 0; i < 4; i++) {
        drawPoint(bottomBump[i][0], bottomBump[i][1], size);
        renderCoordinates(bottomBump[i][0], bottomBump[i][1]);
    }

    //lantern1 thread
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    float lantern1[4][3] = {
        {0.7f, 1.3f, 0.0f},
        {0.7f, 0.97f, 0.0f},
        {0.7f, 0.92f, 0.0f},
        {0.7f, 0.9f, 0.0f}
    };
    drawBezierCurve(lantern1, 30);
    glEnd();

    for (int i = 0; i < 4; i++) {
        drawPoint(lantern1[i][0], lantern1[i][1], size);
        renderCoordinates(lantern1[i][0], lantern1[i][1]);
    }

    // lantern1 right curve
    glColor3f(0.8f, 0.5f, 0.1f);
    glBegin(GL_LINE_STRIP);
    float lantern1Top[4][3] = {
        {0.7f, 0.9f, 0.0f},
        {0.9f, 0.78f, 0.0f},
        {0.9f, 0.65f, 0.0f},
        {0.7f, 0.5f, 0.0f}
    };
    drawBezierCurve(lantern1Top, 30);
    glEnd();

    for (int i = 0; i < 4; i++) {
        drawPoint(lantern1Top[i][0], lantern1Top[i][1], size);
        renderCoordinates(lantern1Top[i][0], lantern1Top[i][1]);
    }

    // lantern1 pattern
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    float lantern1Pat[4][3] = {
        {0.7f, 0.9f, 0.0f},
        {0.9f, 0.78f, 0.0f},
        {0.5f, 0.65f, 0.0f},
        {0.7f, 0.5f, 0.0f}
    };
    drawBezierCurve(lantern1Pat, 30);
    glEnd();

    for (int i = 0; i < 4; i++) {
        drawPoint(lantern1Pat[i][0], lantern1Pat[i][1], size);
        renderCoordinates(lantern1Pat[i][0], lantern1Pat[i][1]);
    }

    // lantern1 left curve
    glColor3f(0.8f, 0.5f, 0.1f);
    glBegin(GL_LINE_STRIP);
    float lantern1Bot[4][3] = {
        {0.7f, 0.9f, 0.0f},
        {0.5f, 0.78f, 0.0f},
        {0.5f, 0.65f, 0.0f},
        {0.7f, 0.5f, 0.0f}
    };
    drawBezierCurve(lantern1Bot, 30);
    glEnd();

    for (int i = 0; i < 4; i++) {
        drawPoint(lantern1Bot[i][0], lantern1Bot[i][1], size);
        renderCoordinates(lantern1Bot[i][0], lantern1Bot[i][1]);
    }

    //lantern2 thread
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    float lantern2[4][3] = {
        {-0.7f, 1.3f, 0.0f},
        {-0.7f, 0.97f, 0.0f},
        {-0.7f, 0.92f, 0.0f},
        {-0.7f, 0.9f, 0.0f}
    };
    drawBezierCurve(lantern2, 30);
    glEnd();

    for (int i = 0; i < 4; i++) {
        drawPoint(lantern2[i][0], lantern2[i][1], size);
        renderCoordinates(lantern2[i][0], lantern2[i][1]);
    }

    // lantern2 right curve
    glColor3f(0.8f, 0.5f, 0.1f);
    glBegin(GL_LINE_STRIP);
    float lantern2Top[4][3] = {
        {-0.7f, 0.9f, 0.0f},
        {-0.5f, 0.78f, 0.0f},
        {-0.5f, 0.65f, 0.0f},
        {-0.7f, 0.5f, 0.0f}
    };
    drawBezierCurve(lantern2Top, 30);
    glEnd();

    for (int i = 0; i < 4; i++) {
        drawPoint(lantern2Top[i][0], lantern2Top[i][1], size);
        renderCoordinates(lantern2Top[i][0], lantern2Top[i][1]);
    }

    // lantern2 pattern
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    float lantern2Pat[4][3] = {
        {-0.7f, 0.9f, 0.0f},
        {-0.9f, 0.78f, 0.0f},
        {-0.5f, 0.65f, 0.0f},
        {-0.7f, 0.5f, 0.0f}
    };
    drawBezierCurve(lantern2Pat, 30);
    glEnd();

    for (int i = 0; i < 4; i++) {
        drawPoint(lantern2Pat[i][0], lantern2Pat[i][1], size);
        renderCoordinates(lantern2Pat[i][0], lantern2Pat[i][1]);
    }


    // lantern2 left curve
    glColor3f(0.8f, 0.5f, 0.1f);
    glBegin(GL_LINE_STRIP);
    float lantern2Bot[4][3] = {
        {-0.7f, 0.9f, 0.0f},
        {-0.9f, 0.78f, 0.0f},
        {-0.9f, 0.65f, 0.0f},
        {-0.7f, 0.5f, 0.0f}
    };
    drawBezierCurve(lantern2Bot, 30);
    glEnd();

    for (int i = 0; i < 4; i++) {
        drawPoint(lantern2Bot[i][0], lantern2Bot[i][1], size);
        renderCoordinates(lantern2Bot[i][0], lantern2Bot[i][1]);
    }

    /*
    // leaf - right side
    glBegin(GL_LINE_STRIP);
    float leafRight[4][3] = {
        {-0.0f, 0.6f, 0.0f},
        {0.2f, 0.65f, 0.0f},
        {0.25f, 0.75f, 0.0f},
        {0.2f, 0.85f, 0.0f}
    };
    drawBezierCurve(leafRight, 20);
    glEnd();

    for (int i = 0; i < 4; i++) {
        drawPoint(leafRight[i][0], leafRight[i][1], size);
        renderCoordinates(leafRight[i][0], leafRight[i][1]);
    }

    // leaf - left side
    glBegin(GL_LINE_STRIP);
    float leafLeft[4][3] = {
        {0.2f, 0.85f, 0.0f},
        {0.05f, 0.8f, 0.0f},
        {0.0f, 0.75f, 0.0f},
        {0.0f, 0.6f, 0.0f}
    };
    drawBezierCurve(leafLeft, 20);
    glEnd();


    for (int i = 0; i < 4; i++) {
        drawPoint(leafLeft[i][0], leafLeft[i][1], size);
        renderCoordinates(leafLeft[i][0], leafLeft[i][1]);
    }
    */
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
    glScalef(0.8f, 0.8f, 1.0f);
    drawAppleLogo();
    glPopMatrix();

    // Draw the lantern in the top-right corner
    glPushMatrix();
    glTranslatef(0.6f, 0.8f, 0.0f); // Positioning the lantern in the top-right corner
    glScalef(0.2f, 0.2f, 1.0f); // Scale down to fit in the window
    drawLantern();
    glPopMatrix();

    // render text
    renderText("Anchit-500107460", -0.1f, 0.9f, GLUT_BITMAP_HELVETICA_12);
    glutSwapBuffers();
}

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Apple Logo-Anchit Das(500107460)");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}