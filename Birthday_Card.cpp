// File ID: Assignment_1.cpp

#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include <math.h>
#include <iostream>
#define PI 3.1415926536

void define_to_OpenGL();
void drawCandle();

GLfloat width = 800;
GLfloat height = 600;

GLfloat flameHeight = 10.0; // Initial flame height
GLfloat flameSpeed = 0.002;
GLfloat BalloonStep = 0.2; // Flight speed
GLfloat maxHeight = 460.0; // The maximum height of the balloon

GLfloat totalTime = 0.0;

GLfloat leftRectX = 0;  // Left rectangle x
GLfloat rightRectX = 0; // Right rectangle x
GLfloat coverSpeed = 2.0;  // Rectangular velocity
GLfloat circleAngle = 0.0; // Logo circle
GLfloat heartAngle = 0.0; // Logo heart
GLfloat keyx; // Special key horizontal offset
GLfloat keyy;  // Special key vertical offset

GLfloat speed[5] = {0.7, 0.85, 1.0, 1.15, 1.3}; // Multiplication factors for different orbitals

// cake
typedef struct { GLfloat x, y; } point;
point cakes[4] = { {width / 2, 100}, {width / 2, 200}, {width / 2, 300}, {width / 2, 400} };

// balloon
struct Balloon {
    GLfloat x;
    GLfloat y;
    GLfloat size;
    GLint precision;
    GLfloat color[3];
};
Balloon bigger[] = {
    {25, 95, 42, 260, {0.8, 0.35, 0.7}},
    {85, 180, 42, 260, {0.5, 0.5, 1.0}},
    {165, 120, 42, 260, {0.5, 1.0, 0.7}},
    {250, 200, 42, 260, {1.0, 0.95, 0.5}},
    {360, 240, 42, 260, {1.0, 0.5, 0.5}},
    {510, 220, 42, 260, {0.2, 0.2, 0.6}},
    {580, 170, 42, 260, {1.0, 1.0, 0.5}},
    {660, 80, 42, 260, {1.0, 0.5, 0.5}},
    {620, 50, 42, 260, {0.5, 0.5, 1.0}},
    {750, 140, 42, 260, {0.75, 0.2, 0.5}}
};
Balloon smaller[] = {
    {65, 60, 25, 160, {1.0, 0.5, 0.0}},
    {75, 110, 25, 160, {0.75, 0.2, 0.5}},
    {115, 150, 25, 160, {1.0, 1.0, 0.5}},
    {225, 160, 25, 160, {0.5, 0.65, 1.0}},
    {560, 120, 25, 160, {1.0, 0.0, 0.0}},
    {620, 160, 25, 160, {0.75, 0.2, 0.5}},
    {700, 100, 25, 160, {1.0, 0.95, 0.5}},
    {760, 220, 25, 160, {0.5, 1.0, 0.5}}
};


// random stars
struct Star {
    float x, y;
    float angle;
    float speed; // falling speed
    bool active; // determine whether it exists on the screen
    float delay; //
};

// set 18 stars
Star stars[18]; // save the state of the star


bool isFlying = false;
bool isPaused = false; // galaxy pause
bool increasing = true; // mark the status of the circle in the logo
bool rotateClockwise = true;  // direction of rotation
bool isCoverOpened = false; // cover


void when_in_mainloop() {
    drawCandle();

    glutPostRedisplay();
}

// Noted in main();
void CandleTimer(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, CandleTimer, 1);
}

void BalloonsTimer(int value) {
    if (isFlying) {
        for (int i = 0; i < sizeof(bigger) / sizeof(Balloon); i++) {
            bigger[i].y += BalloonStep;
            if (bigger[i].y >= maxHeight) {
                if (i % 2 == 0) {
                    bigger[i].x -= 1; // move left
                }
                else {
                    bigger[i].x += 1; // move right
                }
                bigger[i].y = maxHeight;
            }
        }

        for (int i = 0; i < sizeof(smaller) / sizeof(Balloon); i++) {
            smaller[i].y += 1.5 * BalloonStep;
            if (smaller[i].y >= maxHeight) {
                if (i % 2 == 0) {
                    smaller[i].x -= 1; // move left
                }
                else {
                    smaller[i].x += 1; // move right
                }
                smaller[i].y = maxHeight;
            }
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, BalloonsTimer, 2);
}

void OrbitTimer(int value) {
    if (!isPaused) {
        totalTime += 0.0085; // increase time
    }
    glutPostRedisplay();
    glutTimerFunc(16, OrbitTimer, 3);
}

void circleTimer(int value) {
    if (increasing) {
        if (circleAngle < 360.0) {
            circleAngle += 1.0;
        }
        else {
            increasing = false;
        }
    }
    else {
        if (circleAngle > 0.0) {
            circleAngle -= 1.0;
        }
        else {
            increasing = true;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, circleTimer, 4);
}

void heartTimer(int value) {
    if (rotateClockwise) {
        heartAngle += 1.0;  // clockwise rotation
        if (heartAngle >= 50.0) {
            rotateClockwise = false;
        }
    }
    else {
        heartAngle -= 1.0;  // counterclockwise rotation
        if (heartAngle <= -50.0) {
            rotateClockwise = true;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, heartTimer, 5);
}

void coverTimer(int value) {
    if (isCoverOpened) {
        if (leftRectX > -width) {
            leftRectX -= coverSpeed; // move left
        }
        if (rightRectX < width) {
            rightRectX += coverSpeed; // move right
        }
    }
    else {
        if (leftRectX < 0.0) {
            leftRectX += coverSpeed; // move right
        }
        if (rightRectX > 0.0) {
            rightRectX -= coverSpeed; // move left
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, coverTimer, 6);
}

void StarsTimer(int value) {
    for (int i = 0; i < 18; i++) {
        if (stars[i].active) {
            // processing delay
            if (value >= stars[i].delay) {
                stars[i].y -= stars[i].speed;
                stars[i].angle += 1.0; // clockwise

                // whether it reaches the bottom of the screen
                if (stars[i].y < 0) {
                    stars[i].y = 600; // reset to the top
                    stars[i].x = rand() % 800;
                }
            }
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, StarsTimer, value + 16);
}



// keyboard interactions

void keyboard_input(unsigned char key, int x, int y) {
    if (key == 'q' || key == 'Q')
		exit(0);
	else if (key == 's' || key == 'S') // stop the orbit
		isPaused = !isPaused;
	else if (key == 'o' || key == 'O') // open the cover
		isCoverOpened = true;
    else if (key == 'c' || key == 'C')  // close the cover
        isCoverOpened = false;
    else if (key == 'r' || key == 'R') // release the balloons
        isFlying = true;
}

// special key
void SpecialKey(GLint key, GLint x, GLint y) {
    switch (key) {
    case GLUT_KEY_UP:    keyy += 5.0; break;   // move up
    case GLUT_KEY_DOWN:  keyy -= 5.0; break;   // move down
    case GLUT_KEY_LEFT:  keyx -= 5.0; break;   // move left
    case GLUT_KEY_RIGHT: keyx += 5.0; break;   // move right
    }
    glutPostRedisplay();
}


// draw the cake
void drawCake() {
    glPushMatrix();

    glTranslatef(cakes[0].x, cakes[0].y, 0);
    glScalef(1.0, 0.2, 0);
    glTranslatef(-cakes[0].x, -cakes[0].y, 0);

    GLint n = 1000;
    for (int i = 0; i < n; i++) {
        glColor3f(0.8, 0.6, 0.8);
        glPointSize(2);

        glBegin(GL_POLYGON);
        glVertex2f(160 * cos(2 * PI * i / n) + cakes[0].x, 160 * sin(2 * PI * i / n) + cakes[0].y + 100);
        glVertex2f(160 * cos(2 * PI * i / n) + cakes[1].x, 160 * sin(2 * PI * i / n) + cakes[1].y + 300);
    }
    glEnd();

    for (int i = 0; i < n; i++) {
        glColor3f(0.75, 0.75, 0.9);
        glPointSize(2);

        glBegin(GL_POLYGON);
        glVertex2f(100 * cos(2 * PI * i / n) + cakes[2].x, 100 * sin(2 * PI * i / n) + cakes[2].y + 300);
        glVertex2f(100 * cos(2 * PI * i / n) + cakes[3].x, 100 * sin(2 * PI * i / n) + cakes[3].y + 500);
    }
    glEnd();

    for (int i = 0; i < n; i++) {
        glColor3f(1.0, 0.9, 0.8);
        glPointSize(2);
        glBegin(GL_POLYGON);

        glVertex2f(60 * cos(2 * PI * i / n) + cakes[3].x, 60 * sin(2 * PI * i / n) + cakes[3].y + 500);
        glVertex2f(60 * cos(2 * PI * i / n) + cakes[3].x, 60 * sin(2 * PI * i / n) + cakes[3].y + 700);
    }
    glEnd();

    for (int i = 0; i < n; i++) {
        glBegin(GL_POINTS);
        glColor3f(0.8, 0.5, 0.8);
        glVertex2f(160 * cos(2 * PI * i / n) + cakes[0].x, 160 * sin(2 * PI * i / n) + cakes[0].y + 100);

        glEnd();
    }
    glEnd();

    for (int i = 0; i < n; i++) {
        glBegin(GL_POINTS);
        glColor3f(0.6, 0.6, 0.9);
        glVertex2f(100 * cos(2 * PI * i / n) + cakes[1].x, 100 * sin(2 * PI * i / n) + cakes[1].y + 400);

        glEnd();
    }
    glEnd();

    for (int i = 0; i < n; i++) {
        glBegin(GL_POINTS);
        glColor3f(1, 1, 0.8);
        glVertex2f(60 * cos(2 * PI * i / n) + cakes[3].x, 60 * sin(2 * PI * i / n) + cakes[3].y + 500);
        glVertex2f(60 * cos(2 * PI * i / n) + cakes[3].x, 60 * sin(2 * PI * i / n) + cakes[3].y + 700);
        glEnd();
    }
    glEnd();

    glPopMatrix();
}

// draw the candle
void drawCandle() {
    // draw the candle ontology
    glPushMatrix();

    glBegin(GL_QUADS);
    glColor3f(0.0, 0.5, 0.8);
    glVertex2f(width / 2 - 5, height / 2);
    glVertex2f(width / 2 - 5, height / 2 + 50);
    glColor3f(0.8, 0.6, 1.0);
    glVertex2f(width / 2 + 5, height / 2 + 50);
    glVertex2f(width / 2 + 5, height / 2);
    glEnd();
    glPopMatrix();

    flameHeight = 10.0 + 5.0 * sin(glutGet(GLUT_ELAPSED_TIME) * flameSpeed);

    // draw the flame
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 30) {
        float angle = i * PI / 180;
        float x = width / 2 + 6 * cos(angle);
        float y = height / 2 + 55 + flameHeight * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 0.5, 0.0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 30) {
        float angle = i * PI / 180;
        float x = width / 2 + 4 * cos(angle); 
        float y = height / 2 + 55 + flameHeight * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 30) {
        float angle = i * PI / 180;
        float x = width / 2 + 2 * cos(angle);
        float y = height / 2 + 55 + flameHeight * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
    glPopMatrix();
}


// draw the balloons

// draw the balloon
void drawBalloon(Balloon balloon) {
    glPushMatrix();

    glColor3f(balloon.color[0], balloon.color[1], balloon.color[2]);
    glScalef(1.0, 1.2, 0);
    glPointSize(2);
    glBegin(GL_POLYGON);
    for (int i = 0; i < balloon.precision; i++) {
        glVertex2f(balloon.size * cos(2 * PI * i / balloon.precision) + balloon.x, balloon.size * sin(2 * PI * i / balloon.precision) + balloon.y);

    }
    glEnd();

    // draw the eyes
    glColor3f(0.0, 0.0, 0.0);
    glPointSize(3);
    glBegin(GL_POINTS);
    glVertex2f(balloon.x - balloon.size / 4, balloon.y + (balloon.size / 4));  // left
    glVertex2f(balloon.x + balloon.size / 4, balloon.y + (balloon.size / 4));  // right
    glEnd();

    // draw the smiling faces
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
    GLfloat angle;
    for (int i = 0; i <= 180; i++) {
        angle = i * PI / 180;
        glVertex2f(balloon.x + balloon.size / 3 * cos(angle), balloon.y - (balloon.size / 4) *  sin(angle));  // smiling face arc
    }
    glEnd();

    glPopMatrix();
}

//draw the balloon string
void drawRope(Balloon balloon) {
    glPushMatrix();

    glScalef(1.0, 1.2, 0);
    glColor3f(0.0, 0.0, 0.0);
    glPointSize(1.0);

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 90; i++) {
        float y = (float)(-i) + balloon.y - balloon.size;
        float x;
        if (balloon.size > 30) {
            x = -12.0 * sin(i * 20 / 360.0) + balloon.x;
        }
        else {
            x = 6.0 * sin(i * 20 / 360.0) + balloon.x;
        }
        glVertex2f(x, y);
    }
    glEnd();

    glPopMatrix();
}

void drawBalloons() {
    glPushMatrix();

    for (int i = 0; i < sizeof(bigger) / sizeof(Balloon); i++) {
        if (isFlying) {
            bigger[i].y += BalloonStep;
        }
        drawBalloon(bigger[i]);
        drawRope(bigger[i]);
    }

    for (int i = 0; i < sizeof(smaller) / sizeof(Balloon); i++) {
        if (isFlying) {
            smaller[i].y += 2 * BalloonStep;
        }
        drawBalloon(smaller[i]);
        drawRope(smaller[i]);
    }

    glFlush();
    glPopMatrix();
}


// plot the text
void drawText(const char* text, float x, float y, void* font, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    int len = (int)strlen(text);
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(font, text[i]);
    }
}


// draw the planet
void drawPlanet(float x, float y, float r, float startR, float startG, float startB, float endR, float endG, float endB) {
    int num = 800; // the number of segments of a polygon

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(startR, startG, startB);
    glVertex2f(x, y);  // center point

    // create a gradient color
    for (int i = 0; i <= num; i++) {
        // compute the interpolation factor
        GLfloat factor = (float)i / num;

        GLfloat currentR = startR + factor * (endR - startR);
        GLfloat currentG = startG + factor * (endG - startG);
        GLfloat currentB = startB + factor * (endB - startB);

        glColor3f(currentR, currentG, currentB); // set the current vertex color
        GLfloat angle = i * 2.0f * PI / num;
        glVertex2f(x + (r * cos(angle)), y + (r * sin(angle)));
    }
    glEnd();
}


// draw the star's orbit
void drawOrbit(float x, float y, float r, float size, float r1, float g1, float b1, float r2, float g2, float b2, int index) {
    glPushMatrix();
    glTranslatef(rightRectX, 0, 0);

    // draw the orbit
    glBegin(GL_LINE_STRIP);
    glLineWidth(1.5);
    for (int i = -90; i <= 90; i++) {
        float angle = i * PI / 180;
        glVertex2f(x - r * cos(angle), y + r * sin(angle));
    }
    glEnd();

    // calculate the current position of the planet
    float offset = totalTime * speed[index];
    float X = x + r * cos(offset);
    float Y = y + r * sin(offset);

    // draw the planet
    drawPlanet(X, Y, size, r1, g1, b1, r2, g2, b2);
    glPopMatrix();
}


// draw the logo
void drawLogo() {
    glPushMatrix(); // saves the current matrix state
    // rectangle

    glTranslatef(leftRectX, 0, 0);
    glPolygonMode(GL_FRONT, GL_FILL);

    glBegin(GL_POLYGON);
    glColor3ub(255, 182, 193);
    glVertex2f(105, 250);
    glVertex2f(95, 250);
    glVertex2f(125, 320);
    glVertex2f(135, 320);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(255, 182, 193);
    glVertex2f(155, 250);
    glVertex2f(145, 250);
    glVertex2f(175, 320);
    glVertex2f(185, 320);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(255, 182, 193);
    glVertex2f(115, 282);
    glVertex2f(200, 282);
    glVertex2f(200, 292);
    glVertex2f(115, 292);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3ub(255, 182, 193);
    for (int i = -105; i < 85; i++) {
        GLfloat r = i * PI / 180;
        GLfloat x = 170 + 50 * cos(r);
        GLfloat y = 285 + 30 * sin(r);
        glVertex2f(x, y);
    }
    glEnd();
    glPopMatrix(); // restores the previous matrix state

    glPushMatrix();
    // circle

    glTranslatef(leftRectX, 0, 0);

    glTranslatef(155, 280, 0);  // center position
    glLineWidth(2.0);
    glColor3f(11.0, 0.84, 0.0);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= circleAngle; i++) {
        float r = i * PI / 180;
        float x = 70 * cos(r);
        float y = 70 * sin(r);
        glVertex2f(x, y);
    }
    glEnd();
    glPopMatrix();

    glPushMatrix();
    // heart

    glTranslatef(leftRectX, 0, 0);
    glTranslatef(150, 297, 0);  // move to the center of rotation
    // rotate at the vertices of the triangle
    glRotatef(heartAngle, 0, 0, 1);
    glTranslatef(-150, -297, 0); // go back to the original position

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(145, 310);
    for (int i = 0; i <= 360; i++) {
        float r = i * PI / 180;
        float x = (145 + 5 * cos(r));
        float y = 310 + 5 * sin(r);
        glVertex2f(x, y);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(155, 310);
    for (int i = 0; i <= 360; i++) {
        float r = i * PI / 180;
        float x = (155 + 5 * cos(r));
        float y = 310 + 5 * sin(r);
        glVertex2f(x, y);
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(139, 310);
    glVertex2f(150, 297);
    glVertex2f(161, 310);
    glEnd();

    glPopMatrix();
}


// draw the galaxy
void drawGalaxy(float translateX, float translateY) {
    srand(time(NULL)); // set random seeds

    glPushMatrix();
    glTranslatef(translateX, translateY, 0); // translate

    for (int i = 0; i < 300; i++) {
        float x = ((float)rand() / RAND_MAX) * width / 2;
        float y = ((float)rand() / RAND_MAX) * height;

        float alpha = (rand() % 50 + 50) / 100.0; // randomly generating transparency

        glPushMatrix();
        glTranslatef(x, y, 0);
        glColor4f(1.0, 1.0, 0.8, alpha);
        glBegin(GL_POLYGON);
        for (int j = 0; j < 6; j++) {
            float angle = 2 * PI * j / 5;
            float radius = 1.2;
            glVertex2f(radius * cos(angle), radius * sin(angle));
        }
        glEnd();
        glPopMatrix();
    }

    glPopMatrix();
}


// draw the random stars
void drawStar(float x, float y, float angle) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(angle, 0.0, 0.0, 1.0); // rotation

    // draw the five-pointed star

    for (int i = 0; i < 5; i++) {
        glPushMatrix();
        glColor3f(1.0, 0.5, 0.0);
        glRotatef(i * 72.0, 0.0, 0.0, 1.0);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(16.0, 0.0); // first point
        for (int j = 0; j <= 100; j++) {
            float angle = 2.0 * PI * float(j) / 100;
            float dx = 6.0 * cosf(angle);
            float dy = 6.0 * sinf(angle);
            glVertex2f(dx, dy);
        }
        glEnd();
        glPopMatrix();

        // draw solid circles at each corner
        glPushMatrix();
        glColor3f(1.0, 0.84, 0.0);
        glTranslatef(16.0 * cosf(i * 72.0f * PI / 180.0), 16.0 * sinf(i * 72.0 * PI / 180.0), 0.0);
        glBegin(GL_TRIANGLE_FAN);
        for (int j = 0; j <= 100; j++) {
            float angle = 2.0 * PI * float(j) / 100;
            float dx = 3.0 * cosf(angle);
            float dy = 3.0 * sinf(angle);
            glVertex2f(dx, dy);
        }
        glEnd();
        glPopMatrix();
    }

    glPopMatrix();
}


// mouse interaction
void mouse_input(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // flip y-coordinate
        // to make the coordinates correspond, make the axes positive down
        int Y = height - y;

        // reinitialize the star state
        if (x >= width / 2 + 220 && x <= width / 2 + 280 && Y >= height / 2 && Y <= height / 2 + 30) {
            for (int i = 0; i < 18; i++) {
                stars[i].x = rand() % 800;
                stars[i].y = rand() % 600;
                stars[i].angle = 0.0; // set initial rotation Angle to 0
                stars[i].speed = 0.6;
                stars[i].active = true;
                stars[i].delay = rand() % 3000; // set the delay time (randomly)
                // simulate the animation effect and wait a while during initialization before moving
            }
        }
    }
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // double buffer mode
    glutInitWindowPosition(300, 100);
    glutInitWindowSize(width, height);
    glutCreateWindow("Birthday Card");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height); // rebuild the coordinate
    glViewport(0, 0, width, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutDisplayFunc(define_to_OpenGL);

    glutTimerFunc(16, CandleTimer, 1);
    glutTimerFunc(16, BalloonsTimer, 2);
    glutTimerFunc(16, OrbitTimer, 3);
    glutTimerFunc(16, circleTimer, 4);
    glutTimerFunc(16, heartTimer, 5);
    glutTimerFunc(16, coverTimer, 6);
    glutTimerFunc(16, StarsTimer, 7);

    glutMouseFunc(mouse_input); // mouse callback function
    glutKeyboardFunc(keyboard_input); // keyboard callback function
    glutSpecialFunc(SpecialKey); // special keyboard callback function

    glutIdleFunc(when_in_mainloop);
    glutMainLoop();
    return 0;
}


void define_to_OpenGL() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Inneer section
    glBegin(GL_POLYGON); //background
    glColor3f(1.0, 0.8, 0.95);
    glVertex2f(0, 0);
    glColor3f(0.9, 0.95, 1.0);
    glVertex2f(0, height);
    glColor3f(1.0, 1.0, 0.85);
    glVertex2f(width, height);
    glColor3f(0.95, 0.7, 0.85);
    glVertex2f(width, 0);
    glShadeModel(GL_SMOOTH);
    glEnd();

    // draw the content
    drawBalloons();
    drawCake();
    drawCandle();

    // text
    const char* text1 = "Happy Birthday!";
    drawText(text1, width / 2 - 80 + keyx, 450.0 + keyy, GLUT_BITMAP_TIMES_ROMAN_24, 1.0, 0.0, 1.0);

    const char* text2 = "No worries about the past,";
    drawText(text2, width / 2 - 108 + keyx, 420.0 + keyy, GLUT_BITMAP_9_BY_15, 1.0, 0.0, 1.0);
    const char* text3 = "no fear of the future,";
    drawText(text3, width / 2 - 96 + keyx, 400.0 + keyy, GLUT_BITMAP_9_BY_15, 1.0, 0.0, 1.0);
    const char* text4 = "the new year is growing, carefree.";
    drawText(text4, width / 2 - 140 + keyx, 380.0 + keyy, GLUT_BITMAP_9_BY_15, 1.0, 0.0, 1.0);

    const char* text5 = "Press the [r] key to release the balloons.";
    drawText(text5, width / 2 - 185, 50.0, GLUT_BITMAP_9_BY_15, 0.0, 0.0, 0.0);

    // click interaction
    glPolygonMode(GL_FRONT, GL_FILL);
    glBegin(GL_QUADS);
    glColor3f(0.53f, 0.81f, 0.98);
    glVertex2f(width / 2 + 220, height / 2 + 30);
    glVertex2f(width / 2 + 220, height / 2);
    glVertex2f(width / 2 + 280, height / 2);
    glVertex2f(width / 2 + 280, height / 2 + 30);
    glEnd();

    const char* clicktext = "Stars";
    drawText(clicktext, width / 2 + 227, height / 2 + 10, GLUT_BITMAP_9_BY_15, 0.29, 0.0, 0.51);

    // random stars
    for (int i = 0; i < 18; i++) {
        if (stars[i].active) {
            drawStar(stars[i].x, stars[i].y, stars[i].angle);
        }
    }


    //cover

    // draw the left rectangle
    glPolygonMode(GL_FRONT, GL_FILL);

    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(leftRectX, height); // top left
    glColor3f(0.1, 0.1, 0.4);
    glVertex2f(leftRectX, 0); // bottom left
    glColor3f(0.1, 0.1, 0.4);
    glVertex2f(leftRectX + width / 2, 0); // bottom right
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(leftRectX + width / 2, height); // top right
    glEnd();

    glTranslatef(leftRectX, 0, 0); // translate to the left rectangle

    drawPlanet(120, 520, 60, 1.0, 1.0, 0.0, 1.0, 0.5, 0.0); 
    drawPlanet(255, 480, 35, 0.5, 1.0, 0.5, 0.0, 0.0, 1.0);
    drawPlanet(60, 400, 25, 1.0, 0.5, 0.5, 0.5, 0.0, 0.5);
    drawPlanet(100, 50, 45, 0.8, 0.8, 0.2, 0.8, 0.2, 0.7);
    drawPlanet(320, 80, 30, 1.0, 0.5, 0.2, 1.0, 0.2, 0.3);

    drawPlanet(210,100, 15, 0.8, 0.0, 0.8, 0.3, 0.3, 0.3);

    glBegin(GL_LINES);
    glColor3f(0.3, 0.3, 0.3);
    glVertex2f(220, 90);
    glColor3f(0.8, 0.0, 0.8);
    glVertex2f(220 + 50, 100);

    glColor3f(0.3, 0.3, 0.3);
    glVertex2f(220, 108);
    glColor3f(0.8, 0.0, 0.8);
    glVertex2f(220 + 40, 118);

    glColor3f(0.3, 0.3, 0.3);
    glVertex2f(220, 100);
    glColor3f(0.8, 0.0, 0.8);
    glVertex2f(220 + 30, 106);
    glEnd();

    glTranslatef(-leftRectX, 0, 0);


    // text
    glTranslatef(leftRectX, 0, 0);
    const char* text6 = "It's a special day for you!";
    drawText(text6, 150.0, 160.0, GLUT_BITMAP_9_BY_15, 1.0, 1.0, 0.0);
    glTranslatef(-leftRectX, 0, 0);

    // galary
    glTranslatef(leftRectX, 0, 0);
    drawGalaxy(leftRectX, 0);
    glTranslatef(-leftRectX, 0, 0);


    // draw the right rectangle
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(rightRectX + width / 2, height);
    glColor3f(0.1, 0.1, 0.4);
    glVertex2f(rightRectX + width / 2, 0);
    glColor3f(0.1, 0.1, 0.4);
    glVertex2f(rightRectX + width, 0);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(rightRectX + width, height); 
    glEnd();


    glTranslatef(rightRectX, 0, 0);

    drawPlanet(width / 2 + 200, 500, 60, 0.8, 0.6, 1.0, 0.0, 0.0, 0.5);
    drawPlanet(460, 380, 30, 1.0, 0.5, 0.0, 1.0, 0.84, 0.0);
    drawPlanet(width / 2 + 200, 180, 30, 0.8, 0.6, 1.0, 0.3, 0.3, 0.3);
    drawPlanet(width / 2 + 320, 110, 15, 1.0, 0.95, 0.5, 1.0, 0.5, 0.0);
    glTranslatef(-rightRectX, 0, 0);

    // draw the orbit
    drawOrbit(width, height / 2, 155, 25, 1.0, 0.5, 0.5, 0.5, 0.0, 0.5, 4);
    drawOrbit(width, height / 2, 125, 22, 1.0, 0.86, 0.28, 0.8, 0.5, 1.0, 3);
    drawOrbit(width, height / 2, 95, 20, 0.8, 0.0, 0.8, 0.3, 0.3, 0.3, 2);
    drawOrbit(width, height / 2, 70, 17, 0.5, 1.0, 0.5, 0.0, 0.0, 1.0, 1);
    drawOrbit(width, height / 2, 50, 12, 0.8, 0.6, 1.0, 0.0, 1.0, 1.0, 0);

    // text
    glTranslatef(rightRectX, 0, 0);
    const char* text7 = "Press the [o] key to open the card.";
    drawText(text7, width / 2 + 30, 80.0, GLUT_BITMAP_9_BY_15, 1.0, 1.0, 1.0);

    const char* text8 = "Press the [c] key to close the card.";
    drawText(text8, width / 2 + 30, 65.0, GLUT_BITMAP_9_BY_15, 1.0, 1.0, 1.0);
    glTranslatef(-rightRectX, 0, 0);

    // galaxy
    glTranslatef(rightRectX, 0, 0);
    drawGalaxy(rightRectX + width / 2, 0);
    glTranslatef(-rightRectX, 0, 0);

    // logo
    drawLogo();

    glutSwapBuffers();
}
