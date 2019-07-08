#include "Game.h"
#include "resource_manager/resource_manager.h"
#include "../Game/sprite_renderer/sprite_renderer.h"
#include <sstream>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>


SpriteRenderer  *Renderer;
SpriteRenderer  *lightRenderer;

SnakeGame::SnakeGame(GLuint width, GLuint height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

SnakeGame::~SnakeGame()
{
	delete Renderer;
	delete lightRenderer;
}

void SnakeGame::Init()
{
	// rand number
	srand((int)time(0));
	// init snake object
	snakeObj1 = new SnakeObject(0,0);
	snakeObj2 = new SnakeObject(10,5);
	// right
	direction1 = 3;
	direction2 = 3;
	snakeObj1->setDirection(direction1);
	snakeObj2->setDirection(direction2);
	// init bounds
	snakeObj1->setBounds(-15.0f, 11.0f, -21.0f, 21.0f);
	snakeObj2->setBounds(-15.0f, 11.0f, -21.0f, 21.0f);

	boundsBottom = -10.0f;
	boundsTop = 10.0f;
	boundsLeft = -15.0f;
	boundsRight = 15.0f;

	lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	lightPos = glm::vec3(5.0f, 30.0f, 5.0f);
	viewPos = glm::vec3(0.0f, 40.0f, 30.0f);

	currentLength = snakeObj1->getLength();
	foodPos = glm::vec2(0.0f);
	hasInput1 = false;
	hasInput2 = false;

	// Load shaders
	ResourceManager::LoadShader("Game/shaders/sprite.vs", "Game/shaders/sprite.frag", nullptr, "sprite");
	ResourceManager::LoadShader("Game/shaders/lightObject.vs", "Game/shaders/lightObject.frag", nullptr, "lightObject");

	// Configure shaders
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	//projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	projection = glm::perspective(glm::radians(45.0f), static_cast<GLfloat>(this->Width) / static_cast<GLfloat>(this->Height), 0.1f, 100.0f);
	view = glm::lookAt(viewPos, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	ResourceManager::GetShader("sprite").SetMatrix4("view", view);

	ResourceManager::GetShader("lightObject").Use().SetInteger("image", 0);
	ResourceManager::GetShader("lightObject").SetMatrix4("projection", projection);
	ResourceManager::GetShader("lightObject").SetMatrix4("view", view);
	ResourceManager::GetShader("lightObject").SetVector3f("lightColor", lightColor);
	ResourceManager::GetShader("lightObject").SetVector3f("lightPos", lightPos);
	ResourceManager::GetShader("lightObject").SetVector3f("viewPos", viewPos);


	// Load models
	ResourceManager::LoadModel("resources/food/Hamburger.obj", "food");
	ResourceManager::LoadModel("resources/body/PUSHILIN_boulder.obj", "body");
	ResourceManager::LoadModel("resources/head/model.obj", "head");
	ResourceManager::LoadModel("resources/swimming_pool/SwimmingPool.obj", "floor");
	ResourceManager::LoadModel("resources/stone/boulder_03.obj", "stone");

	// Set render-specific controls
	Shader spiritShader = ResourceManager::GetShader("sprite");
	Shader lightShader = ResourceManager::GetShader("lightObject");
	Renderer = new SpriteRenderer(spiritShader);
	Renderer->initRenderData();
	lightRenderer = new SpriteRenderer(lightShader);

	// createFood();
	createStone();
}

void SnakeGame::Update()
{
	if (this->State == GAME_ACTIVE && hasInput1) {
		snakeObj1->Move();
	}
	else if (this->State == GAME_ACTIVE && hasInput2) {
		snakeObj2->Move();
	}

	if (snakeObj1->getLength() > currentLength) {
		currentLength = snakeObj1->getLength();
		// createFood();
	}

	if (snakeObj1->isDead()) {
		this->State = GAME_DEATH;
	}
}


bool SnakeGame::ProcessInput(int frame_count)
{
	if (frame_count == 0) {
		hasInput1 = false;
		hasInput2 = false;
	}

	if (this->State == GAME_ACTIVE && !hasInput1) {
		// 坦克可以四个方向移动
		if (this->Keys[GLFW_KEY_W] && !hasInput1) {
			// if (direction == 1) return false;
			direction1 = 0;
			hasInput1 = true;
		}
		if (this->Keys[GLFW_KEY_S] && !hasInput1) {
			// if (direction == 0) return false;
			direction1 = 1;
			hasInput1 = true;
		}
		if (this->Keys[GLFW_KEY_A] && !hasInput1) {
			// if (direction == 3) return false;
			direction1 = 2;
			hasInput1 = true;
		}
		if (this->Keys[GLFW_KEY_D] && !hasInput1) {
			// if (direction == 2) return false;
			direction1 = 3;
			hasInput1 = true;
		}

	}
	snakeObj1->setDirection(direction1);

	// 坦克2
	if (this->State == GAME_ACTIVE && !hasInput2) {
		// 坦克可以四个方向移动
		if (this->Keys[GLFW_KEY_I] && !hasInput2) {
			// if (direction == 1) return false;
			direction2 = 0;
			hasInput2 = true;
		}
		if (this->Keys[GLFW_KEY_K] && !hasInput2) {
			// if (direction == 0) return false;
			direction2 = 1;
			hasInput2 = true;
		}
		if (this->Keys[GLFW_KEY_J] && !hasInput2) {
			// if (direction == 3) return false;
			direction2 = 2;
			hasInput2 = true;
		}
		if (this->Keys[GLFW_KEY_L] && !hasInput2) {
			// if (direction == 2) return false;
			direction2 = 3;
			hasInput2 = true;
		}

	}
	snakeObj2->setDirection(direction2);
	return hasInput1 || hasInput2;
}

void SnakeGame::createFood() {
	deque<snake_node> nodes = snakeObj1->getNextNodes();
	bool goodPos = 0;
	glm::vec2 p = glm::vec2(0.0f);

	while (!goodPos) {
		int x = rand() % 41 - 20;
		int z = rand() % 21 - 10;
		for (int i = 0; i < nodes.size(); i++) {
			if (x != int(nodes.at(i).x) && z != int(nodes.at(i).z)) {
				goodPos = 1;
				break;
			}
		}
		p = glm::vec2(x, z);
	}

	foodPos = p;
	snakeObj1->setFoodPos(p);
}

void SnakeGame::createStone() {
	deque<snake_node> nodes1 = snakeObj1->getNextNodes();
	deque<snake_node> nodes2 = snakeObj1->getNextNodes();
	bool goodPos = 0;

	int stoneCount = 15;
	for (int i = 0; i < stoneCount; i++) {
		glm::vec2 p;
		int x = rand() % 41 - 20;
		int z = rand() % 21 - 10;
		// 暂时不判断重复

		/*for (int i = 0; i < nodes1.size(); i++) {
			if ((x != int(nodes1.at(i).x) && z != int(nodes1.at(i).z)) &&
				(x != int(nodes2.at(i).x) && z != int(nodes2.at(i).z))) {
				goodPos = 1;
				break;
			}
		}*/
		p = glm::vec2(x, z);
		stonePosVector.push_back(p);
	}
	
	snakeObj1->setStonePos(stonePosVector);
}

void SnakeGame::Render(int frame_count)
{
	// get models
	// Model model_food = ResourceManager::GetModel("food");
	Model model_body = ResourceManager::GetModel("body");
	Model model_head = ResourceManager::GetModel("head");
	Model model_floor = ResourceManager::GetModel("floor");
	Model model_stone = ResourceManager::GetModel("stone");
	
	// render floor
	lightRenderer->DrawSpriteModel(model_floor, glm::vec3(0.0, -10.0, -5.0), glm::vec3(1.1f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// render snake
	snakeObj1->DrawModel(*lightRenderer, model_head, model_body, frame_count);
	snakeObj2->DrawModel(*lightRenderer, model_head, model_body, frame_count);

	// render food
	// lightRenderer->DrawSpriteModel(model_food, glm::vec3(foodPos.x, 1.0, foodPos.y), glm::vec3(0.2f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// render stone
	for (int i = 0; i < stonePosVector.size(); i++) {
		// cout << stonePosVector[i].x << endl;
		lightRenderer->DrawSpriteModel(model_stone, glm::vec3(stonePosVector[i].x, 1.0, stonePosVector[i].y), glm::vec3(0.01f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
}