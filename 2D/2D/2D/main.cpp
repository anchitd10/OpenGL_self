#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

//-----------------------------

void init() {
	glClearColor(0, 0, 0, 1);
};
void display();
void resize(int w, int h);

void keyMain(unsigned char key, int x, int y);
void keySpec(int key, int x, int y);
void mouseClick(int button, int state, int x, int y);
void mouseHover(int x, int y);

void name();
void drawPolygon();
void drawAxis();

void createMenu();
void menuHandler(int option);

void applyMatrixToPoints();
void applyTransformation() {
	applyMatrixToPoints();
};
void matrixMultiply(float mat1[3][3]);

void translate(float, float);
void rotate(float, char);
void scale(float, float);
void reflect(char);
void shear(float, float);
void resetMatrix();
void resetScreen();


//-----------------------------

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	glutInitWindowPosition(500, 200);
	glutInitWindowSize(500, 500);

	glutCreateWindow("2D-Transformations-Anchit");
	glutReshapeFunc(resize);
	glutDisplayFunc(display);

	glutKeyboardFunc(keyMain);
	glutSpecialFunc(keySpec);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseHover);

	createMenu();

	init();
	glutMainLoop();
}

//------------------------------
vector<pair<float, float>> points;
bool polygonComplete = false;
float ox = NULL, oy = NULL;

float transformationMatrix[3][3] = {
	{1, 0, 0},
	{0, 1, 0},
	{0, 0, 1}
};

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	glColor4f(1, 1, 1, 1);
	glPointSize(3);

	drawAxis();
	drawPolygon();
	name();

	glutSwapBuffers();
}


void drawPolygon() {
	// Draw the points as a polygon if more than 2 points are present
	if (points.size() > 2) {
		glColor4f(1, 1, 0, 1);
		glBegin(GL_LINE_STRIP);
		for (const pair<float, float>& point : points) {
			glVertex2f(point.first, point.second);
		}
		glEnd();
	}

	// Draw the individual points (for visual reference)
	glColor4f(1, 1, 1, 1);
	glPointSize(5);
	glBegin(GL_POINTS);
	for (const pair<float, float>& point : points) {
		glVertex2f(point.first, point.second);
	}
	glEnd();
}

void createMenu() {
	glutCreateMenu(menuHandler);
	glutAddMenuEntry("Translate", 1);
	glutAddMenuEntry("Rotate", 2);
	glutAddMenuEntry("Scale", 3);
	glutAddMenuEntry("Reflect X", 4);
	glutAddMenuEntry("Reflect Y", 5);
	glutAddMenuEntry("Reflect Origin", 6);
	glutAddMenuEntry("Shear", 7);
	glutAddMenuEntry("Apply Transformations", 8);
	glutAddMenuEntry("Reset Matrix", 9);
	glutAddMenuEntry("Reset Screen", 10);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


void menuHandler(int option) {
	float x, y;
	char c;
	switch (option) {
	case 1:
		cout << "Translate selected. Enter x and y values: ";
		cin >> x >> y;
		translate(x, y);
		break;
	case 2:
		cout << "Rotate selected. Enter angle and direction (c or a): ";
		cin >> x >> c;
		rotate(x, c);
		break;
	case 3:
		cout << "Scale selected. Enter x and y values: ";
		cin >> x >> y;
		scale(x, y);
		break;
	case 4:
		reflect('x');
		break;
	case 5:
		reflect('y');
		break;
	case 6:
		reflect('o');
		break;
	case 7:
		cout << "Shear selected. Enter x and y values: ";
		cin >> x >> y;
		shear(x, y);
		break;
	case 8:
		applyMatrixToPoints();
		break;
	case 9:
		resetMatrix();
		break;
	case 10:
		resetScreen();
		break;
	}

	glutPostRedisplay();
}

//------------------------------------------------------------------------

pair<float, float> multiplyPointWithMatrix(const pair<float, float>& point, float matrix[3][3]) {
	float x = point.first;
	float y = point.second;

	float newX = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2];
	float newY = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2];

	return make_pair(newX, newY);
}

void matrixMultiply(float mat[3][3]) {
	float result[3][3] = { 0 };

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			result[i][j] = 0;
			for (int k = 0; k < 3; ++k) {
				result[i][j] += transformationMatrix[i][k] * mat[k][j];
			}
		}
	}

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			transformationMatrix[i][j] = result[i][j];
		}
	}
}


void printMatrix(float matrix[3][3]) {
	system("cls");
	std::cout << "\n\nTransformation matrix - \n\n";

	int width = 10;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			std::cout << std::setw(width) << matrix[i][j];  // Set width for each element
		}
		std::cout << std::endl;  // Move to the next line after each row
	}
}

// Function to apply the transformation matrix to all points
void applyMatrixToPoints() {
	for (size_t i = 0; i < points.size(); i++) {
		points[i] = multiplyPointWithMatrix(points[i], transformationMatrix);
	}
}
//------------------------------
void translate(float x, float y) {
	float operation[3][3] = {
		{1, 0, x},
		{0, 1, y},
		{0, 0, 1}
	};

	matrixMultiply(operation);
	printMatrix(transformationMatrix);
}

// Rotation function (angle in degrees, 'c' for clockwise, 'a' for anticlockwise)
void rotate(float angle, char direction) {
	float radianAngle = angle * (3.14159265 / 180.0);

	if (direction == 'c') {
		radianAngle = -radianAngle; // Clockwise rotation
	}

	float operation[3][3] = {
		{cos(radianAngle), -sin(radianAngle), 0},
		{sin(radianAngle),  cos(radianAngle), 0},
		{0,                0,                1}
	};

	matrixMultiply(operation);
	printMatrix(transformationMatrix);
}


void scale(float x, float y) {
	float operation[3][3] = {
		{x, 0, 0},
		{0, y, 0},
		{0, 0, 1}
	};

	matrixMultiply(operation);
	printMatrix(transformationMatrix);
}

// Reflection function ('x' for X-axis, 'y' for Y-axis, else origin)
void reflect(char axis) {
	float operation[3][3];

	if (axis == 'x') {
		operation[0][0] = 1;  operation[0][1] = 0;  operation[0][2] = 0;
		operation[1][0] = 0;  operation[1][1] = -1; operation[1][2] = 0;
		operation[2][0] = 0;  operation[2][1] = 0;  operation[2][2] = 1;
	}
	else if (axis == 'y') {
		operation[0][0] = -1; operation[0][1] = 0;  operation[0][2] = 0;
		operation[1][0] = 0;  operation[1][1] = 1;  operation[1][2] = 0;
		operation[2][0] = 0;  operation[2][1] = 0;  operation[2][2] = 1;
	}
	else {
		operation[0][0] = -1; operation[0][1] = 0;  operation[0][2] = 0;
		operation[1][0] = 0;  operation[1][1] = -1;  operation[1][2] = 0;
		operation[2][0] = 0;  operation[2][1] = 0;  operation[2][2] = 1;
	}


	matrixMultiply(operation);
	printMatrix(transformationMatrix);
}

void shear(float x, float y) {
	float operation[3][3] = {
		{1, x, 0},
		{y, 1, 0},
		{0, 0, 1}
	};

	matrixMultiply(operation);
	printMatrix(transformationMatrix);
}

void resetMatrix() {
	transformationMatrix[0][0] = 1; transformationMatrix[0][1] = 0; transformationMatrix[0][2] = 0;
	transformationMatrix[1][0] = 0; transformationMatrix[1][1] = 1; transformationMatrix[1][2] = 0;
	transformationMatrix[2][0] = 0; transformationMatrix[2][1] = 0; transformationMatrix[2][2] = 1;

	printMatrix(transformationMatrix);
}

void resetScreen() {
	points.clear();
	polygonComplete = false;

	printMatrix(transformationMatrix);
}


//------------------------------

void drawAxis() {
	glColor4f(0.3, 0.3, 0.3, 1);

	glBegin(GL_LINES);
	for (int i = -1000; i <= 1000; i += 100) {
		// Vertical lines
		glVertex2f(i, -1000);
		glVertex2f(i, 1000);

		// Horizontal lines
		glVertex2f(-1000, i);
		glVertex2f(1000, i);
	}
	glEnd();

	glColor4f(1, 0, 0, 1);

	glBegin(GL_LINES);
	glVertex2f(-1000, 0);  // X-axis
	glVertex2f(1000, 0);
	glVertex2f(0, -1000);  // Y-axis
	glVertex2f(0, 1000);
	glEnd();
}

void name() {
	glColor4f(1, 1, 1, 1);

	glRasterPos2f(150.0f, -400.0f);
	const char* name = "Anchit Das(SAP ID:50010746)";
	for (const char* c = name; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
	}
}


void resize(int w, int h) {
	glViewport((w / 2) - (h / 2), 0, (GLsizei)h, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-500, 500, -500, 500);
	glMatrixMode(GL_MODELVIEW);
}


void keyMain(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
		break;
	}
}

void keySpec(int key, int x, int y) {

}

void mouseClick(int button, int state, int x, int y) {
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON && !polygonComplete) {

		// Get the OpenGL viewport and normalize screen coordinates to OpenGL coordinates
		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);

		float normalizedX = (x - viewport[0]) / (float)viewport[2];
		float normalizedY = (y - viewport[1]) / (float)viewport[3];

		float openglX = normalizedX * 1000.0f - 500.0f;
		float openglY = -(normalizedY * 1000.0f - 500.0f);

		points.push_back(make_pair(openglX, openglY));
		if (ox == NULL) {
			ox = openglX;
			oy = openglY;
		}

		glutPostRedisplay();
	}

	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		glutPostRedisplay();
		glutAttachMenu(GLUT_RIGHT_BUTTON);
	}
}

void mouseHover(int x, int y) {
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	float normalizedX = (x - viewport[0]) / (float)viewport[2];
	float normalizedY = (y - viewport[1]) / (float)viewport[3];

	float openglX = normalizedX * 20.0f - 10.0f;
	float openglY = -(normalizedY * 20.0f - 10.0f);
}


