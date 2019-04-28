/************************************************************
Created By - Rohit Muneshwar
E-mail - rohit.muneshwar@ymail.com
Date - 14 Jan 2011

Program Name - Breakout game
*************************************************************/
#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include<iostream>
#include<vector>
#include"game_object.h"
#include"sprite_renderer.h"
#include "resource_manager.h"
#include"include\GL\glew.h"
#include"include\glm\glm.hpp"
/*
	GameObject:
	Contains all state related information of a single game object entity
*/
class GameLevel
{
public:
	std::vector<GameObject> bricks;

	//constructors
	GameLevel();
	
	//load level from file
	void load(const GLchar *file, GLuint levelWidth, GLuint levelHeight);
	//render level
	void draw(SpriteRenderer & renderer);
	//check if level is completed
	GLboolean isCompleted();

private:
	//initialize level from tile data
	void init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};
#endif // !GAMELEVEL_H
