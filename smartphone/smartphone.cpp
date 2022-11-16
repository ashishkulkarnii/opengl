#include <GL/glut.h>
#include <random>
#include <fstream>
#include <sstream>

using namespace std;

const double pi = 3.14159265358979323846;
const float std_size = 1.5;
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
	GLfloat specular[] = { .71, .71, .71 };
	GLfloat position[] = { 0, 20, 4 };

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

class PhoneScreen {
public:
	bool colors = true;
	bool image = false;
	int screen_triangles = 0;
	vector<vector<string>> content;
	string line, word;
	vector<string> row;
	string fname = "./image/image.csv";

	PhoneScreen() {
		fstream file(fname, ios::in);
		if (file.is_open())
		{
			image = true;
			while (getline(file, line))
			{
				row.clear();

				stringstream str(line);

				while (getline(str, word, ','))
					row.push_back(word);
				content.push_back(row);
			}
		}
		else
		{
			colors = false;
		}
		int rw = 0, clm = 0;
	}
};

Thing phone(std_size, 0, 0, 0);


PhoneScreen psn;
void drawPhone(Thing t) {
	bool colors = true;
	int screen_triangles = 0;

	float screenheight = 1.6 * t.size, screenwidth = 0.9 * t.size, phonethickness = t.size * 0.2, bezelwidth = t.size * 0.05;
	int screenresolution[] = { 90, 160 };

	glPushMatrix();
	glTranslatef(t.x, t.y, t.z);
	glRotatef(t.angle, t.rvx, t.rvy, t.rvz);

	glBegin(GL_TRIANGLES);

	//screen
	setMaterial(0.2, 0.2, 0.2, 0.3, 0.3, 0.3, 0.9, 0.9, 0.9, 128);
	long int c = 0;
	for (float i = screenheight / 2 - screenheight / screenresolution[1], rw = 0; i > -screenheight / 2; i -= screenheight / screenresolution[1], rw++) {
		for (float j = -screenwidth / 2, clm = 0; j < screenwidth / 2 - screenwidth / screenresolution[0]; j += screenwidth / screenresolution[0], clm += 3) {
			if (not colors or not psn.image) {
				glColor3f((t.color[++c % no_of_colors]) / 10, (t.color[++c % no_of_colors]) / 10, (t.color[++c % no_of_colors]) / 10);
				glVertex3f(j, i, phonethickness / 2);
				glVertex3f(j + screenwidth / screenresolution[0], i, phonethickness / 2);
				glVertex3f(j, i + screenheight / screenresolution[1], phonethickness / 2);
				glColor3f((t.color[++c % no_of_colors]) / 10, (t.color[++c % no_of_colors]) / 10, (t.color[++c % no_of_colors]) / 10);
				glVertex3f(j + screenwidth / screenresolution[0], i, phonethickness / 2);
				glVertex3f(j, i + screenheight / screenresolution[1], phonethickness / 2);
				glVertex3f(j + screenwidth / screenresolution[0], i + screenheight / screenresolution[1], phonethickness / 2);
				screen_triangles += 2;
			}
			else if (clm >= psn.content[0].size() - 3) {
				glColor3f((t.color[++c % no_of_colors]) / 10, (t.color[++c % no_of_colors]) / 10, (t.color[++c % no_of_colors]) / 10);
				glVertex3f(j, i, phonethickness / 2);
				glVertex3f(j + screenwidth / screenresolution[0], i, phonethickness / 2);
				glVertex3f(j, i + screenheight / screenresolution[1], phonethickness / 2);
				glColor3f((t.color[++c % no_of_colors]) / 10, (t.color[++c % no_of_colors]) / 10, (t.color[++c % no_of_colors]) / 10);
				glVertex3f(j + screenwidth / screenresolution[0], i, phonethickness / 2);
				glVertex3f(j, i + screenheight / screenresolution[1], phonethickness / 2);
				glVertex3f(j + screenwidth / screenresolution[0], i + screenheight / screenresolution[1], phonethickness / 2);
				screen_triangles += 2;
			}
			else if (rw >= psn.content.size() - 1) {
				colors = false;
			}
			else {
				glColor3f(1.1*(float)stoi(psn.content[rw][clm]) / 255, (float)stoi(psn.content[rw][clm + 1]) / 255, (float)stoi(psn.content[rw][clm + 2]) / 255);
				glVertex3f(j, i, phonethickness / 2);
				glVertex3f(j + screenwidth / screenresolution[0], i, phonethickness / 2);
				glVertex3f(j, i + screenheight / screenresolution[1], phonethickness / 2);
				glColor3f(.9*(float)stoi(psn.content[rw][clm]) / 255, (float)stoi(psn.content[rw][clm + 1]) / 255, (float)stoi(psn.content[rw][clm + 2]) / 255);
				glVertex3f(j + screenwidth / screenresolution[0], i, phonethickness / 2);
				glVertex3f(j, i + screenheight / screenresolution[1], phonethickness / 2);
				glVertex3f(j + screenwidth / screenresolution[0], i + screenheight / screenresolution[1], phonethickness / 2);
				screen_triangles += 2;
			}
		}
	}
	c = 0;
	//cout << "number of screen triangles:" << screen_triangles << endl;

	//bezels
	setMaterial(0.2, 0.2, 0.2, 0.3, 0.3, 0.3, 0.5, 0.5, 0.5, 64);
	//left
	glColor3f((float)(7 + 3 * t.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * t.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * t.color[c++ % no_of_colors]) / 10);
	glVertex3f(-screenwidth / 2 - bezelwidth, screenheight / 2, phonethickness / 2);
	glVertex3f(-screenwidth / 2, screenheight / 2, phonethickness / 2);
	glVertex3f(-screenwidth / 2 - bezelwidth, -screenheight / 2, phonethickness / 2);
	glColor3f((float)(7 + 3 * t.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * t.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * t.color[c++ % no_of_colors]) / 10);
	glVertex3f(-screenwidth / 2, screenheight / 2, phonethickness / 2);
	glVertex3f(-screenwidth / 2 - bezelwidth, -screenheight / 2, phonethickness / 2);
	glVertex3f(-screenwidth / 2, -screenheight / 2, phonethickness / 2);
	//right
	glColor3f((float)(7 + 3 * t.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * t.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * t.color[c++ % no_of_colors]) / 10);
	glVertex3f(screenwidth / 2 + bezelwidth, screenheight / 2, phonethickness / 2);
	glVertex3f(screenwidth / 2, screenheight / 2, phonethickness / 2);
	glVertex3f(screenwidth / 2 + bezelwidth, -screenheight / 2, phonethickness / 2);
	glColor3f((float)(7 + 3 * t.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * t.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * t.color[c++ % no_of_colors]) / 10);
	glVertex3f(screenwidth / 2, screenheight / 2, phonethickness / 2);
	glVertex3f(screenwidth / 2 + bezelwidth, -screenheight / 2, phonethickness / 2);
	glVertex3f(screenwidth / 2, -screenheight / 2, phonethickness / 2);
	//top
	glColor3f((float)(7 + 3 * t.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * t.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * t.color[c++ % no_of_colors]) / 10);
	glVertex3f(-screenwidth / 2, screenheight / 2, phonethickness / 2);
	glVertex3f(-screenwidth / 2, screenheight / 2 + bezelwidth, phonethickness / 2);
	glVertex3f(screenwidth / 2, screenheight / 2, phonethickness / 2);
	glColor3f((float)(7 + 3 * t.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * t.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * t.color[c++ % no_of_colors]) / 10);
	glVertex3f(screenwidth / 2, screenheight / 2, phonethickness / 2);
	glVertex3f(screenwidth / 2, screenheight / 2 + bezelwidth, phonethickness / 2);
	glVertex3f(-screenwidth / 2, screenheight / 2 + bezelwidth, phonethickness / 2);
	//bottom
	glColor3f((float)(7 + 3 * t.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * t.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * t.color[c++ % no_of_colors]) / 10);
	glVertex3f(-screenwidth / 2, -screenheight / 2, phonethickness / 2);
	glVertex3f(-screenwidth / 2, -screenheight / 2 - bezelwidth, phonethickness / 2);
	glVertex3f(screenwidth / 2, -screenheight / 2, phonethickness / 2);
	glColor3f((float)(7 + 3 * t.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * t.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * t.color[c++ % no_of_colors]) / 10);
	glVertex3f(screenwidth / 2, -screenheight / 2, phonethickness / 2);
	glVertex3f(screenwidth / 2, -screenheight / 2 - bezelwidth, phonethickness / 2);
	glVertex3f(-screenwidth / 2, -screenheight / 2 - bezelwidth, phonethickness / 2);
	c = 0;
	//top left
	float theta, r = bezelwidth, incr = 1;
	for (theta = 0; theta < 90; theta += incr) {
		glColor3f((float)(7 + 3 * t.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * t.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * t.color[c++ % no_of_colors]) / 10);
		glVertex3f(-screenwidth / 2, screenheight / 2, phonethickness / 2);
		glVertex3f(-screenwidth / 2 - r * cos(theta * pi / 180), screenheight / 2 + r * sin(theta * pi / 180), phonethickness / 2);
		glVertex3f(-screenwidth / 2 - r * cos((theta + incr) * pi / 180), screenheight / 2 + r * sin((theta + incr) * pi / 180), phonethickness / 2);
	}
	//top right
	for (theta = 0; theta < 90; theta += incr) {
		glColor3f((float)(7 + 3 * t.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * t.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * t.color[c++ % no_of_colors]) / 10);
		glVertex3f(screenwidth / 2, screenheight / 2, phonethickness / 2);
		glVertex3f(screenwidth / 2 + r * cos(theta * pi / 180), screenheight / 2 + r * sin(theta * pi / 180), phonethickness / 2);
		glVertex3f(screenwidth / 2 + r * cos((theta + incr) * pi / 180), screenheight / 2 + r * sin((theta + incr) * pi / 180), phonethickness / 2);
	}
	//bottom left
	for (theta = 0; theta < 90; theta += incr) {
		glColor3f((float)(7 + 3 * t.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * t.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * t.color[c++ % no_of_colors]) / 10);
		glVertex3f(-screenwidth / 2, -screenheight / 2, phonethickness / 2);
		glVertex3f(-screenwidth / 2 - r * cos(theta * pi / 180), -screenheight / 2 - r * sin(theta * pi / 180), phonethickness / 2);
		glVertex3f(-screenwidth / 2 - r * cos((theta + incr) * pi / 180), -screenheight / 2 - r * sin((theta + incr) * pi / 180), phonethickness / 2);
	}
	//bottom right
	for (theta = 0; theta < 90; theta += incr) {
		glColor3f((float)(7 + 3 * t.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * t.color[c++ % no_of_colors]) / 10, (float)(7 + 3 * t.color[c++ % no_of_colors]) / 10);
		glVertex3f(screenwidth / 2, -screenheight / 2, phonethickness / 2);
		glVertex3f(screenwidth / 2 + r * cos(theta * pi / 180), -screenheight / 2 - r * sin(theta * pi / 180), phonethickness / 2);
		glVertex3f(screenwidth / 2 + r * cos((theta + incr) * pi / 180), -screenheight / 2 - r * sin((theta + incr) * pi / 180), phonethickness / 2);
	}
	c = 0;

	//hole-punch camera
	r = bezelwidth / 2;
	float posx = 0, posy = -bezelwidth + screenheight / 2;
	for (theta = 0; theta < 360; theta += incr) {
		glColor3f((float)(t.color[c++ % no_of_colors]) / 10, (float)(t.color[c++ % no_of_colors]) / 10, (float)(t.color[c++ % no_of_colors]) / 10);
		glVertex3f(posx, posy, phonethickness / 2 + .001);
		glVertex3f(posx + r * cos(theta * pi / 180), posy - r * sin(theta * pi / 180), phonethickness / 2 + .001);
		glVertex3f(posx + r * cos((theta + incr) * pi / 180), posy - r * sin((theta + incr) * pi / 180), phonethickness / 2 + .001);
	}
	c = 0;

	//sides
	setMaterial(0.2, 0.2, 0.2, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 128);
	//left
	for (float i = -screenheight / 2; i < screenheight / 2 - screenheight / screenresolution[1]; i += screenheight / screenresolution[1]) {
		glColor3f((float)(3 + 7 * t.color[++c % no_of_colors]) / 10, (float)(3 + 7 * t.color[c % no_of_colors]) / 10, (float)(3 + 7 * t.color[c % no_of_colors]) / 10);
		glVertex3f(-screenwidth / 2 - bezelwidth, i, phonethickness / 2);
		glVertex3f(-screenwidth / 2 - bezelwidth, i + screenheight / screenresolution[1], phonethickness / 2);
		glVertex3f(-screenwidth / 2 - bezelwidth, i, -phonethickness / 2);
		glColor3f((float)(3 + 7 * t.color[++c % no_of_colors]) / 10, (float)(3 + 7 * t.color[c % no_of_colors]) / 10, (float)(3 + 7 * t.color[c++ % no_of_colors]) / 10);
		glVertex3f(-screenwidth / 2 - bezelwidth, i, -phonethickness / 2);
		glVertex3f(-screenwidth / 2 - bezelwidth, i + screenheight / screenresolution[1], -phonethickness / 2);
		glVertex3f(-screenwidth / 2 - bezelwidth, i + screenheight / screenresolution[1], phonethickness / 2);
	}
	//right
	for (float i = -screenheight / 2; i < screenheight / 2 - screenheight / screenresolution[1]; i += screenheight / screenresolution[1]) {
		glColor3f((float)(3 + 7 * t.color[++c % no_of_colors]) / 10, (float)(3 + 7 * t.color[c % no_of_colors]) / 10, (float)(3 + 7 * t.color[c % no_of_colors]) / 10);
		glVertex3f(screenwidth / 2 + bezelwidth, i, phonethickness / 2);
		glVertex3f(screenwidth / 2 + bezelwidth, i + screenheight / screenresolution[1], phonethickness / 2);
		glVertex3f(screenwidth / 2 + bezelwidth, i, -phonethickness / 2);
		glColor3f((float)(3 + 7 * t.color[++c % no_of_colors]) / 10, (float)(3 + 7 * t.color[c % no_of_colors]) / 10, (float)(3 + 7 * t.color[c++ % no_of_colors]) / 10);
		glVertex3f(screenwidth / 2 + bezelwidth, i, -phonethickness / 2);
		glVertex3f(screenwidth / 2 + bezelwidth, i + screenheight / screenresolution[1], -phonethickness / 2);
		glVertex3f(screenwidth / 2 + bezelwidth, i + screenheight / screenresolution[1], phonethickness / 2);
	}
	c = 0;
	//top
	for (float i = -screenwidth / 2; i < screenwidth / 2 - screenwidth / screenresolution[0]; i += screenwidth / screenresolution[0]) {
		glColor3f((float)(3 + 7 * t.color[++c % no_of_colors]) / 10, (float)(3 + 7 * t.color[c % no_of_colors]) / 10, (float)(3 + 7 * t.color[c % no_of_colors]) / 10);
		glVertex3f(i, screenheight / 2 + bezelwidth, phonethickness / 2);
		glVertex3f(i + screenwidth / screenresolution[0], screenheight / 2 + bezelwidth, phonethickness / 2);
		glVertex3f(i, screenheight / 2 + bezelwidth, -phonethickness / 2);
		glColor3f((float)(3 + 7 * t.color[++c % no_of_colors]) / 10, (float)(3 + 7 * t.color[c % no_of_colors]) / 10, (float)(3 + 7 * t.color[c++ % no_of_colors]) / 10);
		glVertex3f(i, screenheight / 2 + bezelwidth, -phonethickness / 2);
		glVertex3f(i + screenwidth / screenresolution[0], screenheight / 2 + bezelwidth, phonethickness / 2);
		glVertex3f(i + screenwidth / screenresolution[0], screenheight / 2 + bezelwidth, -phonethickness / 2);
	}
	//bottom
	for (float i = -screenwidth / 2; i < screenwidth / 2 - screenwidth / screenresolution[0]; i += screenwidth / screenresolution[0]) {
		glColor3f((float)(3 + 7 * t.color[++c % no_of_colors]) / 10, (float)(3 + 7 * t.color[c % no_of_colors]) / 10, (float)(3 + 7 * t.color[c % no_of_colors]) / 10);
		glVertex3f(i, -screenheight / 2 - bezelwidth, phonethickness / 2);
		glVertex3f(i + screenwidth / screenresolution[0], -screenheight / 2 - bezelwidth, phonethickness / 2);
		glVertex3f(i, -screenheight / 2 - bezelwidth, -phonethickness / 2);
		glColor3f((float)(3 + 7 * t.color[++c % no_of_colors]) / 10, (float)(3 + 7 * t.color[c % no_of_colors]) / 10, (float)(3 + 7 * t.color[c++ % no_of_colors]) / 10);
		glVertex3f(i, -screenheight / 2 - bezelwidth, -phonethickness / 2);
		glVertex3f(i + screenwidth / screenresolution[0], -screenheight / 2 - bezelwidth, phonethickness / 2);
		glVertex3f(i + screenwidth / screenresolution[0], -screenheight / 2 - bezelwidth, -phonethickness / 2);
	}
	c = 0;
	//top left
	r = bezelwidth; incr = 1;
	for (theta = 0; theta < 90; theta += incr) {
		glColor3f((float)(3 + 7 * t.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * t.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * t.color[c++ % no_of_colors]) / 10);
		glVertex3f(-screenwidth / 2 - r * cos(theta * pi / 180), screenheight / 2 + r * sin(theta * pi / 180), phonethickness / 2);
		glVertex3f(-screenwidth / 2 - r * cos(theta * pi / 180), screenheight / 2 + r * sin(theta * pi / 180), -phonethickness / 2);
		glVertex3f(-screenwidth / 2 - r * cos((theta + incr) * pi / 180), screenheight / 2 + r * sin((theta + incr) * pi / 180), phonethickness / 2);
		glVertex3f(-screenwidth / 2 - r * cos((theta + incr) * pi / 180), screenheight / 2 + r * sin((theta + incr) * pi / 180), -phonethickness / 2);
		glVertex3f(-screenwidth / 2 - r * cos((theta + incr) * pi / 180), screenheight / 2 + r * sin(theta * pi / 180), -phonethickness / 2);
		glVertex3f(-screenwidth / 2 - r * cos(theta * pi / 180), screenheight / 2 + r * sin(theta * pi / 180), phonethickness / 2);
	}
	//top right
	r = bezelwidth; incr = 1;
	for (theta = 0; theta < 90; theta += incr) {
		glColor3f((float)(3 + 7 * t.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * t.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * t.color[c++ % no_of_colors]) / 10);
		glVertex3f(screenwidth / 2 + r * cos(theta * pi / 180), screenheight / 2 + r * sin(theta * pi / 180), phonethickness / 2);
		glVertex3f(screenwidth / 2 + r * cos(theta * pi / 180), screenheight / 2 + r * sin(theta * pi / 180), -phonethickness / 2);
		glVertex3f(screenwidth / 2 + r * cos((theta + incr) * pi / 180), screenheight / 2 + r * sin((theta + incr) * pi / 180), phonethickness / 2);
		glVertex3f(screenwidth / 2 + r * cos((theta + incr) * pi / 180), screenheight / 2 + r * sin((theta + incr) * pi / 180), -phonethickness / 2);
		glVertex3f(screenwidth / 2 + r * cos((theta + incr) * pi / 180), screenheight / 2 + r * sin(theta * pi / 180), -phonethickness / 2);
		glVertex3f(screenwidth / 2 + r * cos(theta * pi / 180), screenheight / 2 + r * sin(theta * pi / 180), phonethickness / 2);
	}
	//bottom left
	r = bezelwidth; incr = 1;
	for (theta = 0; theta < 90; theta += incr) {
		glColor3f((float)(3 + 7 * t.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * t.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * t.color[c++ % no_of_colors]) / 10);
		glVertex3f(-screenwidth / 2 - r * cos(theta * pi / 180), -(screenheight / 2 + r * sin(theta * pi / 180)), phonethickness / 2);
		glVertex3f(-screenwidth / 2 - r * cos(theta * pi / 180), -(screenheight / 2 + r * sin(theta * pi / 180)), -phonethickness / 2);
		glVertex3f(-screenwidth / 2 - r * cos((theta + incr) * pi / 180), -(screenheight / 2 + r * sin((theta + incr) * pi / 180)), phonethickness / 2);
		glVertex3f(-screenwidth / 2 - r * cos((theta + incr) * pi / 180), -(screenheight / 2 + r * sin((theta + incr) * pi / 180)), -phonethickness / 2);
		glVertex3f(-screenwidth / 2 - r * cos((theta + incr) * pi / 180), -(screenheight / 2 + r * sin(theta * pi / 180)), -phonethickness / 2);
		glVertex3f(-screenwidth / 2 - r * cos(theta * pi / 180), -(screenheight / 2 + r * sin(theta * pi / 180)), phonethickness / 2);
	}
	//bottom right
	r = bezelwidth; incr = 1;
	for (theta = 0; theta < 90; theta += incr) {
		glColor3f((float)(3 + 7 * t.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * t.color[c++ % no_of_colors]) / 10, (float)(3 + 7 * t.color[c++ % no_of_colors]) / 10);
		glVertex3f(screenwidth / 2 + r * cos(theta * pi / 180), -(screenheight / 2 + r * sin(theta * pi / 180)), phonethickness / 2);
		glVertex3f(screenwidth / 2 + r * cos(theta * pi / 180), -(screenheight / 2 + r * sin(theta * pi / 180)), -phonethickness / 2);
		glVertex3f(screenwidth / 2 + r * cos((theta + incr) * pi / 180), -(screenheight / 2 + r * sin((theta + incr) * pi / 180)), phonethickness / 2);
		glVertex3f(screenwidth / 2 + r * cos((theta + incr) * pi / 180), -(screenheight / 2 + r * sin((theta + incr) * pi / 180)), -phonethickness / 2);
		glVertex3f(screenwidth / 2 + r * cos((theta + incr) * pi / 180), -(screenheight / 2 + r * sin(theta * pi / 180)), -phonethickness / 2);
		glVertex3f(screenwidth / 2 + r * cos(theta * pi / 180), -(screenheight / 2 + r * sin(theta * pi / 180)), phonethickness / 2);
	}
	c = 0;

	//back
	//bezels
	setMaterial(0.2, 0.2, 0.2, 0.3, 0.3, 0.3, 0.5, 0.5, 0.5, 32);
	//left
	glColor3f((float)(3 * t.color[c++ % no_of_colors]) / 10, (float)(3 * t.color[c++ % no_of_colors]) / 10, (float)(3 * t.color[c++ % no_of_colors]) / 10);
	glVertex3f(-screenwidth / 2 - bezelwidth, screenheight / 2, -phonethickness / 2);
	glVertex3f(-screenwidth / 2, screenheight / 2, -phonethickness / 2);
	glVertex3f(-screenwidth / 2 - bezelwidth, -screenheight / 2, -phonethickness / 2);
	glColor3f((float)(3 * t.color[c++ % no_of_colors]) / 10, (float)(3 * t.color[c++ % no_of_colors]) / 10, (float)(3 * t.color[c++ % no_of_colors]) / 10);
	glVertex3f(-screenwidth / 2, screenheight / 2, -phonethickness / 2);
	glVertex3f(-screenwidth / 2 - bezelwidth, -screenheight / 2, -phonethickness / 2);
	glVertex3f(-screenwidth / 2, -screenheight / 2, -phonethickness / 2);
	//right
	glColor3f((float)(3 * t.color[c++ % no_of_colors]) / 10, (float)(3 * t.color[c++ % no_of_colors]) / 10, (float)(3 * t.color[c++ % no_of_colors]) / 10);
	glVertex3f(screenwidth / 2 + bezelwidth, screenheight / 2, -phonethickness / 2);
	glVertex3f(screenwidth / 2, screenheight / 2, -phonethickness / 2);
	glVertex3f(screenwidth / 2 + bezelwidth, -screenheight / 2, -phonethickness / 2);
	glColor3f((float)(3 * t.color[c++ % no_of_colors]) / 10, (float)(3 * t.color[c++ % no_of_colors]) / 10, (float)(3 * t.color[c++ % no_of_colors]) / 10);
	glVertex3f(screenwidth / 2, screenheight / 2, -phonethickness / 2);
	glVertex3f(screenwidth / 2 + bezelwidth, -screenheight / 2, -phonethickness / 2);
	glVertex3f(screenwidth / 2, -screenheight / 2, -phonethickness / 2);
	//top
	glColor3f((float)(3 * t.color[c++ % no_of_colors]) / 10, (float)(3 * t.color[c++ % no_of_colors]) / 10, (float)(3 * t.color[c++ % no_of_colors]) / 10);
	glVertex3f(-screenwidth / 2, screenheight / 2, -phonethickness / 2);
	glVertex3f(-screenwidth / 2, screenheight / 2 + bezelwidth, -phonethickness / 2);
	glVertex3f(screenwidth / 2, screenheight / 2, -phonethickness / 2);
	glColor3f((float)(3 * t.color[c++ % no_of_colors]) / 10, (float)(3 * t.color[c++ % no_of_colors]) / 10, (float)(3 * t.color[c++ % no_of_colors]) / 10);
	glVertex3f(screenwidth / 2, screenheight / 2, -phonethickness / 2);
	glVertex3f(screenwidth / 2, screenheight / 2 + bezelwidth, -phonethickness / 2);
	glVertex3f(-screenwidth / 2, screenheight / 2 + bezelwidth, -phonethickness / 2);
	//bottom
	glColor3f((float)(3 * t.color[c++ % no_of_colors]) / 10, (float)(3 * t.color[c++ % no_of_colors]) / 10, (float)(3 * t.color[c++ % no_of_colors]) / 10);
	glVertex3f(-screenwidth / 2, -screenheight / 2, -phonethickness / 2);
	glVertex3f(-screenwidth / 2, -screenheight / 2 - bezelwidth, -phonethickness / 2);
	glVertex3f(screenwidth / 2, -screenheight / 2, -phonethickness / 2);
	glColor3f((float)(3 * t.color[c++ % no_of_colors]) / 10, (float)(3 * t.color[c++ % no_of_colors]) / 10, (float)(3 * t.color[c++ % no_of_colors]) / 10);
	glVertex3f(screenwidth / 2, -screenheight / 2, -phonethickness / 2);
	glVertex3f(screenwidth / 2, -screenheight / 2 - bezelwidth, -phonethickness / 2);
	glVertex3f(-screenwidth / 2, -screenheight / 2 - bezelwidth, -phonethickness / 2);
	c = 0;
	//top left
	r = bezelwidth; incr = 1;
	for (theta = 0; theta < 90; theta += incr) {
		glColor3f((float)(3 * t.color[c++ % no_of_colors]) / 10, (float)(3 * t.color[c++ % no_of_colors]) / 10, (float)(3 * t.color[c++ % no_of_colors]) / 10);
		glVertex3f(-screenwidth / 2, screenheight / 2, -phonethickness / 2);
		glVertex3f(-screenwidth / 2 - r * cos(theta * pi / 180), screenheight / 2 + r * sin(theta * pi / 180), -phonethickness / 2);
		glVertex3f(-screenwidth / 2 - r * cos((theta + incr) * pi / 180), screenheight / 2 + r * sin((theta + incr) * pi / 180), -phonethickness / 2);
	}
	//top right
	for (theta = 0; theta < 90; theta += incr) {
		glColor3f((float)(3 * t.color[c++ % no_of_colors]) / 10, (float)(3 * t.color[c++ % no_of_colors]) / 10, (float)(3 * t.color[c++ % no_of_colors]) / 10);
		glVertex3f(screenwidth / 2, screenheight / 2, -phonethickness / 2);
		glVertex3f(screenwidth / 2 + r * cos(theta * pi / 180), screenheight / 2 + r * sin(theta * pi / 180), -phonethickness / 2);
		glVertex3f(screenwidth / 2 + r * cos((theta + incr) * pi / 180), screenheight / 2 + r * sin((theta + incr) * pi / 180), -phonethickness / 2);
	}
	//bottom left
	for (theta = 0; theta < 90; theta += incr) {
		glColor3f((float)(3 * t.color[c++ % no_of_colors]) / 10, (float)(3 * t.color[c++ % no_of_colors]) / 10, (float)(3 * t.color[c++ % no_of_colors]) / 10);
		glVertex3f(-screenwidth / 2, -screenheight / 2, -phonethickness / 2);
		glVertex3f(-screenwidth / 2 - r * cos(theta * pi / 180), -screenheight / 2 - r * sin(theta * pi / 180), -phonethickness / 2);
		glVertex3f(-screenwidth / 2 - r * cos((theta + incr) * pi / 180), -screenheight / 2 - r * sin((theta + incr) * pi / 180), -phonethickness / 2);
	}
	//bottom right
	for (theta = 0; theta < 90; theta += incr) {
		glColor3f((float)(3 * t.color[c++ % no_of_colors]) / 10, (float)(3 * t.color[c++ % no_of_colors]) / 10, (float)(3 * t.color[c++ % no_of_colors]) / 10);
		glVertex3f(screenwidth / 2, -screenheight / 2, -phonethickness / 2);
		glVertex3f(screenwidth / 2 + r * cos(theta * pi / 180), -screenheight / 2 - r * sin(theta * pi / 180), -phonethickness / 2);
		glVertex3f(screenwidth / 2 + r * cos((theta + incr) * pi / 180), -screenheight / 2 - r * sin((theta + incr) * pi / 180), -phonethickness / 2);
	}
	c = 0;
	setMaterial(0.2, 0.2, 0.2, 0.3, 0.3, 0.3, 0.7, 0.7, 0.7, 128);
	for (float i = -screenheight / 2; i < screenheight / 2 - screenheight / screenresolution[1]; i += 10 * screenheight / screenresolution[1]) {
		for (float j = -screenwidth / 2; j < screenwidth / 2 - screenwidth / screenresolution[0]; j += 10 * screenwidth / screenresolution[0]) {
			glColor3f((7 + t.color[++c % no_of_colors]) / 10, (7 + t.color[++c % no_of_colors]) / 10, (7 + t.color[++c % no_of_colors]) / 10);
			glVertex3f(j, i, -phonethickness / 2);
			glVertex3f(j + 10 * screenwidth / screenresolution[0], i, -phonethickness / 2);
			glVertex3f(j, i + 10 * screenheight / screenresolution[1], -phonethickness / 2);
			glColor3f((7 + t.color[++c % no_of_colors]) / 10, (7 + t.color[++c % no_of_colors]) / 10, (7 + t.color[++c % no_of_colors]) / 10);
			glVertex3f(j + 10 * screenwidth / screenresolution[0], i, -phonethickness / 2);
			glVertex3f(j, i + 10 * screenheight / screenresolution[1], -phonethickness / 2);
			glVertex3f(j + 10 * screenwidth / screenresolution[0], i + 10 * screenheight / screenresolution[1], -phonethickness / 2);
		}
	}
	c = 0;

	//back camera lenses
	//lens 1
	r = bezelwidth * 2;
	posx = screenwidth / 2 - bezelwidth * 3; posy = -3 * bezelwidth + screenheight / 2;
	for (theta = 0; theta < 360; theta += incr) {
		glColor3f((float)(t.color[c++ % no_of_colors]) / 10, (float)(t.color[c++ % no_of_colors]) / 10, (float)(t.color[c++ % no_of_colors]) / 10);
		glVertex3f(posx, posy, -phonethickness / 2 - .001);
		glVertex3f(posx + r * cos(theta * pi / 180), posy - r * sin(theta * pi / 180), -phonethickness / 2 - .001);
		glVertex3f(posx + r * cos((theta + incr) * pi / 180), posy - r * sin((theta + incr) * pi / 180), -phonethickness / 2 - .001);
	}
	r = bezelwidth / 2;
	posx = screenwidth / 2 - bezelwidth * 3; posy = -3 * bezelwidth + screenheight / 2;
	for (theta = 0; theta < 360; theta += incr) {
		glColor3f((float)(8 + t.color[c++ % no_of_colors]) / 10, (float)(8 + t.color[c++ % no_of_colors]) / 10, (float)(8 + t.color[c++ % no_of_colors]) / 10);
		glVertex3f(posx, posy, -phonethickness / 2 - .002);
		glVertex3f(posx + r * cos(theta * pi / 180), posy - r * sin(theta * pi / 180), -phonethickness / 2 - .002);
		glVertex3f(posx + r * cos((theta + incr) * pi / 180), posy - r * sin((theta + incr) * pi / 180), -phonethickness / 2 - .002);
	}
	//lens 2
	r = bezelwidth * 2;
	posx = screenwidth / 2 - bezelwidth * 3; posy = -8 * bezelwidth + screenheight / 2;
	for (theta = 0; theta < 360; theta += incr) {
		glColor3f((float)(t.color[c++ % no_of_colors]) / 10, (float)(t.color[c++ % no_of_colors]) / 10, (float)(t.color[c++ % no_of_colors]) / 10);
		glVertex3f(posx, posy, -phonethickness / 2 - .001);
		glVertex3f(posx + r * cos(theta * pi / 180), posy - r * sin(theta * pi / 180), -phonethickness / 2 - .001);
		glVertex3f(posx + r * cos((theta + incr) * pi / 180), posy - r * sin((theta + incr) * pi / 180), -phonethickness / 2 - .001);
	}
	r = bezelwidth / 2;
	posx = screenwidth / 2 - bezelwidth * 3; posy = -8 * bezelwidth + screenheight / 2;
	for (theta = 0; theta < 360; theta += incr) {
		glColor3f((float)(8 + t.color[c++ % no_of_colors]) / 10, (float)(8 + t.color[c++ % no_of_colors]) / 10, (float)(8 + t.color[c++ % no_of_colors]) / 10);
		glVertex3f(posx, posy, -phonethickness / 2 - .002);
		glVertex3f(posx + r * cos(theta * pi / 180), posy - r * sin(theta * pi / 180), -phonethickness / 2 - .002);
		glVertex3f(posx + r * cos((theta + incr) * pi / 180), posy - r * sin((theta + incr) * pi / 180), -phonethickness / 2 - .002);
	}
	c = 0;

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

	glStencilFunc(GL_ALWAYS, 2, -1);
	drawPhone(phone);

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
	glutCreateWindow("Smartphone");

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