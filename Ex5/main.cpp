// main.cpp : 定义控制台应用程序的入口点。
//

#include "head.h"

#pragma warning(disable:4996)

Myscene scene;

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480, 480);
	int windowHandle = glutCreateWindow("Ex 5");

	// Initiate the Menu
	initMenu();
	// Initiate the display list
	scene.List = genTableList();
	// Initiate the Lights
	initLight();
	initPoint();
	initSpot();

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutMouseFunc(processMouse);
	glutKeyboardFunc(processNormalKey);
	glutSpecialFunc(processSpecialKey);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}