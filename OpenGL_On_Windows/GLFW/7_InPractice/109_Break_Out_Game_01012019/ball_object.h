/************************************************************
Created By - Rohit Muneshwar
E-mail - rohit.muneshwar@ymail.com
Date - 14 Jan 2011

Program Name - Breakout game
*************************************************************/
#ifndef BALLOBJECT_H
#define BALLOBJECT_H

#include<iostream>
#include "game_object.h"
#include"include\GL\glew.h"
#include"texture.h"
#include"include\glm\glm.hpp"

class BallObject : public GameObject
{
public:
	GLfloat radius;
	GLboolean stuck;

	BallObject();
	BallObject(glm::vec2 pos, GLfloat r, glm::vec2 vel, Texture2D sprite);

	glm::vec2 move(GLfloat dt, GLuint window_width);
	void reset(glm::vec2 pos, glm::vec2 vel);
};

#endif // !BALLOBJECT_H
