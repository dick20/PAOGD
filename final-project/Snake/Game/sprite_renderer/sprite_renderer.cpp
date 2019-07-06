#include "sprite_renderer.h"


SpriteRenderer::SpriteRenderer(Shader &shader)
{
	this->shader = shader;
	//this->initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::DrawSprite(Texture2D &texture, glm::vec3 position, glm::vec3 size, GLfloat rotate, glm::vec3 rotateDirection)
{
	// Prepare transformations
	float getTime = (float)glfwGetTime();
	this->shader.Use();
	glm::mat4 model = glm::mat4(1.0f);

	//model = glm::scale(model, glm::vec3(size, 1.0f));
	//model = glm::scale(model, size);
	model = glm::translate(model, position);  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)
	model = glm::scale(model, size);

	/*
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.5f * size.z)); // Move origin of rotation to center of quad
	model = glm::rotate(model, rotate, glm::vec3(0.0f, 1.0f, 0.0f)); // Then rotate
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z)); // Move origin back
	*/

	//model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale

	this->shader.SetMatrix4("model", model);

	// Render textured quad
	//this->shader.SetVector3f("spriteColor", color);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void SpriteRenderer::DrawSpriteModel(Model &sprite_model, glm::vec3 position, glm::vec3 size, GLfloat rotate, glm::vec3 rotateDirection) {
	// Prepare transformations
	this->shader.Use();
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::scale(model, glm::vec3(0.1f));
	model = glm::translate(model, position);
	//model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.5f * size.z)); // Move origin of rotation to center of quad
	model = glm::rotate(model, rotate, rotateDirection); // Then rotate
	model = glm::scale(model, size);

	this->shader.SetMatrix4("model", model);
	sprite_model.Draw(&(this->shader));
}

void SpriteRenderer::initRenderData()
{
	// Configure VAO/VBO
	GLuint VBO;
	
	GLfloat vertices[] = {
		// Pos                // Tex
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	

	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->quadVAO);

	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

	//位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//纹理属性
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}