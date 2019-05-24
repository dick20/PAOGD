# 中山大学数据科学与计算机学院本科生实验报告

## （2019年春季学期）

| 课程名称 |    PAOGD    |   任课老师   |           郑贵锋           |
| :------: | :---------: | :----------: | :------------------------: |
|   年级   |     16      | 专业（方向） | 软件工程（计算机应用方向） |
|   学号   |  16340132   |     姓名     |           梁颖霖           |
|   电话   | 13680473185 |    Email     |        dic0k@qq.com        |
| 开始日期 |  2019/5/20  |   完成日期   |         2019/5/24          |

------

## 一、实验题目

**HW4 利用OpenGL设计贪吃蛇游戏**

## 二、实现内容

#### 任务介绍
- 贪吃蛇游戏：玩家控制贪吃蛇在游戏区域里驰骋，避免碰到自己或障碍物，尽可能地吃更多的食物以生长！

##### 游戏玩法：
- WASD控制蛇的移动
- 游戏开始，每隔一定时间会在地图空闲位置刷新一个食物，蛇触碰到食物后食物消失，蛇会增加一个单位的长度
- 当蛇触碰到自己或者障碍物，则游戏失败
- 当蛇接触到地图边界，蛇会在地图另一端重新进入地图

#### 开发环境
- OpenGL3
- GLFW
- GLAD

#### 要求：
1. 完成贪吃蛇游戏的框架搭建（60%）
2. 完成蛇以及食物的 **3D** 精灵加载和渲染，其中模型可以用简单的纯色几何模型实现如立方体、球体；或者网上下载合适的3D模型如[Apple-Poly](https://poly.google.com/view/5hRReRDr0v4)、[Snake-Poly](https://poly.google.com/view/2ovwPNrRijL)（20%）
3. 实现蛇的控制（20%）
4. Bonus：蛇的碰撞检测与响应

#### 参考资料

1. [OpenGL游戏-框架设计](https://learnopengl.com/In-Practice/2D-Game/Setting-up)
2. [OpenGL游戏-精灵渲染](https://learnopengl.com/In-Practice/2D-Game/Rendering-Sprites)
3. [OpenGL游戏-碰撞检测](https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-detection)
4. [OpenGL模型创建](https://learnopengl.com/Getting-started/Hello-Triangle)
5. [Google Poly-3D模型库](https://poly.google.com)

## 三、实验结果

#### 1. 蛇的身体组成基类Block

这里主要是被Snake类所调用，本质是建立一个正方体，里面可以设置这个正方体的大小，颜色，并且最重要要定义draw函数，当snake的draw被调用的时候，实际上就是再调用block的draw。

这里这给出头文件的函数定义，其实现也比较简单

```c++
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
```

其中的draw函数

```c++
//function to draw the block
void Block::draw() { 
	glPushMatrix();
	glTranslatef(-x, y, z);
	if (r > 1 || g > 1 || b > 1) {
		glColor3ub(r, g, b);
	}
	else {
		glColor3f(r, g, b);
	}
	glCallList(cube);
	glPopMatrix();
}
```

#### 2. 蛇的基类Snake

蛇所要做的操作，包括对位置的判断，是否更新，如何增加身体的长度。还需要一个draw函数，描绘蛇的形状。

```c++

class Snake { //contains the logic for the snake movement and collision
public:
	std::vector<Block*> blocks; //snake is just an array of blocks

	Snake(float startX, float startZ, int blockCount);
	~Snake();
	void draw();
    // 移动
	void move(Direction direction);
    // 添加蛇的身体长度
	void pushSnake();
    // 更新
	void update(Fruit* fruit, Stone* stones[]);
    // 碰撞检测，检查是否吃食物，或者碰到石头
	void collisionDetection(Fruit* fruit, Stone* stones[]);
	void addBlock();
	Direction getDirection();

private:
	Direction currentDirection;
	bool isUpdated;
	int score;
};
```

先说draw函数，调用的是蛇的身体block的draw函数

```c++
void Snake::draw() { 
	for (int i = 0; i <= blocks.size() - 1; i++) {
		Block* temp = blocks.at(i);
		temp->draw();
	}
}
```

蛇的移动，需要判断当前的方向，以及玩家操纵的方向。

判断完成后，改变蛇头的位置，并将蛇的身体附加到蛇头的后面

```c++
void Snake::move(Direction direction) { 
	Block* snakeHead = blocks.at(0);
	if (direction == D_LEFT && currentDirection != D_RIGHT) {
		pushSnake();
		snakeHead->setX(snakeHead->getX() - 1.0f);
		currentDirection = direction;
		isUpdated = true;
	}
	else if (direction == D_RIGHT && currentDirection != D_LEFT) {
		pushSnake();
		snakeHead->setX(snakeHead->getX() + 1.0f);
		currentDirection = direction;
		isUpdated = true;
	}
	else if (direction == D_UP && currentDirection != D_DOWN) {
		pushSnake();
		snakeHead->setZ(snakeHead->getZ() + 1.0f);
		currentDirection = direction;
		isUpdated = true;
	}
	else if (direction == D_DOWN && currentDirection != D_UP) {
		pushSnake();
		snakeHead->setZ(snakeHead->getZ() - 1.0f);
		currentDirection = direction;
		isUpdated = true;
	}
};
```

完成上述函数就可以实现蛇的移动

下面要做的是碰撞检测，检测蛇头的位置是否为食物，或者建筑物。并且还要对蛇头是否到达边缘进行判断，实现穿越边界从另一边出来的功能需求。

```c++
void Snake::collisionDetection(Fruit* fruit, Stone* stones[]) {
	float x = blocks.at(0)->getX();
	float z = blocks.at(0)->getZ();
	// 检测食物
	if (fruit->getX() == x && fruit->getZ() == z) { 
		score++;
		printf("Score = %i\n", score);
		bool repeat = false;
		// 避免食物生成是在蛇的身体上
		do { 
			repeat = false;
			fruit->setX(rand() % 11 - 5);
			fruit->setZ(rand() % 11 - 5);
			for (int i = 0; i <= blocks.size() - 1;i++) {
				x = blocks.at(i)->getX();
				z = blocks.at(i)->getZ();
				if (fruit->getX() == x && fruit->getZ() == z)
					repeat = true;
			}
		} while (repeat);
		addBlock();
	}
	// 判断蛇头是否接触到边界
	Block* snakeHead = blocks.at(0);
	if (-6 >= x && currentDirection == D_LEFT) {
		pushSnake();
		snakeHead->setX(6.0f);
		currentDirection = D_LEFT;
		isUpdated = true;
	}
	else if (x >= 6 && currentDirection == D_RIGHT) {
		pushSnake();
		snakeHead->setX(-6.0f);
		currentDirection = D_RIGHT;
		isUpdated = true;
	}
	else if (z >= 6 && currentDirection == D_UP) {
		pushSnake();
		snakeHead->setZ(-6.0f);
		currentDirection = D_UP;
		isUpdated = true;
	}
	else if (z <= -6 && currentDirection == D_DOWN) {
		pushSnake();
		snakeHead->setZ(6.0f);
		currentDirection = D_DOWN;
		isUpdated = true;
	}
	// 检查蛇是否碰撞到了石头，如果是则结束游戏
	for (int i = 0; i < 1; i++) {
		if (stones[i]->getX() == x && stones[i]->getZ() == z) {
			printf("Stone collision. You loss!\n");
			// exit(0); 
			glutHideWindow();
		}
	}
	// 检查蛇是否碰撞到了自己的身体，如果是则结束游戏
	for (int i = 1; i <= blocks.size() - 1; i++) { 
		if (blocks.at(i)->getX() == blocks.at(0)->getX() && blocks.at(i)->getZ() == blocks.at(0)->getZ()) {
			printf("Body collision. You loss!\n"); 
			glutHideWindow();
			blocks.at(i)->setColor(1, 0, 0); 
		}
	}
};
```

#### 3. 食物的基类Fruit与障碍物的基类Stone

这两个类也类似，其区别只是在于如何draw显示。我将石头显示成正方体，将食物显示成三棱锥。

```c++
class Stone {
public:
	Stone(int pX, int pZ);
	~Stone();
	void draw();
	float getX();
	float getZ();
	void setX(float pX);
	void setZ(float pZ);
	float getAngle();
private:
	float x, z, angle;
};
```

Stone的draw函数

```c++
void Stone::draw() {
	angle += 2.0f;
	glPushMatrix();
	glTranslatef(-x, 0, z);
	glColor3f(0, 1, 1);
	glutSolidCube(0.9);
}
```

Fruit的draw函数

```c++

void Fruit::draw() {
	angle += 2.0f;
	glPushMatrix();
	glTranslatef(-x, 0, z);
	glRotatef(angle, 0, 1, 0);
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
```



#### 4.画图的基类Draw

该类用于画出贪吃蛇的运动区域，为一个11*11的矩形，并且画出边界的正方体线。

这里比较简单，只是循环布置正方体即可，就不放代码。

#### 5. 主函数Main

##### 键位回调函数

使用wsad来控制蛇的上下左右移动。

```c++
void keyEvents(unsigned char key, int x, int y) { 
	switch (key) {
	case 27: 
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
```

##### 窗口大小改变

这里定义resize函数，当游戏窗口在改变大小的时候，保证游戏界面的横纵比保持一定，不会出现变形的情况，也可以用作全屏展示。

```c++
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
```

#### 绘制

首先要定义出食物，蛇，石头，然后分别调用这些基类的draw函数来画图

```c++
Fruit* fruit = new Fruit(1, 1);

Snake* snake = new Snake(0, 0, 3);

// 这里以后可以设置关卡，定义不同的石头位置
Stone* stone1 = new Stone(2, 2);
Stone* stones[1] = { stone1 };

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
```

##### 主函数

定义窗口的大小，位置，标题等信息，绑定函数

```c++
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
```

#### 实验截图

实验截图如下：

粉红色三棱锥为果实，蓝色正方体为障碍物。绿色正方体为蛇的头部。蛇在接触果实后会自动增长一个格子，碰到障碍物的时候，游戏结束，命令行显示游戏结束。

### ![demo1](C:\Users\asus\Desktop\demo1.gif)

第二个截图演示的是，蛇在碰到自己身体后，游戏结束的情形。

![demo2](C:\Users\asus\Desktop\demo2.gif)

### 四、实验感想

​	本周的贪吃蛇任务功能并不复杂，主要的难度在于如何使用opengl来绘制一个3d的蛇，以及食物。这里只是简单的使用正方体、三棱锥以及不同的颜色来代表不同的物品。关于碰撞检测方面，我是利用坐标来进行判断，获取蛇头的坐标与地图中食物、石头、身体的这些坐标进行比较，在每一次更新的时候进行检测。在更新动画方面，我也是以蛇头为主，身体跟随蛇头来进行绘制，绘制蛇的身体其实就是绘制多个正方体，只需要按顺序根据蛇头的坐标进行绘制即可。