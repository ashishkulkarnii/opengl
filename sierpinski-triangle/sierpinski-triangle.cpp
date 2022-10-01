#include <GL/glut.h>
#include <random>

std::random_device device;
std::mt19937 rng(device());
std::uniform_real_distribution<>distribution(0.0f, 1.0f);

float random() {
    return distribution(rng);
}

float area(int x1, int y1, int x2, int y2, int x3, int y3)
{
    return abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}

bool isInside(float x1, float y1, float x2, float y2, float x3, float y3, float x, float y)
{
    /* Calculate area of triangle ABC */
    float A = area(x1, y1, x2, y2, x3, y3);

    /* Calculate area of triangle PBC */
    float A1 = area(x, y, x2, y2, x3, y3);

    /* Calculate area of triangle PAC */
    float A2 = area(x1, y1, x, y, x3, y3);

    /* Calculate area of triangle PAB */
    float A3 = area(x1, y1, x2, y2, x, y);

    /* Check if sum of A1, A2 and A3 is same as A */
    return (A == A1 + A2 + A3);
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);

    float v1x = 250, v1y = 490,
        v2x = 10, v2y = 10,
        v3x = 490, v3y = 10;
    float triangle[] = { v1x, v1y, v2x, v2y, v3x, v3y };


    float l = 10;
    float u = 490;

    glColor3f(1.000, 1.000, 0.000); // yellow

    glVertex3f(v1x, v1y, 0);
    glVertex3f(v2x, v2y, 0);
    glVertex3f(v3x, v3y, 0);

    glColor3f(0.000, 1.000, 1.000); // cyan

    float p[2];
    bool b = 1;
    while (true) {
        p[0] = l + random() * (u - l);
        p[1] = l + random() * (u - l);
        if (isInside(v1x, v1y, v2x, v2y, v3x, v3y, p[0], p[1])) {
            break;
        }
    }

    for (int i = 0; i < 50000; i++) {
        // choose vertex
        float vx, vy;
        float r = distribution(rng);
        if (r < (float)1/3) {
            vx = v1x;
            vy = v1y;
        }
        else if (r < (float)2/3) {
            vx = v2x;
            vy = v2y;
        }
        else {
            vx = v3x;
            vy = v3y;
        }

        // calculate midpoint
        float q[2];
        q[0] = (float)(vx + p[0]) / 2.0f;
        q[1] = (float)(vy + p[1]) / 2.0f;

        glVertex2f(q[0], q[1]);

        p[0] = q[0];
        p[1] = q[1];
    }

    glEnd();
    glFlush();	// flush drawing routines to the window
}

int main(int argc, char* argv[])
{
    // initialize GLUT using any command-line arguments
    glutInit(&argc, argv);

    // set up size, position and display mode for new windows
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_RGB);

    // create and set up a new window
    glutCreateWindow("Sierpinski Triangle");
    glutDisplayFunc(display);

    // set up a projection window
    glMatrixMode(GL_PROJECTION); //select projection matrix
    glLoadIdentity(); // clears projection matrix, replaces by I
    gluOrtho2D(0, 500, 0, 500);
    glPointSize(1);

    // tell GLUT to wait for events
    glutMainLoop();
}

