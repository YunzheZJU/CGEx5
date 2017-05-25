#pragma once

// Include related head files and IO classes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <iomanip>
// Include glut.h
#include <gl/glut.h>

// Using namespace std for cin and cout
using namespace std;

// Define shortcuts for coordinate in location array
#define X 0
#define Y 1
#define Z 2
// Define abbreviate for "radius" and "angle"
#define R 0
#define A 1
// Define Pi in math
#define Pi 3.1415926

// Declare height and width variables for size of new window
int wHeight = 0;							// Height of new window
int wWidth = 0;								// Width of new window

// Initiate message string to be shown
char message[70] = "Welcome!";				// Message string to be shown

// Initiate parameters for global and local rotating
GLfloat fRotate = 0.0f;						// Global roatating
GLfloat fTpRtt = 0.0f;						// Local rotating

// Initiate switch of the scene
GLboolean bpoint = GL_TRUE;					// Switch for point light
GLboolean bspot = GL_TRUE;					// Switch for spot light
GLboolean bPersp = GL_TRUE;					// Switch for perspective mode
GLboolean bWire = GL_FALSE;					// Switch for line mode
GLboolean bAnim = GL_FALSE;					// Switch for global rotating
GLboolean bRtt = GL_FALSE;					// Switch for local rotating

// Initiate location of elements
GLfloat teapot[] = { 0, 0, 0 };				// Position of teapot
GLfloat camera[] = { 0, 1, 4 };				// Position of camera
GLfloat camera_polar[] = { 4, 0 };			// Polar coordinates of camera
GLfloat camera_target[] = { 0, 1, 0 };		// Position of target of camera
GLfloat point[] = { 1, 1.75, 1, 1 };			// Position of point light
GLfloat spot[] = { 0, 3, 0, 1 };			// Position of spot light
GLfloat spot_target[] = { 0, 0, 0 };		// Position of target of spot light

// Initiate list num for display list
GLint List = 0;								// List num of display list

// Define colors for materials
GLfloat red[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat green[] = { 0.0f, 1.0f, 0.0f, 1.0f };
GLfloat blue[] = { 0.0f, 0.0f, 1.0f, 1.0f };
GLfloat yellow[] = { 1.0f, 1.0f, 0.0f, 1.0f };
GLfloat turquoise[] = { 0.0f, 1.0f, 1.0f, 1.0f };
GLfloat golden[] = { 1.0f, 0.843f, 0.0f, 1.0f };
GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };

// Initiate cut off degree for spot light
GLfloat spot_cutoff = 8.6f;					// Cut off degree of spot light
// Define colors for lights
GLfloat point_white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat point_red[] = { 1.0f, 0.2f, 0.2f, 1.0f };
GLfloat point_green[] = { 0.2f, 1.0f, 0.2f, 1.0f };
GLfloat point_blue[] = { 0.2f, 0.2f, 1.0f, 1.0f };
GLfloat point_yellow[] = { 1.0f, 1.0f, 0.2f, 1.0f };
// Initiate the color of point light
GLfloat* point_color = point_white;			// Pointer for current color of point light

// Define enum for menu items
enum {
	NOTHING,
	RED,
	GREEN,
	BLUE,
	YELLOW,
	DEFAULT,
	DISABLE,
	EXIT
};