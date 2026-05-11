#include <windows.h>
#include <GL/glut.h>

float fx = 50;    
float fy = 250;   

int stage = 0;    

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    //rectangle
    glColor3f(0.0f, .7f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(fx - 50, fy + 35);
    glVertex2f(fx + 50, fy + 35);
    glVertex2f(fx + 50, fy - 35);
    glVertex2f(fx - 50, fy - 35);
    glEnd();

    //CIRCLE
    glColor3f(1.0f, 0.0f, 0.0f);
    int r = 20;
    int pk = 1 - r;
    int x = 0, y = r;

    glBegin(GL_POLYGON);
    while (x < y) {
        glVertex2f(fx + x, fy + y);
        glVertex2f(fx + y, fy + x);
        glVertex2f(fx + y, fy - x);
        glVertex2f(fx + x, fy - y);
        glVertex2f(fx - x, fy - y);
        glVertex2f(fx - y, fy - x);
        glVertex2f(fx - y, fy + x);
        glVertex2f(fx - x, fy + y);

        if (pk < 0) {
            pk += 2 * x + 3;
        } else {
            pk += 2 * x - 2 * y + 5;
            y--;
        }
        x++;
    }
    glEnd();

    //MOVEMENT 
    if (stage == 0) {
        fx += 0.01f;
        fy += 0.01f;

        if (fx >= 250 && fy >= 450)
            stage = 1;
    }
    
    else if (stage == 1) {
        fx += 0.1f;
        fy -= 0.1f;
    }

    glFlush();
    glutPostRedisplay();
}

void init() {
    glClearColor(0, 0, 0, 1);
    glOrtho(0, 500, 0, 500, -1, 1);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Moving Flag");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}