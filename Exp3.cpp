#include <GL/glut.h>
#include <cmath>

// Define center and radius of the circle
int radius = 50;
int xc = 0, yc = 0; // Center at origin (0,0) for 4-quadrant symmetry

void setPixel(int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void drawCirclePoints(int xc, int yc, int x, int y)
{
    // 8-way symmetry
    setPixel(xc + x, yc + y);
    setPixel(xc - x, yc + y);
    setPixel(xc + x, yc - y);
    setPixel(xc - x, yc - y);
    setPixel(xc + y, yc + x);
    setPixel(xc - y, yc + x);
    setPixel(xc + y, yc - x);
    setPixel(xc - y, yc - x);
}

void bresenhamCircle(int xc, int yc, int r)
{
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while (x <= y)
    {
        drawCirclePoints(xc, yc, x, y);
        if (d < 0)
        {
            d += 4 * x + 6;
        }
        else
        {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 0, 0);

    // Circle in all four quadrants centered at origin
    bresenhamCircle(0, 0, radius);

    glFlush();
}

void init()
{
    glClearColor(1, 1, 1, 1); // White background
    glColor3f(1, 0, 0);       // Red circle
    glPointSize(2.0);

    // 2D projection for 4 quadrants
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-300, 300, -300, 300);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Bresenham Circle Drawing in All Quadrants");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
