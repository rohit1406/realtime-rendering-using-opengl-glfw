/************************************************************
Created By - Rohit Muneshwar
E-mail - rohit.muneshwar@ymail.com
Date - 11 Jan 2011

Program Name - Breakout game
*************************************************************/
//header files
#include"sprite_renderer.h"


SpriteRenderer::SpriteRenderer(Shader &shader)
{
	this->shader = shader;

	this->initRenderData();
	printf("data initialized for %d",shader.id);
}

SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &this->quadVAO);
}

//initialize render data
void SpriteRenderer::initRenderData()
{
	//data
	GLfloat vertices[] = {
		// Pos      // Tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	//VAO,VBO
	GLuint quadVBO;

	//generate VAO
	glGenVertexArrays(1, &this->quadVAO);
	//bind VAO
	glBindVertexArray(this->quadVAO);

	//generate VBO
	glGenBuffers(1, &quadVBO);
	//bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	//fill data to buffer
	glBufferData(
		GL_ARRAY_BUFFER, //target
		sizeof(vertices), //size
		vertices, //actual data
		GL_STATIC_DRAW //usage
	);
	//enable the vertex attribute
	glVertexAttribPointer(
		0, //index
		4, //size - made up of 4 floats
		GL_FLOAT, //type
		GL_FALSE, //normalize?
		4 * sizeof(GLfloat), //stride
		(void *) 0 //offset
	);
	glEnableVertexAttribArray(0);

	//unbind buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//unbind VAO
	glBindVertexArray(0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SpriteRenderer::drawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size,
	GLfloat rotate, glm::vec3 color)
{
	this->shader.use();

	//prepare transformation matrices
	glm::mat4 modelMatrix;// = glm::mat4(1.0); //identity
	modelMatrix = glm::translate(modelMatrix, glm::vec3(position, 0.0f));

	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	modelMatrix = glm::scale(modelMatrix, glm::vec3(size, 1.0f));

	this->shader.setMatrix4("u_model", modelMatrix);
	this->shader.setVector3f("u_sprite_color", color);

	glActiveTexture(GL_TEXTURE0); //activate texture unit 0
	texture.bind(); //bind texture

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}