#include <windows.h>
#include <GL/glut.h>
#include <math.h> 

void bresenhams(int x1, int y1, int x2, int y2) {
    int pk, x, y;
    float xdif = fabs(x2 - x1);
    float ydif = fabs(y2 - y1);
    float m = float(y2 - y1) / float(x2 - x1);

    if (m <= 1 && m >= 0) 
    { 
        pk = (2 * ydif) - xdif; 
        x = x1, y = y1; 
        glBegin(GL_POINTS); 
        for (int i = 0; i < xdif; i++) 
        { 
            glVertex2f(x, y); 
            if (pk < 0) 
            { 
                x++; 
                pk = pk + (2 * ydif); 
            } 
            else { 
                x++, y++; 
                pk = pk + (2 * ydif) - (2 * xdif); 
            } 
        } 
        glEnd(); 
    }

    else if (m > 1) 
    { 
        pk = (2 * xdif) - ydif; 
        x = x1, y = y1; 
        glBegin(GL_POINTS); 
        for (int i = 0; i < ydif; i++) 
        { 
            glVertex2f(x, y); 
            if (pk < 0) 
            { 
                y++; 
                pk = pk + (2 * xdif); 
            } 
            else { 
                x++, y++; 
                pk = pk + (2 * xdif) - (2 * ydif); 
            } 
        } 
        glEnd(); 
    }

    else if (m >= -1 && m <= 0) { 
        pk = (2 * ydif) - xdif; 
        x = x1, y = y1; 
        glBegin(GL_POINTS); 
        for (int i = 0; i < xdif; i++) 
        { 
            glVertex2f(x, y); 
            if (pk < 0) 
            { 
                x++; 
                pk = pk + (2 * ydif); 
            } 
            else { 
                x++, y--; 
                pk = pk + (2 * ydif) - (2 * xdif); 
            } 
        } 
        glEnd(); 
    } 
    else { 
        pk = (2 * xdif) - ydif; 
        x = x1, y = y1; 
        glBegin(GL_POINTS); 
        for (int i = 0; i < ydif; i++) 
        { 
            glVertex2f(x, y); 
            if (pk < 0) 
            { 
                y--; 
                pk = pk + (2 * xdif); 
            } 
            else { 
                x++, y--; 
                pk = pk + (2 * xdif) - (2 * ydif); 
            } 
        } 
        glEnd(); 
    }

}
void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glOrtho(-500, 500, -500, 500, -500, 500);
    glClear(GL_COLOR_BUFFER_BIT);

    //red
    glColor3f(1.0f, 0.0f, 0.0f);
    bresenhams(0, 180, 0, 0);

    //blue
    glColor3f(0.0f, 0.0f, 1.0f);
    bresenhams(0, 180, 210, 300);

    glColor3f(0.0f, 1.0f, 1.0f);
    bresenhams(210, 300, 400, 180);

    glColor3f(1.0f, 0.0f, 1.0f);
    bresenhams(400, 180, 400, 0);

    glColor3f(1.0f, 1.0f, 1.0f);
    bresenhams(0, 0, 400, 0);

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Bresensham");
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}