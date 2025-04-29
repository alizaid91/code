#include <GL/glut.h>
#include <cmath>

struct Point {
    float x, y;
};

void drawBezier(Point p0, Point p1, Point p2, Point p3, int depth) {
    if (depth == 0) {
        glBegin(GL_LINE_STRIP);
        for (float t = 0; t <= 1.0; t += 0.01) {
            float x = pow(1 - t, 3) * p0.x +
                      3 * t * pow(1 - t, 2) * p1.x +
                      3 * pow(t, 2) * (1 - t) * p2.x +
                      pow(t, 3) * p3.x;

            float y = pow(1 - t, 3) * p0.y +
                      3 * t * pow(1 - t, 2) * p1.y +
                      3 * pow(t, 2) * (1 - t) * p2.y +
                      pow(t, 3) * p3.y;

            glVertex2f(x, y);
        }
        glEnd();
        return;
    }

    // Recursively divide
    Point mid = {(p0.x + p3.x) / 2, (p0.y + p3.y) / 2};
    Point l1 = {(p0.x + p1.x) / 2, (p0.y + p1.y) / 2};
    Point l2 = {(p1.x + p2.x) / 2, (p1.y + p2.y) / 2};
    Point l3 = {(p2.x + p3.x) / 2, (p2.y + p3.y) / 2};

    Point m1 = {(l1.x + l2.x) / 2, (l1.y + l2.y) / 2};
    Point m2 = {(l2.x + l3.x) / 2, (l2.y + l3.y) / 2};
    Point center = {(m1.x + m2.x) / 2, (m1.y + m2.y) / 2};

    drawBezier(p0, l1, m1, center, depth - 1);
    drawBezier(center, m2, l3, p3, depth - 1);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.2, 0.5, 0.8);

    Point p0 = {-0.8f, -0.8f};
    Point p1 = {-0.4f, 0.8f};
    Point p2 = {0.4f, -0.8f};
    Point p3 = {0.8f, 0.8f};

    drawBezier(p0, p1, p2, p3, 3); // Recursion depth 3

    glFlush();
}

void init() {
    glClearColor(1, 1, 1, 1);
    gluOrtho2D(-1, 1, -1, 1);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutCreateWindow("Bezier Fractal Curve");
    glutInitWindowSize(600, 600);
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
