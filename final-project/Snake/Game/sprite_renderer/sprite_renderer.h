#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glad/glad.h>
//#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "../texture/texture.h"
#include "../shader/shader.h"
#include "../model/model.h"


class SpriteRenderer
{
public:
	// Constructor (inits shaders/shapes)
	SpriteRenderer(Shader &shader);
	// Destructor
	~SpriteRenderer();
	// Renders a defined quad textured with given sprite
	void DrawSprite(Texture2D &texture, glm::vec3 position, glm::vec3 size, GLfloat rotate, glm::vec3 rotateDirection);
	void DrawSpriteModel(Model &sprite_model, glm::vec3 position, glm::vec3 size, GLfloat rotate, glm::vec3 rotateDirection);
	// Initializes and configures the quad's buffer and vertex attributes
	void initRenderData();

private:
	// Render state
	Shader shader;
	GLuint quadVAO;
};

#endif