#include "Stone.h"

Stone::Stone(int pX, int pZ) {
	x = pX;
	z = pZ;
	angle = 0.0f;
}

Stone::~Stone(){

}

void Stone::draw() {
	angle += 2.0f;
	glPushMatrix();
	glTranslatef(-x, 0, z);
	glColor3f(0, 1, 1);
	glutSolidCube(0.9);
}

float Stone::getX() {
	return x; 
}

float Stone::getZ() {
	return z; 
}

void Stone::setX(float pX) {
	x = pX; 
}

void Stone::setZ(float pZ) {
	z = pZ; 
}

float Stone::getAngle() {
	return angle; 
}