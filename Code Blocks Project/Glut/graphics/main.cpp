#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

struct Mosquito {
    float x, y;
    bool isBiting;
    float targetX, targetY;
};

std::vector<Mosquito> mosquitoes;
float waterLevel = -0.6f;
bool dengueTransmission = false;
bool mosquitoesActive = false;

void getRandomBiteLocation(Mosquito &mosquito) {
    mosquito.targetX = 0.35f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (0.45f - 0.35f)));
    mosquito.targetY = -0.1f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (-0.5f + 0.1f)));
}

void displayText(float x, float y, const std::string &text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void drawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= num_segments; i++) {
        float angle = 2.0f * M_PI * float(i) / float(num_segments);
        float x = r * cosf(angle);
        float y = r * sinf(angle);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

void drawMosquito(const Mosquito &mosquito) {
    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircle(mosquito.x, mosquito.y, 0.02f, 20);

    glBegin(GL_LINES);
    glVertex2f(mosquito.x, mosquito.y);
    glVertex2f(mosquito.x + 0.05f, mosquito.y + 0.05f);
    glVertex2f(mosquito.x, mosquito.y);
    glVertex2f(mosquito.x - 0.05f, mosquito.y + 0.05f);
    glVertex2f(mosquito.x, mosquito.y - 0.02f);
    glVertex2f(mosquito.x + 0.03f, mosquito.y - 0.07f);
    glVertex2f(mosquito.x, mosquito.y - 0.02f);
    glVertex2f(mosquito.x - 0.03f, mosquito.y - 0.07f);
    glEnd();
}

void drawHuman() {
    glColor3f(1.0f, 0.8f, 0.6f);
    drawCircle(0.4f, -0.1f, 0.05f, 20);

    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex2f(0.4f, -0.15f);
    glVertex2f(0.4f, -0.5f);
    glVertex2f(0.4f, -0.2f);
    glVertex2f(0.3f, -0.35f);
    glVertex2f(0.4f, -0.2f);
    glVertex2f(0.5f, -0.35f);
    glVertex2f(0.4f, -0.5f);
    glVertex2f(0.35f, -0.7f);
    glVertex2f(0.4f, -0.5f);
    glVertex2f(0.45f, -0.7f);
    glEnd();

    if (dengueTransmission) {
        glColor3f(1.0f, 0.0f, 0.0f);
        displayText(0.5f, -0.2f, "Dengue Virus Transmitted!");
    }
}

void drawTree() {
    glColor3f(0.5f, 0.35f, 0.05f);
    glBegin(GL_QUADS);
    glVertex2f(-0.9f, -0.8f);
    glVertex2f(-0.85f, -0.8f);
    glVertex2f(-0.85f, -0.2f);
    glVertex2f(-0.9f, -0.2f);
    glEnd();

    glColor3f(0.0f, 0.5f, 0.0f);
    drawCircle(-0.875f, 0.0f, 0.3f, 30);
    drawCircle(-0.975f, 0.15f, 0.25f, 30);
    drawCircle(-0.775f, 0.15f, 0.25f, 30);
}

void drawBucket() {
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(-0.5f, -0.8f);
    glVertex2f(-0.3f, -0.8f);
    glVertex2f(-0.3f, -0.4f);
    glVertex2f(-0.5f, -0.4f);
    glEnd();

    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.5f, -0.8f);
    glVertex2f(-0.3f, -0.8f);
    glVertex2f(-0.3f, waterLevel);
    glVertex2f(-0.5f, waterLevel);
    glEnd();
}

void drawSun() {
    glColor3f(1.0f, 1.0f, 0.0f);
    drawCircle(0.7f, 0.7f, 0.1f, 20);
}

void drawCloud(float x, float y) {
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCircle(x, y, 0.1f, 20);
    drawCircle(x + 0.1f, y, 0.1f, 20);
    drawCircle(x - 0.1f, y, 0.1f, 20);
}

void drawBird(float x, float y) {
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(x + 0.05f, y + 0.05f);
    glVertex2f(x, y);
    glVertex2f(x - 0.05f, y + 0.05f);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.7f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glVertex2f(1.0f, 1.0f);
    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(-1.0f, -1.0f);
    glEnd();

    drawSun();
    drawCloud(0.5f, 0.8f);
    drawCloud(-0.5f, 0.9f);
    drawTree();
    drawHuman();
    drawBucket();

    for (const Mosquito &mosquito : mosquitoes) {
        drawMosquito(mosquito);
    }

    if (dengueTransmission) {
        glColor3f(1.0f, 0.0f, 0.0f);
        displayText(-0.2f, 0.8f, "Mosquito is biting the human!");
    } else {
        glColor3f(0.0f, 0.0f, 0.0f);
        displayText(-0.2f, 0.8f, "Mosquitoes are breeding in containers of stagnant water...");
    }

    glutSwapBuffers();
}

void timer(int value) {
    if (mosquitoesActive) {
        if (mosquitoes.empty() || rand() % 50 == 0) {
            mosquitoes.push_back({-0.4f, -0.6f, false, 0.0f, 0.0f});
            getRandomBiteLocation(mosquitoes.back());
        }

        for (auto &mosquito : mosquitoes) {
            if (!mosquito.isBiting) {
                mosquito.x += (mosquito.targetX - mosquito.x) * 0.02f;
                mosquito.y += (mosquito.targetY - mosquito.y) * 0.02f;

                if (abs(mosquito.x - 0.4f) < 0.05f && abs(mosquito.y + 0.1f) < 0.05f) {
                    mosquito.isBiting = true;
                    dengueTransmission = true;
                }
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void handleKeypress(unsigned char key, int x, int y) {
    if (key == 'b' || key == 'B') {
        mosquitoesActive = true;
    }
}

int main(int argc, char **argv) {
    srand(time(0));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Mosquito Simulation");

    glClearColor(0.0f, 0.7f, 1.0f, 1.0f);

    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeypress);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();
    return 0;
}
