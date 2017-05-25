#include "head.h"

#pragma warning(disable:4996)

void drawLeg() {
	glScalef(1, 1, 3);
	glutSolidCube(1.0);
}

void drawScene() {
	glPushAttrib(GL_LIGHTING_BIT);
	// 在这个函数范围内，横x深y纵z
	// teapot
	glPushMatrix();
	glTranslatef(scene.teapot[X], scene.teapot[Y], scene.teapot[Z]);
	glPushMatrix();
	glTranslatef(0, 0, 4.75);
	glRotatef(90, 1, 0, 0);
	// 以下横x纵y深z
	glRotatef(scene.fTpRtt, 0, 1, 0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, scene.black);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, scene.golden);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, scene.white);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 80);
	glutSolidTeapot(1);
	glPopMatrix();
	glPopMatrix();

	// table
	glPushMatrix();
	glTranslatef(0, 0, 3.5);
	glScalef(5, 4, 1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, scene.red);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, scene.black);
	glutSolidCube(1.0);
	glPopMatrix();

	// leg1
	glPushMatrix();
	glTranslatef(1.5, 1, 1.5);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, scene.green);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, scene.black);
	drawLeg();
	glPopMatrix();

	// leg2
	glPushMatrix();
	glTranslatef(-1.5, 1, 1.5);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, scene.yellow);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, scene.black);
	drawLeg();
	glPopMatrix();

	// leg3
	glPushMatrix();
	glTranslatef(1.5, -1, 1.5);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, scene.turquoise);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, scene.black);
	drawLeg();
	glPopMatrix();

	// leg4
	glPushMatrix();
	glTranslatef(-1.5, -1, 1.5);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, scene.blue);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, scene.black);
	drawLeg();
	glPopMatrix();
	glPopAttrib();
}

GLint genTableList() {
	GLint lid = glGenLists(1);

	glNewList(lid, GL_COMPILE);
	drawScene();
	glEndList();

	return lid;
}

void drawLight(GLfloat* center, GLfloat radius) {
	glPushAttrib(GL_LIGHTING_BIT);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, scene.white);
	glPushMatrix();
	glTranslatef(center[X], center[Y], center[Z]);
	glScalef(0.2, 0.2, 0.2);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 20; i++) {
		glVertex3f(2 * radius * cos(2 * Pi / 20 * i), radius * sin(2 * Pi / 20 * i), 0);
	}
	glEnd();
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 20; i++) {
		glVertex3f(2 * radius * cos(2 * Pi / 20 * i), 0, radius * sin(2 * Pi / 20 * i));
	}
	glEnd();
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 20; i++) {
		glVertex3f(0, radius * sin(2 * Pi / 20 * i), radius * cos(2 * Pi / 20 * i));
	}
	glEnd();
	glPopMatrix();
	glPopAttrib();
}
