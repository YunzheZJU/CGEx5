// glutEx1.cpp : 定义控制台应用程序的入口点。
//

#include <stdlib.h>
#include <gl/glut.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <iomanip>

#pragma warning(disable:4996)

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

bool bpoint = true;									// 点光源开关
bool bspot = true;									// 聚光灯开关
bool blight = true;									// 灯光模式开关
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
float spot[] = { 0, 6, 0, 1 };							// 聚光灯位置
float spot_target[] = { 0, 0, 0 };					// 聚光灯目标

char message[70] = "Welcome!";

GLint List = 0;

GLfloat spot_cutoff = 6.0;
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
		glTranslatef(teapot[X], teapot[Y], teapot[Z]);
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

void updateList() {
	List = GenTableList();
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

void updateCamera() {
	camera[X] = camera_polar[R] * sin(camera_polar[A]);
	camera[Z] = camera_polar[R] * cos(camera_polar[A]);
}

void normalkey(unsigned char k, int x, int y) {
	switch (k) {
	// 全局操作
	case 27:
	case 'Q':
	case 'q': {
		cout << "Bye." << endl;
		exit(0);
		break;
	}
	case 'M':
	case 'm': {
		cout << "M pressed.\n\tRight click to check for menu items." << endl;
		strcpy(message, "M pressed. Right click to check for menu items.");
		glutAttachMenu(GLUT_RIGHT_BUTTON);
		break;
	}
	case 'E':
	case 'e': {
		bRtt = !bRtt;
		if (bRtt) {
			cout << "E pressed.\n\tTeapot is rotating." << endl;
			strcpy(message, "E pressed. Teapot is rotating.");
		}
		else {
			cout << "E pressed.\n\tTeapot rotating is stopped." << endl;
			strcpy(message, "Teapot rotating is stopped.");
		}
		break;
	}
	case 'O':
	case 'o': {
		bWire = !bWire;
		if (bWire) {
			cout << "O pressed.\n\tSwitch on line mode." << endl;
			strcpy(message, "O pressed. Switch on line mode.");
		}
		else {
			cout << "O pressed.\n\tSwitch off line mode." << endl;
			strcpy(message, "O pressed. Switch off line mode.");
		}
		break;
	}
	case ' ': {
		bAnim = !bAnim;
		if (bAnim) {
			cout << "Space pressed.\n\tThe whole model is rotating." << endl;
			strcpy(message, "Space pressed. The whole model is rotating.");
		}
		else {
			cout << "Space pressed.\n\tModel rotating is stopped." << endl;
			strcpy(message, "Model rotating is stopped.");
		}
		break;
	}

	// 摄像机相关操作
	case 'P':
	case 'p': {
		bPersp = !bPersp;
		updateView(wHeight, wWidth);
		break;
	}
	case 'A':
	case 'a': {
		camera_polar[A] -= 0.1;
		updateCamera();
		cout << fixed << setprecision(1) << "A pressed.\n\tPosition of camera is set to (" <<
			camera[X] << ", " << camera[Y] << ", " << camera[Z] << ")." << endl;
		strcpy(message, "A pressed. Watch carefully!");
		break;
	}
	case 'D':
	case 'd': {
		camera_polar[A] += 0.1;
		updateCamera();
		cout << fixed << setprecision(1) << "D pressed.\n\tPosition of camera is set to (" <<
			camera[X] << ", " << camera[Y] << ", " << camera[Z] << ")." << endl;
		strcpy(message, "D pressed. Watch carefully!");
		break;
	}
	case 'W':
	case 'w': {
		camera_target[Y] += 0.05;
		camera[Y] += 0.05;
		cout << fixed << setprecision(1) << "W pressed.\n\tPosition of camera is set to (" <<
			camera[X] << ", " << camera[Y] << ", " << camera[Z] << ")." << endl;
		strcpy(message, "W pressed. Watch carefully!");
		break;
	}
	case 'S':
	case 's': {
		camera_target[Y] -= 0.05;
		camera[Y] -= 0.05;
		cout << fixed << setprecision(1) << "S pressed.\n\tPosition of camera is set to (" <<
			camera[X] << ", " << camera[Y] << ", " << camera[Z] << ")." << endl;
		strcpy(message, "S pressed. Watch carefully!");
		break;
	}
	case 'Z':
	case 'z': {
		camera_polar[R] *= 0.95;
		updateCamera();
		cout << fixed << setprecision(1) << "Z pressed.\n\tPosition of camera is set to (" <<
			camera[X] << ", " << camera[Y] << ", " << camera[Z] << ")." << endl;
		strcpy(message, "Z pressed. Camera is moved...nearer!");
		break;
	}
	case 'C':
	case 'c': {
		camera_polar[R] *= 1.05;
		updateCamera();
		cout << fixed << setprecision(1) << "C pressed.\n\tPosition of camera is set to (" <<
			camera[X] << ", " << camera[Y] << ", " << camera[Z] << ")." << endl;
		strcpy(message, "C pressed. Camera is moved...farther!");
		break;
	}

	//茶壶相关操作
	case 'L':
	case 'l': {
		if (teapot[X] <= 2) {
			teapot[X] += 0.2;
			cout << fixed << setprecision(1) << "L pressed.\n\tPosition of teapot is set to (" <<
				teapot[X] << ", " << teapot[Y] << ", " << teapot[Z] << ")." << endl;
			strcpy(message, "L pressed. Teapot is moved!");
			updateList();
		}
		else {
			cout << "L pressed.\n\tOut of border!" << endl;
			strcpy(message, "L pressed. Teapot is at the border!");
		}
		break;
	}
	case 'J':
	case 'j': {
		if (teapot[X] >= -2) {
			teapot[X] -= 0.2;
			cout << fixed << setprecision(1) << "J pressed.\n\tPosition of teapot is set to (" <<
				teapot[X] << ", " << teapot[Y] << ", " << teapot[Z] << ")." << endl;
			strcpy(message, "J pressed. Teapot is moved!");
			updateList();
		}
		else {
			cout << "J pressed.\n\tOut of border!" << endl;
			strcpy(message, "J pressed. Teapot is at the border!");
		}
		break;
	}
	case 'I':
	case 'i': {
		if (teapot[Y] <= 1.5) {
			teapot[Y] += 0.2;
			cout << fixed << setprecision(1) << "I pressed.\n\tPosition of teapot is set to (" <<
				teapot[X] << ", " << teapot[Y] << ", " << teapot[Z] << ")." << endl;
			strcpy(message, "I pressed. Teapot is moved!");
			updateList();
		}
		else {
			cout << "I pressed.\n\tOut of border!" << endl;
			strcpy(message, "I pressed. Teapot is at the border!");
		}
		break;
	}
	case 'K':
	case 'k': {
		if (teapot[Y] >= -1.5) {
			teapot[Y] -= 0.2;
			cout << fixed << setprecision(1) << "K pressed.\n\tPosition of teapot is set to (" <<
				teapot[X] << ", " << teapot[Y] << ", " << teapot[Z] << ")." << endl;
			strcpy(message, "K pressed. Teapot is moved!");
			updateList();
		}
		else {
			cout << "K pressed.\n\tOut of border!" << endl;
			strcpy(message, "K pressed. Teapot is at the border!");
		}
		break;
	}

	// 聚光灯目标操作
	case 'T':
	case 't': {
		spot_target[Z] -= 0.1;
		cout << fixed << setprecision(1) << "T pressed.\n\tTarget of spot light is set to (" <<
			spot_target[X] << ", " << spot_target[Y] << ", " << spot_target[Z] << ")." << endl;
		strcpy(message, "T pressed. What will happen to the spot light?");
		break;
	}
	case 'G':
	case 'g': {
		spot_target[Z] += 0.1;
		cout << fixed << setprecision(1) << "G pressed.\n\tTarget of spot light is set to (" <<
			spot_target[X] << ", " << spot_target[Y] << ", " << spot_target[Z] << ")." << endl;
		strcpy(message, "G pressed. What will happen to the spot light?");
		break;
	}
	case 'F':
	case 'f': {
		spot_target[X] -= 0.1;
		cout << fixed << setprecision(1) << "F pressed.\n\tTarget of spot light is set to (" <<
			spot_target[X] << ", " << spot_target[Y] << ", " << spot_target[Z] << ")." << endl;
		strcpy(message, "F pressed. What will happen to the spot light?");
		break;
	}
	case 'H':
	case 'h': {
		spot_target[X] += 0.1;
		cout << fixed << setprecision(1) << "H pressed.\n\tTarget of spot light is set to (" << 
			spot_target[X] << ", " << spot_target[Y] << ", " << spot_target[Z] << ")." << endl;
		strcpy(message, "H pressed. What will happen to the spot light?");
		break;
	}

	// 聚光灯圆锥角度
	case 'R':
	case 'r': {
		if (spot_cutoff >= 1) {
			spot_cutoff -= 0.1;
			cout << "R pressed.\n\tCut Off is set to " << spot_cutoff << " degrees." << endl;
			strcpy(message, "R pressed. The spot light will light less.");
		}
		else {
			cout << "R pressed.\n\tOut of range!" << endl;
			strcpy(message, "R pressed. Cut off is at its minimum!");
		}
		break;
	}
	case 'Y':
	case 'y': {
		if (spot_cutoff <= 179) {
			spot_cutoff += 0.1;
			cout << "Y pressed.\n\tCut Off is set to " << spot_cutoff << " degrees." << endl;
			strcpy(message, "Y pressed. The spot light will light more.");
		}
		else {
			cout << "Y pressed.\n\tOut of range!" << endl;
			strcpy(message, "Y pressed. Cut off is at its maximum!");
		}
		break;
	}
	}
}

void specialkey(int k, int x, int y) {
	switch (k) {
	// 点光源位置相关操作
	case 100: {
		point[X] -= 0.2;
		cout << fixed << setprecision(1) << "Left Arrow pressed.\n\tPosition of point light is set to (" <<
			point[X] << ", " << point[Y] << ", " << point[Z] << ")." << endl;
		strcpy(message, "Left Arrow pressed. Pay attention to the point light.");
		break;
	}
	case 101: {
		point[Z] -= 0.2;
		cout << fixed << setprecision(1) << "Up Arrow pressed.\n\tPosition of point light is set to (" <<
			point[X] << ", " << point[Y] << ", " << point[Z] << ")." << endl;
		strcpy(message, "Up Arrow pressed. Pay attention to the point light.");
		break;
	}
	case 102: {
		point[X] += 0.2;
		cout << fixed << setprecision(1) << "Right Arrow pressed.\n\tPosition of point light is set to (" <<
			point[X] << ", " << point[Y] << ", " << point[Z] << ")." << endl;
		strcpy(message, "Right Arrow pressed. Pay attention to the point light.");
		break;
	}
	case 103: {
		point[Z] += 0.2;
		cout << fixed << setprecision(1) << "Down Arrow pressed.\n\tPosition of point light is set to (" <<
			point[X] << ", " << point[Y] << ", " << point[Z] << ")." << endl;
		strcpy(message, "Down Arrow pressed. Pay attention to the point light.");
		break;
	}
	case 104: {
		point[Y] += 0.2;
		cout << fixed << setprecision(1) << "PageUp pressed.\n\tPosition of point light is set to (" <<
			point[X] << ", " << point[Y] << ", " << point[Z] << ")." << endl;
		strcpy(message, "PageUp pressed. Pay attention to the point light.");
		break;
	}
	case 105: {
		point[Y] -= 0.2;
		cout << fixed << setprecision(1) << "PageDown pressed.\n\tPosition of point light is set to (" <<
			point[X] << ", " << point[Y] << ", " << point[Z] << ")." << endl;
		strcpy(message, "PageDown pressed. Pay attention to the point light.");
		break;
	}
	}
}

void menu(int value) {
	switch (value) {
	case RED:
		cout << "Point light color is set to: RED." << endl;
		color = red;
		strcpy(message, "Point light color changed.");
		break;
	case GREEN:
		cout << "Point light color is set to: GREEN." << endl;
		color = green;
		strcpy(message, "Point light color changed.");
		break;
	case BLUE:
		cout << "Point light color is set to: BLUE." << endl;
		color = blue;
		strcpy(message, "Point light color changed.");
		break;
	case DEFAULT:
		cout << "Point light color is set to: DEFAULT." << endl;
		color = yellow;
		strcpy(message, "Point light color changed.");
		break;
	case DISABLE:
		cout << "Menu is disabled. Press M to enable menu." << endl;
		glutDetachMenu(GLUT_RIGHT_BUTTON);
		strcpy(message, "Menu is disabled. Press M to enable menu.");
		break;
	case EXIT:
		cout << "Bye." << endl;
		exit(0);
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
	glRasterPos2f(-460, -460);
	for (c = message; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
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

void initPoint() {
	glLightfv(GL_LIGHT0, GL_POSITION, point);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
	glLightfv(GL_LIGHT0, GL_SPECULAR, blue);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.5f);
	glEnable(GL_LIGHT0);
}

void initSpot() {
	GLfloat direction[] = { spot_target[X] - spot[X], spot_target[Y] - spot[Y], spot_target[Z] - spot[Z] };
	glLightfv(GL_LIGHT1, GL_POSITION, spot);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, spot_cutoff);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 30.0f);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.5f);
	glEnable(GL_LIGHT1);
}

void redraw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	gluLookAt(camera[X], camera[Y], camera[Z],
		camera_target[X], camera_target[Y], camera_target[Z],
		0, 1, 0);										// 摄像机（0，1，4）的视点中心（0, 1, 0），Y轴向上

	if (bWire) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	if (blight) {
		glEnable(GL_LIGHTING);
		if (bpoint) {
			initPoint();
		}
		if (bspot) {
			initSpot();
		}
	}

	Draw_Light(point, 0.5);
	Draw_Light(spot, 0.25);
	Draw_Light(spot_target, 0.25);

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
	// Initiate the display list
	updateList();

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(normalkey);
	glutSpecialFunc(specialkey);
	glutIdleFunc(idle);


	glutMainLoop();
	return 0;
}


