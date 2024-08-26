#include <GL/glut.h>
#include <math.h>

bool isDay = FALSE;
float sunPosition = 0.7f;  //start position of the sun
float moonPosition = 2.1f;  //start position of moon
bool animationStarted = FALSE;

float skyColorR = 0.53f, skyColorG = 0.81f, skyColorB = 0.98f;  // Initial sky blue color
//float dayColor[3] = { 0.53f, 0.81f, 0.98f }; //sky blue
//float nightColor[3] = { 0.1f, 0.1f, 0.3f };  //dark blue

void renderBitmapString(float x, float y, void* font, const char* string);

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    /*
    //sky
    if (isDay) {
        glColor3f(0.53f, 0.81f, 0.98f);  //sky blue
    }
    else {
        glColor3f(0.1f, 0.1f, 0.3f);  //dark blue
    }
    */

    glColor3f(skyColorR, skyColorG, skyColorB);
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

    // Sun
    glColor3f(1.0f, 1.0f, 0.0f);
    float x, y, radius = 0.1f;
    int i;
    int triangleAmount = 20;
    float twicePi = 2.0f * 3.14159f;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(sunPosition, 0.8f);  //animate sun
    for (i = 0; i <= triangleAmount; i++) {
        x = sunPosition + (radius * cos(i * twicePi / triangleAmount));
        y = 0.8f + (radius * sin(i * twicePi / triangleAmount));
        glVertex2f(x, y);
    }
    glEnd();

    // Moon
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(moonPosition, 0.8f);  // animate moon
    for (i = 0; i <= triangleAmount; i++) {
        x = moonPosition + (radius * cos(i * twicePi / triangleAmount));
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

    //hut shadow
    glBegin(GL_QUADS);
    glColor3f(0.55f, 0.27f, 0.07f);
    glVertex2f(-0.4f, -0.3f);
    glVertex2f(0.4f, -0.3f);
    glVertex2f(0.6f, -1.0f);
    glVertex2f(-0.6f, -1.0f);
    glEnd();

    //text
    glColor3f(1.0f, 1.0f, 1.0f);  //white
    renderBitmapString(-0.9f, -0.85f, GLUT_BITMAP_HELVETICA_12, "Anchit Das");
    renderBitmapString(-0.9f, -0.95f, GLUT_BITMAP_HELVETICA_12, "SAP ID: 500107460");
    glEnd();

    glutSwapBuffers();

    glFlush();
}

/*
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        isDay = !isDay;
        //isDay = !(bool)isDay;
        glutPostRedisplay();  // redraw
    }
}
*/

void update(int value) {
    if (animationStarted) {
        sunPosition -= 0.01f;  //move sun
        moonPosition -= 0.01f;  //move moon

        //day to night
        if (sunPosition < -1.0f) {
            isDay = FALSE;
            moonPosition = 0.7f;
        }

        //interpolate sky color
        float t = (0.7f - sunPosition) / (0.7f + 1.0f);
        skyColorR = 0.53f * (1.0f - t) + 0.1f * t;  //interpolate between sky blue and dark blue
        skyColorG = 0.81f * (1.0f - t) + 0.1f * t;
        skyColorB = 0.98f * (1.0f - t) + 0.3f * t;
    }

    //redisplay and recall update function
    glutPostRedisplay();
    glutTimerFunc(30, update, 0);  //call update every 30ms
}


void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        isDay = !isDay;
        sunPosition = isDay ? 0.7f : -1.5f;
        sunPosition != isDay ? 2.5f : 0.7f;

        moonPosition = isDay ? 2.5f : 0.7f;
        moonPosition != isDay ? 0.7f : -1.5f;
        animationStarted = true; // start the animation
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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  //black
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutInitWindowSize(640, 480);
    //glutInitWindowSize(1600, 1400);
    glutInitWindowPosition(50, 50);

    glutCreateWindow("HUT");
    
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);  // click for day/night
    glutTimerFunc(50, update, 0);  //start the animation loop
    glutMainLoop();
    return 0;
}
