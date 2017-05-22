// glutEx1.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include <stdlib.h>
#include <gl/glut.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define X 0
#define Y 1
#define Z 2
#define W 3
#define R 0
#define A 1

float fTranslate;
float fRotate = 0.0f;
float fScale = 1.0f;	// set inital scale value to 1.0f
float fTpRtt = 0.0f;

bool bPersp = false;								// ͸��ģʽ����
bool bWire = false;									// �߿�ģʽ����
bool bAnim = false;									// ������ת����
bool bRtt = false;									// �����ת����

int wHeight = 0;
int wWidth = 0;

float teapot[] = { 0, 0, 0 };						// ���λ��
float camera[] = { 0, 0, 8 };						// �����λ��
float camera_polar[] = { 8, 0 };					// �����������
float camera_target[] = { 0, 0, 0 };				// �����Ŀ��
float point[] = { 5, 5, 5, 1 };						// ���Դλ��
float spot[] = { 0, 8, 0 };							// �۹��λ��
float spot_target[] = { 0, 0, 0 };					// �۹��Ŀ��

GLint List = 0;

GLfloat yellow[] = { 1.0, 1.0, 0.0, 1.0 };
GLfloat red[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat blue[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat point_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };	// �������Ĭ��Ϊ��ɫ

void Draw_Leg() {
	glScalef(1, 1, 3);
	glutSolidCube(1.0);
}

void Draw_Scene() {
	// �����������Χ�ڣ���x��y��z
	// teapot
	glPushMatrix();
	glTranslatef(teapot[0], teapot[1], teapot[2]);
	glPushMatrix();
	glTranslatef(0, 0, 4 + 1);
	glRotatef(90, 1, 0, 0);
	// ���º�x��y��z
	glRotatef(fTpRtt, 0, 1, 0);
	glutSolidTeapot(1);
	glPopMatrix();
	glPopMatrix();

	// table
	glPushMatrix();
	glTranslatef(0, 0, 3.5);
	glScalef(5, 4, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	// leg1
	glPushMatrix();
	glTranslatef(1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	// leg2
	glPushMatrix();
	glTranslatef(-1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	// leg3
	glPushMatrix();
	glTranslatef(1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

	// leg4
	glPushMatrix();
	glTranslatef(-1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();
}

GLint GenTableList() {
	GLint lid = glGenLists(1);
	glNewList(lid, GL_COMPILE);
		Draw_Scene();
	glEndList();
	return lid;
}

void Draw_Scene_List() {
	glCallList(List);
}

void updateView(int width, int height) {
	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width/(GLfloat)height;

	if (bPersp) {
		gluPerspective(45.0f, whRatio,0.1f,100.0f);
		//glFrustum(-3, 3, -3, 3, 3,100);				// ��Ϊ͸��ͶӰ�Ļ�������
	}
	else {
		glOrtho(-3 ,3, -3, 3,-100,100);
	}

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void reshape(int width, int height) {
	if (height == 0) {									// Prevent A Divide By Zero By
		height = 1;										// Making Height Equal One
	}

	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth);
}

void idle() {
	glutPostRedisplay();
}

void normalkey(unsigned char k, int x, int y) {
	switch (k) {
	// ȫ�ֲ���
	case 27:
	case 'q': {
		exit(0);
		break;
	}

	// �������ز���
	case 'p': {
		bPersp = !bPersp;
		updateView(wHeight, wWidth);
		break;
	}
	case 'a': {
		//camera[X] -= 0.1;
		camera_polar[A] -= 0.1;
		break;
	}
	case 'd': {
		//camera[X] += 0.1;
		camera_polar[A] += 0.1;
		break;
	}
	case 'w': {
		camera[Y] += 0.1;
		break;
	}
	case 's': {
		camera[Y] -= 0.1;
		break;
	}
	case 'z': {
		//camera[Z] *= 0.95;
		camera_polar[R] *= 0.95;
		break;
	}
	case 'c': {
		//camera[Z] *= 1.05;
		camera_polar[R] *= 1.05;
		break;
	}

	//�����ز���
	case 'l': {
		if (teapot[X] <= 2) {
			teapot[X] += 0.2;
		}
		break;
	}
	case 'j': {
		if (teapot[X] >= -2) {
			teapot[X] -= 0.2;
		}
		break;
	}
	case 'i': {
		if (teapot[Y] <= 1.5) {
			teapot[Y] += 0.2;
		}
		break;
	}
	case 'k': {
		if (teapot[Y] >= -1.5) {
			teapot[Y] -= 0.2;
		}
		break;
	}
	case 'e': {
		bRtt = !bRtt;
		break;
	}
	case ' ': {
		bAnim = !bAnim; break;
	}
	case 'o': {
		bWire = !bWire; break;
	}

	// �۹��Ŀ�����
	case 't': {
		spot_target[Y] += 0.2;
		break;
	}
	case 'g': {
		spot_target[Y] -= 0.2;
		break;
	}
	case 'f': {
		spot_target[X] -= 0.2;
		break;
	}
	case 'h': {
		spot_target[X] += 0.2;
		break;
	}
	}
	camera_target[Y] = 0.4 * camera[Y];
	camera[X] = camera_polar[R] * sin(camera_polar[A]);
	camera[Z] = camera_polar[R] * cos(camera_polar[A]);
}

void specialkey(int k, int x, int y) {
	switch (k) {
	// ���Դλ����ز���
	case 101: {
		point[Z] += 0.2;
		break;
	}
	case 103: {
		point[Z] -= 0.2;
		break;
	}
	case 100: {
		point[X] -= 0.2;
		break;
	}
	case 102: {
		point[X] += 0.2;
		break;
	}
	case 104: {
		point[Y] += 0.2;
		break;
	}
	case 105: {
		point[Y] += 0.2;
		break;
	}
	}
}

void getStatus() {
	static int frame = 0, time, timebase = 0;
	static char fpstext[50];
	static char lightposition[50];

	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf(fpstext, "FPS:%4.2f",
			frame*1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}
	sprintf(lightposition, "123 %s",
		"sd");

	char *c;
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);	// ѡ��ͶӰ����
	glPushMatrix();					// ����ԭ����
	glLoadIdentity();				// װ�뵥λ����
	glOrtho(0, 480, 0, 480, -1, 1);	// λ����ͶӰ
	glMatrixMode(GL_MODELVIEW);		// ѡ��Modelview����
	glPushMatrix();					// ����ԭ����
	glLoadIdentity();				// װ�뵥λ����
	glRasterPos2f(10, 100);
	for (c = fpstext; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
	glRasterPos2f(10, 80);
	for (c = lightposition; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
	// �����л�MatrixMode��Why?
	glMatrixMode(GL_PROJECTION);	// ѡ��ͶӰ����
	glPopMatrix();					// ����Ϊԭ�������
	glMatrixMode(GL_MODELVIEW);		// ѡ��Modelview����
	glPopMatrix();					// ����Ϊԭ�������
	glEnable(GL_DEPTH_TEST);
}

void redraw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	gluLookAt(camera[X], camera[Y], camera[Z],
		camera_target[X], camera_target[Y], camera_target[Z],
		0, 1, 0);										// �������0��0��8�����ӵ����ģ�0, 0, 0����Y������

	if (bWire) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_POSITION, point);
	glLightfv(GL_LIGHT0, GL_AMBIENT, yellow);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, red);
	glLightfv(GL_LIGHT0, GL_SPECULAR, blue);
	glEnable(GL_LIGHT0);

	glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
	glRotatef(-90, 1, 0, 0);
	glScalef(0.2, 0.2, 0.2);
	Draw_Scene_List();						// Draw Scene with display list
	getStatus();							// Get status of FPS and lights

	if (bAnim) fRotate += 0.5f;
	if (bRtt) {
		List = GenTableList();
		fTpRtt += 0.5f;
	}
	glutSwapBuffers();
}

int main (int argc,  char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480, 480);
	int windowHandle = glutCreateWindow("Ex 5");

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(normalkey);
	glutSpecialFunc(specialkey);
	glutIdleFunc(idle);

	List = GenTableList();

	glutMainLoop();
	return 0;
}


