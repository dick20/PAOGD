#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glad/glad.h>
//#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../resource_manager/resource_manager.h"
#include "../sprite_renderer/sprite_renderer.h"


// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject
{
public:
	// Object state
	glm::vec3   Position, Size, Velocity;
	glm::vec3   Color;
	GLfloat     Rotation;
	GLboolean   IsSolid;
	GLboolean   Destroyed;
	// Render state
	Texture2D   Sprite;
	// Constructor(s)
	GameObject();
	GameObject(glm::vec3 pos, glm::vec3 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f));
	// Draw sprite
	virtual void Draw(SpriteRenderer &renderer);
};

#endif