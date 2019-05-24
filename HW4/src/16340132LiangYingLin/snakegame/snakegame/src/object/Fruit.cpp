#include "Fruit.h"

Fruit::Fruit(int pX, int pZ) {
	x = pX;
	z = pZ;
	angle = 0.0f;
}

Fruit::~Fruit(){

}

void Fruit::draw() {
	angle += 2.0f;
	glPushMatrix();
	glTranslatef(-x, 0, z);
	//glRotatef(45,0,0,1);
	glRotatef(angle, 0, 1, 0);
	//glutSolidCube(1);

	float gC = 0.5;
	glColor3f(128, 1, 1);
	glBegin(GL_TRIANGLES);
	glVertex3f(0 - gC, 0 - gC, 0 - gC);
	glVertex3f(1 - gC, 0 - gC, 0 - gC);
	glVertex3f(0.5 - gC, 0 - gC, 1 - gC);
	glTexCoord2f(0.5, 1);glVertex3f(0.5 - gC, 1 - gC, 0.5 - gC);
	glTexCoord2f(0, 0); glVertex3f(0 - gC, 0 - gC, 0 - gC);
	glTexCoord2f(1, 0); glVertex3f(1 - gC, 0 - gC, 0 - gC);
	glTexCoord2f(0.5, 1);glVertex3f(0.5 - gC, 1 - gC, 0.5 - gC);
	glTexCoord2f(1, 0); glVertex3f(0 - gC, 0 - gC, 0 - gC);
	glTexCoord2f(0, 0); glVertex3f(0.5 - gC, 0 - gC, 1 - gC);
	glTexCoord2f(0.5, 1);glVertex3f(0.5 - gC, 1 - gC, 0.5 - gC);
	glTexCoord2f(0, 0); glVertex3f(1 - gC, 0 - gC, 0 - gC);
	glTexCoord2f(1, 0); glVertex3f(0.5 - gC, 0 - gC, 1 - gC);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

float Fruit::getX() {
	return x; 
}

float Fruit::getZ() { 
	return z; 
}

void Fruit::setX(float pX) {
	x = pX; 
}

void Fruit::setZ(float pZ) {
	z = pZ; 
}

float Fruit::getAngle() {
	return angle; 
}