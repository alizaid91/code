#include <GL/glut.h>
#include <vector>
#include <stack>

struct Point
{
    int x, y;
};

std::vector<Point> polygon;
bool fillStarted = false;
int fillChoice = 0; // 1: Flood fill, 2: Boundary fill

void setPixel(int x, int y, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

void getPixelColor(int x, int y, float *color)
{
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, color);
}

bool isSameColor(float *color1, float *color2)
{
    return (color1[0] == color2[0] && color1[1] == color2[1] && color1[2] == color2[2]);
}

void floodFill(int x, int y, float *oldColor, float r, float g, float b)
{
    float currentColor[3];
    getPixelColor(x, y, currentColor);
    if (isSameColor(currentColor, oldColor))
    {
        setPixel(x, y, r, g, b);
        floodFill(x + 1, y, oldColor, r, g, b);
        floodFill(x - 1, y, oldColor, r, g, b);
        floodFill(x, y + 1, oldColor, r, g, b);
        floodFill(x, y - 1, oldColor, r, g, b);
    }
}

void boundaryFill(int x, int y, float *boundaryColor, float r, float g, float b)
{
    float currentColor[3];
    getPixelColor(x, y, currentColor);
    if (!isSameColor(currentColor, boundaryColor) &&
        !(currentColor[0] == r && currentColor[1] == g && currentColor[2] == b))
    {
        setPixel(x, y, r, g, b);
        boundaryFill(x + 1, y, boundaryColor, r, g, b);
        boundaryFill(x - 1, y, boundaryColor, r, g, b);
        boundaryFill(x, y + 1, boundaryColor, r, g, b);
        boundaryFill(x, y - 1, boundaryColor, r, g, b);
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    for (auto &p : polygon)
        glVertex2i(p.x, p.y);
    glEnd();
    glFlush();
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !fillStarted)
    {
        int xi = x;
        int yi = 600 - y;
        polygon.push_back({xi, yi});
        glutPostRedisplay();
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        int xi = x;
        int yi = 600 - y;
        fillStarted = true;
        float oldColor[3];
        getPixelColor(xi, yi, oldColor);

        if (fillChoice == 1)
            floodFill(xi, yi, oldColor, 1.0f, 1.0f, 0.0f); // Yellow
        else if (fillChoice == 2)
            boundaryFill(xi, yi, (float[]){0.0f, 0.0f, 0.0f}, 0.0f, 1.0f, 0.0f); // Green
    }
}

void keyboard(unsigned char key, int, int)
{
    if (key == 'f')
    {
        fillChoice = 1;
        fillStarted = false;
    }
    else if (key == 'b')
    {
        fillChoice = 2;
        fillStarted = false;
    }
    else if (key == 'r')
    {
        polygon.clear();
        fillStarted = false;
        fillChoice = 0;
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
    }
}

void createMenu()
{
    glutCreateMenu([](int option)
                   {
        if (option == 1) fillChoice = 1;     // Flood Fill
        else if (option == 2) fillChoice = 2; // Boundary Fill
        else if (option == 3) {
            polygon.clear();
            fillChoice = 0;
            glClear(GL_COLOR_BUFFER_BIT);
            glFlush();
        } });
    glutAddMenuEntry("Flood Fill", 1);
    glutAddMenuEntry("Boundary Fill", 2);
    glutAddMenuEntry("Reset", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 600, 0, 600);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Polygon Fill: Flood & Boundary");
    init();
    createMenu();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
