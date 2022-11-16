#include <GL/glut.h>
#include <random>

using namespace std;

const double pi = 3.14159265358979323846;
const float std_size = 2;
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
	GLfloat ambient[] = { 0.7, 0.7, 0.7 };
	GLfloat diffuse[] = { 0.5, 0.5, 0.5 };
	GLfloat specular[] = { 1, 1, 1 };
	GLfloat position[] = { 0, 4, 4 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//Tell the opengl to allow us to change the colors of the material
	glEnable(GL_COLOR_MATERIAL);
}


class Thing {
public:
	float x = 0.5 - random(), y = 0.5 - random(), z = 0.5 - random();
	float temp[3] = { 0,0,0 };
	bool should_pause = false;
	bool paused = false;
	float size = 0;
	float color[10] = { 0 };
	float vx = (-0.5f + random()) / 1500, vy = (-0.5f + random()) / 1500, vz = (-0.5f + random()) / 1500;
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


Thing hexpyr(std_size, 0, 0, 0);

void drawHexPyr(Thing t) {
	int i = 0;

	glPushMatrix();
	glTranslatef(t.x, t.y, t.z);
	glRotatef(t.angle, t.rvx, t.rvy, t.rvz);
	setMaterial(0.2, 0.2, 0.2, 0.3, 0.3, 0.3, 0.5, 0.5, 0.5, 64);

	glBegin(GL_TRIANGLES);

	glColor3f(t.color[i++ % no_of_colors], t.color[i++ % no_of_colors], t.color[i++ % no_of_colors]);
	glVertex3f(0, t.size / 2, 0);
	glVertex3f(-t.size / 4, -t.size / 2, t.size * pow(3, .5) / 4);
	glVertex3f(t.size / 4, -t.size / 2, t.size * pow(3, .5) / 4);

	glColor3f(t.color[i++ % no_of_colors], t.color[i++ % no_of_colors], t.color[i++ % no_of_colors]);
	glVertex3f(0, t.size / 2, 0);
	glVertex3f(-t.size / 4, -t.size / 2, t.size * pow(3, .5) / 4);
	glVertex3f(-t.size / 2, -t.size / 2, 0);

	glColor3f(t.color[i++ % no_of_colors], t.color[i++ % no_of_colors], t.color[i++ % no_of_colors]);
	glVertex3f(0, t.size / 2, 0);
	glVertex3f(-t.size / 2, -t.size / 2, 0);
	glVertex3f(-t.size / 4, -t.size / 2, -t.size * pow(3, .5) / 4);

	glColor3f(t.color[i++ % no_of_colors], t.color[i++ % no_of_colors], t.color[i++ % no_of_colors]);
	glVertex3f(0, t.size / 2, 0);
	glVertex3f(-t.size / 4, -t.size / 2, -t.size * pow(3, .5) / 4);
	glVertex3f(t.size / 4, -t.size / 2, -t.size * pow(3, .5) / 4);

	glColor3f(t.color[i++ % no_of_colors], t.color[i++ % no_of_colors], t.color[i++ % no_of_colors]);
	glVertex3f(0, t.size / 2, 0);
	glVertex3f(t.size / 4, -t.size / 2, -t.size * pow(3, .5) / 4);
	glVertex3f(t.size / 2, -t.size / 2, 0);

	glColor3f(t.color[i++ % no_of_colors], t.color[i++ % no_of_colors], t.color[i++ % no_of_colors]);
	glVertex3f(0, t.size / 2, 0);
	glVertex3f(t.size / 2, -t.size / 2, 0);
	glVertex3f(t.size / 4, -t.size / 2, t.size * pow(3, .5) / 4);


	glColor3f(t.color[i++ % no_of_colors], t.color[i++ % no_of_colors], t.color[i++ % no_of_colors]);
	glVertex3f(0, -t.size / 2, 0);
	glVertex3f(-t.size / 4, -t.size / 2, t.size * pow(3, .5) / 4);
	glVertex3f(t.size / 4, -t.size / 2, t.size * pow(3, .5) / 4);

	glColor3f(t.color[i++ % no_of_colors], t.color[i++ % no_of_colors], t.color[i++ % no_of_colors]);
	glVertex3f(0, -t.size / 2, 0);
	glVertex3f(-t.size / 4, -t.size / 2, t.size * pow(3, .5) / 4);
	glVertex3f(-t.size / 2, -t.size / 2, 0);

	glColor3f(t.color[i++ % no_of_colors], t.color[i++ % no_of_colors], t.color[i++ % no_of_colors]);
	glVertex3f(0, -t.size / 2, 0);
	glVertex3f(-t.size / 2, -t.size / 2, 0);
	glVertex3f(-t.size / 4, -t.size / 2, -t.size * pow(3, .5) / 4);

	glColor3f(t.color[i++ % no_of_colors], t.color[i++ % no_of_colors], t.color[i++ % no_of_colors]);
	glVertex3f(0, -t.size / 2, 0);
	glVertex3f(-t.size / 4, -t.size / 2, -t.size * pow(3, .5) / 4);
	glVertex3f(t.size / 4, -t.size / 2, -t.size * pow(3, .5) / 4);

	glColor3f(t.color[i++ % no_of_colors], t.color[i++ % no_of_colors], t.color[i++ % no_of_colors]);
	glVertex3f(0, -t.size / 2, 0);
	glVertex3f(t.size / 4, -t.size / 2, -t.size * pow(3, .5) / 4);
	glVertex3f(t.size / 2, -t.size / 2, 0);

	glColor3f(t.color[i++ % no_of_colors], t.color[i++ % no_of_colors], t.color[i++ % no_of_colors]);
	glVertex3f(0, -t.size / 2, 0);
	glVertex3f(t.size / 2, -t.size / 2, 0);
	glVertex3f(t.size / 4, -t.size / 2, t.size * pow(3, .5) / 4);

	glEnd();
	glPopMatrix();
}


class Camera {
public:
	float upx = 0, upy = 1, upz = 0;
	float theta = 0, r = 3;
	float x = 0, y = 0, z = r;
	float lax = 0, lay = 0, laz = 0;
};

Camera cam;


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

void moveCamUD(float y) {
	cam.y += y;
	cam.lay += y;
}

void camReset() {
	Camera temp;
	cam = temp;
}


void handleKeypress(unsigned char key, int cursor_x, int cursor_y) {
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
	case 'I':
	case'i':
		moveCamUD(.1);
		break;
	case 'K':
	case 'k':
		moveCamUD(-.1);
		break;
	case 'R':
	case 'r':
		camReset();
		break;
	}
	glutPostRedisplay();
}

bool selected = false, waspaused = false, dragaround = false;
int temp;

void drawSphere(float r = 1, float x = 0, float y = 0, float z = 0) {
	glTranslatef(x, y, z);

	setMaterial(0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_TRIANGLES);

	glColor3f(1, 1, 1);

	float i = 10 * pi / 180;
	for (float theta = -pi / 2; theta < pi / 2 - i; theta += i) {
		for (float phi = 0; phi < 2 * pi - i; phi += i) {
			glVertex3f(r * cos(phi) * cos(theta), r * sin(phi), r * cos(phi) * sin(theta));
			glVertex3f(r * cos(phi) * cos(theta + i), r * sin(phi), r * cos(phi) * sin(theta + i));
			glVertex3f(r * cos(phi + i) * cos(theta + i), r * sin(phi + i), r * cos(phi + i) * sin(theta + i));
		}
	}

	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


float dist(float ax, float ay, float az, float bx, float by, float bz) {
	float ans = 0;
	ans += pow(ax - bx, 2);
	ans += pow(ay - by, 2);
	ans += pow(az - bz, 2);
	ans = pow(ans, 0.5);
	return ans;
}


void display()
{
	glEnable(GL_DEPTH_TEST);
	// Clear your Window
	glClearStencil(0); // this is the default value
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//selecting the Model View Matrix for making modification to the view
	glMatrixMode(GL_MODELVIEW);
	//clearing the model view matrix
	glLoadIdentity();
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	//setting up the position for camara based on the calculation in timeCallBack()
	gluLookAt(cam.x, cam.y, cam.z, cam.lax, cam.lay, cam.laz, cam.upx, cam.upy, cam.upz);

	glColor3f(.5, .5, .5);

	glStencilFunc(GL_ALWAYS, 3, -1);
	drawHexPyr(hexpyr);

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
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE | GLUT_STENCIL);

	// Step3: creating the window
	glutCreateWindow("Hexagonal Pyramid");

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