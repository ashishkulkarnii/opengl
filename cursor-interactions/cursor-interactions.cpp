#include <iostream>
#include <GL/glut.h>
#include <random>
#include <tuple>

using namespace std;

const double pi = 3.14159265358979323846;
const float small_sphere_size = .5, sphere_size = 2.5;
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
	GLfloat ambient[] = { 0.6, 0.6, 0.6 };
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
	float color[3] = { random(), random(), random() };
	float vx = (-0.5f + random()) / 1000, vy = (-0.5f + random()) / 1000, vz = (-0.5f + random()) / 1000;
	float angle = 0, rvx = random() / 100, rvy = random() / 100, rvz = random() / 100;

	Thing(float sz = 0, float stx = -0.5f + random()) {
		x = stx;
		size = sz;
	}
};

void sphere(Thing t) {
	glPushMatrix();
	setMaterial(0.2, 0.2, 0.2, 0.3, 0.3, 0.3, 0.9, 0.9, 0.9, 128);
	glColor3f(t.color[0], t.color[1], t.color[2]);
	glTranslatef(t.x, t.y, t.z);
	glRotatef(t.angle, t.rvx, t.rvy, t.rvz);
	setMaterial(0.2, 0.2, 0.2, 0.3, 0.3, 0.3, 0.5, 0.5, 0.5, 64);
	glutSolidSphere(t.size, 20, 20);
	glPopMatrix();
}

class Camera {
public:
	float upx = 0, upy = 1, upz = 0;
	float theta = 0, phi = 0, r = 2;
	float x = 0, y = 0, z = r;
	float lax = 0, lay = 0, laz = 0;
};


Thing sphere1(small_sphere_size, -1);
Thing sphere2(small_sphere_size, 0);
Thing sphere3(small_sphere_size, 1);
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
	case ' ':
		sphere1.should_pause = !sphere1.should_pause;
		sphere2.should_pause = !sphere2.should_pause;
		sphere3.should_pause = !sphere3.should_pause;
		break;
	case '1':
		sphere1.should_pause = !sphere1.should_pause;
		break;
	case '2':
		sphere2.should_pause = !sphere2.should_pause;
		break;
	case '3':
		sphere3.should_pause = !sphere3.should_pause;
		break;
	case 'e':
		if (sphere1.paused) {
			sphere1.angle += 1;
		}
		if (sphere2.paused) {
			sphere2.angle += 1;
		}
		if (sphere3.paused) {
			sphere3.angle += 1;
		}
		break;
	case 'q':
		if (sphere1.paused) {
			sphere1.angle -= 1;
		}
		if (sphere2.paused) {
			sphere2.angle -= 1;
		}
		if (sphere3.paused) {
			sphere3.angle -= 1;
		}
		break;
	case '=':
	case '+':
		if (sphere1.paused) {
			sphere1.size += .05;
		}
		if (sphere2.paused) {
			sphere2.size += .05;
		}
		if (sphere3.paused) {
			sphere3.size += .05;
		}
		break;
	case '-':
		if (sphere1.paused) {
			sphere1.size -= .05;
		}
		if (sphere2.paused) {
			sphere2.size -= .05;
		}
		if (sphere3.paused) {
			sphere3.size -= .05;
		}
		break;
	}
	glutPostRedisplay();
}


bool selected = false, waspaused = false, dragaround = false;
int temp;

void currMousePos(int x, int y) { // handles moving selected object around
	float relx, rely, relz;
	if (dragaround) {
		relx = (sphere_size * 7.0f / 6.0f) * (float)cos(cam.theta * pi / 180) * (float)(x - WINDOW_SIZE[0] / 2) / WINDOW_SIZE[0];
		rely = -(sphere_size * 7.0f / 6.0f) * (float)(y - WINDOW_SIZE[1] / 2) / WINDOW_SIZE[1];
		relz = -(sphere_size * 7.0f / 6.0f) * (float)sin(cam.theta * pi / 180) * (float)(x - WINDOW_SIZE[0] / 2) / WINDOW_SIZE[0];
		switch (temp) {
		case 1:
			sphere1.x = relx;
			sphere1.y = rely;
			sphere1.z = relz;
			break;
		case 2:
			sphere2.x = relx;
			sphere2.y = rely;
			sphere2.z = relz;
			break;
		case 3:
			sphere3.x = relx;
			sphere3.y = rely;
			sphere3.z = relz;
			break;
		default:
			break;
		}
	}
}

void onMouse(int button, int state, int x, int y) { // handles identifying clicked object
	if (state != GLUT_DOWN) {
		return;
	}

	// click to select an object, it moves with change in cursor position, then click to drop

	GLbyte color[4];
	GLfloat depth;
	GLuint index;

	glReadPixels(x, WINDOW_SIZE[1] - y - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
	glReadPixels(x, WINDOW_SIZE[1] - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
	glReadPixels(x, WINDOW_SIZE[1] - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

	printf("\nClicked on pixel: (%d, %d), color: %02hhx%02hhx%02hhx%02hhx, depth: %f, stencil index: %u.\n", x, y, color[0], color[1], color[2], color[3], depth, index);

	if (dragaround) {
		dragaround = false;
		switch (temp) {
		case 1:
			printf("Sphere 1 has been released at (%.2f, %.2f, %.2f) world coordinates.\n\n", sphere1.x, sphere1.y, sphere1.z);
			sphere1.should_pause = waspaused;
			break;
		case 2:
			printf("Sphere 2 has been released at (%.2f, %.2f, %.2f) world coordinates.\n\n", sphere2.x, sphere2.y, sphere2.z);
			sphere2.should_pause = waspaused;
			break;
		case 3:
			printf("Sphere 3 has been released at (%.2f, %.2f, %.2f) world coordinates.\n\n", sphere3.x, sphere3.y, sphere3.z);
			sphere3.should_pause = waspaused;
			break;
		default:
			break;
		}
	}

	switch (index) {
	case 1:
		printf("Sphere 1 has been captured at (%.2f, %.2f, %.2f) world coordinates.\n", sphere1.x, sphere1.y, sphere1.z);
		temp = 1;
		waspaused = sphere1.paused;
		sphere1.should_pause = true;
		dragaround = true;
		break;
	case 2:
		printf("Sphere 2 has been captured at (%.2f, %.2f, %.2f) world coordinates.\n", sphere2.x, sphere2.y, sphere2.z);
		temp = 2;
		waspaused = sphere2.paused;
		sphere2.should_pause = true;
		dragaround = true;
		break;
	case 3:
		printf("Sphere 3 has been captured at (%.2f, %.2f, %.2f) world coordinates.\n", sphere3.x, sphere3.y, sphere3.z);
		temp = 3;
		waspaused = sphere3.paused;
		sphere3.should_pause = true;
		dragaround = true;
		break;
	}
}



void sphere(float size = sphere_size) {
	glPushMatrix();
	glColor3f(.6, .6, .6);
	glutWireSphere(size, 18, 18);
	glPopMatrix();
}


float dist(float ax, float ay, float az, float bx, float by, float bz) { // calculating distance between two points in 3-d space
	float ans = 0;
	ans += pow(ax - bx, 2);
	ans += pow(ay - by, 2);
	ans += pow(az - bz, 2);
	ans = pow(ans, 0.5);
	return ans;
}


Thing handleSphereCollision(Thing t) { // handling collisions between objects and the container sphere
	float n = dist(t.x, t.y, t.z, 0, 0, 0);
	if (n > sphere_size - (float)(t.size)) {
		float nx = t.x / n, ny = t.y / n, nz = t.z / n; // finding normal
		float ndotv = t.vx * nx + t.vy * ny + t.vz * nz; // dot product of normal and object velocity
		// updating direction of object
		t.vx = t.vx - 2 * ndotv * nx;
		t.vy = t.vy - 2 * ndotv * ny;
		t.vz = t.vz - 2 * ndotv * nz;
	}
	return t;
}


Thing handlePause(Thing t) {
	if (t.should_pause == true and t.paused == false) {
		t.temp[0] = t.vx; t.temp[1] = t.vy; t.temp[2] = t.vz;
		t.vx = 0; t.vy = 0; t.vz = 0;
		t.paused = true;
	}
	if (t.should_pause == false and t.paused == true) {
		t.vx = t.temp[0]; t.vy = t.temp[1]; t.vz = t.temp[2];
		t.paused = false;
	}
	return t;
}


Thing randomMotion(Thing t) {
	// changing position by incrementing by velocity
	t.x = t.x + t.vx;
	t.y = t.y + t.vy;
	t.z = t.z + t.vz;
	if (!t.paused) t.angle += 0.01; // incrementing angle about randomly selected axis
	t = handleSphereCollision(t); // handling collision with containing sphere
	t = handlePause(t); // pausing object
	glutPostRedisplay();
	return t;
}

tuple<Thing, Thing> handleCollision(Thing t1, Thing t2) { // handling collisions between objects
	float n = dist(t1.x, t1.y, t1.z, t2.x, t2.y, t2.z);
	if (n < (float)(t1.size + t2.size)) {
		float temp;

		temp = t1.vx;
		t1.vx = t2.vx;
		t2.vx = temp;

		temp = t1.vy;
		t1.vy = t2.vy;
		t2.vy = temp;

		temp = t1.vz;
		t1.vz = t2.vz;
		t2.vz = temp;
	}
	return make_tuple(t1, t2);
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
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);


	//setting up the position for camara based on the calculation in timeCallBack()
	gluLookAt(cam.x, cam.y, cam.z, cam.lax, cam.lay, cam.laz, cam.upx, cam.upy, cam.upz);

	glColor3f(.5, .5, .5);

	glStencilFunc(GL_ALWAYS, 1, -1);
	sphere(sphere1);
	sphere1 = randomMotion(sphere1);

	glStencilFunc(GL_ALWAYS, 2, -1);
	sphere(sphere2);
	sphere2 = randomMotion(sphere2);

	glStencilFunc(GL_ALWAYS, 3, -1);
	sphere(sphere3);
	sphere3 = randomMotion(sphere3);

	tie(sphere1, sphere2) = handleCollision(sphere1, sphere2);
	tie(sphere2, sphere3) = handleCollision(sphere2, sphere3);
	tie(sphere3, sphere1) = handleCollision(sphere3, sphere1);

	glStencilFunc(GL_ALWAYS, 4, -1);
	sphere();

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
	glutCreateWindow("3 bouncing spheres");

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

	glutMouseFunc(onMouse);

	glutPassiveMotionFunc(currMousePos);

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
