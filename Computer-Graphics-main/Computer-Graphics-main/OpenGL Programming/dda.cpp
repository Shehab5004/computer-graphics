#include <windows.h>
#include <GL/glut.h>
#include<math.h>

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glOrtho(0, 500, 0, 500, 0, 500);
    glClear(GL_COLOR_BUFFER_BIT);

    float x1 = 20, y1 = 10, x2 = 400, y2 = 10;

    float xdif = x2 - x1;
    float ydif = y2 - y1;

    int steps;
    if (fabs(xdif) > fabs(ydif)) {
        steps = fabs(xdif);
    }
    else {
        steps = fabs(ydif);
    }

    float xinc = xdif / steps;
    float yinc = ydif / steps;

    int x = x1;
    int y = y1;
    for (int i = 0; i < steps;i++) {
        glBegin(GL_POINTS);
        glColor3f(1.0f, 0.0f, 0.0f);

        glVertex2f(x, y);
        x = x + xinc;
        y = y + yinc;


    }
    glEnd();

    //line 2

    x1 = 20; y1 = 210; x2 = 210; y2 = 400;
    float dx = x2 - x1;
    float dy = y2 - y1;

    int steps2;
    if (fabs(dx) > fabs(dy)) {
        steps2 = fabs(dx);
    }
    else {
        steps2 = fabs(dy);
    }
    xinc = dx / steps2;
    yinc = dy / steps2;

    x = x1;
    y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps2; i++) {
        glVertex2f(x, y);
        x += xinc;
        y += yinc;
    }
    glEnd();

    //line 3

    x1 = 20; y1 = 210; x2 = 400; y2 = 210;
    float dx2 = x2 - x1;
    float dy2 = y2 - y1;

    int steps3;
    if (fabs(dx2) > fabs(dy2)) {
        steps3 = fabs(dx2);
    }
    else {
        steps3 = fabs(dy2);
    }
    xinc = dx2 / steps3;
    yinc = dy2 / steps3;

    x = x1;
    y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps3; i++) {
        glVertex2f(x, y);
        x += xinc;
        y += yinc;
    }
    glEnd();

    //line 4
    x1 = 400; y1 = 210; x2 = 210; y2 = 400;
    float dx3 = x2 - x1;
    float dy3 = y2 - y1;

    int steps4;
    if (fabs(dx3) > fabs(dy3)) {
        steps4 = fabs(dx3);
    }
    else {
        steps4 = fabs(dy3);
    }
    xinc = dx3 / steps4;
    yinc = dy3 / steps4;
    
    x = x1;
    y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps4; i++) {
        glVertex2f(x, y);
        x += xinc;
        y += yinc;
    }
    glEnd();
    //line 5
    x1 = 20; y1 = 10; x2 = 20; y2 = 210;
    float dx4 = x2 - x1;
    float dy4 = y2 - y1;

    int steps5;
    if (fabs(dx4) > fabs(dy4)) {
        steps5 = fabs(dx4);
    }
    else {
        steps5 = fabs(dy4);
    }
    xinc = dx4 / steps5;
    yinc = dy4 / steps5;

    x = x1;
    y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps5; i++) {
        glVertex2f(x, y);
        x += xinc;
        y += yinc;
    }
    glEnd();

    //line 6
    x1 = 400; y1 = 10; x2 = 400; y2 = 210;
    float dx5 = x2 - x1;
    float dy5 = y2 - y1;

    int steps6;
    if (fabs(dx5) > fabs(dy5)) {
        steps6 = fabs(dx5);
    }
    else {
        steps6 = fabs(dy5);
    }
    xinc = dx5 / steps6;
    yinc = dy5 / steps6;

    x = x1;
    y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps5; i++) {
        glVertex2f(x, y);
        x += xinc;
        y += yinc;
    }
    glEnd();

    //line 7
    x1 = 260; y1 = 10; x2 = 260; y2 = 150;
    float dx6 = x2 - x1;
    float dy6 = y2 - y1;

    int steps7;
    if (fabs(dx6) > fabs(dy6)) {
        steps7 = fabs(dx6);
    }
    else {
        steps7 = fabs(dy6);
    }
    xinc = dx6 / steps7;
    yinc = dy6 / steps7;

    x = x1;
    y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps7; i++) {
        glVertex2f(x, y);
        x += xinc;
        y += yinc;
    }
    glEnd();

    //line 8
    x1 = 170; y1 = 10; x2 = 170; y2 = 150;
    float dx7 = x2 - x1;
    float dy7 = y2 - y1;

    int steps8;
    if (fabs(dx7) > fabs(dy7)) {
        steps8 = fabs(dx7);
    }
    else {
        steps8 = fabs(dy7);
    }
    xinc = dx7 / steps8;
    yinc = dy7 / steps8;

    x = x1;
    y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps8; i++) {
        glVertex2f(x, y);
        x += xinc;
        y += yinc;
    }
    glEnd();

    //line 9
    x1 = 170; y1 = 150; x2 = 260; y2 = 150;
    float dx8 = x2 - x1;
    float dy8 = y2 - y1;

    int steps9;
    if (fabs(dx8) > fabs(dy8)) {
        steps9 = fabs(dx8);
    }
    else {
        steps7 = fabs(dy8);
    }
    xinc = dx8 / steps9;
    yinc = dy8 / steps9;

    x = x1;
    y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps9; i++) {
        glVertex2f(x, y);
        x += xinc;
        y += yinc;
    }
    glEnd();

    //line 10
    x1 = 310; y1 = 150; x2 = 370; y2 = 150;
    float dx9 = x2 - x1;
    float dy9 = y2 - y1;

    int steps10;
    if (fabs(dx9) > fabs(dy9)) {
        steps10 = fabs(dx9);
    }
    else {
        steps10 = fabs(dy9);
    }
    xinc = dx9 / steps10;
    yinc = dy9 / steps10;

    x = x1;
    y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps10; i++) {
        glVertex2f(x, y);
        x += xinc;
        y += yinc;
    }
    glEnd();

    //line 11
    x1 = 310; y1 = 90; x2 = 310; y2 = 150;
    float dx10 = x2 - x1;
    float dy10 = y2 - y1;

    int steps11;
    if (fabs(dx10) > fabs(dy10)) {
        steps11 = fabs(dx10);
    }
    else {
        steps11 = fabs(dy10);
    }
    xinc = dx10 / steps11;
    yinc = dy10 / steps11;

    x = x1;
    y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps11; i++) {
        glVertex2f(x, y);
        x += xinc;
        y += yinc;
    }
    glEnd();

    //line 12
    x1 = 370; y1 = 90; x2 = 370; y2 = 150;
    float dx11 = x2 - x1;
    float dy11 = y2 - y1;

    int steps12;
    if (fabs(dx11) > fabs(dy11)) {
        steps12 = fabs(dx11);
    }
    else {
        steps12 = fabs(dy11);
    }
    xinc = dx11 / steps12;
    yinc = dy11 / steps12;

    x = x1;
    y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps11; i++) {
        glVertex2f(x, y);
        x += xinc;
        y += yinc;
    }
    glEnd();

    //line 13
    x1 = 310; y1 = 90; x2 = 370; y2 = 90;
    float dx12 = x2 - x1;
    float dy12 = y2 - y1;

    int steps13;
    if (fabs(dx12) > fabs(dy12)) {
        steps13 = fabs(dx12);
    }
    else {
        steps13 = fabs(dy12);
    }
    xinc = dx12 / steps13;
    yinc = dy12 / steps13;

    x = x1;
    y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps13; i++) {
        glVertex2f(x, y);
        x += xinc;
        y += yinc;
    }
    glEnd();


    //line 14
    x1 = 50; y1 = 150; x2 = 110; y2 = 150;
    float dx13 = x2 - x1;
    float dy13 = y2 - y1;

    int steps14;
    if (fabs(dx13) > fabs(dy13)) {
        steps14 = fabs(dx13);
    }
    else {
        steps14 = fabs(dy13);
    }
    xinc = dx13 / steps14;
    yinc = dy13 / steps14;

    x = x1;
    y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps14; i++) {
        glVertex2f(x, y);
        x += xinc;
        y += yinc;
    }
    glEnd();

    //line 15
    x1 = 50; y1 = 90; x2 = 50; y2 = 150;
    float dx14 = x2 - x1;
    float dy14 = y2 - y1;

    int steps15;
    if (fabs(dx14) > fabs(dy14)) {
        steps15 = fabs(dx14);
    }
    else {
        steps15 = fabs(dy14);
    }
    xinc = dx14 / steps15;
    yinc = dy14 / steps15;

    x = x1;
    y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps15; i++) {
        glVertex2f(x, y);
        x += xinc;
        y += yinc;
    }
    glEnd();

    //line 16
    x1 = 110; y1 = 90; x2 = 110; y2 = 150;
    float dx15 = x2 - x1;
    float dy15 = y2 - y1;

    int steps16;
    if (fabs(dx15) > fabs(dy15)) {
        steps16 = fabs(dx15);
    }
    else {
        steps16 = fabs(dy15);
    }
    xinc = dx15 / steps16;
    yinc = dy15 / steps16;

    x = x1;
    y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps16; i++) {
        glVertex2f(x, y);
        x += xinc;
        y += yinc;
    }
    glEnd();

    //line 17
    x1 = 50; y1 = 90; x2 = 110; y2 = 90;
    float dx16 = x2 - x1;
    float dy16 = y2 - y1;

    int steps17;
    if (fabs(dx16) > fabs(dy16)) {
        steps17 = fabs(dx16);
    }
    else {
        steps17 = fabs(dy16);
    }
    xinc = dx16 / steps17;
    yinc = dy16 / steps17;

    x = x1;
    y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps17; i++) {
        glVertex2f(x, y);
        x += xinc;
        y += yinc;
    }
    glEnd();



    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("DDA");
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}