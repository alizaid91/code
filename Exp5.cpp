#include <GL/glut.h>
#include <vector>
#include <algorithm>

#define INSIDE 0 // 0000
#define LEFT 1   // 0001
#define RIGHT 2  // 0010
#define BOTTOM 4 // 0100
#define TOP 8    // 1000

// Define clipping window boundaries
const float xmin = -0.5, xmax = 0.5;
const float ymin = -0.5, ymax = 0.5;

struct Point
{
    float x, y;
};

std::vector<Point> polygonPoints;
bool clipped = false;
std::vector<std::pair<Point, Point>> clippedLines;

// Region code function
int computeCode(float x, float y)
{
    int code = INSIDE;

    if (x < xmin)
        code |= LEFT;
    else if (x > xmax)
        code |= RIGHT;
    if (y < ymin)
        code |= BOTTOM;
    else if (y > ymax)
        code |= TOP;

    return code;
}

// Cohen-Sutherland line clipping
bool cohenSutherlandClip(Point p1, Point p2, Point &clippedP1, Point &clippedP2)
{
    float x1 = p1.x, y1 = p1.y;
    float x2 = p2.x, y2 = p2.y;

    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);
    bool accept = false;

    while (true)
    {
        if ((code1 | code2) == 0)
        {
            accept = true;
            break;
        }
        else if (code1 & code2)
        {
            break;
        }
        else
        {
            int codeOut = code1 ? code1 : code2;
            float x, y;

            if (codeOut & TOP)
            {
                x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
                y = ymax;
            }
            else if (codeOut & BOTTOM)
            {
                x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
                y = ymin;
            }
            else if (codeOut & RIGHT)
            {
                y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
                x = xmax;
            }
            else
            {
                y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
                x = xmin;
            }

            if (codeOut == code1)
            {
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1);
            }
            else
            {
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2);
            }
        }
    }

    if (accept)
    {
        clippedP1 = {x1, y1};
        clippedP2 = {x2, y2};
        return true;
    }
    return false;
}

void drawClippingWindow()
{
    glColor3f(1.0, 0.0, 0.0); // Red box
    glBegin(GL_LINE_LOOP);
    glVertex2f(xmin, ymin);
    glVertex2f(xmax, ymin);
    glVertex2f(xmax, ymax);
    glVertex2f(xmin, ymax);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawClippingWindow();

    glColor3f(0.0, 0.5, 1.0);
    glBegin(GL_LINE_LOOP);
    for (const auto &p : polygonPoints)
        glVertex2f(p.x, p.y);
    glEnd();

    if (clipped)
    {
        glColor3f(0.0, 1.0, 0.0); // Clipped polygon in green
        for (const auto &line : clippedLines)
        {
            glBegin(GL_LINES);
            glVertex2f(line.first.x, line.first.y);
            glVertex2f(line.second.x, line.second.y);
            glEnd();
        }
    }

    glFlush();
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        float fx = (float)(x - 300) / 300.0; // Normalize to [-1, 1]
        float fy = (float)(300 - y) / 300.0;
        polygonPoints.push_back({fx, fy});
        clipped = false;
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int, int)
{
    if (key == 'c')
    {
        clippedLines.clear();
        for (size_t i = 0; i < polygonPoints.size(); i++)
        {
            Point p1 = polygonPoints[i];
            Point p2 = polygonPoints[(i + 1) % polygonPoints.size()];
            Point cp1, cp2;
            if (cohenSutherlandClip(p1, p2, cp1, cp2))
                clippedLines.push_back({cp1, cp2});
        }
        clipped = true;
        glutPostRedisplay();
    }
    else if (key == 'r')
    {
        polygonPoints.clear();
        clippedLines.clear();
        clipped = false;
        glutPostRedisplay();
    }
}

void init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    gluOrtho2D(-1, 1, -1, 1);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Cohen Sutherland Polygon Clipping");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
