#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <windows.h>

//#include "Game/game.h"
#include "Game/Game.h"
#include "Game/resource_manager/resource_manager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define MAX_POINT 42000
#define SCREEN_W 800
#define SCREEN_H 600

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const char* glsl_version = "#version 330";

// DeltaTime variables
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float last_mouseX = 400.0f, last_mouseY = 300.0f, yaw = -90.0f, pitch = 0.0f;
bool firstMove = true;


SnakeGame Snake(SCREEN_W, SCREEN_H);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_W, SCREEN_H, "SnakeGame", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// init game
	Snake.Init();
	Snake.State = GAME_ACTIVE;

	int frame_count = -1;

	while (!glfwWindowShouldClose(window))
	{
		// Calculate delta time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();

		// Manage user input
		bool hasInput = Snake.ProcessInput(frame_count);
		frame_count++;
		if (frame_count >= 35) {
			frame_count = 0;
			if (hasInput) {
				Snake.Update();
			}
		}
		
		// Render
		glClearColor(0.2f, 0.2f, 0.25f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Snake.Render(2);

		glfwSwapBuffers(window);

		if (Snake.State == GAME_DEATH) {
			Sleep(1000);
			break;
		}
	}

	// Delete all resources as loaded using the resource manager
	ResourceManager::Clear();
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);


	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			Snake.Keys[key] = GL_TRUE;
		else if (action == GLFW_RELEASE)
			Snake.Keys[key] = GL_FALSE;
	}
	
}
