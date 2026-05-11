#include <GL/glut.h>
#include <math.h>
#define PI 3.14159265

float h = 0.0f, k = 0.0f;
float a = 200.0f, b = 90.0f;

float thetaMove = PI;
float cx, cy;

void display() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-500, 500, -500, 500, -500, 500);


    glColor3f(0.0f, 0.6f, 0.0f);
    glBegin(GL_LINE_LOOP);
    for (float theta = 0.0f; theta <= 2 * PI; theta += 0.01f) {
        float x = h + a * cos(theta);
        float y = k + b * sin(theta);
        glVertex2f(x, y);
    }
    glEnd();

    cx = h + a * cos(ttaMv);
    cy = k + b * sin(ttaMv);

    int r = 20;
    int pk = 1 - r;
    int x = 0, y = r;
#include
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_POLYGON);
    while (x <= y) {
        glVertex2f(x + cx, y + cy);
        glVertex2f(y + cx, x + cy);
        glVertex2f(y + cx, -x + cy);
        glVertex2f(x + cx, -y + cy);
        glVertex2f(-x + cx, -y + cy);
        glVertex2f(-y + cx, -x + cy);
        glVertex2f(-y + cx, x + cy);
        glVertex2f(-x + cx, y + cy);

        if (pk < 0) {
            pk = pk + (2 * x) + 3;
            x = x + 1;
        }
        else {
            pk = pk + (2 * x) - (2 * y) + 5;
            x = x + 1;
            y = y - 1;
        }
    }
    glEnd();

    if (thetaMove < 2 * PI) {
        thetaMove += 0.0005f;
    }
    else {
        thetaMove = PI;
    }

    glFlush();
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Left to Right on Lower Half of Ellipse");
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}





// g++ Final_eva.cpp -lGL -lGLU -lglut -o Final_eva
// ./Final_eva