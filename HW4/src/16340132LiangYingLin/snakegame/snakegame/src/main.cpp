#include <GL/glut.h>
#include <SOIL.h>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <string>

#include "object/Fruit.h"
#include "object/Block.h"
#include "object/Snake.h"
#include "object/Stone.h"

#include "draw/Draw.h"
#include "sys.h"

float yCamPos = 13.0f;
GLfloat lightAmbient[] = { 0.5,0.5,0.5,1 };
GLfloat lightDiffuse[] = { 1,1,1,1 };
GLfloat lightPosition[] = { 0,3,0 };

Fruit* fruit = new Fruit(1, 1);

Snake* snake = new Snake(0, 0, 3);

Stone* stone1 = new Stone(2, 2);
Stone* stones[1] = { stone1 };

void resize(int w, int h) { //function called on resize of window
	if (h == 0)
		h = 1;
	float ratio = w * 1.0f / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(50, ratio, 1, 200);
	glMatrixMode(GL_MODELVIEW);
}

void keyEvents(unsigned char key, int x, int y) { //function called when normal keys are pressed
	switch (key) {
	case 27: //escape key
		exit(0);
		break;
	case 'a':
		snake->move(D_LEFT);
		break;
	case 'd':
		snake->move(D_RIGHT);
		break;
	case 'w':
		snake->move(D_UP);
		break;
	case 's':
		snake->move(D_DOWN);
		break;
	}
}

void specialKeys(int key, int x, int y) { //function called when special keys are pressed
	switch (key) {
	case GLUT_KEY_F1:
		exit(0);
		break;
	}
}

void initGL(void) {
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
}

void mouseEvents(int button, int dir, int x, int y) { //function called on mouse events
	switch (button) {
	case 3: //scroll up and zoom in
		printf("zoom in\n");
		yCamPos -= 0.5;
		break;
	case 4: //scroll down and zoom out
		printf("zoom out\n");
		yCamPos += 0.5;
		break;
	}
}

void draw() {
	snake->update(fruit, stones); //update snake position
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(
		0, yCamPos, -5, //eye
		0, 0, 0,  //centre
		0, 1, 0   //up
	);
	Draw d;
	d.drawGrid();
	snake->draw();
	fruit->draw();
	
	// build stones
	stone1->draw();

	glutSwapBuffers();
}

int main(int argc, char **argv) {
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitWindowPosition(-1, -1);
	glutInitWindowSize(800, 600);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow("Snake Game");

	initGL();
	Draw d;
	d.buildDLs();

	glutDisplayFunc(draw);
	glutIdleFunc(draw);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyEvents);
	glutSpecialFunc(specialKeys);
	glutMouseFunc(mouseEvents);

	glutMainLoop();

	return 1;
}
