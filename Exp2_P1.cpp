#include <GL/glut.h>
#include <cmath>
#include <iostream>

int xStart, yStart, xEnd, yEnd;  // Renamed variables

int lineType = 0; // 0: Simple, 1: Dotted, 2: Dashed, 3: Solid

// Set pixel color
void setPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

// DDA Line Drawing Algorithm
void ddaLine(int xStart, int yStart, int xEnd, int yEnd) {
    int dx = xEnd - xStart;
    int dy = yEnd - yStart;
    int steps;
    float x = xStart, y = yStart;

    // Calculate the number of steps
    if (abs(dx) > abs(dy)) steps = abs(dx);
    else steps = abs(dy);

    float xIncrement = dx / float(steps);
    float yIncrement = dy / float(steps);

    for (int i = 0; i <= steps; i++) {
        // Apply line type logic
        if (lineType == 0 || (lineType == 1 && i % 5 == 0) || (lineType == 2 && i % 10 < 5)) {
            setPixel(round(x), round(y));
        }
        x += xIncrement;
        y += yIncrement;
    }
}

// Function to handle mouse clicks and draw lines
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (xStart == 0 && yStart == 0) {
            // Set the first point
            xStart = x - 300;  // Translate to OpenGL's coordinate system
            yStart = 300 - y;  // Flip the y-coordinate
        } else {
            // Set the second point
            xEnd = x - 300;
            yEnd = 300 - y;

            // Draw the line using DDA algorithm
            ddaLine(xStart, yStart, xEnd, yEnd);
            xStart = yStart = 0;  // Reset first point
        }
    }
}

// Keyboard function for line type selection
void keyboard(unsigned char key, int x, int y) {
    if (key == 's') {
        lineType = 3; // Solid line
    } else if (key == 'd') {
        lineType = 1; // Dotted line
    } else if (key == 'a') {
        lineType = 2; // Dashed line
    } else if (key == 'f') {
        lineType = 0; // Simple line
    }
}

// Display function for OpenGL rendering
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

// Initialize OpenGL settings
void init() {
    glClearColor(1, 1, 1, 1); // White background
    glColor3f(0, 0, 0);       // Black lines
    glPointSize(2.0);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-300, 300, -300, 300); // Coordinates from -300 to +300
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("DDA Line Drawing Algorithm");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
