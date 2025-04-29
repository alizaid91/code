#include <GL/glut.h>
#include <cmath>

float ballY = 0.0f;
float velocity = 0.02f;
float gravity = -0.00098f;
float floorY = -0.8f;
bool goingDown = true;

void drawBall(float radius) {
    int triangleAmount = 100;
    float twicePi = 2.0f * 3.1415926f;

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0, 0.5, 0.0);  // Orange color
    glVertex2f(0.0f, 0.0f);    // Center of circle
    for(int i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            radius * cos(i * twicePi / triangleAmount),
            radius * sin(i * twicePi / triangleAmount)
        );
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f, ballY, 0.0f);

    // Stretch/Squash based on direction and speed
    float stretch = goingDown ? 0.8f : 1.2f;
    float squash = goingDown ? 1.2f : 0.8f;
    glScalef(1.0f * squash, 1.0f * stretch, 1.0f);

    drawBall(0.1f);
    glutSwapBuffers();
}

void update(int value) {
    ballY += velocity;
    velocity += gravity;

    // Bounce condition
    if (ballY <= floorY) {
        ballY = floorY;
        velocity = -velocity * 0.85f; // Lose some energy
        goingDown = false;
    } else {
        goingDown = velocity < 0;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // ~60 FPS
}

void init() {
    glClearColor(0.9f, 0.9f, 1.0f, 1.0f); // Light background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Bouncing Ball Animation - CG Lab");

    init();
    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}
