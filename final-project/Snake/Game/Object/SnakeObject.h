#ifndef SNAKE_OBJECT_H
#define SNAKE_OBJECT_H

#include "GameObject.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../resource_manager/resource_manager.h"
#include "../sprite_renderer/sprite_renderer.h"

#include <deque>
#include <vector>
using namespace std;

struct snake_node {
	float x;
	float z;
	int direction;
	snake_node() { x = 0; z = 0; direction = 0; }
	snake_node(float x1, float x2, int dir2) :x(x1), z(x2), direction(dir2){}
};

class SnakeObject : public GameObject {
private:
	deque<snake_node> nodes;
	deque<snake_node> next_nodes;
	float speed;
	int direction;
	GLfloat rotate4[4] = { glm::radians(180.0f), glm::radians(0.0f), glm::radians(-90.0f), glm::radians(90.0f) };
	glm::vec2 foodPos;
	vector<glm::vec2> stonePosVector;
	bool dead;
	float bounds[4];
public:
	SnakeObject(int x, int z);
	void setDirection(int d);
	void Move();	//move to direction
	int getLength();
	void Draw(SpriteRenderer &renderer, Texture2D texture_body, Texture2D texture_head);
	void DrawModel(SpriteRenderer &renderer, Model model_head, Model model_body, int frame_count);
	void setFoodPos(glm::vec2 p);
	void setStonePos(vector<glm::vec2> p);
	deque<snake_node> getNextNodes();
	bool isDead();
	void setBounds(float top, float bottom, float left, float right);
};

#endif