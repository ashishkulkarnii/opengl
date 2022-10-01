#include <GL/glut.h>
#include <random>

using namespace std;

const double pi = 3.14159265358979323846;
const float sphere_radius = 1.2;
const int no_of_colors = 10;
int WINDOW_SIZE[] = { 640, 640 };

random_device device;
mt19937 rng(device());
uniform_real_distribution<>distribution(0.0f, 1.0f);

float random() {
	return distribution(rng);
}

void setMaterial(float ambientR, float ambientG, float ambientB, float diffuseR, float diffuseG, float diffuseB, float specularR, float specularG, float specularB, float shine) {
	GLfloat ambient[] = { ambientR, ambientG, ambientB };
	GLfloat diffuse[] = { diffuseR, diffuseG, diffuseB };
	GLfloat specular[] = { specularR, specularG, specularB };

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shine);
}

void initLight()
{
	GLfloat ambient[] = { 0.3, 0.3, 0.3 };
	GLfloat diffuse[] = { 0.5, 0.5, 0.5 };
	GLfloat specular[] = { 1, 1, 1 };
	GLfloat position[] = { -2,0.5,4,1 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//Tell the opengl to allow us to change the colors of the material
	glEnable(GL_COLOR_MATERIAL);
}


class Camera {
public:
	float upx = 0, upy = 1, upz = 0;
	float theta = 0, phi = 0, r = 1;
	float x = 0, y = 0, z = r;
	float lax = 0, lay = 0, laz = 0;
};

class Thing {
public:
	float x = 0.5 - random(), y = 0.5 - random(), z = 0.5 - random();
	float temp[3] = { 0,0,0 };
	bool should_pause = false;
	bool paused = false;
	float size = 0;
	float color[no_of_colors] = { 0 };//{ random(), random(), random(), random(), random(), random(), random(), random(), random(), random() };
	float vx = (-0.5f + random()) / 2000, vy = (-0.5f + random()) / 2000, vz = (-0.5f + random()) / 2000;
	float angle = 0, rvx = random() / 100, rvy = random() / 100, rvz = random() / 100;

	Thing(float sz = .1, float stx = -0.5f + random(), float sty = -0.5f + random(), float stz = -0.5f + random()) {
		x = stx;
		y = sty;
		z = stz;
		size = sz;
		for (int i = 0; i < no_of_colors; i++) {
			color[i] = random();
		}
	}
};


Camera cam;
Thing sphere(.2, 0, 0, 0);


void moveCamRot(float x) {
	cam.theta += x;
	cam.x = cam.r * sin(cam.theta * pi / 180);
	cam.z = cam.r * cos(cam.theta * pi / 180);
}

void moveCamFB(float z) {
	cam.r = abs(cam.r);
	cam.r += z;
	cam.x = cam.r * sin(cam.theta * pi / 180);
	cam.z = cam.r * cos(cam.theta * pi / 180);
}

void camReset() {
	Camera temp;
	cam = temp;
}


void handleKeypress(unsigned char key, int cursor_x, int cursor_y) { // handles camera movements, object rotation and triggering pausing
	switch (key) {
	case 'A':
	case 'a':
		moveCamRot(-10);
		break;
	case 'D':
	case 'd':
		moveCamRot(10);
		break;
	case 'W':
	case 'w':
		moveCamFB(-.1);
		break;
	case 'S':
	case 's':
		moveCamFB(.1);
		break;
	case 'R':
	case 'r':
		camReset();
		break;
	}
	glutPostRedisplay();
}


void drawSphere(float r = 1, float x = 0, float y = 0, float z = 0) {
	long int c = 0;

	glTranslatef(x, y, z);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_TRIANGLES);

	float i = 15 * pi / 180; // replace the number 15 with any number (in degrees) to create triangles with sides that subtend that angle at the center
							 // a number which perfectly divides 90 will look more uniform (eg: 9, 10, 15, 30, etc.)
	for (float theta = -pi / 2; theta < pi / 2 - i; theta += i) {
		for (float phi = 0; phi < 2 * pi - i; phi += i) {
			glColor3f(sphere.color[c++ % no_of_colors], sphere.color[c++ % no_of_colors], sphere.color[c++ % no_of_colors]);
			glVertex3f(r * cos(phi) * cos(theta), r * sin(phi), r * cos(phi) * sin(theta));
			glVertex3f(r * cos(phi) * cos(theta + i), r * sin(phi), r * cos(phi) * sin(theta + i));
			glVertex3f(r * cos(phi + i) * cos(theta + i), r * sin(phi + i), r * cos(phi + i) * sin(theta + i));
		}
	}

	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


void display()
{
	glEnable(GL_DEPTH_TEST);
	// Clear your Window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//selecting the Model View Matrix for making modification to the view
	glMatrixMode(GL_MODELVIEW);
	//clearing the model view matrix
	glLoadIdentity();

	//setting up the position for camara based on the calculation in timeCallBack()
	gluLookAt(cam.x, cam.y, cam.z, 0, 0, 0, cam.upx, cam.upy, cam.upz);

	glPushMatrix();
	drawSphere(sphere_radius);
	glPopMatrix();
	// Flush the drawing routines to the window
	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	// Step1:  initialize GLUT using the command line parameters
	glutInit(&argc, argv);

	// Step2: Setup the size of the opengl window, display mode
	glutInitWindowSize(WINDOW_SIZE[0], WINDOW_SIZE[1]);
	glutInitWindowPosition(10, 10);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

	// Step3: creating the window
	glutCreateWindow("Sphere");

	//enable the depth test
	glEnable(GL_DEPTH_TEST);

	//Set up Lighting for the scene
	initLight();

	//set window background color
	glClearColor(.1, .1, .1, 0);

	// Step4: defining a callback function for looping into the rasterizer
	glutDisplayFunc(display);


	//define callback for keyboard interactions
	glutKeyboardFunc(handleKeypress);

	// calls x function on timer (few milliseconds)
	//glutIdleFunc(x);

	//Setup projection window
	glMatrixMode(GL_PROJECTION); // Select Projection Matrix
	glLoadIdentity();

	//ortho graphic view for 3D
	//glOrtho(-10, 10, -10, 10, -10, 10);

	//setup perspective projection
	gluPerspective(90, 1, 0.5, 1000);

	// Step5: Telling the GLUT to loop into the callback
	glutMainLoop();
}