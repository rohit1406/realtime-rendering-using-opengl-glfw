/************************************************************
Created By - Rohit Muneshwar
E-mail - rohit.muneshwar@ymail.com
Date - 1 Jan 2011

Program Name - Breakout game
*************************************************************/
#ifndef GAME_H
#define GAME_H

#include "include\GL\glew.h"
#include "include\GLFW\glfw3.h"
#include<vector>
#include"game_level.h"
#include"ball_object.h"
#include<tuple>


//represetn current state of the game
enum GameState
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

enum Direction
{
	UP, RIGHT, DOWN, LEFT
};

typedef std::tuple<GLboolean, Direction, glm::vec2> Collission;

//Game holds all game related state and functionality.
//Combines all game related data into a single class for easy access to each of the components and manageability
class Game
{
public:
	//Game state
	GameState state;
	GLuint consoleWidth;
	GLuint consoleHeight;
	GLboolean keys[1024];
	std::vector<GameLevel> levels;
	GLuint level;
	//constructor/destructor
	Game(GLuint width, GLuint height);
	~Game();

	//initialize game state - load shaders/textures/levels
	void init();
	//game loop
	void processInput(GLfloat dt);
	void update(GLfloat dt);
	void render();

	//collissions
	GLboolean checkCollision(GameObject &one, GameObject &two);
	Collission checkCollision(BallObject &one, GameObject &two);
	void doCollisions();
	void resetLevel();
	void resetPlayer();
};
#endif // !GAME_H
