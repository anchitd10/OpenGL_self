#include <GL/glut.h>
#include <math.h>

bool isDay = FALSE;  //toggle between day and night

void renderBitmapString(float x, float y, void* font, const char* string);

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    //sky
    if (isDay) {
        glColor3f(0.53f, 0.81f, 0.98f);  //sky blue
    }
    else {
        glColor3f(0.1f, 0.1f, 0.3f);  //dark blue
    }
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, 0.2f);
    glVertex2f(1.0f, 0.2f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();

    //ground
    glBegin(GL_QUADS);
    glColor3f(0.13f, 0.55f, 0.13f);  //green
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, 0.2f);
    glVertex2f(-1.0f, 0.2f);
    glEnd();

    //sun or moon
    glColor3f(1.0f, 1.0f, 0.0f);  //sun color
    if (!isDay) {
        glColor3f(0.9f, 0.9f, 0.9f);  //moon color
    }
    float x, y, radius = 0.1f;
    int i;
    int triangleAmount = 20;  //number of triangles used to draw the circle
    float twicePi = 2.0f * 3.14159f;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.7f, 0.8f);
    for (i = 0; i <= triangleAmount; i++) {
        x = 0.7f + (radius * cos(i * twicePi / triangleAmount));
        y = 0.8f + (radius * sin(i * twicePi / triangleAmount));
        glVertex2f(x, y);
    }
    glEnd();

    //mountain 1
    glBegin(GL_TRIANGLES);
    glColor3f(0.42f, 0.26f, 0.15f);  // brown
    glVertex2f(-0.9f, 0.2f);
    glVertex2f(-0.5f, 0.8f);
    glVertex2f(-0.1f, 0.2f);
    glEnd();

    // Mountain 2
    glBegin(GL_TRIANGLES);
    glColor3f(0.42f, 0.26f, 0.15f);  // brown
    glVertex2f(0.1f, 0.2f);
    glVertex2f(0.5f, 0.8f);
    glVertex2f(0.9f, 0.2f);
    glEnd();

    //hut base
    glBegin(GL_QUADS);
    glColor3f(0.87f, 0.72f, 0.53f);  // light brown
    glVertex2f(-0.4f, -0.3f);
    glVertex2f(0.4f, -0.3f);
    glVertex2f(0.4f, 0.3f);
    glVertex2f(-0.4f, 0.3f);
    glEnd();

    //hut roof
    glBegin(GL_TRIANGLES);
    glColor3f(0.55f, 0.27f, 0.07f);  // dark brown
    glVertex2f(-0.45f, 0.3f);
    glVertex2f(0.45f, 0.3f);
    glVertex2f(0.0f, 0.6f);
    glEnd();

    // Hut door
    glBegin(GL_QUADS);
    glColor3f(0.36f, 0.25f, 0.20f);  // darker brown
    glVertex2f(-0.1f, -0.3f);
    glVertex2f(0.1f, -0.3f);
    glVertex2f(0.1f, 0.0f);
    glVertex2f(-0.1f, 0.0f);
    glEnd();

    //left window
    glColor3f(0.7f, 0.8f, 1.0f);  //light blue
    radius = 0.07f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(-0.25f, 0.1f);
    for (i = 0; i <= triangleAmount; i++) {
        x = -0.25f + (radius * cos(i * twicePi / triangleAmount));
        y = 0.1f + (radius * sin(i * twicePi / triangleAmount));
        glVertex2f(x, y);
    }
    glEnd();

    //right window
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.25f, 0.1f);
    for (i = 0; i <= triangleAmount; i++) {
        x = 0.25f + (radius * cos(i * twicePi / triangleAmount));
        y = 0.1f + (radius * sin(i * twicePi / triangleAmount));
        glVertex2f(x, y);
    }
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);  //white
    renderBitmapString(0.1f, -0.85f, GLUT_BITMAP_HELVETICA_12, "Anchit Das");
    renderBitmapString(0.1f, -0.95f, GLUT_BITMAP_HELVETICA_12, "SAP ID: 500107460");
    glEnd();


    glFlush();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        isDay = !isDay;
        //isDay = !(bool)isDay;
        glutPostRedisplay();  // redraw
    }
}

void renderBitmapString(float x, float y, void* font, const char* string) {
    const char* c;
    glRasterPos2f(x, y);  //position the text
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  //balck
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutCreateWindow("HUT");
    //glutInitWindowSize(640, 480);
    glutInitWindowSize(720, 640);
    glutInitWindowPosition(50, 50);
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);  // click for day/night
    glutMainLoop();
    return 0;
}
