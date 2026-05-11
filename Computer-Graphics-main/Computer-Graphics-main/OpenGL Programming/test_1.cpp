#include <GL/glut.h>
#include <cmath>

const float CX = 0.0f;
const float CY = 0.25f;  
const float RX = 0.72f; 
const float RY = 0.38f; 


float angle = (float)M_PI;
const float SPEED = 0.005f;

void drawFilledCircle(float x, float y, float r, int seg = 60)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= seg; ++i) {
        float a = 2.0f * (float)M_PI * i / seg;
        glVertex2f(x + r * cosf(a), y + r * sinf(a));
    }
    glEnd();
}

void drawEllipseOutline(int seg = 200)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= seg; ++i) {
        float a = 2.0f * (float)M_PI * i / seg;
        glVertex2f(CX + RX * cosf(a), CY + RY * sinf(a));
    }
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.3f, 0.75f, 0.3f);
    glLineWidth(2.0f);
    drawEllipseOutline();

    float lx = CX + RX * cosf((float)M_PI);
    float ly = CY + RY * sinf((float)M_PI);
    glColor3f(0.2f, 0.4f, 0.8f);
    drawFilledCircle(lx, ly, 0.055f);

    float rx2 = CX + RX;
    float ry2 = CY;
    glColor3f(0.2f, 0.4f, 0.8f);
    drawFilledCircle(rx2, ry2, 0.055f);

    float px = CX + RX * cosf(angle);
    float py = CY + RY * sinf(angle);

    glColor3f(0.1f, 0.4f, 0.95f);
    drawFilledCircle(px, py, 0.055f);

    float tailA = angle - 0.22f;
    if (tailA < (float)M_PI) tailA = (float)M_PI;
    float tx = CX + RX * cosf(tailA);
    float ty = CY + RY * sinf(tailA);

    float dx = px - tx, dy = py - ty;
    float len = sqrtf(dx*dx + dy*dy);

    if (len > 0.001f) {
        dx /= len; dy /= len;

        glColor3f(0.45f, 0.65f, 0.95f);
        glLineWidth(1.8f);
        glBegin(GL_LINES);
            glVertex2f(tx, ty);
            glVertex2f(px, py);
        glEnd();

        float as = 0.045f;
        float ax = px - as * dx;
        float ay = py - as * dy;
        glBegin(GL_TRIANGLES);
            glVertex2f(px, py);
            glVertex2f(ax + as * 0.5f * dy,  ay - as * 0.5f * dx);
            glVertex2f(ax - as * 0.5f * dy,  ay + as * 0.5f * dx);
        glEnd();
    }

    glutSwapBuffers();
}

void update(int)
{
    angle += SPEED;
    if (angle >= 2.0f * (float)M_PI)
        angle = (float)M_PI;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(750, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Ball round enimation");

    glClearColor(0.95f, 0.95f, 0.98f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(-1.1, 1.1, -0.25, 0.75);

    glutDisplayFunc(display);
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}