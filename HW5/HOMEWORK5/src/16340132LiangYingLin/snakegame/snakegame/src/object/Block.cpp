#include "Block.h"

float bR = 47; float bG = 181; float bB = 243; //default block color

Block::Block(float pX, float pY, float pZ, float pSize) {
	x = pX;
	y = pY;
	z = pZ;
	size = pSize;
	//rgb colors are set to their default (initialized at the top)
	r = bR; g = bG; b = bB; 
}

Block::~Block(){

}

void Block::setX(float pX) { 
	x = pX; 
}

void Block::setY(float pY) { 
	y = pY; 
}

void Block::setZ(float pZ) { 
	z = pZ; 
}

float Block::getX() { 
	return x; 
}

float Block::getY() {
	return y; 
}

float Block::getZ() { 
	return z; 
}

void Block::setColor(float pR, float pG, float pB) {
	r = pR;g = pG;b = pB; 
}

//function to draw the block
void Block::draw(int type) { 
	glPushMatrix();
	glTranslatef(-x, y, z);
	if (r > 1 || g > 1 || b > 1) {
		if (type == 1) {
			glColor3ub(r, g, b);
		}
		else {
			glColor3ub(b, r, g);
		}
	}
	else {
		glColor3f(r, g, b);
	}
	glCallList(cube);
	glPopMatrix();
}