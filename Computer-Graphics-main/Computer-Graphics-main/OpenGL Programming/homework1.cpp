#include <windows.h>
#include <GL/glut.h>
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glOrtho(-10, 10, -10, 10, -10, 10);
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(4.0f);          

    glBegin(GL_LINE_LOOP);     
    glVertex2f(-7.0f, 3.0f);
    glVertex2f(0.0f, 7.5f);
    glVertex2f(7.0f, 3.0f);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(-5.0f, -5.0f);
    glVertex2f(5.0f, -5.0f);
    glVertex2f(5.0f, 3.0f);
    glVertex2f(-5.0f, 3.0f);
    glEnd();

    glBegin(GL_LINE_LOOP);       
    glVertex2f(-4.0f, -2.5f);
    glVertex2f(-2.0f, -2.5f);
    glVertex2f(-2.0f, -0.5f);
    glVertex2f(-4.0f, -0.5f);
    glEnd();

    glBegin(GL_LINE_LOOP);         
    glVertex2f(2.0f, -2.5f);
    glVertex2f(4.0f, -2.5f);
    glVertex2f(4.0f, -0.5f);
    glVertex2f(2.0f, -0.5f);
    glEnd();

    glBegin(GL_LINE_LOOP);         
    glVertex2f(-1.5f, -5.0f);
    glVertex2f(1.5f, -5.0f);
    glVertex2f(1.5f, 0.0f);
    glVertex2f(-1.5f, 0.0f);
    glEnd();
    glFlush();
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("House");
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
