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

class Wheel {
public:
	float axisx = 0, axisy = 0, axisz = 1; // axis of rotation
	float omega = 0; // rotational velocity
	float angle = 10;
	float size_ratio = (float)1 / 5; // wheel size : car size
	float size = 0.2;
	float x = 0, y = 0, z = 0;
	float color[no_of_colors] = { 0 };

	Wheel(Thing t, float w = 3, float xx = 0, float yy = 0, float zz = 0) {
		size = t.size * size_ratio;
		omega = w;
		x = xx; y = yy; z = zz;
		for (int i = 0; i < no_of_colors; i++) {
			color[i] = random();
		}
	}
};


Thing car(std_size, 0, 0, 0);
Wheel wheelfr(car);
Wheel wheelfl(car);
Wheel wheelbr(car);
Wheel wheelbl(car);

Wheel drawWheel(Wheel wheel, float x, float y, float z) {
	int c = 0;

	glTranslatef(x, y, z);
	glRotatef(wheel.angle, wheel.axisx, wheel.axisy, wheel.axisz);
	wheel.angle += wheel.omega;

	glBegin(GL_TRIANGLES);

	// spokes
	setMaterial(0.2, 0.2, 0.2, 0.3, 0.3, 0.3, 0.9, 0.9, 0.9, 128);
	glColor3f((float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10);
	glVertex3f(-wheel.size * 0.45, -wheel.size * 0.04, -wheel.size * 0.04);
	glVertex3f(-wheel.size * 0.45, wheel.size * 0.04, -wheel.size * 0.04);
	glVertex3f(wheel.size * 0.45, wheel.size * 0.04, -wheel.size * 0.04);
	glColor3f((float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10);
	glVertex3f(wheel.size * 0.45, wheel.size * 0.04, -wheel.size * 0.04);
	glVertex3f(-wheel.size * 0.45, -wheel.size * 0.04, -wheel.size * 0.04);
	glVertex3f(wheel.size * 0.45, -wheel.size * 0.04, -wheel.size * 0.04);

	glColor3f((float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10);
	glVertex3f(-wheel.size * 0.45, -wheel.size * 0.04, wheel.size * 0.04);
	glVertex3f(-wheel.size * 0.45, wheel.size * 0.04, wheel.size * 0.04);
	glVertex3f(wheel.size * 0.45, wheel.size * 0.04, wheel.size * 0.04);
	glColor3f((float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10);
	glVertex3f(wheel.size * 0.45, wheel.size * 0.04, wheel.size * 0.04);
	glVertex3f(-wheel.size * 0.45, -wheel.size * 0.04, wheel.size * 0.04);
	glVertex3f(wheel.size * 0.45, -wheel.size * 0.04, wheel.size * 0.04);

	glColor3f((float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10);
	glVertex3f(-wheel.size * 0.45, wheel.size * 0.04, wheel.size * 0.04);
	glVertex3f(-wheel.size * 0.45, wheel.size * 0.04, -wheel.size * 0.04);
	glVertex3f(wheel.size * 0.45, wheel.size * 0.04, wheel.size * 0.04);
	glColor3f((float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10);
	glVertex3f(-wheel.size * 0.45, wheel.size * 0.04, -wheel.size * 0.04);
	glVertex3f(wheel.size * 0.45, wheel.size * 0.04, wheel.size * 0.04);
	glVertex3f(wheel.size * 0.45, wheel.size * 0.04, -wheel.size * 0.04);

	glColor3f((float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10);
	glVertex3f(-wheel.size * 0.45, -wheel.size * 0.04, wheel.size * 0.04);
	glVertex3f(-wheel.size * 0.45, -wheel.size * 0.04, -wheel.size * 0.04);
	glVertex3f(wheel.size * 0.45, -wheel.size * 0.04, wheel.size * 0.04);
	glColor3f((float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10);
	glVertex3f(-wheel.size * 0.45, -wheel.size * 0.04, -wheel.size * 0.04);
	glVertex3f(wheel.size * 0.45, -wheel.size * 0.04, wheel.size * 0.04);
	glVertex3f(wheel.size * 0.45, -wheel.size * 0.04, -wheel.size * 0.04);


	glColor3f((float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10);
	glVertex3f(-wheel.size * 0.04, -wheel.size * 0.45, -wheel.size * 0.04);
	glVertex3f(wheel.size * 0.04, -wheel.size * 0.45, -wheel.size * 0.04);
	glVertex3f(wheel.size * 0.04, wheel.size * 0.45, -wheel.size * 0.04);
	glColor3f((float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10);
	glVertex3f(wheel.size * 0.04, wheel.size * 0.45, -wheel.size * 0.04);
	glVertex3f(-wheel.size * 0.04, -wheel.size * 0.45, -wheel.size * 0.04);
	glVertex3f(-wheel.size * 0.04, wheel.size * 0.45, -wheel.size * 0.04);

	glColor3f((float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10);
	glVertex3f(-wheel.size * 0.04, -wheel.size * 0.45, wheel.size * 0.04);
	glVertex3f(wheel.size * 0.04, -wheel.size * 0.45, wheel.size * 0.04);
	glVertex3f(wheel.size * 0.04, wheel.size * 0.45, wheel.size * 0.04);
	glColor3f((float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10);
	glVertex3f(wheel.size * 0.04, wheel.size * 0.45, wheel.size * 0.04);
	glVertex3f(-wheel.size * 0.04, -wheel.size * 0.45, wheel.size * 0.04);
	glVertex3f(-wheel.size * 0.04, wheel.size * 0.45, wheel.size * 0.04);

	glColor3f((float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10);
	glVertex3f(wheel.size * 0.04, -wheel.size * 0.45, wheel.size * 0.04);
	glVertex3f(wheel.size * 0.04, -wheel.size * 0.45, -wheel.size * 0.04);
	glVertex3f(wheel.size * 0.04, wheel.size * 0.45, wheel.size * 0.04);
	glColor3f((float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10);
	glVertex3f(wheel.size * 0.04, -wheel.size * 0.45, -wheel.size * 0.04);
	glVertex3f(wheel.size * 0.04, wheel.size * 0.45, wheel.size * 0.04);
	glVertex3f(wheel.size * 0.04, wheel.size * 0.45, -wheel.size * 0.04);

	glColor3f((float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10);
	glVertex3f(-wheel.size * 0.04, -wheel.size * 0.45, wheel.size * 0.04);
	glVertex3f(-wheel.size * 0.04, -wheel.size * 0.45, -wheel.size * 0.04);
	glVertex3f(-wheel.size * 0.04, wheel.size * 0.45, wheel.size * 0.04);
	glColor3f((float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10);
	glVertex3f(-wheel.size * 0.04, -wheel.size * 0.45, -wheel.size * 0.04);
	glVertex3f(-wheel.size * 0.04, wheel.size * 0.45, wheel.size * 0.04);
	glVertex3f(-wheel.size * 0.04, wheel.size * 0.45, -wheel.size * 0.04);

	// center
	float r = .1 * wheel.size;
	for (int theta = 0; theta < 360; theta++) {
		glColor3f((float)(7 + 3 * wheel.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * wheel.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * wheel.color[c++ % no_of_colors]) / 10);
		glVertex3f(0, 0, .05 * wheel.size);
		glVertex3f(r * cos(theta * pi / 180), -r * sin(theta * pi / 180), .05 * wheel.size);
		glVertex3f(r * cos((theta + 1) * pi / 180), -r * sin((theta + 1) * pi / 180), .05 * wheel.size);

		glColor3f((float)(7 + 3 * wheel.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * wheel.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * wheel.color[c++ % no_of_colors]) / 10);
		glVertex3f(0, 0, -.05 * wheel.size);
		glVertex3f(r * cos(theta * pi / 180), -r * sin(theta * pi / 180), -.05 * wheel.size);
		glVertex3f(r * cos((theta + 1) * pi / 180), -r * sin((theta + 1) * pi / 180), -.05 * wheel.size);

		glColor3f((float)(7 + 3 * wheel.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * wheel.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * wheel.color[c++ % no_of_colors]) / 10);
		glVertex3f(r * cos(theta * pi / 180), -r * sin(theta * pi / 180), .05 * wheel.size);
		glVertex3f(r * cos((theta + 1) * pi / 180), -r * sin((theta + 1) * pi / 180), .05 * wheel.size);
		glVertex3f(r * cos(theta * pi / 180), -r * sin(theta * pi / 180), -.05 * wheel.size);

		glColor3f((float)(7 + 3 * wheel.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * wheel.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * wheel.color[c++ % no_of_colors]) / 10);
		glVertex3f(r * cos((theta + 1) * pi / 180), -r * sin((theta + 1) * pi / 180), .05 * wheel.size);
		glVertex3f(r * cos(theta * pi / 180), -r * sin(theta * pi / 180), -.05 * wheel.size);
		glVertex3f(r * cos((theta + 1) * pi / 180), -r * sin((theta + 1) * pi / 180), -.05 * wheel.size);
	}

	// rim
	r = .4 * wheel.size;
	float w = .05 * wheel.size;
	for (int theta = 0; theta < 360; theta++) {
		glColor3f((float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10);
		glVertex3f(r * cos(theta * pi / 180), -r * sin(theta * pi / 180), .2 * wheel.size);
		glVertex3f(r * cos((theta + 1) * pi / 180), -r * sin((theta + 1) * pi / 180), .2 * wheel.size);
		glVertex3f(r * cos(theta * pi / 180), -r * sin(theta * pi / 180), -.2 * wheel.size);

		glColor3f((float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10);
		glVertex3f(r * cos((theta + 1) * pi / 180), -r * sin((theta + 1) * pi / 180), .2 * wheel.size);
		glVertex3f(r * cos(theta * pi / 180), -r * sin(theta * pi / 180), -.2 * wheel.size);
		glVertex3f(r * cos((theta + 1) * pi / 180), -r * sin((theta + 1) * pi / 180), -.2 * wheel.size);

		glColor3f((float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10);
		glVertex3f((r + w) * cos(theta * pi / 180), -(r + w) * sin(theta * pi / 180), .2 * wheel.size);
		glVertex3f((r + w) * cos((theta + 1) * pi / 180), -(r + w) * sin((theta + 1) * pi / 180), .2 * wheel.size);
		glVertex3f(r * cos(theta * pi / 180), -r * sin(theta * pi / 180), .2 * wheel.size);

		glColor3f((float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10);
		glVertex3f((r + w) * cos((theta + 1) * pi / 180), -(r + w) * sin((theta + 1) * pi / 180), .2 * wheel.size);
		glVertex3f(r * cos(theta * pi / 180), -r * sin(theta * pi / 180), .2 * wheel.size);
		glVertex3f(r * cos((theta + 1) * pi / 180), -r * sin((theta + 1) * pi / 180), .2 * wheel.size);

		glColor3f((float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10);
		glVertex3f((r + w) * cos(theta * pi / 180), -(r + w) * sin(theta * pi / 180), -.2 * wheel.size);
		glVertex3f((r + w) * cos((theta + 1) * pi / 180), -(r + w) * sin((theta + 1) * pi / 180), -.2 * wheel.size);
		glVertex3f(r * cos(theta * pi / 180), -r * sin(theta * pi / 180), -.2 * wheel.size);

		glColor3f((float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * wheel.color[c++ % no_of_colors]) / 10);
		glVertex3f((r + w) * cos((theta + 1) * pi / 180), -(r + w) * sin((theta + 1) * pi / 180), -.2 * wheel.size);
		glVertex3f(r * cos(theta * pi / 180), -r * sin(theta * pi / 180), -.2 * wheel.size);
		glVertex3f(r * cos((theta + 1) * pi / 180), -r * sin((theta + 1) * pi / 180), -.2 * wheel.size);
	}

	// tyre
	setMaterial(0.2, 0.2, 0.2, 0.3, 0.3, 0.3, 0.5, 0.5, 0.5, 64);
	r = .45 * wheel.size;
	w = .1 * wheel.size;
	for (int theta = 0; theta < 360; theta++) {
		glColor3f((wheel.color[++c % no_of_colors]) / 10, (wheel.color[++c % no_of_colors]) / 10, (wheel.color[++c % no_of_colors]) / 10);
		glVertex3f((r + w) * cos(theta * pi / 180), -(r + w) * sin(theta * pi / 180), .2 * wheel.size);
		glVertex3f((r + w) * cos((theta + 1) * pi / 180), -(r + w) * sin((theta + 1) * pi / 180), .2 * wheel.size);
		glVertex3f((r + w) * cos(theta * pi / 180), -(r + w) * sin(theta * pi / 180), -.2 * wheel.size);

		glColor3f((wheel.color[++c % no_of_colors]) / 10, (wheel.color[++c % no_of_colors]) / 10, (wheel.color[++c % no_of_colors]) / 10);
		glVertex3f((r + w) * cos((theta + 1) * pi / 180), -(r + w) * sin((theta + 1) * pi / 180), .2 * wheel.size);
		glVertex3f((r + w) * cos(theta * pi / 180), -(r + w) * sin(theta * pi / 180), -.2 * wheel.size);
		glVertex3f((r + w) * cos((theta + 1) * pi / 180), -(r + w) * sin((theta + 1) * pi / 180), -.2 * wheel.size);

		glColor3f((wheel.color[++c % no_of_colors]) / 10, (wheel.color[++c % no_of_colors]) / 10, (wheel.color[++c % no_of_colors]) / 10);
		glVertex3f((r + w) * cos(theta * pi / 180), -(r + w) * sin(theta * pi / 180), .2 * wheel.size);
		glVertex3f((r + w) * cos((theta + 1) * pi / 180), -(r + w) * sin((theta + 1) * pi / 180), .2 * wheel.size);
		glVertex3f(r * cos(theta * pi / 180), -r * sin(theta * pi / 180), .2 * wheel.size);

		glColor3f((wheel.color[++c % no_of_colors]) / 10, (wheel.color[++c % no_of_colors]) / 10, (wheel.color[++c % no_of_colors]) / 10);
		glVertex3f((r + w) * cos((theta + 1) * pi / 180), -(r + w) * sin((theta + 1) * pi / 180), .2 * wheel.size);
		glVertex3f(r * cos(theta * pi / 180), -r * sin(theta * pi / 180), .2 * wheel.size);
		glVertex3f(r * cos((theta + 1) * pi / 180), -r * sin((theta + 1) * pi / 180), .2 * wheel.size);

		glColor3f((wheel.color[++c % no_of_colors]) / 10, (wheel.color[++c % no_of_colors]) / 10, (wheel.color[++c % no_of_colors]) / 10);
		glVertex3f((r + w) * cos(theta * pi / 180), -(r + w) * sin(theta * pi / 180), -.2 * wheel.size);
		glVertex3f((r + w) * cos((theta + 1) * pi / 180), -(r + w) * sin((theta + 1) * pi / 180), -.2 * wheel.size);
		glVertex3f(r * cos(theta * pi / 180), -r * sin(theta * pi / 180), -.2 * wheel.size);

		glColor3f((wheel.color[++c % no_of_colors]) / 10, (wheel.color[++c % no_of_colors]) / 10, (wheel.color[++c % no_of_colors]) / 10);
		glVertex3f((r + w) * cos((theta + 1) * pi / 180), -(r + w) * sin((theta + 1) * pi / 180), -.2 * wheel.size);
		glVertex3f(r * cos(theta * pi / 180), -r * sin(theta * pi / 180), -.2 * wheel.size);
		glVertex3f(r * cos((theta + 1) * pi / 180), -r * sin((theta + 1) * pi / 180), -.2 * wheel.size);
	}

	glEnd();

	return wheel;
}

void drawCar(Thing t) {
	int c = 0;

	glPushMatrix();


	glTranslatef(t.x, t.y, t.z);
	glRotatef(t.angle, t.rvx, t.rvy, t.rvz);


	glPushMatrix();
	wheelfl = drawWheel(wheelfl, -.2 * t.size, -.25 * t.size, .35 * t.size);
	glPopMatrix();

	glPushMatrix();
	wheelbl = drawWheel(wheelbl, .2 * t.size, -.25 * t.size, .35 * t.size);
	glPopMatrix();

	glPushMatrix();
	wheelfr = drawWheel(wheelfr, -.2 * t.size, -.25 * t.size, -.35 * t.size);
	glPopMatrix();

	glPushMatrix();
	wheelbr = drawWheel(wheelbr, .2 * t.size, -.25 * t.size, -.35 * t.size);
	glPopMatrix();

	glBegin(GL_TRIANGLES);


	// wheel arches
	setMaterial(0.2, 0.2, 0.2, 0.3, 0.3, 0.3, 0.5, 0.5, 0.5, 64);
	float r = .7 * wheelfl.size;
	for (int theta = 0; theta < 180; theta++) {
		glColor3f((t.color[++c % no_of_colors]) / 10, (t.color[++c % no_of_colors]) / 10, (t.color[++c % no_of_colors]) / 10);
		glVertex3f(r * cos(theta * pi / 180) - .2 * t.size, r * sin(theta * pi / 180) - .25 * t.size, .45 * t.size);
		glVertex3f(r * cos((theta + 1) * pi / 180) - .2 * t.size, r * sin((theta + 1) * pi / 180) - .25 * t.size, .45 * t.size);
		glVertex3f(r * cos(theta * pi / 180) - .2 * t.size, r * sin(theta * pi / 180) - .25 * t.size, -.45 * t.size);

		glColor3f((t.color[++c % no_of_colors]) / 10, (t.color[++c % no_of_colors]) / 10, (t.color[++c % no_of_colors]) / 10);
		glVertex3f(r * cos(theta * pi / 180) - .2 * t.size, r * sin(theta * pi / 180) - .25 * t.size, -.45 * t.size);
		glVertex3f(r * cos((theta + 1) * pi / 180) - .2 * t.size, r * sin((theta + 1) * pi / 180) - .25 * t.size, -.45 * t.size);
		glVertex3f(r * cos((theta + 1) * pi / 180) - .2 * t.size, r * sin((theta + 1) * pi / 180) - .25 * t.size, .45 * t.size);

		glColor3f((t.color[++c % no_of_colors]) / 10, (t.color[++c % no_of_colors]) / 10, (t.color[++c % no_of_colors]) / 10);
		glVertex3f(r * cos(theta * pi / 180) + .2 * t.size, r * sin(theta * pi / 180) - .25 * t.size, .45 * t.size);
		glVertex3f(r * cos((theta + 1) * pi / 180) + .2 * t.size, r * sin((theta + 1) * pi / 180) - .25 * t.size, .45 * t.size);
		glVertex3f(r * cos(theta * pi / 180) + .2 * t.size, r * sin(theta * pi / 180) - .25 * t.size, -.45 * t.size);

		glColor3f((t.color[++c % no_of_colors]) / 10, (t.color[++c % no_of_colors]) / 10, (t.color[++c % no_of_colors]) / 10);
		glVertex3f(r * cos(theta * pi / 180) + .2 * t.size, r * sin(theta * pi / 180) - .25 * t.size, -.45 * t.size);
		glVertex3f(r * cos((theta + 1) * pi / 180) + .2 * t.size, r * sin((theta + 1) * pi / 180) - .25 * t.size, -.45 * t.size);
		glVertex3f(r * cos((theta + 1) * pi / 180) + .2 * t.size, r * sin((theta + 1) * pi / 180) - .25 * t.size, .45 * t.size);
	}

	// undercarriage
	glColor3f((t.color[++c % no_of_colors]) / 10, (t.color[++c % no_of_colors]) / 10, (t.color[++c % no_of_colors]) / 10);
	glVertex3f(-.5 * t.size, -.25 * t.size, .25 * t.size);
	glVertex3f(-.5 * t.size, -.25 * t.size, -.25 * t.size);
	glVertex3f(.5 * t.size, -.25 * t.size, .25 * t.size);

	glColor3f((t.color[++c % no_of_colors]) / 10, (t.color[++c % no_of_colors]) / 10, (t.color[++c % no_of_colors]) / 10);
	glVertex3f(-.5 * t.size, -.25 * t.size, -.25 * t.size);
	glVertex3f(.5 * t.size, -.25 * t.size, .25 * t.size);
	glVertex3f(.5 * t.size, -.25 * t.size, -.25 * t.size);


	// front
	r = .30 * t.size;
	for (int theta = 90; theta < 180; theta++) {
		glColor3f(0, t.color[++c % no_of_colors] * .5 + .5, 0);
		glVertex3f(r * cos(theta * pi / 180) - .2 * t.size, r * sin(theta * pi / 180) - .25 * t.size, .45 * t.size);
		glVertex3f(r * cos((theta + 1) * pi / 180) - .2 * t.size, r * sin((theta + 1) * pi / 180) - .25 * t.size, .45 * t.size);
		glVertex3f(r * cos(theta * pi / 180) - .2 * t.size, r * sin(theta * pi / 180) - .25 * t.size, -.45 * t.size);

		glColor3f(0, t.color[++c % no_of_colors] * .5 + .5, 0);
		glVertex3f(r * cos(theta * pi / 180) - .2 * t.size, r * sin(theta * pi / 180) - .25 * t.size, -.45 * t.size);
		glVertex3f(r * cos((theta + 1) * pi / 180) - .2 * t.size, r * sin((theta + 1) * pi / 180) - .25 * t.size, -.45 * t.size);
		glVertex3f(r * cos((theta + 1) * pi / 180) - .2 * t.size, r * sin((theta + 1) * pi / 180) - .25 * t.size, .45 * t.size);
	}

	// hood
	glColor3f(0, t.color[++c % no_of_colors] * .5 + .5, 0);
	glVertex3f(-.2 * t.size, .05 * t.size, .45 * t.size);
	glVertex3f(-.2 * t.size, .05 * t.size, -.45 * t.size);
	glVertex3f(-.05 * t.size, .05 * t.size, -.45 * t.size);

	glColor3f(0, t.color[++c % no_of_colors] * .5 + .5, 0);
	glVertex3f(-.2 * t.size, .05 * t.size, .45 * t.size);
	glVertex3f(-.05 * t.size, .05 * t.size, -.45 * t.size);
	glVertex3f(-.05 * t.size, .05 * t.size, .45 * t.size);

	// windshield
	setMaterial(0.2, 0.2, 0.2, 0.3, 0.3, 0.3, 0.9, 0.9, 0.9, 128);
	glColor3f(0, 0, t.color[++c % no_of_colors] * .3 + .7);
	glVertex3f(-.05 * t.size, .05 * t.size, -.45 * t.size);
	glVertex3f(-.05 * t.size, .05 * t.size, .45 * t.size);
	glVertex3f(.05 * t.size, .35 * t.size, .45 * t.size);

	glColor3f(0, 0, t.color[++c % no_of_colors] * .3 + .7);
	glVertex3f(-.05 * t.size, .05 * t.size, -.45 * t.size);
	glVertex3f(.05 * t.size, .35 * t.size, .45 * t.size);
	glVertex3f(.05 * t.size, .35 * t.size, -.45 * t.size);

	// roof
	setMaterial(0.2, 0.2, 0.2, 0.3, 0.3, 0.3, 0.5, 0.5, 0.5, 64);
	glColor3f(0, t.color[++c % no_of_colors] * .5 + .5, 0);
	glVertex3f(.05 * t.size, .35 * t.size, .45 * t.size);
	glVertex3f(.05 * t.size, .35 * t.size, -.45 * t.size);
	glVertex3f(.35 * t.size, .35 * t.size, -.45 * t.size);

	glColor3f(0, t.color[++c % no_of_colors] * .5 + .5, 0);
	glVertex3f(.05 * t.size, .35 * t.size, .45 * t.size);
	glVertex3f(.35 * t.size, .35 * t.size, -.45 * t.size);
	glVertex3f(.35 * t.size, .35 * t.size, .45 * t.size);

	// back glass
	setMaterial(0.2, 0.2, 0.2, 0.3, 0.3, 0.3, 0.9, 0.9, 0.9, 128);
	r = .15 * t.size;
	for (int theta = 0; theta < 90; theta++) {
		glColor3f(0, 0, t.color[++c % no_of_colors] * .3 + .7);
		glVertex3f(r * cos(theta * pi / 180) + .35 * t.size, r * sin(theta * pi / 180) + .2 * t.size, .45 * t.size);
		glVertex3f(r * cos((theta + 1) * pi / 180) + .35 * t.size, r * sin((theta + 1) * pi / 180) + .2 * t.size, .45 * t.size);
		glVertex3f(r * cos(theta * pi / 180) + .35 * t.size, r * sin(theta * pi / 180) + .2 * t.size, -.45 * t.size);

		glColor3f(0, 0, t.color[++c % no_of_colors] * .3 + .7);
		glVertex3f(r * cos(theta * pi / 180) + .35 * t.size, r * sin(theta * pi / 180) + .2 * t.size, -.45 * t.size);
		glVertex3f(r * cos((theta + 1) * pi / 180) + .35 * t.size, r * sin((theta + 1) * pi / 180) + .2 * t.size, -.45 * t.size);
		glVertex3f(r * cos((theta + 1) * pi / 180) + .35 * t.size, r * sin((theta + 1) * pi / 180) + .2 * t.size, .45 * t.size);
	}

	glColor3f(0, 0, t.color[++c % no_of_colors] * .3 + .7);
	glVertex3f(.5 * t.size, .2 * t.size, .45 * t.size);
	glVertex3f(.5 * t.size, .2 * t.size, -.45 * t.size);
	glVertex3f(.5 * t.size, .1 * t.size, -.45 * t.size);

	glColor3f(0, 0, t.color[++c % no_of_colors] * .3 + .7);
	glVertex3f(.5 * t.size, .1 * t.size, -.45 * t.size);
	glVertex3f(.5 * t.size, .2 * t.size, .45 * t.size);
	glVertex3f(.5 * t.size, .1 * t.size, .45 * t.size);

	// back
	setMaterial(0.2, 0.2, 0.2, 0.3, 0.3, 0.3, 0.5, 0.5, 0.5, 64);
	glColor3f(0, t.color[++c % no_of_colors] * .5 + .5, 0);
	glVertex3f(.5 * t.size, .1 * t.size, .45 * t.size);
	glVertex3f(.5 * t.size, .1 * t.size, -.45 * t.size);
	glVertex3f(.5 * t.size, -.25 * t.size, -.45 * t.size);

	glColor3f(0, t.color[++c % no_of_colors] * .5 + .5, 0);
	glVertex3f(.5 * t.size, -.25 * t.size, -.45 * t.size);
	glVertex3f(.5 * t.size, .1 * t.size, .45 * t.size);
	glVertex3f(.5 * t.size, -.25 * t.size, .45 * t.size);

	// brake lights
	float posy = -.05 * t.size, posz = .35 * t.size;
	r = .035 * t.size;
	for (int theta = 0; theta < 360; theta++) {
		glColor3f(t.color[++c % no_of_colors] * .5 + .5, 0, 0);
		glVertex3f(.5 * t.size + .001, posy, posz);
		glVertex3f(.5 * t.size + .001, posy - r * sin(theta * pi / 180), posz + r * cos(theta * pi / 180));
		glVertex3f(.5 * t.size + .001, posy - r * sin((theta + 1) * pi / 180), posz + r * cos((theta + 1) * pi / 180));

		glVertex3f(.5 * t.size + .001, posy, -posz);
		glVertex3f(.5 * t.size + .001, posy - r * sin(theta * pi / 180), -posz - r * cos(theta * pi / 180));
		glVertex3f(.5 * t.size + .001, posy - r * sin((theta + 1) * pi / 180), -posz - r * cos((theta + 1) * pi / 180));
	}
	posy = -.15 * t.size;
	r = .05 * t.size;
	for (int theta = 0; theta < 360; theta++) {
		glColor3f(t.color[++c % no_of_colors] * .5 + .5, 0, 0);
		glVertex3f(.5 * t.size + .001, posy, posz);
		glVertex3f(.5 * t.size + .001, posy - r * sin(theta * pi / 180), posz + r * cos(theta * pi / 180));
		glVertex3f(.5 * t.size + .001, posy - r * sin((theta + 1) * pi / 180), posz + r * cos((theta + 1) * pi / 180));

		glVertex3f(.5 * t.size + .001, posy, -posz);
		glVertex3f(.5 * t.size + .001, posy - r * sin(theta * pi / 180), -posz - r * cos(theta * pi / 180));
		glVertex3f(.5 * t.size + .001, posy - r * sin((theta + 1) * pi / 180), -posz - r * cos((theta + 1) * pi / 180));
	}


	// plate
	glColor3f(t.color[++c % no_of_colors] * .3 + .7, t.color[c % no_of_colors] * .3 + .7, t.color[c % no_of_colors] * .3 + .7);
	glVertex3f(.5 * t.size + 0.001, -.05 * t.size, .125 * t.size);
	glVertex3f(.5 * t.size + 0.001, -.05 * t.size, -.125 * t.size);
	glVertex3f(.5 * t.size + 0.001, -.15 * t.size, -.125 * t.size);

	glColor3f(t.color[++c % no_of_colors] * .3 + .7, t.color[c % no_of_colors] * .3 + .7, t.color[c % no_of_colors] * .3 + .7);
	glVertex3f(.5 * t.size + 0.001, -.15 * t.size, -.125 * t.size);
	glVertex3f(.5 * t.size + 0.001, -.05 * t.size, .125 * t.size);
	glVertex3f(.5 * t.size + 0.001, -.15 * t.size, .125 * t.size);

	// headlights
	posy = -.15 * t.size;
	r = .05 * t.size;
	for (int theta = 0; theta < 360; theta++) {
		glColor3f(t.color[++c % no_of_colors] * .3 + .7, t.color[c % no_of_colors] * .3 + .7, t.color[c % no_of_colors] * .3 + .7);
		glVertex3f(-.5 * t.size + .001, posy, posz);
		glVertex3f(-.5 * t.size + .001, posy - r * sin(theta * pi / 180), posz + r * cos(theta * pi / 180));
		glVertex3f(-.5 * t.size + .001, posy - r * sin((theta + 1) * pi / 180), posz + r * cos((theta + 1) * pi / 180));

		glVertex3f(-.5 * t.size + .001, posy, -posz);
		glVertex3f(-.5 * t.size + .001, posy - r * sin(theta * pi / 180), -posz - r * cos(theta * pi / 180));
		glVertex3f(-.5 * t.size + .001, posy - r * sin((theta + 1) * pi / 180), -posz - r * cos((theta + 1) * pi / 180));
	}

	// around front wheel arch
	r = .3 * t.size;
	float posx = -.2 * t.size, r2 = .15 * t.size;
	posy = -.25 * t.size;
	for (float theta = 90; theta < 180; theta++) {
		glColor3f(0, t.color[++c % no_of_colors] * .5 + .5, 0);
		glVertex3f(posx + r * cos(theta * pi / 180), posy + r * sin(theta * pi / 180), .45 * t.size);
		glVertex3f(posx + r * cos((1 + theta) * pi / 180), posy + r * sin((1 + theta) * pi / 180), .45 * t.size);
		glVertex3f(posx + r2 * cos(theta * pi / 180), posy + r2 * sin(theta * pi / 180), .45 * t.size);

		glVertex3f(posx + r * cos(theta * pi / 180), posy + r * sin(theta * pi / 180), -.45 * t.size);
		glVertex3f(posx + r * cos((1 + theta) * pi / 180), posy + r * sin((1 + theta) * pi / 180), -.45 * t.size);
		glVertex3f(posx + r2 * cos(theta * pi / 180), posy + r2 * sin(theta * pi / 180), -.45 * t.size);


		glColor3f(0, t.color[++c % no_of_colors] * .5 + .5, 0);
		glVertex3f(posx + r * cos((1 + theta) * pi / 180), posy + r * sin((1 + theta) * pi / 180), .45 * t.size);
		glVertex3f(posx + r2 * cos(theta * pi / 180), posy + r2 * sin(theta * pi / 180), .45 * t.size);
		glVertex3f(posx + r2 * cos((1 + theta) * pi / 180), posy + r2 * sin((1 + theta) * pi / 180), .45 * t.size);

		glVertex3f(posx + r * cos((1 + theta) * pi / 180), posy + r * sin((1 + theta) * pi / 180), -.45 * t.size);
		glVertex3f(posx + r2 * cos(theta * pi / 180), posy + r2 * sin(theta * pi / 180), -.45 * t.size);
		glVertex3f(posx + r2 * cos((1 + theta) * pi / 180), posy + r2 * sin((1 + theta) * pi / 180), -.45 * t.size);
	}

	// back of front wheel arches
	for (int theta = 0; theta < 90; theta++) {
		glColor3f(0, t.color[++c % no_of_colors] * .5 + .5, 0);

		glVertex3f(posx + r2 * cos(theta * pi / 180), posy + r2 * sin(theta * pi / 180), .45 * t.size);
		glVertex3f(posx + r2 * cos((1 + theta) * pi / 180), posy + r2 * sin((1 + theta) * pi / 180), .45 * t.size);
		glVertex3f(posx + .15 * t.size, posy + .15 * t.size, .45 * t.size);

		glVertex3f(posx + r2 * cos(theta * pi / 180), posy + r2 * sin(theta * pi / 180), -.45 * t.size);
		glVertex3f(posx + r2 * cos((1 + theta) * pi / 180), posy + r2 * sin((1 + theta) * pi / 180), -.45 * t.size);
		glVertex3f(posx + .15 * t.size, posy + .15 * t.size, -.45 * t.size);
	}

	// front of back wheel arches
	for (int theta = 0; theta < 90; theta++) {
		glColor3f(0, t.color[++c % no_of_colors] * .5 + .5, 0);

		glVertex3f(-posx - r2 * cos(theta * pi / 180), posy + r2 * sin(theta * pi / 180), .45 * t.size);
		glVertex3f(-posx - r2 * cos((1 + theta) * pi / 180), posy + r2 * sin((1 + theta) * pi / 180), .45 * t.size);
		glVertex3f(-posx - .15 * t.size, posy + .15 * t.size, .45 * t.size);

		glVertex3f(-posx - r2 * cos(theta * pi / 180), posy + r2 * sin(theta * pi / 180), -.45 * t.size);
		glVertex3f(-posx - r2 * cos((1 + theta) * pi / 180), posy + r2 * sin((1 + theta) * pi / 180), -.45 * t.size);
		glVertex3f(-posx - .15 * t.size, posy + .15 * t.size, -.45 * t.size);
	}

	// behind back wheel arches
	posx = -posx;
	for (int theta = 0; theta < 90; theta++) {
		glColor3f(0, t.color[++c % no_of_colors] * .5 + .5, 0);

		glVertex3f(posx + r2 * cos(theta * pi / 180), posy + r2 * sin(theta * pi / 180), .45 * t.size);
		glVertex3f(posx + r2 * cos((1 + theta) * pi / 180), posy + r2 * sin((1 + theta) * pi / 180), .45 * t.size);
		glVertex3f(posx + .15 * t.size, posy + .15 * t.size, .45 * t.size);

		glVertex3f(posx + r2 * cos(theta * pi / 180), posy + r2 * sin(theta * pi / 180), -.45 * t.size);
		glVertex3f(posx + r2 * cos((1 + theta) * pi / 180), posy + r2 * sin((1 + theta) * pi / 180), -.45 * t.size);
		glVertex3f(posx + .15 * t.size, posy + .15 * t.size, -.45 * t.size);
	}

	// rectangle between wheel arches
	glColor3f(0, t.color[++c % no_of_colors] * .5 + .5, 0);

	glVertex3f(-.05 * t.size, -.25 * t.size, .45 * t.size);
	glVertex3f(.05 * t.size, -.25 * t.size, .45 * t.size);
	glVertex3f(-.05 * t.size, -.1 * t.size, .45 * t.size);

	glVertex3f(-.05 * t.size, -.25 * t.size, -.45 * t.size);
	glVertex3f(.05 * t.size, -.25 * t.size, -.45 * t.size);
	glVertex3f(-.05 * t.size, -.1 * t.size, -.45 * t.size);

	glColor3f(0, t.color[++c % no_of_colors] * .5 + .5, 0);

	glVertex3f(.05 * t.size, -.25 * t.size, .45 * t.size);
	glVertex3f(-.05 * t.size, -.1 * t.size, .45 * t.size);
	glVertex3f(.05 * t.size, -.1 * t.size, .45 * t.size);

	glVertex3f(.05 * t.size, -.25 * t.size, -.45 * t.size);
	glVertex3f(-.05 * t.size, -.1 * t.size, -.45 * t.size);
	glVertex3f(.05 * t.size, -.1 * t.size, -.45 * t.size);

	// square above front wheel arches
	posx = -posx;
	glColor3f(0, t.color[++c % no_of_colors] * .5 + .5, 0);

	glVertex3f(posx, posy + .3 * t.size, .45 * t.size);
	glVertex3f(posx, posy + .15 * t.size, .45 * t.size);
	glVertex3f(posx + .15 * t.size, posy + .15 * t.size, .45 * t.size);

	glVertex3f(posx, posy + .3 * t.size, -.45 * t.size);
	glVertex3f(posx, posy + .15 * t.size, -.45 * t.size);
	glVertex3f(posx + .15 * t.size, posy + .15 * t.size, -.45 * t.size);

	glColor3f(0, t.color[++c % no_of_colors] * .5 + .5, 0);

	glVertex3f(posx, posy + .3 * t.size, .45 * t.size);
	glVertex3f(posx + .15 * t.size, posy + .15 * t.size, .45 * t.size);
	glVertex3f(posx + .15 * t.size, posy + .3 * t.size, .45 * t.size);

	glVertex3f(posx, posy + .3 * t.size, -.45 * t.size);
	glVertex3f(posx + .15 * t.size, posy + .15 * t.size, -.45 * t.size);
	glVertex3f(posx + .15 * t.size, posy + .3 * t.size, -.45 * t.size);

	// long horizontal portion
	glColor3f(0, t.color[++c % no_of_colors] * .5 + .5, 0);

	glVertex3f(-.05 * t.size, -.1 * t.size, .45 * t.size);
	glVertex3f(-.05 * t.size, .05 * t.size, .45 * t.size);
	glVertex3f(.35 * t.size, -.1 * t.size, .45 * t.size);

	glVertex3f(-.05 * t.size, -.1 * t.size, -.45 * t.size);
	glVertex3f(-.05 * t.size, .05 * t.size, -.45 * t.size);
	glVertex3f(.35 * t.size, -.1 * t.size, -.45 * t.size);

	glColor3f(0, t.color[++c % no_of_colors] * .5 + .5, 0);

	glVertex3f(-.05 * t.size, .05 * t.size, .45 * t.size);
	glVertex3f(.35 * t.size, -.1 * t.size, .45 * t.size);
	glVertex3f(.35 * t.size, .05 * t.size, .45 * t.size);

	glVertex3f(-.05 * t.size, .05 * t.size, -.45 * t.size);
	glVertex3f(.35 * t.size, -.1 * t.size, -.45 * t.size);
	glVertex3f(.35 * t.size, .05 * t.size, -.45 * t.size);

	// vertical side portion
	glColor3f(0, t.color[++c % no_of_colors] * .5 + .5, 0);

	glVertex3f(.35 * t.size, .2 * t.size, -.45 * t.size);
	glVertex3f(.35 * t.size, -.25 * t.size, -.45 * t.size);
	glVertex3f(.5 * t.size, -.25 * t.size, -.45 * t.size);

	glVertex3f(.35 * t.size, .2 * t.size, .45 * t.size);
	glVertex3f(.35 * t.size, -.25 * t.size, .45 * t.size);
	glVertex3f(.5 * t.size, -.25 * t.size, .45 * t.size);

	glColor3f(0, t.color[++c % no_of_colors] * .5 + .5, 0);

	glVertex3f(.35 * t.size, .2 * t.size, .45 * t.size);
	glVertex3f(.5 * t.size, -.25 * t.size, .45 * t.size);
	glVertex3f(.5 * t.size, .2 * t.size, .45 * t.size);

	glVertex3f(.35 * t.size, .2 * t.size, -.45 * t.size);
	glVertex3f(.5 * t.size, -.25 * t.size, -.45 * t.size);
	glVertex3f(.5 * t.size, .2 * t.size, -.45 * t.size);

	// side triangular window
	setMaterial(0.2, 0.2, 0.2, 0.3, 0.3, 0.3, 0.9, 0.9, 0.9, 128);
	glColor3f(0, 0, t.color[++c % no_of_colors] * .3 + .7);

	glVertex3f(-.05 * t.size, .05 * t.size, .45 * t.size);
	glVertex3f(.05 * t.size, .05 * t.size, .45 * t.size);
	glVertex3f(.05 * t.size, .35 * t.size, .45 * t.size);

	glVertex3f(-.05 * t.size, .05 * t.size, -.45 * t.size);
	glVertex3f(.05 * t.size, .05 * t.size, -.45 * t.size);
	glVertex3f(.05 * t.size, .35 * t.size, -.45 * t.size);

	// side window
	glColor3f(0, 0, t.color[++c % no_of_colors] * .3 + .7);

	glVertex3f(.05 * t.size, .05 * t.size, .45 * t.size);
	glVertex3f(.05 * t.size, .35 * t.size, .45 * t.size);
	glVertex3f(.35 * t.size, .35 * t.size, .45 * t.size);

	glVertex3f(.05 * t.size, .05 * t.size, -.45 * t.size);
	glVertex3f(.05 * t.size, .35 * t.size, -.45 * t.size);
	glVertex3f(.35 * t.size, .35 * t.size, -.45 * t.size);

	glColor3f(0, 0, t.color[++c % no_of_colors] * .3 + .7);

	glVertex3f(.05 * t.size, .05 * t.size, .45 * t.size);
	glVertex3f(.35 * t.size, .05 * t.size, .45 * t.size);
	glVertex3f(.35 * t.size, .35 * t.size, .45 * t.size);

	glVertex3f(.05 * t.size, .05 * t.size, -.45 * t.size);
	glVertex3f(.35 * t.size, .05 * t.size, -.45 * t.size);
	glVertex3f(.35 * t.size, .35 * t.size, -.45 * t.size);

	// d pillar top
	setMaterial(0.2, 0.2, 0.2, 0.3, 0.3, 0.3, 0.5, 0.5, 0.5, 64);
	r = .15 * t.size;
	posx = .35 * t.size; posy = .2 * t.size;
	for (int theta = 0; theta < 90; theta++) {
		glColor3f(0, t.color[++c % no_of_colors] * .5 + .5, 0);

		glVertex3f(posx + r * sin(theta * pi / 180), posy + r * cos(theta * pi / 180), .45 * t.size);
		glVertex3f(posx + r * sin((theta + 1) * pi / 180), posy + r * cos((theta + 1) * pi / 180), .45 * t.size);
		glVertex3f(posx, posy, .45 * t.size);

		glVertex3f(posx + r * sin(theta * pi / 180), posy + r * cos(theta * pi / 180), -.45 * t.size);
		glVertex3f(posx + r * sin((theta + 1) * pi / 180), posy + r * cos((theta + 1) * pi / 180), -.45 * t.size);
		glVertex3f(posx, posy, -.45 * t.size);
	}

	// door handles
	glColor3f(t.color[++c % no_of_colors] * .3, t.color[++c % no_of_colors] * .3, t.color[++c % no_of_colors] * .3);

	glVertex3f(.12 * t.size, -.03 * t.size, .45 * t.size + 0.001);
	glVertex3f(.12 * t.size, .0 * t.size, .45 * t.size + 0.001);
	glVertex3f(.05 * t.size, -.03 * t.size, .45 * t.size + 0.001);

	glVertex3f(.12 * t.size, -.03 * t.size, -.45 * t.size - 0.001);
	glVertex3f(.12 * t.size, .0 * t.size, -.45 * t.size - 0.001);
	glVertex3f(.05 * t.size, -.03 * t.size, -.45 * t.size - 0.001);

	glColor3f(t.color[++c % no_of_colors] * .3, t.color[++c % no_of_colors] * .3, t.color[++c % no_of_colors] * .3);

	glVertex3f(.12 * t.size, .0 * t.size, .45 * t.size + 0.001);
	glVertex3f(.05 * t.size, -.03 * t.size, .45 * t.size + 0.001);
	glVertex3f(.05 * t.size, .0 * t.size, .45 * t.size + 0.001);

	glVertex3f(.12 * t.size, .0 * t.size, -.45 * t.size - 0.001);
	glVertex3f(.05 * t.size, -.03 * t.size, -.45 * t.size - 0.001);
	glVertex3f(.05 * t.size, .0 * t.size, -.45 * t.size - 0.001);

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


Thing randomMotion(Thing t) {
	glutPostRedisplay();
	return t;
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

	glStencilFunc(GL_ALWAYS, 1, -1);
	drawCar(car);
	car = randomMotion(car); // car stencil: 1

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
	glutCreateWindow("Car");

	//enable the depth test
	glEnable(GL_DEPTH_TEST);

	//Set up Lighting for the scene
	initLight();

	//set window background color
	glClearColor(.3, .3, .3, 0);

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