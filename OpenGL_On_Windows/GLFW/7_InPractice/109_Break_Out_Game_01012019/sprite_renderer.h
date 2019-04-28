/************************************************************
Created By - Rohit Muneshwar
E-mail - rohit.muneshwar@ymail.com
Date - 11 Jan 2011

Program Name - Breakout game
*************************************************************/
#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H
//header files
#include<iostream>
#include"include\GL\glew.h"
#include"include\glm\glm.hpp"
#include"include\glm\gtc\type_ptr.hpp"
#include"include\glm\gtc\matrix_transform.hpp"
#include"shader.h"
#include"texture.h"

class SpriteRenderer
{
	//private
private:
	Shader shader;
	GLuint quadVAO;
	void initRenderData();

public:
	SpriteRenderer(Shader &shader); //constructor
	~SpriteRenderer(); //destructor

	void drawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10),
		GLfloat rotate = 0.0, glm::vec3 color = glm::vec3(1.0f));
};

#endif