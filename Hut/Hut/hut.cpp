#include <GLFW/glfw3.h>
#include <cmath>


// Function to draw the hut
void drawHut() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // sky
    glColor3f(0.53f, 0.81f, 0.98f); // Light blue color
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.5f);
    glVertex2f(1.0f, -0.5f);
    glVertex2f(1.0f, 1.0f); 
    glVertex2f(-1.0f, 1.0f);
    glEnd();

    // sun
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.7f, 0.7f);
    int num_segments = 100;
    float r = 0.2f;
    for (int i = 0; i <= num_segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(x + 0.7f, y + 0.7f);
    }
    glEnd();

    // background grass
    glColor3f(0.0f, 0.8f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f); 
    glVertex2f(1.0f, -0.5f);
    glVertex2f(-1.0f, -0.5f);
    glEnd();

    /*
    // Draw the first mountain (a triangle)
    glColor3f(0.6f, 0.3f, 0.1f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.8f, -0.5f);
    glVertex2f(-0.2f, -0.5f);
    glVertex2f(-0.5f, 0.3f);
    glEnd();

    // Draw the second mountain (a triangle)
    glColor3f(0.6f, 0.3f, 0.1f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.2f, -0.5f);
    glVertex2f(0.8f, -0.5f);
    glVertex2f(0.5f, 0.3f);
    glEnd();
    */

    // walls
    glColor3f(0.6f, 0.4f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.5f, 0.0f);
    glVertex2f(-0.5f, 0.0f);
    glEnd();

    // roof
    glColor3f(0.8f, 0.2f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.6f, 0.0f);
    glVertex2f(0.6f, 0.0f);
    glVertex2f(0.0f, 0.5f);
    glEnd();

    // door
    glColor3f(0.4f, 0.2f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.15f, -0.5f);
    glVertex2f(0.15f, -0.5f);
    glVertex2f(0.15f, -0.2f);
    glVertex2f(-0.15f, -0.2f);
    glEnd();

    // windows
    glColor3f(0.0f, 0.5f, 1.0f);
    glBegin(GL_QUADS);
    // Left window
    glVertex2f(-0.4f, -0.3f);
    glVertex2f(-0.2f, -0.3f);
    glVertex2f(-0.2f, -0.1f);
    glVertex2f(-0.4f, -0.1f);

    // Right window
    glVertex2f(0.2f, -0.3f);
    glVertex2f(0.4f, -0.3f);
    glVertex2f(0.4f, -0.1f);
    glVertex2f(0.2f, -0.1f);
    glEnd();

    // Swap the front and back buffers
    glfwSwapBuffers(glfwGetCurrentContext());
}

int main() {
    // Initialize the GLFW library
    if (!glfwInit()) {
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(500, 500, "Simple Hut", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Set up the viewport and projection
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background to white
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // Set the coordinate system

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Render the hut
        drawHut();

        // Poll for and process events
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();
    return 0;
}
