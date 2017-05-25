#include "head.h"

#pragma warning(disable:4996)

void idle() {
	glutPostRedisplay();
}

void initMenu(void) {
	glutCreateMenu(processMenu);

	glutAddMenuEntry("Please select as quick as possible!", NOTHING);
	glutAddMenuEntry("---------Change light color here---------", NOTHING);
	glutAddMenuEntry("    Red", RED);
	glutAddMenuEntry("    Green", GREEN);
	glutAddMenuEntry("    Blue", BLUE);
	glutAddMenuEntry("    Yellow", YELLOW);
	glutAddMenuEntry("    Default", DEFAULT);
	glutAddMenuEntry("------------------------------------------", NOTHING);
	glutAddMenuEntry("Disable menu", DISABLE);
	glutAddMenuEntry("------------------------------------------", NOTHING);
	glutAddMenuEntry("Exit", EXIT);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void processMenu(int value) {
	switch (value) {
	case RED:
		cout << "Point light color is set to: RED." << endl;
		scene.point_color = scene.point_red;
		strcpy(scene.message, "Point light color changed.");
		break;
	case GREEN:
		cout << "Point light color is set to: GREEN." << endl;
		scene.point_color = scene.point_green;
		strcpy(scene.message, "Point light color changed.");
		break;
	case BLUE:
		cout << "Point light color is set to: BLUE." << endl;
		scene.point_color = scene.point_blue;
		strcpy(scene.message, "Point light color changed.");
		break;
	case YELLOW:
		cout << "Point light color is set to: YELLOW." << endl;
		scene.point_color = scene.point_yellow;
		strcpy(scene.message, "Point light color changed.");
		break;
	case DEFAULT:
		cout << "Point light color is set to: DEFAULT." << endl;
		scene.point_color = scene.point_white;
		strcpy(scene.message, "Point light color changed to default.");
		break;
	case DISABLE:
		cout << "Menu is disabled. Press M to enable menu." << endl;
		glutDetachMenu(GLUT_RIGHT_BUTTON);
		strcpy(scene.message, "Menu is disabled. Press M to enable menu.");
		break;
	case EXIT:
		cout << "Bye." << endl;
		exit(0);
	}
}

void redraw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();						// Reset The Current Modelview Matrix

	gluLookAt(scene.camera[X], scene.camera[Y], scene.camera[Z],
		scene.camera_target[X], scene.camera_target[Y], scene.camera_target[Z],
		0, 1, 0);							// 摄像机（0，1，4）的视点中心（0, 1, 0），Y轴向上

	if (scene.bWire) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (scene.bpoint) {
		updatePoint();						// 更新点光源信息并启用
		drawLight(scene.point, 0.5);
	}
	else {
		glDisable(GL_LIGHT0);
	}
	if (scene.bspot) {
		updateSpot();						// 更新聚光灯信息并启用
		drawLight(scene.spot, 0.25);
		drawLight(scene.spot_target, 0.25);
	}
	else {
		glDisable(GL_LIGHT1);
	}

	glRotatef(scene.fRotate, 0, 1, 0);			// Rotate around Y axis
	glRotatef(-90, 1, 0, 0);
	glScalef(0.2, 0.2, 0.2);
	callList();								// Draw Scene with display List
	showSysStatus();						// Show information like fps, location and message.

	if (scene.bAnim) scene.fRotate += 0.5;
	if (scene.bRtt) {
		scene.List = genTableList();
		scene.fTpRtt -= 0.8;
	}
	glutSwapBuffers();
}

void reshape(int width, int height) {
	if (height == 0) {									// Prevent A Divide By Zero By
		height = 1;										// Making Height Equal One
	}

	scene.wHeight = height;
	scene.wWidth = width;

	updateView(scene.wHeight, scene.wWidth);
}

void updateView(int width, int height) {
	glViewport(0, 0, width, height);					// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width / (GLfloat)height;

	if (scene.bPersp) {
		gluPerspective(45.0f, whRatio, 0.1f, 100.0f);
		//glFrustum(-3, 3, -3, 3, 3,100);				// 此为透视投影的基本方法
	}
	else {
		glOrtho(-3, 3, -3, 3, -100, 100);
	}

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void processNormalKey(unsigned char k, int x, int y) {
	switch (k) {
		// 退出程序
	case 27:
	case 'Q':
	case 'q': {
		cout << "Bye." << endl;
		exit(0);
		break;
	}
			  // 显示菜单
	case 'M':
	case 'm': {
		cout << "M pressed.\n\tRight click to check for menu items." << endl;
		strcpy(scene.message, "M pressed. Right click to check for menu items.");
		glutAttachMenu(GLUT_RIGHT_BUTTON);
		break;
	}
			  // 线框模式
	case 'O':
	case 'o': {
		scene.bWire = !scene.bWire;
		if (scene.bWire) {
			cout << "O pressed.\n\tSwitch on line mode." << endl;
			strcpy(scene.message, "O pressed. Switch on line mode.");
		}
		else {
			cout << "O pressed.\n\tSwitch off line mode." << endl;
			strcpy(scene.message, "O pressed. Switch off line mode.");
		}
		break;
	}
			  // 转动茶壶
	case 'E':
	case 'e': {
		scene.bRtt = !scene.bRtt;
		if (scene.bRtt) {
			cout << "E pressed.\n\tTeapot is rotating." << endl;
			strcpy(scene.message, "E pressed. Teapot is rotating.");
		}
		else {
			cout << "E pressed.\n\tTeapot rotating is stopped." << endl;
			strcpy(scene.message, "Teapot rotating is stopped.");
		}
		break;
	}
			  // 整体转动
	case ' ': {
		scene.bAnim = !scene.bAnim;
		if (scene.bAnim) {
			cout << "Space pressed.\n\tThe whole model is rotating." << endl;
			strcpy(scene.message, "Space pressed. The whole model is rotating.");
		}
		else {
			cout << "Space pressed.\n\tModel rotating is stopped." << endl;
			strcpy(scene.message, "Model rotating is stopped.");
		}
		break;
	}
			  // 关闭点光源
	case '0': {
		scene.bpoint = !scene.bpoint;
		if (scene.bpoint) {
			cout << "0 pressed.\n\tTurn on point light." << endl;
			strcpy(scene.message, "0 pressed. Turn on point light.");
		}
		else {
			cout << "0 pressed.\n\tTurn off point light." << endl;
			strcpy(scene.message, "0 pressed. Turn off point light.");
		}
		break;
	}
			  // 关闭聚光灯
	case '1': {
		scene.bspot = !scene.bspot;
		if (scene.bspot) {
			cout << "1 pressed.\n\tTurn on spot light." << endl;
			strcpy(scene.message, "0 pressed. Turn on spot light.");
		}
		else {
			cout << "1 pressed.\n\tTurn off spot light." << endl;
			strcpy(scene.message, "0 pressed. Turn off spot light.");
		}
		break;
	}

			  // 摄像机移动
	case 'P':
	case 'p': {
		scene.bPersp = !scene.bPersp;
		updateView(scene.wHeight, scene.wWidth);
		break;
	}
	case 'A':
	case 'a': {
		scene.camera_polar[A] -= 0.1;
		updateCamera();
		cout << fixed << setprecision(1) << "A pressed.\n\tPosition of camera is set to (" <<
			scene.camera[X] << ", " << scene.camera[Y] << ", " << scene.camera[Z] << ")." << endl;
		strcpy(scene.message, "A pressed. Watch carefully!");
		break;
	}
	case 'D':
	case 'd': {
		scene.camera_polar[A] += 0.1;
		updateCamera();
		cout << fixed << setprecision(1) << "D pressed.\n\tPosition of camera is set to (" <<
			scene.camera[X] << ", " << scene.camera[Y] << ", " << scene.camera[Z] << ")." << endl;
		strcpy(scene.message, "D pressed. Watch carefully!");
		break;
	}
	case 'W':
	case 'w': {
		scene.camera_target[Y] += 0.05;
		scene.camera[Y] += 0.05;
		cout << fixed << setprecision(1) << "W pressed.\n\tPosition of camera is set to (" <<
			scene.camera[X] << ", " << scene.camera[Y] << ", " << scene.camera[Z] << ")." << endl;
		strcpy(scene.message, "W pressed. Watch carefully!");
		break;
	}
	case 'S':
	case 's': {
		scene.camera_target[Y] -= 0.05;
		scene.camera[Y] -= 0.05;
		cout << fixed << setprecision(1) << "S pressed.\n\tPosition of camera is set to (" <<
			scene.camera[X] << ", " << scene.camera[Y] << ", " << scene.camera[Z] << ")." << endl;
		strcpy(scene.message, "S pressed. Watch carefully!");
		break;
	}
	case 'Z':
	case 'z': {
		scene.camera_polar[R] *= 0.95;
		updateCamera();
		cout << fixed << setprecision(1) << "Z pressed.\n\tPosition of camera is set to (" <<
			scene.camera[X] << ", " << scene.camera[Y] << ", " << scene.camera[Z] << ")." << endl;
		strcpy(scene.message, "Z pressed. Camera is moved...nearer!");
		break;
	}
	case 'C':
	case 'c': {
		scene.camera_polar[R] *= 1.05;
		updateCamera();
		cout << fixed << setprecision(1) << "C pressed.\n\tPosition of camera is set to (" <<
			scene.camera[X] << ", " << scene.camera[Y] << ", " << scene.camera[Z] << ")." << endl;
		strcpy(scene.message, "C pressed. Camera is moved...farther!");
		break;
	}

			  //茶壶平移
	case 'L':
	case 'l': {
		if (scene.teapot[X] <= 2) {
			scene.teapot[X] += 0.2;
			cout << fixed << setprecision(1) << "L pressed.\n\tPosition of teapot is set to (" <<
				scene.teapot[X] << ", " << scene.teapot[Y] << ", " << scene.teapot[Z] << ")." << endl;
			strcpy(scene.message, "L pressed. Teapot is moved!");
			updateList();
		}
		else {
			cout << "L pressed.\n\tOut of border!" << endl;
			strcpy(scene.message, "L pressed. Teapot is at the border!");
		}
		break;
	}
	case 'J':
	case 'j': {
		if (scene.teapot[X] >= -2) {
			scene.teapot[X] -= 0.2;
			cout << fixed << setprecision(1) << "J pressed.\n\tPosition of teapot is set to (" <<
				scene.teapot[X] << ", " << scene.teapot[Y] << ", " << scene.teapot[Z] << ")." << endl;
			strcpy(scene.message, "J pressed. Teapot is moved!");
			updateList();
		}
		else {
			cout << "J pressed.\n\tOut of border!" << endl;
			strcpy(scene.message, "J pressed. Teapot is at the border!");
		}
		break;
	}
	case 'I':
	case 'i': {
		if (scene.teapot[Y] <= 1.5) {
			scene.teapot[Y] += 0.2;
			cout << fixed << setprecision(1) << "I pressed.\n\tPosition of teapot is set to (" <<
				scene.teapot[X] << ", " << scene.teapot[Y] << ", " << scene.teapot[Z] << ")." << endl;
			strcpy(scene.message, "I pressed. Teapot is moved!");
			updateList();
		}
		else {
			cout << "I pressed.\n\tOut of border!" << endl;
			strcpy(scene.message, "I pressed. Teapot is at the border!");
		}
		break;
	}
	case 'K':
	case 'k': {
		if (scene.teapot[Y] >= -1.5) {
			scene.teapot[Y] -= 0.2;
			cout << fixed << setprecision(1) << "K pressed.\n\tPosition of teapot is set to (" <<
				scene.teapot[X] << ", " << scene.teapot[Y] << ", " << scene.teapot[Z] << ")." << endl;
			strcpy(scene.message, "K pressed. Teapot is moved!");
			updateList();
		}
		else {
			cout << "K pressed.\n\tOut of border!" << endl;
			strcpy(scene.message, "K pressed. Teapot is at the border!");
		}
		break;
	}

			  // 聚光灯目标平移
	case 'T':
	case 't': {
		scene.spot_target[Z] -= 0.1;
		cout << fixed << setprecision(1) << "T pressed.\n\tTarget of spot light is set to (" <<
			scene.spot_target[X] << ", " << scene.spot_target[Y] << ", " << scene.spot_target[Z] << ")." << endl;
		strcpy(scene.message, "T pressed. What will happen to the spot light?");
		break;
	}
	case 'G':
	case 'g': {
		scene.spot_target[Z] += 0.1;
		cout << fixed << setprecision(1) << "G pressed.\n\tTarget of spot light is set to (" <<
			scene.spot_target[X] << ", " << scene.spot_target[Y] << ", " << scene.spot_target[Z] << ")." << endl;
		strcpy(scene.message, "G pressed. What will happen to the spot light?");
		break;
	}
	case 'F':
	case 'f': {
		scene.spot_target[X] -= 0.1;
		cout << fixed << setprecision(1) << "F pressed.\n\tTarget of spot light is set to (" <<
			scene.spot_target[X] << ", " << scene.spot_target[Y] << ", " << scene.spot_target[Z] << ")." << endl;
		strcpy(scene.message, "F pressed. What will happen to the spot light?");
		break;
	}
	case 'H':
	case 'h': {
		scene.spot_target[X] += 0.1;
		cout << fixed << setprecision(1) << "H pressed.\n\tTarget of spot light is set to (" <<
			scene.spot_target[X] << ", " << scene.spot_target[Y] << ", " << scene.spot_target[Z] << ")." << endl;
		strcpy(scene.message, "H pressed. What will happen to the spot light?");
		break;
	}

			  // 聚光灯圆锥角度
	case 'R':
	case 'r': {
		if (scene.spot_cutoff >= 1) {
			scene.spot_cutoff -= 0.1;
			cout << "R pressed.\n\tCut Off is set to " << scene.spot_cutoff << " degrees." << endl;
			strcpy(scene.message, "R pressed. The spot light will light less.");
		}
		else {
			cout << "R pressed.\n\tOut of range!" << endl;
			strcpy(scene.message, "R pressed. Cut off is at its minimum!");
		}
		break;
	}
	case 'Y':
	case 'y': {
		if (scene.spot_cutoff <= 179) {
			scene.spot_cutoff += 0.1;
			cout << "Y pressed.\n\tCut Off is set to " << scene.spot_cutoff << " degrees." << endl;
			strcpy(scene.message, "Y pressed. The spot light will light more.");
		}
		else {
			cout << "Y pressed.\n\tOut of range!" << endl;
			strcpy(scene.message, "Y pressed. Cut off is at its maximum!");
		}
		break;
	}
	}
}

void processSpecialKey(int k, int x, int y) {
	switch (k) {
		// 点光源位置相关操作
	case 100: {
		scene.point[X] -= 0.1;
		cout << fixed << setprecision(1) << "Left Arrow pressed.\n\tPosition of point light is set to (" <<
			scene.point[X] << ", " << scene.point[Y] << ", " << scene.point[Z] << ")." << endl;
		strcpy(scene.message, "Left Arrow pressed. Pay attention to the point light.");
		break;
	}
	case 101: {
		scene.point[Z] -= 0.1;
		cout << fixed << setprecision(1) << "Up Arrow pressed.\n\tPosition of point light is set to (" <<
			scene.point[X] << ", " << scene.point[Y] << ", " << scene.point[Z] << ")." << endl;
		strcpy(scene.message, "Up Arrow pressed. Pay attention to the point light.");
		break;
	}
	case 102: {
		scene.point[X] += 0.1;
		cout << fixed << setprecision(1) << "Right Arrow pressed.\n\tPosition of point light is set to (" <<
			scene.point[X] << ", " << scene.point[Y] << ", " << scene.point[Z] << ")." << endl;
		strcpy(scene.message, "Right Arrow pressed. Pay attention to the point light.");
		break;
	}
	case 103: {
		scene.point[Z] += 0.1;
		cout << fixed << setprecision(1) << "Down Arrow pressed.\n\tPosition of point light is set to (" <<
			scene.point[X] << ", " << scene.point[Y] << ", " << scene.point[Z] << ")." << endl;
		strcpy(scene.message, "Down Arrow pressed. Pay attention to the point light.");
		break;
	}
	case 104: {
		scene.point[Y] += 0.1;
		cout << fixed << setprecision(1) << "PageUp pressed.\n\tPosition of point light is set to (" <<
			scene.point[X] << ", " << scene.point[Y] << ", " << scene.point[Z] << ")." << endl;
		strcpy(scene.message, "PageUp pressed. Pay attention to the point light.");
		break;
	}
	case 105: {
		scene.point[Y] -= 0.1;
		cout << fixed << setprecision(1) << "PageDown pressed.\n\tPosition of point light is set to (" <<
			scene.point[X] << ", " << scene.point[Y] << ", " << scene.point[Z] << ")." << endl;
		strcpy(scene.message, "PageDown pressed. Pay attention to the point light.");
		break;
	}
	}
}
