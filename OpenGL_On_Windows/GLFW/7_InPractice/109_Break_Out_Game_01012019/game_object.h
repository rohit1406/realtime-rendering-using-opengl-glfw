/************************************************************
Created By - Rohit Muneshwar
E-mail - rohit.muneshwar@ymail.com
Date - 14 Jan 2011

Program Name - Breakout game
*************************************************************/
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include<iostream>
#include"include\GL\glew.h"
#include"include\glm\glm.hpp"
#include"texture.h"
#include"sprite_renderer.h"
/*
	GameObject:
	Contains all state related information of a single game object entity
*/
class GameObject
{
public:
	glm::vec2 position, size, velocity;
	glm::vec3 color;
	GLfloat rotation;
	GLboolean isSolid;
	GLboolean destroyed;
	//render state
	Texture2D sprite;

	//constructors
	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color=glm::vec3(1.0f),
		glm::vec2 velocity= glm::vec2(0.0f, 0.0f));

	//draw sprites
	virtual void draw(SpriteRenderer &renderer);
};
#endif // !GAMEOBJECT_H
