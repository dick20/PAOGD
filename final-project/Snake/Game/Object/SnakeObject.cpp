#include "SnakeObject.h"

SnakeObject::SnakeObject() {
	speed = 1.0f;
	foodPos = glm::vec2(0.0f);
	stonePos = glm::vec2(0.0f);
	dead = false;

	for (float i = 0.0f; i <= 6.0f; i += 1.0f) {
		snake_node node;
		node.x = i;
		node.z = 0.0f;
		node.direction = 3;
		nodes.push_front(node);
		next_nodes.push_front(node);
	}
}

void SnakeObject::setDirection(int d) {
	direction = d;
}

deque<snake_node> SnakeObject::getNextNodes() {
	return next_nodes;
}

void SnakeObject::Move(GLfloat dt) {
	nodes = next_nodes;

	if (nodes.at(0).x == foodPos.x && nodes.at(0).z == foodPos.y) {
		next_nodes.push_back(nodes.at(nodes.size() - 1));
		nodes.push_back(nodes.at(nodes.size() - 1));
	}

	if (nodes.at(0).x == stonePos.x && nodes.at(0).z == stonePos.y) {
		dead = true;
		return;
	}

	for (int i = 1; i < nodes.size(); i++) {
		if (nodes.at(0).x == nodes.at(i).x && nodes.at(0).z == nodes.at(i).z) {
			dead = true;
			return;
		}
	}

	next_nodes.clear();

	snake_node new_head;
	float x = nodes.at(0).x;
	float z = nodes.at(0).z;

	switch (direction) {
	case 0:
		z -= speed;
		break;
	case 1:
		z += speed;
		break;
	case 2:
		x -= speed;
		break;
	case 3:
		x += speed;
		break;
	default:
		break;
	}

	z = z > bounds[1] ? bounds[0] : z;
	z = z < bounds[0] ? bounds[1] : z;
	x = x > bounds[3] ? bounds[2] : x;
	x = x < bounds[2] ? bounds[3] : x;

	new_head.direction = direction;
	new_head.x = x;
	new_head.z = z;

	for (int i = 0; i < nodes.size() - 1; i++) {
		next_nodes.push_back(nodes.at(i));
	}

	next_nodes.push_front(new_head);
	
}

void SnakeObject::setFoodPos(glm::vec2 p) {
	foodPos = p;
}

void SnakeObject::setStonePos(glm::vec2 p) {
	stonePos = p;
}

bool SnakeObject::isDead() {
	return dead;
}

int SnakeObject::getLength() {
	return nodes.size();
}

void SnakeObject::setBounds(float top, float bottom, float left, float right) {
	bounds[0] = top;
	bounds[1] = bottom;
	bounds[2] = left;
	bounds[3] = right;
}

void SnakeObject::Draw(SpriteRenderer &renderer, Texture2D texture_body, Texture2D texture_head) {
	float head_x = nodes.at(0).x;
	float head_z = nodes.at(0).z;
	renderer.DrawSprite(texture_head, glm::vec3(head_x, 0.0, head_z), glm::vec3(1.0f), 45.0f, glm::vec3(0.0f, 0.0f, 0.0f));
}

void SnakeObject::DrawModel(SpriteRenderer &renderer, Model model_head, Model model_body, int frame_count) {
	for (int i = 0; i < nodes.size(); i++) {
		float x = nodes.at(i).x;
		float x1 = next_nodes.at(i).x;
		float z = nodes.at(i).z;
		float z1 = next_nodes.at(i).z;
		int d = nodes.at(i).direction;
		int d1 = next_nodes.at(i).direction;

		x1 = (x1 - x) > 2 ? x : x1;
		x1 = (x1 - x) < -2 ? x : x1;
		z1 = (z1 - z) > 2 ? z : z1;
		z1 = (z1 - z) < -2 ? z : z1;

		float move_x = x + (x1 - x) / 35 * frame_count;
		float move_z = z + (z1 - z) / 35 * frame_count;

		float temp = rotate4[d1] - rotate4[d];
		if (temp > glm::radians(180.0f)) {
			temp = glm::radians(-90.0f);
		}
		else if (temp < glm::radians(-180.0f)) {
			temp = glm::radians(90.0f);
		}
		float rotate_offset = rotate4[d] + temp / 35 * frame_count;

		if (i == 0) {
			renderer.DrawSpriteModel(model_head, glm::vec3(move_x, 2.0, move_z), glm::vec3(5.0f), rotate_offset, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else {
			renderer.DrawSpriteModel(model_body, glm::vec3(move_x, 1.0, move_z), glm::vec3(1.0f), rotate_offset, glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}
}