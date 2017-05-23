// glutEx1.cpp : 定义控制台应用程序的入口点。
//

#include <stdlib.h>
#include <gl/glut.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>

using namespace std;

#define X 0
#define Y 1
#define Z 2
#define W 3
#define R 0
#define A 1
#define Pi 3.1415926

float fTranslate;
float fRotate = 0.0f;
float fScale = 1.0f;								// set inital scale value to 1.0f
float fTpRtt = 0.0f;

bool bPersp = true;									// 透视模式开关
bool bWire = false;									// 线框模式开关
bool bAnim = false;									// 整体旋转开关
bool bRtt = false;									// 茶壶旋转开关

int wHeight = 0;
int wWidth = 0;

float teapot[] = { 0, 0, 0 };						// 茶壶位置
float camera[] = { 0, 1, 4 };						// 摄像机位置
float camera_polar[] = { 4, 0 };					// 摄像机极坐标
float camera_target[] = { 0, 1, 0 };				// 摄像机目标
float point[] = { 5, 5, 5, 1 };						// 点光源位置
float spot[] = { 0, 8, 0 };							// 聚光灯位置
float spot_target[] = { 0, 0, 0 };					// 聚光灯目标

char message[70] = "Welcome!";

GLint List = 0;

GLfloat red[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat green[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat blue[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat yellow[] = { 1.0, 1.0, 0.0, 1.0 };
GLfloat turquoise[] = { 0.0, 1.0, 1.0, 1.0 };
GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat point_diffuse[] = { 1.0, 1.0, 0.0, 1.0 };	// 漫反射光默认为黄色
GLfloat* color = yellow;

enum {
	NOTHING,
	RED,
	GREEN,
	BLUE,
	DEFAULT,
	DISABLE,
	EXIT
};

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
	glTranslatef(0, 0, 4.75);
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
	glViewport(0, 0, width, height);					// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width / (GLfloat)height;

	if (bPersp) {
		gluPerspective(45.0f, whRatio, 0.1f, 100.0f);
		//glFrustum(-3, 3, -3, 3, 3,100);				// 此为透视投影的基本方法
	}
	else {
		glOrtho(-3, 3, -3, 3, -100, 100);
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
		camera_polar[A] -= 0.1;
		break;
	}
	case 'd': {
		camera_polar[A] += 0.1;
		break;
	}
	case 'w': {
		camera_target[Y] += 0.05;
		camera[Y] += 0.05;
		break;
	}
	case 's': {
		camera_target[Y] -= 0.05;
		camera[Y] -= 0.05;
		break;
	}
	case 'z': {
		camera_polar[R] *= 0.95;
		break;
	}
	case 'c': {
		camera_polar[R] *= 1.05;
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
	camera[X] = camera_polar[R] * sin(camera_polar[A]);
	camera[Z] = camera_polar[R] * cos(camera_polar[A]);
}

void specialkey(int k, int x, int y) {
	switch (k) {
	// 点光源位置相关操作
	case 100: {
		point[X] -= 0.2;
		break;
	}
	case 101: {
		point[Z] -= 0.2;
		break;
	}
	case 102: {
		point[X] += 0.2;
		break;
	}
	case 103: {
		point[Z] += 0.2;
		break;
	}
	case 104: {
		point[Y] += 0.2;
		break;
	}
	case 105: {
		point[Y] -= 0.2;
		break;
	}
	}
}

void menu(int value) {
	switch (value) {
	case RED:
		cout << "point light color is set to: RED." << endl;
		color = red;
		strcpy(message, "point light color changed.");
		break;
	case GREEN:
		cout << "point light color is set to: GREEN." << endl;
		color = green;
		strcpy(message, "point light color changed.");
		break;
	case BLUE:
		cout << "point light color is set to: BLUE." << endl;
		color = blue;
		strcpy(message, "point light color changed.");
		break;
	case DEFAULT:
		cout << "point light color is set to: DEFAULT." << endl;
		color = yellow;
		strcpy(message, "point light color changed.");
		break;
	case EXIT:
		cout << "Bye." << endl;
		exit(0);
	case DISABLE:
		cout << "Menu is disabled. Press M to enable menu." << endl;
		glutDetachMenu(GLUT_RIGHT_BUTTON);
		strcpy(message, "Menu is disabled. Press M to enable menu.");
		break;
	}
}

void getStatus() {
	static int frame = 0, time, timebase = 0;
	static char fpstext[50];
	static char cameraposition[100];
	static char pointposition[100];
	static char spottarget[100];

	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf(fpstext, "FPS:%4.2f",
			frame*1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}
	sprintf(cameraposition, "camera position  %2.1f   %2.1f   %2.1f",
		camera[X], camera[Y], camera[Z]);
	sprintf(pointposition, "point position     %2.1f   %2.1f   %2.1f",
		point[X] * 0.2, point[Y] * 0.2, point[Z] * 0.2);
	sprintf(spottarget, "spot target         %2.1f   %2.1f   %2.1f",
		spot_target[X], spot_target[Y], spot_target[Z]);

	char *c;
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);			// 选择投影矩阵
	glPushMatrix();							// 保存原矩阵
	glLoadIdentity();						// 装入单位矩阵
	glOrtho(-480, 480, -480, 480, -1, 1);	// 设置裁减区域
	glMatrixMode(GL_MODELVIEW);				// 选择Modelview矩阵
	glPushMatrix();							// 保存原矩阵
	glLoadIdentity();						// 装入单位矩阵
	glRasterPos2f(-470, 440);
	for (c = fpstext; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
	glRasterPos2f(-460, 390);
	for (c = cameraposition; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
	}
	glRasterPos2f(-460, 340);
	for (c = pointposition; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
	}
	glRasterPos2f(-460, 290);
	for (c = spottarget; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
	}
	glMatrixMode(GL_PROJECTION);			// 选择投影矩阵
	glPopMatrix();							// 重置为原保存矩阵
	glMatrixMode(GL_MODELVIEW);				// 选择Modelview矩阵
	glPopMatrix();							// 重置为原保存矩阵
	glEnable(GL_DEPTH_TEST);
}

void Draw_Light(GLfloat* center, GLfloat radius) {
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glScalef(0.2, 0.2, 0.2);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 20; i++) {
		glVertex3f(2 * radius * cos(2 * Pi / 20 * i) + center[X], radius * sin(2 * Pi / 20 * i) - center[Z], center[Y]);
	}
	glEnd();
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 20; i++) {
		glVertex3f(2 * radius * cos(2 * Pi / 20 * i) + center[X], - center[Z], radius * sin(2 * Pi / 20 * i) + center[Y]);
	}
	glEnd();
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 20; i++) {
		glVertex3f(center[X], radius * sin(2 * Pi / 20 * i) - center[Z], radius * cos(2 * Pi / 20 * i) + center[Y]);
	}
	glEnd();
	glPopMatrix();
}

void redraw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	gluLookAt(camera[X], camera[Y], camera[Z],
		camera_target[X], camera_target[Y], camera_target[Z],
		0, 1, 0);										// 摄像机（0，0，4）的视点中心（0, 0, 0），Y轴向上

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
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
	glLightfv(GL_LIGHT0, GL_SPECULAR, blue);
	glEnable(GL_LIGHT0);

	Draw_Light(point, 0.5);

	glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
	glRotatef(-90, 1, 0, 0);
	glScalef(0.2, 0.2, 0.2);
	Draw_Scene_List();						// Draw Scene with display list
	getStatus();							// Get status of FPS and lights

	if (bAnim) fRotate += 0.5f;
	if (bRtt) {
		List = GenTableList();
		fTpRtt -= 0.8f;
	}
	glutSwapBuffers();
}

void initMenu(void) {
	glutCreateMenu(menu);

	glutAddMenuEntry("Please select as quick as possible!", NOTHING);
	glutAddMenuEntry("---------Change light color here---------", NOTHING);
	glutAddMenuEntry("    Red", RED);
	glutAddMenuEntry("    Green", GREEN);
	glutAddMenuEntry("    Blue", BLUE);
	glutAddMenuEntry("    Default", DEFAULT);
	glutAddMenuEntry("------------------------------------------", NOTHING);
	glutAddMenuEntry("Disable menu", DISABLE);
	glutAddMenuEntry("------------------------------------------", NOTHING);
	glutAddMenuEntry("Exit", EXIT);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480, 480);
	int windowHandle = glutCreateWindow("Ex 5");

	// Initiate the menu
	initMenu();

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(normalkey);
	glutSpecialFunc(specialkey);
	glutIdleFunc(idle);

	List = GenTableList();

	glutMainLoop();
	return 0;
}


