#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <deque>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Object/SnakeObject.h"

#include <vector>

using namespace std;


enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN,
	GAME_DEATH
};

class SnakeGame
{
private:
	SnakeObject* snakeObj1;
	SnakeObject* snakeObj2;
	int direction1;
	int direction2;
	float boundsTop;
	float boundsBottom;
	float boundsLeft;
	float boundsRight;
	int currentLength;
	bool hasInput1;
	bool hasInput2;
	glm::vec2 foodPos;
	vector<glm::vec2> stonePosVector;

	glm::vec3 lightPos;
	glm::vec3 lightColor;
	glm::vec3 viewPos;


public:
	// Game state
	GameState              State;
	GLboolean              Keys[1024];
	GLuint                 Width, Height;
	// Constructor/Destructor
	SnakeGame(GLuint width, GLuint height);
	~SnakeGame();
	// Initialize game state (load all shaders/textures/levels)
	void Init();
	// GameLoop
	bool ProcessInput(int frame_count);
	void Update();
	void Render(int frame_count);
	void createFood();
	void createStone();
};

#endif