#include <cmath>
#include <GL/glut.h>

float x = 0.0f;
float y = 0.0f;
int colorState = 0;  

void display(){
    glClear(GL_COLOR_BUFFER_BIT);

    if(colorState == 0)
        glColor3f(1.0f, 0.0f, 0.0f);   
    else if(colorState == 1)
        glColor3f(0.0f, 1.0f, 0.0f);   
    else if(colorState == 2)
        glColor3f(0.0f, 0.0f, 1.0f);   
    else
        glColor3f(1.0f, 1.0f, 1.0f);   

    glBegin(GL_TRIANGLES);
    glVertex2f(x, y);
    glVertex2f(x, y + 100);
    glVertex2f(x + 100, y);
    glEnd();

    if(y > -500){
        y = y - 0.1f;
    } else {
        y = 500;
    }

    static int frame = 0;
    frame++;
    if(frame > 200){
        colorState = (colorState + 1) % 4;
        frame = 0;
    }

    glFlush();
    glutPostRedisplay();
}

void init(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glOrtho(-500, 500, -500, 500, -500, 500);
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Motion Blink RGBW");
    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}