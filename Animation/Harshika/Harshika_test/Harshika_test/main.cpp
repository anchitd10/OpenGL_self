#include <GL/glut.h>
#include <cmath>

// Function to draw the hut
void drawHut() {
    // Draw the house walls (yellow color)
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow
    glBegin(GL_QUADS);
    glVertex3f(-0.5f, -0.5f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.0f);
    glVertex3f(0.5f, 0.5f, 0.0f);
    glVertex3f(-0.5f, 0.5f, 0.0f);
    glEnd();

    // Draw the roof (light brown color)
    glColor3f(0.6f, 0.4f, 0.2f); // Light brown
    glBegin(GL_TRIANGLES);
    glVertex3f(-0.6f, 0.5f, 0.0f);
    glVertex3f(0.6f, 0.5f, 0.0f);
    glVertex3f(0.0f, 0.9f, 0.0f);
    glEnd();

    // Draw the door (red color)
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glBegin(GL_QUADS);
    glVertex3f(-0.1f, -0.5f, 0.0f);
    glVertex3f(0.1f, -0.5f, 0.0f);
    glVertex3f(0.1f, 0.0f, 0.0f);
    glVertex3f(-0.1f, 0.0f, 0.0f);
    glEnd();

    // Draw the door handle (silver color)
    glColor3f(0.75f, 0.75f, 0.75f); // Silver
    float handle_radius = 0.02f;
    float handle_center_x = 0.08f;
    float handle_center_y = -0.25f;
    int num_segments = 100;

    glBegin(GL_POLYGON);
    for (int i = 0; i < num_segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float x = handle_radius * cosf(theta);
        float y = handle_radius * sinf(theta);
        glVertex3f(x + handle_center_x, y + handle_center_y, 0.0f);
    }
    glEnd();
}


// Function to draw the ground
void drawGround() {
    glColor3f(0.3f, 0.7f, 0.3f); // Green color for ground
    glBegin(GL_QUADS);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, -0.5f, 0.0f);
    glVertex3f(-1.0f, -0.5f, 0.0f);
    glEnd();

}

// Function to draw the sky
void drawSky() {
    glColor3f(0.5f, 0.8f, 1.0f); // Light blue color for sky
    glBegin(GL_QUADS);
    glVertex3f(-1.0f, -0.5f, 0.0f);
    glVertex3f(1.0f, -0.5f, 0.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glEnd();
}

// Display callback function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Draw the sky
    drawSky();

    // Draw the ground
    drawGround();

    // Draw the hut
    drawHut();

    glFlush();
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Hut with Ground, Grass, and Sky");

    // Set the clear color (background color)
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f); // Light blue background

    // Set up the view (orthogonal projection)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

    // Register the display callback function
    glutDisplayFunc(display);

    // Enter the GLUT main loop
    glutMainLoop();

    return 0;
}