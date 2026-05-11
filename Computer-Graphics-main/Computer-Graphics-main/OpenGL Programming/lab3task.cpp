#include <windows.h> 
#include <GL/glut.h> 
#include <math.h> 
void bresenhams(int x1, int x2, int y1, int y2) { 
int pk, x, y; 
float m = float(y2 - y1) / float(x2 - x1); 
float xdiff = fabs(x2 - x1); 
float ydiff = fabs(y2 - y1); 
if (m <= 1 && m >= 0) 
    { 
        pk = (2 * ydiff) - xdiff; 
        x = x1, y = y1; 
        glBegin(GL_POINTS); 
        for (int i = 0; i < xdiff; i++) 
        { 
            glVertex2f(x, y); 
            if (pk < 0) 
            { 
                x++; 
                pk = pk + (2 * ydiff); 
            } 
            else { 
                x++, y++; 
                pk = pk + (2 * ydiff) - (2 * xdiff); 
            } 
        } 
        glEnd(); 
    } 
    else if (m > 1) 
    { 
        pk = (2 * xdiff) - ydiff; 
        x = x1, y = y1; 
        glBegin(GL_POINTS); 
        for (int i = 0; i < ydiff; i++) 
        { 
            glVertex2f(x, y); 
            if (pk < 0) 
            { 
                y++; 
                pk = pk + (2 * xdiff); 
            } 
            else { 
                x++, y++; 
                pk = pk + (2 * xdiff) - (2 * ydiff); 
            } 
        } 
        glEnd(); 
    } 
    else if (m >= 1 && m <= 0) { 
        pk = (2 * ydiff) - xdiff; 
        x = x1, y = y1; 
        glBegin(GL_POINTS); 
        for (int i = 0; i < xdiff; i++) 
        { 
            glVertex2f(x, y); 
            if (pk < 0) 
            { 
                x++; 
                pk = pk + (2 * ydiff); 
            } 
            else { 
                x++, y--; 
                pk = pk + (2 * ydiff) - (2 * xdiff); 
            } 
        } 
        glEnd(); 
    } 
    else { 
        pk = (2 * xdiff) - ydiff; 
        x = x1, y = y1; 
        glBegin(GL_POINTS); 
        for (int i = 0; i < ydiff; i++) 
        { 
            glVertex2f(x, y); 
            if (pk < 0) 
            { 
                y--; 
                pk = pk + (2 * xdiff); 
            } 
            else { 
                x++, y--; 
                pk = pk + (2 * xdiff) - (2 * ydiff); 
            } 
        } 
        glEnd(); 
    } 
} 
void display() { 
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
    glOrtho(0, 500, 0, 500, 0, 500); 
    glClear(GL_COLOR_BUFFER_BIT); 
    int x = 30; 
    glColor3f(0.0f, 1.0f, 1.0f); 
    bresenhams(150, 180, 170, 210); 
    bresenhams(180, 210, 210, 170); 
    bresenhams(150, 180, 170, 130); 
    bresenhams(180, 210, 130, 170);
    glFlush();
} 
int main(int argc, char** argv) { 
    glutInit(&argc, argv); 
    glutInitWindowSize(500, 500); 
    glutInitWindowPosition(50, 50); 
    glutCreateWindow("Diamond"); 
    glutDisplayFunc(display); 
    glutMainLoop(); 
    return 0; 
}