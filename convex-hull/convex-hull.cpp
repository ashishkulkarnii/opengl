#include <iostream>
#include <GL/glut.h>
#include <random>
#include <stack>

using namespace std;


int SCREEN_SIZE[] = { 500, 500 };

random_device device;
mt19937 rng(device());
uniform_real_distribution<>distribution(0.0f, 1.0f);

struct Point {
    float x = 0, y = 0, z = 0, arctan = INT_MAX;
};

void copyPoint(Point a, Point &b) {
    b.x = a.x;
    b.y = a.y;
    b.z = a.z;
    b.arctan = a.arctan;
}

bool areEqualPoints(Point a, Point b) {
    return a.x == b.x and a.y == b.y and a.z == b.z;
}

void printPoint(Point p) {
    glVertex3f(p.x, p.y, p.z);
}

float random() {
    return distribution(rng);
}

bool comparePolarAngle(Point a, Point b) {
    return (a.arctan < b.arctan);
}

Point generateRandomPoint() {
    Point r;
    r.x = random() * SCREEN_SIZE[0];
    r.y = random() * SCREEN_SIZE[1];
    r.z = 0;
    return(r);
}

bool orientation(Point p, Point q, Point r) {
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    return val < 0;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
    glColor3f(0.000, 1.000, 1.000); // cyan

    int n = 20; // number of points
    Point points[200];

    for (int i = 0; i < n; i++) {
        points[i] = generateRandomPoint();
        printPoint(points[i]);
    }

    // finding left-most point and making it the first point
    int min_pos = 0;
    Point temp;
    copyPoint(points[min_pos], temp);
    for (int i = 0; i < n; i++) {
        if (points[i].x < temp.x) {
            copyPoint(points[i], temp);
            min_pos = i;
        }
    }
    swap(points[0], points[min_pos]);


    // storing polar angle from p0
    for (int i = 1; i < n; i++) {
        points[i].arctan = atan((points[i].y - points[0].y) / (points[i].x - points[0].x));
    }

    // sorting all points but the first point by their polar angle subtended at the first point
    sort(&points[1], &points[n], comparePolarAngle);

    for (int i = 0; i < n; i++) {
        cout << points[i].x << " " << points[i].y << " " << points[i].arctan << endl;
    }

    // creating hull
    stack<Point> s;
    int i = 0, j = 0;
    Point a, b, c;
    s.push(points[i]);
    while (i <= n) {
        if (s.size() < 3) {
            s.push(points[i]);
            i++;
        }
        if (s.size() >= 3) {
            c = s.top();
            s.pop();
            b = s.top();
            s.pop();
            a = s.top();
            s.pop();
            if (orientation(a, b, c)) {
                s.push(a);
                s.push(b);
                s.push(c);
                s.push(points[i]);
                i++;
            }
            else {
                s.push(a);
                s.push(c);
            }
        }
    }
    s.pop();
 
    glEnd();
    glBegin(GL_LINES);
    
    Point first = s.top();
    glColor3f(1.000, 1.000, 0.000); // yellow
    while (!s.empty()) {
        printPoint(s.top());
        s.pop();
        if(!s.empty())
            printPoint(s.top());
    }
    printPoint(first);

    glEnd();
    glFlush();	// flush drawing routines to the window
}

int main(int argc, char* argv[])
{
    // initialize GLUT using any command-line arguments
    glutInit(&argc, argv);

    // set up size, position and display mode for new windows
    glutInitWindowSize(SCREEN_SIZE[0], SCREEN_SIZE[1]);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_RGB);

    // create and set up a new window
    glutCreateWindow("Finding Convex Hull");
    glutDisplayFunc(display);

    // set up a projection window
    glMatrixMode(GL_PROJECTION); //select projection matrix
    glLoadIdentity(); // clears projection matrix, replaces by I
    gluOrtho2D(0, SCREEN_SIZE[0], 0, SCREEN_SIZE[1]);
    glPointSize(5);

    // tell GLUT to wait for events
    glutMainLoop();
}

