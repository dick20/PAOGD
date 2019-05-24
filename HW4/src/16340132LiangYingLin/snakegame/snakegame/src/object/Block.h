#ifndef BLOCK
#define BLOCK
#include <GL/glut.h>
#include "../sys.h"

class Block { 
public:
	Block(float pX, float pY, float pZ, float pSize = 1.0f);
	~Block();
	void setX(float pX);
	void setY(float pY);
	void setZ(float pZ);
	float getX();
	float getY();
	float getZ();
	void setColor(float pR, float pG, float pB); 
	void draw();

private:
	float x, y, z, size, r, g, b;
};

#endif