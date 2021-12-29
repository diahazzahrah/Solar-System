#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>
#include "imageloader.h"
#pragma comment(lib, "glew32.lib")

using namespace std;

class Planet {
public:
	// Mendeklarasikan 
	float radius, distance, orbit, orbitSpeed, axisTilt, axisAni;
	Planet(float _radius, float _distance, float _orbit, float _orbitSpeed, float _axisTilt, float _axisAni) {
		radius = _radius;
		distance = _distance;
		orbit = _orbit;
		orbitSpeed = _orbitSpeed;
		axisTilt = _axisTilt;
		axisAni = _axisAni;
	}
// Small Orbit
	void drawSmallOrbit(void) {
		glPushMatrix();
		glColor3ub(255, 255, 255);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glutWireTorus(0.001, distance, 100.0, 100.0);
		glPopMatrix();
	}

// Satelit 
	void drawMoon(void) {
		GLUquadricObj* quadric;
		quadric = gluNewQuadric();
		glPushMatrix();
		glColor3ub(255, 255, 255);
		glRotatef(orbit, 0.0, 1.0, 0.0);
		glTranslatef(distance, 0.0, 0.0);
		gluSphere(quadric, radius, 20.0, 20.0);
		glPopMatrix();
	}

};

// Matahari, Planets dan Bintang
Planet sun(5.0, 0, 0, 0, 0, 0);				// Matahari
Planet mer(1.0, 7, 0, 4.74, 02.11, 0);		// Merkurius
Planet ven(1.5, 11, 0, 3.50, 177.0, 0);		// Venus
Planet ear(2.0, 16, 0, 2.98, 23.44, 0);		// Bumi
Planet mar(1.2, 21, 0, 2.41, 25.00, 0);		// Mars
Planet jup(3.5, 28, 0, 1.31, 03.13, 0);		// Jupiter
Planet sat(3.0, 37, 0, 0.97, 26.70, 0);		// Saturnus
Planet ura(2.5, 45.5, 0, 0.68, 97.77, 0);	// Uranus
Planet nep(2.3, 53.6, 0, 0.54, 28.32, 0);	// Neptunus
Planet plu(0.3, 59, 0, 0.47, 119.6, 0);		// Pluto
Planet lun(.40, 3, 0, 5.40, 0, 0);			// Luna				(Bumi)
Planet pho(.20, 1.8, 0, 2.30, 0, 0);		// Phobos			(Mars)
Planet dei(.24, 2.4, 0, 3.60, 0, 0);		// Deimos			(Mars)
Planet eur(.24, 4, 0, 4.40, 0, 0);			// Europa			(Jupiter)
Planet gan(.24, 4.7, 0, 5.00, 0, 0);		// Ganymede			(Jupiter)
Planet cal(.24, 5.3, 0, 2.30, 0, 0);		// Callisto			(Jupiter)
Planet tit(.75, 3.7, 0, 2.40, 0, 0);		// Titan			(Saturnus)
Planet nix(.10, 1.5, 0, 5.00, 0, 0);		// Nix				(Pluto)
Planet puc(.26, 2.9, 0, 7.00, 0, 0);		// Puck				(Uranus)
Planet tri(.36, 3.2, 0, 3.40, 0, 0);		// Triton			(Neptunus)

int isAnimate = 0;
int bigOrbitActive = 1;
int smallOrbitActive = 1;
int moonsActive = 1;
int changeCamera = 0;
int frameCount = 0;
int labelsActive = 0;
int zoom = 50;
int logoScene = 1;


float lightPos[] = { 0.0, 0.0, -75.0, 1.0 }; // Spotlight position.
static float spotAngle = 40; // Spotlight cone half-angle.
float spotDirection[] = { 1.0, 0.0, 0.0 }; // Spotlight direction.
static float spotExponent = 1.0; // Spotlight exponent = attenuation factor.

// Load Image Planets
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	return textureId;
}

GLuint sunTexture, merTexture, venTexture, earTexture, marTexture, jupTexture, satTexture, uraTexture, nepTexture, pluTexture, staTexture, logTexture;

// Label / Tulisan keterangan 
void writeBitmapString(void* font, const char* string)
{
	const char* c;
	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

void setup(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	// TEXTURING SETUP
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	Image* sta = loadBMP("outer.bmp");		staTexture = loadTexture(sta);		delete sta;
	Image* sun = loadBMP("sun.bmp");		sunTexture = loadTexture(sun);		delete sun;
	Image* mer = loadBMP("mercury.bmp");	merTexture = loadTexture(mer);		delete mer;
	Image* ven = loadBMP("venus.bmp");		venTexture = loadTexture(ven);		delete ven;
	Image* ear = loadBMP("earth.bmp");		earTexture = loadTexture(ear);		delete ear;
	Image* mar = loadBMP("mars.bmp");		marTexture = loadTexture(mar);		delete mar;
	Image* jup = loadBMP("jupiter.bmp");	jupTexture = loadTexture(jup);		delete jup;
	Image* sat = loadBMP("saturn.bmp");		satTexture = loadTexture(sat);		delete sat;
	Image* ura = loadBMP("uranus.bmp");		uraTexture = loadTexture(ura);		delete ura;
	Image* nep = loadBMP("neptune.bmp");	nepTexture = loadTexture(nep);		delete nep;
	Image* plu = loadBMP("pluto.bmp");		pluTexture = loadTexture(plu);		delete plu;
	Image* log = loadBMP("depan.bmp");		logTexture = loadTexture(log);		delete log;

	// LIGHTING SETUP
	glEnable(GL_LIGHTING);
	float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
	float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float globAmb[] = { 0.5, 0.5, 0.5, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
	glEnable(GL_LIGHT0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spotAngle);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirection);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spotExponent);
}

// Orbital Trails (Jalur Orbital) 
void orbitalTrails(void) {
	glPushMatrix();
	glColor3ub(255, 255, 255);
	glTranslatef(0.0, 0.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glutWireTorus(0.001, mer.distance, 100.0, 100.0);
	glutWireTorus(0.001, ven.distance, 100.0, 100.0);
	glutWireTorus(0.001, ear.distance, 100.0, 100.0);
	glutWireTorus(0.001, mar.distance, 100.0, 100.0);
	glutWireTorus(0.001, jup.distance, 100.0, 100.0);
	glutWireTorus(0.001, sat.distance, 100.0, 100.0);
	glutWireTorus(0.001, ura.distance, 100.0, 100.0);
	glutWireTorus(0.001, nep.distance, 100.0, 100.0);
	glutWireTorus(0.001, plu.distance, 100.0, 100.0);
	glPopMatrix();
}

// Tampilan Awal (Cover) 
void drawLogoScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, logTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-100, -100, -100);
	glTexCoord2f(1.0, 0.0); glVertex3f(100, -100, -100);
	glTexCoord2f(1.0, 1.0); glVertex3f(100, 100, -100);
	glTexCoord2f(0.0, 1.0); glVertex3f(-100, 100, -100);
	glEnd();

	glutSwapBuffers();
}

// Scene 
void drawScene(void) {
	frameCount++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	// Posisi Kamera 
	if (changeCamera == 0)gluLookAt(0.0, zoom, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	if (changeCamera == 1)gluLookAt(0.0, 0.0, zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	if (changeCamera == 2)gluLookAt(0.0, zoom, 0.00001, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	if (changeCamera == 3)gluLookAt(0.0, 0.0, zoom, 0.0, 0.0, 0.0, 1.1, 1.0, 1.0);
	if (changeCamera == 4)gluLookAt(10.0, 2.1, 5.5, 1.0, 4.0, 8.0, 2.5, 0.5, zoom);
	if (changeCamera == 5)gluLookAt(2.0, 5.0, 2.0, 6.0, 2.0, 1.0, zoom, 3.0, 2.0);

	if (bigOrbitActive == 1) orbitalTrails();

	GLUquadric* quadric;
	quadric = gluNewQuadric();

	// Matahari
	glPushMatrix();
	glRotatef(sun.orbit, 0.0, 1.0, 0.0);
	glTranslatef(sun.distance, 0.0, 0.0);
	if (labelsActive == 1) {
		glRasterPos3f(-1.2, 7.0, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Matahari");
	}
	glPushMatrix();
	glRotatef(sun.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(sun.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sunTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, sun.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	// Merkurius
	glPushMatrix();
	glRotatef(mer.orbit, 0.0, 1.0, 0.0);
	glTranslatef(mer.distance, 0.0, 0.0);
	if (labelsActive == 1) {
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Merkurius");
	}
	glPushMatrix();
	glRotatef(mer.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(mer.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, merTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, mer.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	// Venus
	glPushMatrix();
	glRotatef(ven.orbit, 0.0, 1.0, 0.0);
	glTranslatef(ven.distance, 0.0, 0.0);
	if (labelsActive == 1) {
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Venus");
	}
	glPushMatrix();
	glRotatef(ven.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(ven.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, venTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, ven.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	// Bumi, Orbit, Satelit
	glPushMatrix();
	glRotatef(ear.orbit, 0.0, 1.0, 0.0);
	glTranslatef(ear.distance, 0.0, 0.0);
	if (labelsActive == 1) {
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Bumi");
	}
	glPushMatrix();
	glRotatef(ear.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(ear.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, earTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, ear.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (smallOrbitActive == 1) {
		lun.drawSmallOrbit();
	}
	if (moonsActive == 1) {
		lun.drawMoon();
	}
	glPopMatrix();

	// Mars, Orbits, Satelit 
	glPushMatrix();
	glRotatef(mar.orbit, 0.0, 1.0, 0.0);
	glTranslatef(mar.distance, 0.0, 0.0);
	if (labelsActive == 1) {
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Mars");
	}
	glPushMatrix();
	glRotatef(mar.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(mar.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, marTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, mar.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (smallOrbitActive == 1) {
		pho.drawSmallOrbit();
		dei.drawSmallOrbit();
	}
	if (moonsActive == 1) {
		pho.drawMoon();
		dei.drawMoon();
	}
	glPopMatrix();

	// Jupiter, Orbits, Satelit 
	glPushMatrix();
	glRotatef(jup.orbit, 0.0, 1.0, 0.0);
	glTranslatef(jup.distance, 0.0, 0.0);
	if (labelsActive == 1) {
		glRasterPos3f(0.0, 4.4, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12,"Jupiter");
	}
	glPushMatrix();
	glRotatef(jup.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(jup.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, jupTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, jup.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (smallOrbitActive == 1) {
		eur.drawSmallOrbit();
		gan.drawSmallOrbit();
		cal.drawSmallOrbit();
	}
	if (moonsActive == 1) {
		eur.drawMoon();
		gan.drawMoon();
		cal.drawMoon();
	}
	glPopMatrix();

	// Saturnus, Orbit, Satelit 
	glPushMatrix();
	glRotatef(sat.orbit, 0.0, 1.0, 0.0);
	glTranslatef(sat.distance, 0.0, 0.0);
	if (labelsActive == 1) {
		glRasterPos3f(0.0, 4.4, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Saturnus");
	}
	glPushMatrix();
	glRotatef(sat.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(sat.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, satTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, sat.radius, 20.0, 20.0);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glColor3ub(158, 145, 137);
	glRotatef(-63.0, 1.0, 0.0, 0.0);
	glutWireTorus(0.2, 6.0, 30.0, 30.0);
	glutWireTorus(0.4, 5.0, 30.0, 30.0);
	glPopMatrix();
	if (smallOrbitActive == 1) {
		tit.drawSmallOrbit();
	}
	if (moonsActive == 1) {
		tit.drawMoon();
	}
	glPopMatrix();

	glColor3ub(255, 255, 255);		//FIXES SHADING ISSUE

	// Uranus, Orbit, Satelit 
	glPushMatrix();
	glRotatef(ura.orbit, 0.0, 1.0, 0.0);
	glTranslatef(ura.distance, 0.0, 0.0);
	if (labelsActive == 1) {
		glRasterPos3f(0.0, 4.4, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Uranus");
	}
	glPushMatrix();
	glRotatef(ura.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(ura.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, uraTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, ura.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (smallOrbitActive == 1) {
		puc.drawSmallOrbit();
	}
	if (moonsActive == 1) {
		puc.drawMoon();
	}
	glPopMatrix();

	// Neptunus, Orbit, Satelit 
	glPushMatrix();
	glRotatef(nep.orbit, 0.0, 1.0, 0.0);
	glTranslatef(nep.distance, 0.0, 0.0);
	if (labelsActive == 1) {
		glRasterPos3f(0.0, 4.4, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Neptunus");
	}
	glPushMatrix();
	glRotatef(nep.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(nep.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, nepTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, nep.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (smallOrbitActive == 1) {
		tri.drawSmallOrbit();
	}
	if (moonsActive == 1) {
		tri.drawMoon();
	}
	glPopMatrix();

	// Pluto, Orbit, Satelit 
	glPushMatrix();
	glRotatef(plu.orbit, 0.0, 1.0, 0.0);
	glTranslatef(plu.distance, 0.0, 0.0);
	if (labelsActive == 1) {
		glRasterPos3f(0.0, 3.0, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Pluto");
	}
	glPushMatrix();
	glRotatef(plu.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(plu.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, pluTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, plu.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (smallOrbitActive == 1) {
		nix.drawSmallOrbit();
	}
	if (moonsActive == 1) {
		nix.drawMoon();
	}
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, staTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBegin(GL_POLYGON);
	glTexCoord2f(-1.0, 0.0); glVertex3f(-200, -200, -100);
	glTexCoord2f(2.0, 0.0); glVertex3f(200, -200, -100);
	glTexCoord2f(2.0, 2.0); glVertex3f(200, 200, -100);
	glTexCoord2f(-1.0, 2.0); glVertex3f(-200, 200, -100);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, staTexture);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-200, -83, 200);
	glTexCoord2f(8.0, 0.0); glVertex3f(200, -83, 200);
	glTexCoord2f(8.0, 8.0); glVertex3f(200, -83, -200);
	glTexCoord2f(0.0, 8.0); glVertex3f(-200, -83, -200);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glutSwapBuffers();
}

// Pemanggilan fungsi drawScene 
void drawScenesInOrder(void) {
	if (logoScene == 1) {
		drawLogoScene();
	}
	else {
		drawScene();
	}
}

void resize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
}

// Kecepatan Gerak 
void animate(int n) {
	if (isAnimate) {
		mer.orbit += mer.orbitSpeed;
		ven.orbit += ven.orbitSpeed;
		ear.orbit += ear.orbitSpeed;
		mar.orbit += mar.orbitSpeed;
		jup.orbit += jup.orbitSpeed;
		sat.orbit += sat.orbitSpeed;
		ura.orbit += ura.orbitSpeed;
		nep.orbit += nep.orbitSpeed;
		plu.orbit += plu.orbitSpeed;
		lun.orbit += lun.orbitSpeed;
		pho.orbit += pho.orbitSpeed;
		dei.orbit += dei.orbitSpeed;
		eur.orbit += eur.orbitSpeed;
		gan.orbit += gan.orbitSpeed;
		cal.orbit += cal.orbitSpeed;
		tit.orbit += tit.orbitSpeed;
		nix.orbit += nix.orbitSpeed;
		puc.orbit += puc.orbitSpeed;
		tri.orbit += tri.orbitSpeed;
		if (mer, ven, ear, mar, jup, sat, ura, nep, plu, lun, pho, dei, eur, gan, cal, tit, nix, puc, tri.orbit > 360.0) {
			mer, ven, ear, mar, jup, sat, ura, nep, plu, lun, pho, dei, eur, gan, cal, tit, nix, puc, tri.orbit -= 360.0;
		}
		mer.axisAni += 10.0;
		ven.axisAni += 10.0;
		ear.axisAni += 10.0;
		mar.axisAni += 10.0;
		jup.axisAni += 10.0;
		sat.axisAni += 10.0;
		ura.axisAni += 10.0;
		nep.axisAni += 10.0;
		plu.axisAni += 10.0;
		if (mer, ven, ear, mar, jup, sat, ura, nep, plu.axisAni > 360.0) {
			mer, ven, ear, mar, jup, sat, ura, nep, plu.axisAni -= 360.0;
		}
		glutPostRedisplay();
		glutTimerFunc(30, animate, 1);
	}
}

// Fungsi mouse (klik kanan / kiri) 
void mouseControl(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		if (logoScene) logoScene = 0;

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) exit(0);
	glutPostRedisplay();
}

// Scroll Mouse
void mouseWheel(int wheel, int direction, int x, int y)
{
	if (direction > 0 && zoom < 100) zoom++;
	if (direction < 0 && zoom > -75) zoom--;
	glutPostRedisplay();
}

// Keyboard 
void keyInput(unsigned char key, int x, int y) {
	switch (key) {
	case 27: exit(0); break;
	case ' ': if (isAnimate) isAnimate = 0; else { isAnimate = 1; animate(1); } break;
	case 'q': if (smallOrbitActive) smallOrbitActive = 0; else smallOrbitActive = 1; glutPostRedisplay(); break;
	case 'w': if (bigOrbitActive) bigOrbitActive = 0; else bigOrbitActive = 1; glutPostRedisplay(); break;
	case 'e': if (moonsActive) moonsActive = 0; else moonsActive = 1; glutPostRedisplay(); break;
	case 'r': if (labelsActive) labelsActive = 0; else labelsActive = 1; glutPostRedisplay(); break;
	
	case '1': changeCamera = 0; glutPostRedisplay(); break;
	case '2': changeCamera = 1; glutPostRedisplay(); break;
	case '3': changeCamera = 2; glutPostRedisplay(); break;
	case '4': changeCamera = 3; glutPostRedisplay(); break;
	case '5': changeCamera = 4; glutPostRedisplay(); break;
	case '6': changeCamera = 5; glutPostRedisplay(); break;
	}
}
// Instruksi / Perintah
void intructions(void) {
	cout << "Kelompok 9 - Solar System - Grafika Komputer" << endl; 
	cout << "---- Instruksi Cara Menjalankan Simulasi ----\n" << endl;
	cout << "Klik Kiri Mouse			:Masuk dalam program simulasi." << endl; 
	cout << "Spasi				:Memulai atau memberhentikan simulasi." << endl;
	cout << "ESC / Klik Kanan Mouse		:Keluar dari simulasi." << endl;
	cout << "w				:Menampilkan atau menyembunyikan jalur orbital besar." << endl;
	cout << "q				:Menampilkan atau menyembunyikan jalur orbital kecil." << endl;
	cout << "e				:Menampilkan atau menyembunyikan satelit." << endl;
	cout << "r				:Menampilkan atau menyembunyikan nama planet" << endl;
	cout << "1, 2, 3, 4, 5, 6		:Mengubah sudut kamera." << endl;
	cout << "Mouse Scroll			:Menscroll untuk mengubah gerakan kamera (Zoom out / Zoom in)" << endl;
}

// Panggilan fungsi - fungsi 
int main(int argc, char** argv) {
	intructions();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 2);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 800);	// Ukuran Layar
	glutInitWindowPosition(500, 0);
	glutCreateWindow("Project Kelompok 9 - Solar System");  //Judul 
	glutDisplayFunc(drawScenesInOrder);
	glutReshapeFunc(resize);
	glutMouseFunc(mouseControl);
	glutKeyboardFunc(keyInput);
	glutMouseWheelFunc(mouseWheel);
	glewExperimental = GL_TRUE;
	glewInit();

	setup();
	glutMainLoop();
}