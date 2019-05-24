#include "Draw.h"

Draw::Draw(){

}

Draw::~Draw(){

}

void Draw::buildDLs() { 
	cube = glGenLists(1);
	glNewList(cube, GL_COMPILE);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glVertex3f(0.5, 0.5, 0.5); //back face
	glVertex3f(-0.5, 0.5, 0.5);
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(0.5, -0.5, 0.5);
	glNormal3f(0, 0, -1);
	glVertex3f(0.5, 0.5, -0.5); //front face
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(0.5, -0.5, -0.5);
	glNormal3f(0, -1, 0); //bottom face
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(0.5, -0.5, 0.5);
	glVertex3f(0.5, -0.5, -0.5);
	glNormal3f(0, 1, 0); //top face
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(-0.5, 0.5, 0.5);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(0.5, 0.5, -0.5);
	glNormal3f(-1, 0, 0); //right face
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(-0.5, 0.5, 0.5);
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(-0.5, -0.5, -0.5);
	glNormal3f(1, 0, 0); //left face
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(0.5, -0.5, 0.5);
	glVertex3f(0.5, -0.5, -0.5);
	glEnd();
	glEndList();
}



void Draw::drawGrid() { //hardcoded values for grid...
	glColor3ub(24, 221, 0);
	glLineWidth(2);
	glBegin(GL_LINES);
	for (float i = -7; i <= 5; i++) {
		glVertex3f(-5 - 0.5, 0 - 0.5f, i + 0.5f);
		glVertex3f(6 - 0.5, 0 - 0.5f, i + 0.5f);
	}
	for (float i = -5; i <= 6; i++) {
		glVertex3f(i - 0.5, -0.5f, -6 - 0.5f);
		glVertex3f(i - 0.5, -0.5f, 6 - 0.5f);
	}
	glEnd();
	glLineWidth(1);
	glColor3ub(30, 2, 63);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(-5 - 0.5, -0.51, -6.5);
	glVertex3f(-5 - 0.5, -0.51, 6.5);
	glVertex3f(6 - 0.5, -0.51, 6.5);
	glVertex3f(6 - 0.5, -0.51, -6.5);
	glEnd();

	glColor3ub(30, 2, 63);
	for (int i = -5; i <= 5; i++) {
		glPushMatrix();
		glTranslatef(-6, 0, i);
		glCallList(cube);

		float cR = 0; float cG = 0; float cB = 0; //centre line color
		glColor3f(cR, cG, cB);
		glLineWidth(10);
		glBegin(GL_LINES);
		glVertex3f(0, 0.51, -0.5);
		glVertex3f(0, 0.51, 0.5);
		glEnd();
		glLineWidth(2);
		glBegin(GL_LINES);
		glVertex3f(0.5, 0.51, -0.5);
		glVertex3f(0.5, 0.51, 0.5);
		glVertex3f(-0.5, 0.51, -0.5);
		glVertex3f(-0.5, 0.51, 0.5);
		glEnd();
		glColor3ub(30, 2, 63);


		glPopMatrix();
		glPushMatrix();
		glTranslatef(6, 0, i);
		glCallList(cube);

		glColor3f(cR, cG, cB);
		glLineWidth(10);
		glBegin(GL_LINES);
		glVertex3f(0, 0.51, -0.5);
		glVertex3f(0, 0.51, 0.5);
		glEnd();
		glLineWidth(2);
		glBegin(GL_LINES);
		glVertex3f(0.5, 0.51, -0.5);
		glVertex3f(0.5, 0.51, 0.5);
		glVertex3f(-0.5, 0.51, -0.5);
		glVertex3f(-0.5, 0.51, 0.5);
		glEnd();
		glColor3ub(30, 2, 63);

		glPopMatrix();
		glPushMatrix();
		glTranslatef(i, 0, -6);
		glCallList(cube);

		glColor3f(cR, cG, cB);
		glLineWidth(10);
		glBegin(GL_LINES);
		glVertex3f(-0.5, 0.51, 0);
		glVertex3f(0.5, 0.51, 0);
		glEnd();
		glLineWidth(2);
		glBegin(GL_LINES);
		glVertex3f(-0.5, 0.51, 0.5);
		glVertex3f(0.5, 0.51, 0.5);
		glVertex3f(-0.5, 0.51, -0.5);
		glVertex3f(0.5, 0.51, -0.5);
		glEnd();
		glColor3ub(30, 2, 63);

		glPopMatrix();
		glPushMatrix();
		glTranslatef(i, 0, 6);
		glCallList(cube);

		glColor3f(cR, cG, cB);
		glLineWidth(10);
		glBegin(GL_LINES);
		glVertex3f(-0.5, 0.51, 0);
		glVertex3f(0.5, 0.51, 0);
		glEnd();
		glLineWidth(2);
		glBegin(GL_LINES);
		glVertex3f(-0.5, 0.51, 0.5);
		glVertex3f(0.5, 0.51, 0.5);
		glVertex3f(-0.5, 0.51, -0.5);
		glVertex3f(0.5, 0.51, -0.5);
		glEnd();
		glColor3ub(30, 2, 63);

		glPopMatrix();
	}
}