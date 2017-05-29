// main.cpp : 定义控制台应用程序的入口点。
//

#include "head.h"

#pragma warning(disable:4996)

Myscene scene;

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutInitWindowSize(480, 480);
	int windowHandle = glutCreateWindow("Ex 5");

	GLint buf, sbuf;

	glGetIntegerv(GL_SAMPLE_BUFFERS, &buf);
	printf("number of sample buffers is %d\n", buf);
	glGetIntegerv(GL_SAMPLES, &sbuf);
	printf("number of samples is %d\n", sbuf);

	glDisable(GL_MULTISAMPLE);

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
	glutKeyboardFunc(processNormalKey);
	glutSpecialFunc(processSpecialKey);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}