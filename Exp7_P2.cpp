#include <GL/glut.h>
#include <cmath>

void koch(float x1, float y1, float x2, float y2, int depth)
{
    if (depth == 0)
    {
        glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
        return;
    }

    float dx = (x2 - x1) / 3.0;
    float dy = (y2 - y1) / 3.0;

    float xA = x1 + dx;
    float yA = y1 + dy;

    float xB = x1 + 2 * dx;
    float yB = y1 + 2 * dy;

    // Coordinates of peak of the triangle
    float mx = (x1 + x2) / 2;
    float my = (y1 + y2) / 2;

    float x = xA + (xB - xA) / 2 - sqrt(3) * (yB - yA) / 2;
    float y = yA + (yB - yA) / 2 + sqrt(3) * (xB - xA) / 2;

    koch(x1, y1, xA, yA, depth - 1);
    koch(xA, yA, x, y, depth - 1);
    koch(x, y, xB, yB, depth - 1);
    koch(xB, yB, x2, y2, depth - 1);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.2f, 0.6f, 0.1f);
    koch(-0.8f, 0.0f, 0.8f, 0.0f, 4); // Adjust depth here
    glFlush();
}

void init()
{
    glClearColor(1, 1, 1, 1);
    gluOrtho2D(-1, 1, -1, 1);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutCreateWindow("Koch Curve - CG Lab");
    glutInitWindowSize(600, 600);
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
