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
	snakeObj = new SnakeObject();
	// right
	direction = 3;
	snakeObj->setDirection(direction);
	// init bounds
	snakeObj->setBounds(-15.0f, 11.0f, -21.0f, 21.0f);

	boundsBottom = -10.0f;
	boundsTop = 10.0f;
	boundsLeft = -15.0f;
	boundsRight = 15.0f;

	lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	lightPos = glm::vec3(5.0f, 30.0f, 5.0f);
	viewPos = glm::vec3(0.0f, 40.0f, 30.0f);

	currentLength = snakeObj->getLength();
	foodPos = glm::vec2(0.0f);
	stonePos = glm::vec2(0.0f);
	hasInput = false;

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

	createFood();
	createStone();
}

void SnakeGame::Update(GLfloat dt)
{
	if (this->State == GAME_ACTIVE) {
		snakeObj->Move(dt);
	}

	if (snakeObj->getLength() > currentLength) {
		currentLength = snakeObj->getLength();
		createFood();
	}

	if (snakeObj->isDead()) {
		this->State = GAME_DEATH;
	}
}


void SnakeGame::ProcessInput(int frame_count)
{
	if (frame_count == 0) {
		hasInput = false;
	}

	if (this->State == GAME_ACTIVE && !hasInput) {

		if (this->Keys[GLFW_KEY_W] && !hasInput) {
			if (direction == 1) return;
			direction = 0;
			hasInput = true;
		}
		if (this->Keys[GLFW_KEY_S] && !hasInput) {
			if (direction == 0) return;
			direction = 1;
			hasInput = true;
		}
		if (this->Keys[GLFW_KEY_A] && !hasInput) {
			if (direction == 3) return;
			direction = 2;
			hasInput = true;
		}
		if (this->Keys[GLFW_KEY_D] && !hasInput) {
			if (direction == 2) return;
			direction = 3;
			hasInput = true;
		}

	}
	snakeObj->setDirection(direction);
}

void SnakeGame::createFood() {
	deque<snake_node> nodes = snakeObj->getNextNodes();
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
	snakeObj->setFoodPos(p);
}

void SnakeGame::createStone() {
	deque<snake_node> nodes = snakeObj->getNextNodes();
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

	stonePos = p;
	snakeObj->setStonePos(p);
}

void SnakeGame::Render(int frame_count)
{
	// get models
	Model model_food = ResourceManager::GetModel("food");
	Model model_body = ResourceManager::GetModel("body");
	Model model_head = ResourceManager::GetModel("head");
	Model model_floor = ResourceManager::GetModel("floor");
	Model model_stone = ResourceManager::GetModel("stone");
	
	// render floor
	lightRenderer->DrawSpriteModel(model_floor, glm::vec3(0.0, -10.0, -5.0), glm::vec3(1.1f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// render snake
	snakeObj->DrawModel(*lightRenderer, model_head, model_body, frame_count);

	// render food
	lightRenderer->DrawSpriteModel(model_food, glm::vec3(foodPos.x, 1.0, foodPos.y), glm::vec3(0.2f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// render stone
	lightRenderer->DrawSpriteModel(model_stone, glm::vec3(stonePos.x, 1.0, stonePos.y), glm::vec3(0.01f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}