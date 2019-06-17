#ifndef FRUIT
#define FRUIT
#include <GL/glut.h>
#include <SOIL.h>
#include <string>

class Fruit {
public:
	Fruit(int pX, int pZ);
	~Fruit();
	void draw();
	float getX();
	float getZ();
	void setX(float pX);
	void setZ(float pZ);
	float getAngle();
private:
	float x, z, angle;
};


#endif