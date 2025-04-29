#include <GL/glut.h>
#include <cmath>

struct Point
{
    float x, y;
};

Point triangle[3] = {
    {-0.2f, -0.2f},
    {0.2f, -0.2f},
    {0.0f, 0.2f}};

void drawPolygon()
{
    glBegin(GL_POLYGON);
    glColor3f(0.2, 0.5, 0.8);
    for (int i = 0; i < 3; ++i)
        glVertex2f(triangle[i].x, triangle[i].y);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawPolygon();
    glFlush();
}

void scale(float sx, float sy)
{
    for (int i = 0; i < 3; i++)
    {
        triangle[i].x *= sx;
        triangle[i].y *= sy;
    }
    glutPostRedisplay();
}

void rotate(float angleDeg, float px, float py)
{
    float angleRad = angleDeg * M_PI / 180.0;
    for (int i = 0; i < 3; i++)
    {
        float x = triangle[i].x - px;
        float y = triangle[i].y - py;
        triangle[i].x = px + x * cos(angleRad) - y * sin(angleRad);
        triangle[i].y = py + x * sin(angleRad) + y * cos(angleRad);
    }
    glutPostRedisplay();
}

void reflect(bool xAxis, bool yAxis)
{
    for (int i = 0; i < 3; i++)
    {
        if (xAxis)
            triangle[i].y *= -1;
        if (yAxis)
            triangle[i].x *= -1;
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int, int)
{
    switch (key)
    {
    case 's':
        scale(1.5f, 1.5f);
        break; // Scale up
    case 'r':
        rotate(45, 0.0f, 0.0f);
        break; // Rotate 45° about origin
    case 'a':
        rotate(45, 0.2f, 0.2f);
        break; // Rotate 45° about arbitrary point
    case 'x':
        reflect(true, false);
        break; // Reflect across X-axis
    case 'y':
        reflect(false, true);
        break; // Reflect across Y-axis
    case 'b':
        reflect(true, true);
        break; // Reflect across origin (both axes)
    case 27:
        exit(0); // ESC key to quit
    }
}

void init()
{
    glClearColor(1, 1, 1, 1);
    gluOrtho2D(-1, 1, -1, 1);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutCreateWindow("2D Transformations - CG Lab");
    glutInitWindowSize(600, 600);
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
