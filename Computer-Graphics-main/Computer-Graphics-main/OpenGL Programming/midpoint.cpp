#include <windows.h>
#include <GL/glut.h>

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glOrtho(-500, 500, -500, 500, -500, 500);
    glClear(GL_COLOR_BUFFER_BIT);

    int r = 200;

    int pk = 1 - r;

    int x = 0, y = r;

    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.0f, 0.0f);

    

    while (x < y)
    {
        glVertex2f(x, y);
        glVertex2f(y, x);
        glVertex2f(y, -x);
        glVertex2f(x, -y);
        glVertex2f(-x, -y);
        glVertex2f(-y, -x);
        glVertex2f(-y, x);
        glVertex2f(-x, y);

        if (pk < 0)
        {
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

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Midpoint circle drawing algorithm");
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}