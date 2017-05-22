// glutEx1.cpp : 定义控制台应用程序的入口点。
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

float fTranslate;
float fRotate = 0.0f;
float fScale = 1.0f;	// set inital scale value to 1.0f
float fTpRtt = 0.0f;

bool bPersp = false;
bool bAnim = false;
bool bRtt = false;
bool bWire = false;

int wHeight = 0;
int wWidth = 0;

float eye[] = { 0, 0, 8 };
float center[] = { 0, 0, 0 };
float teapot[] = { 0, 0, 0 };
float spot[] = { 0, 8, 0 };
float target[] = { 0, 0, 0 };
float point[] = { 5, 5, 5, 1 };
float polar[] = { 8, 0 };

GLint List = 0;

GLfloat yellow[] = { 1.0, 1.0, 0.0, 1.0 };
GLfloat red[] = { 1.0, 0.0, 1.0, 1.0 };
GLfloat blue[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };


void Draw_Leg() {
	glScalef(1, 1, 3);
	glutSolidCube(1.0);
}

void Draw_Scene() {
	// 在这个函数范围内，横x深y纵z
	// teapot
	glPushMatrix();
	glTranslatef(teapot[0], teapot[1], teapot[2]);
	glPushMatrix();
	glTranslatef(0, 0, 4 + 1);
	glRotatef(90, 1, 0, 0);
	// 以下横x纵y深z
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
		//glFrustum(-3, 3, -3, 3, 3,100);				// 此为透视投影的基本方法
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
	// 全局操作
	case 27:
	case 'q': {
		exit(0);
		break;
	}

	// 摄像机相关操作
	case 'p': {
		bPersp = !bPersp;
		updateView(wHeight, wWidth);
		break;
	}
	case 'a': {
		eye[X] -= 0.1;
		center[X] -= 0.1;
		break;
	}
	case 'd': {
		eye[X] += 0.1;
		center[X] += 0.1;
		break;
	}
	case 'w': {
		eye[Y] += 0.1;
		center[Y] += 0.1;
		break;
	}
	case 's': {
		eye[Y] -= 0.1;
		center[Y] -= 0.1;
		break;
	}
	case 'z': {
		eye[Z] *= 0.95;
		break;
	}
	case 'c': {
		eye[Z] *= 1.05;
		break;
	}

	//茶壶相关操作
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

	// 聚光灯目标操作
	case 't': {
		target[Y] += 0.2;
		break;
	}
	case 'g': {
		target[Y] -= 0.2;
		break;
	}
	case 'f': {
		target[X] -= 0.2;
		break;
	}
	case 'h': {
		target[X] += 0.2;
		break;
	}
	}
}

void specialkey(int k, int x, int y) {
	switch (k) {
	// 点光源位置相关操作
	case 101: {
		point[Y] += 0.2;
		break;
	}
	case 103: {
		point[Y] -= 0.2;
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
		point[Z] += 0.2;
		break;
	}
	case 105: {
		point[Z] += 0.2;
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
	glMatrixMode(GL_PROJECTION);	// 选择投影矩阵
	glPushMatrix();					// 保存原矩阵
	glLoadIdentity();				// 装入单位矩阵
	glOrtho(0, 480, 0, 480, -1, 1);	// 位置正投影
	glMatrixMode(GL_MODELVIEW);		// 选择Modelview矩阵
	glPushMatrix();					// 保存原矩阵
	glLoadIdentity();				// 装入单位矩阵
	glRasterPos2f(10, 100);
	for (c = fpstext; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
	glRasterPos2f(10, 80);
	for (c = lightposition; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
	// 逆序切换MatrixMode，Why?
	glMatrixMode(GL_PROJECTION);	// 选择投影矩阵
	glPopMatrix();					// 重置为原保存矩阵
	glMatrixMode(GL_MODELVIEW);		// 选择Modelview矩阵
	glPopMatrix();					// 重置为原保存矩阵
	glEnable(GL_DEPTH_TEST);
}

void redraw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	gluLookAt(eye[X], eye[Y], eye[Z],
		center[X], center[Y], center[Z],
		0, 1, 0);				// 摄像机（0，0，8）的视点中心（0, 0, 0），Y轴向上

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


