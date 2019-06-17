#ifndef DRAW
#define DRAW
#include <GL/glut.h>
#include "../sys.h"
#include "../object/Snake.h"



class Draw
{
public:
	Draw();
	~Draw();
	void buildDLs();
	void drawGrid();
};

#endif
