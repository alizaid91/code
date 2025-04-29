#include <GL/glut.h>
#include <cmath>
#include <iostream>

int xStart, yStart, xEnd, yEnd; // Renamed variables

int lineType = 0; // 0: Simple, 1: Dotted, 2: Dashed, 3: Solid

// Set pixel color
void setPixel(int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

// Bresenham Line Drawing Algorithm
void bresenhamLine(int xStart, int yStart, int xEnd, int yEnd)
{
    int dx = abs(xEnd - xStart);
    int dy = abs(yEnd - yStart);
    int p = 2 * dy - dx;
    int twoDy = 2 * dy;
    int twoDyMinusDx = 2 * (dy - dx);

    int x = xStart, y = yStart;
    int xInc = (xEnd > xStart) ? 1 : -1;
    int yInc = (yEnd > yStart) ? 1 : -1;

    // Apply line type logic
    if (lineType == 0 || (lineType == 1 && x % 5 == 0) || (lineType == 2 && x % 10 < 5))
    {
        setPixel(x, y);
    }

    // Drawing the line
    if (dx > dy)
    { // The line is more horizontal than vertical
        while (x != xEnd)
        {
            x += xInc;
            if (p < 0)
            {
                p += twoDy;
            }
            else
            {
                y += yInc;
                p += twoDyMinusDx;
            }
            // Apply line type logic
            if (lineType == 0 || (lineType == 1 && x % 5 == 0) || (lineType == 2 && x % 10 < 5))
            {
                setPixel(x, y);
            }
        }
    }
    else
    { // The line is more vertical than horizontal
        while (y != yEnd)
        {
            y += yInc;
            if (p <= 0)
            {
                p += twoDy;
            }
            else
            {
                x += xInc;
                p += twoDyMinusDx;
            }
            // Apply line type logic
            if (lineType == 0 || (lineType == 1 && y % 5 == 0) || (lineType == 2 && y % 10 < 5))
            {
                setPixel(x, y);
            }
        }
    }
}

// Function to handle mouse clicks and draw lines
void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (xStart == 0 && yStart == 0)
        {
            // Set the first point
            xStart = x - 300; // Translate to OpenGL's coordinate system
            yStart = 300 - y; // Flip the y-coordinate
        }
        else
        {
            // Set the second point
            xEnd = x - 300;
            yEnd = 300 - y;

            // Draw the line using Bresenham's algorithm
            bresenhamLine(xStart, yStart, xEnd, yEnd);
            xStart = yStart = 0; // Reset first point
        }
    }
}

// Keyboard function for line type selection
void keyboard(unsigned char key, int x, int y)
{
    if (key == 's')
    {
        lineType = 3; // Solid line
    }
    else if (key == 'd')
    {
        lineType = 1; // Dotted line
    }
    else if (key == 'a')
    {
        lineType = 2; // Dashed line
    }
    else if (key == 'f')
    {
        lineType = 0; // Simple line
    }
}

// Display function for OpenGL rendering
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

// Initialize OpenGL settings
void init()
{
    glClearColor(1, 1, 1, 1); // White background
    glColor3f(0, 0, 0);       // Black lines
    glPointSize(2.0);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-300, 300, -300, 300); // Coordinates from -300 to +300
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Bresenham Line Drawing Algorithm");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
